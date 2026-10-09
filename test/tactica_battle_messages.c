#include "global.h"
#include "test/test.h"
#include "battle.h"
#include "battle_controllers.h"
#include "battle_message.h"
#include "bg.h"
#include "event_data.h"
#include "main.h"
#include "malloc.h"
#include "native_speed.h"
#include "string_util.h"
#include "text.h"
#include "window.h"
#include "constants/battle_string_ids.h"
#include "constants/characters.h"

#if IS_HNS
extern bool32 TestTacticaPrepareBattleMessage(enum BattlerId, enum StringID, bool32);
extern void TestTacticaWaitForBattleMessage(enum BattlerId);
extern void TestTacticaHandlePrintString(enum BattlerId);
extern const struct WindowTemplate *const gBattleWindowTemplates[];

static u32 sCompleted;

static void RecordComplete(enum BattlerId battler)
{
    sCompleted++;
}

static void InitMessageWindow(void)
{
    const struct BgTemplate backgrounds[] = {
        {.bg = 0, .charBaseIndex = 0, .mapBaseIndex = 31},
    };
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, backgrounds, ARRAY_COUNT(backgrounds));
    InitWindows(gBattleWindowTemplates[B_WIN_TYPE_NORMAL]);
    SetDefaultFontsPointer();
    gBattleScripting.windowsType = B_WIN_TYPE_NORMAL;
    gBattleTypeFlags = 0;
    gBattlerControllerEndFuncs[0] = RecordComplete;
    sCompleted = 0;
}

TEST("Tactica battle messages: real rendering never completes without a fresh press at x1 through x4")
{
    u32 speed;
    PARAMETRIZE { speed = 1; }
    PARAMETRIZE { speed = 2; }
    PARAMETRIZE { speed = 3; }
    PARAMETRIZE { speed = 4; }
    InitEventData();
    InitMessageWindow();
    SetNativeGameSpeed(speed);
    for (u32 textSpeed = OPTIONS_TEXT_SPEED_SLOW; textSpeed <= OPTIONS_TEXT_SPEED_INSTANT; textSpeed++)
        for (u32 fastBattle = 0; fastBattle < 2; fastBattle++)
        {
            gSaveBlock2Ptr->optionsTextSpeed = textSpeed;
            gSaveBlock3Ptr->challengeSettings.fastBattle = fastBattle;
            StringCopy(gDisplayedStringBattle, COMPOUND_STRING("CHARCADET used TACKLE!"));
            EXPECT(TestTacticaPrepareBattleMessage(0, STRINGID_USEDMOVE, FALSE));
            BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_MSG);
            gTextFlags.autoScroll = FALSE; // Simulate a player, not the headless runner.
            u32 completed = sCompleted;
            gMain.heldKeys = A_BUTTON;
            for (u32 frame = 0; frame < 512; frame++)
            {
                gMain.newKeys = frame == 0 ? A_BUTTON : 0;
                for (u32 tick = 0; tick < speed; tick++)
                {
                    RunTextPrinters();
                    TestTacticaWaitForBattleMessage(0);
                    EXPECT_EQ(sCompleted, completed);
                    NativeSpeed_ClearInputEdges();
                }
                if (!IsTextPrinterActiveOnWindow(B_WIN_MSG))
                    break;
            }
            EXPECT(!IsTextPrinterActiveOnWindow(B_WIN_MSG));
            // Wait longer than every native battle-message timer, while holding A.
            for (u32 tick = 0; tick < 512; tick++)
                TestTacticaWaitForBattleMessage(0);
            EXPECT_EQ(sCompleted, completed);
            gMain.heldKeys = 0;
            gMain.newKeys = A_BUTTON;
            TestTacticaWaitForBattleMessage(0);
            EXPECT_EQ(sCompleted, completed + 1);
            EXPECT_EQ(GetNativeGameSpeed(), speed);
            EXPECT_EQ((u32)gSaveBlock2Ptr->optionsTextSpeed, textSpeed);
            EXPECT_EQ((u32)gSaveBlock3Ptr->challengeSettings.fastBattle, fastBattle);
            NativeSpeed_ClearInputEdges();
        }
    gMain.heldKeys = 0;
    SetNativeGameSpeed(1);
    FreeAllWindowBuffers();
}

TEST("Tactica battle messages: the rendering press and extra ticks cannot dismiss consecutive messages")
{
    InitMessageWindow();
    for (u32 message = 0; message < 2; message++)
    {
        StringCopy(gDisplayedStringBattle, COMPOUND_STRING("CHARCADET accuracy fell!"));
        EXPECT(TestTacticaPrepareBattleMessage(0, STRINGID_DEFENDERSSTATFELL, FALSE));
        gMain.heldKeys = gMain.newKeys = A_BUTTON;
        TestTacticaWaitForBattleMessage(0);
        EXPECT_EQ(sCompleted, message);
        NativeSpeed_ClearInputEdges();
        for (u32 tick = 0; tick < 16; tick++)
            TestTacticaWaitForBattleMessage(0);
        EXPECT_EQ(sCompleted, message);
        gMain.heldKeys = 0;
        gMain.newKeys = A_BUTTON;
        TestTacticaWaitForBattleMessage(0);
        EXPECT_EQ(sCompleted, message + 1);
    }
    NativeSpeed_ClearInputEdges();
    gMain.heldKeys = 0;
    FreeAllWindowBuffers();
}

TEST("Tactica battle messages: Yes No prompts and automatic battles retain their original flow")
{
    u32 id, type;
    bool32 automated;
    PARAMETRIZE { id = STRINGID_USENEXTPKMN; type = 0; automated = FALSE; }
    PARAMETRIZE { id = STRINGID_ENEMYABOUTTOSWITCHPKMN; type = 0; automated = FALSE; }
    PARAMETRIZE { id = STRINGID_TRYTOLEARNMOVE3; type = 0; automated = FALSE; }
    PARAMETRIZE { id = STRINGID_STOPLEARNINGMOVE; type = 0; automated = FALSE; }
    PARAMETRIZE { id = STRINGID_USEDMOVE; type = BATTLE_TYPE_LINK; automated = FALSE; }
    PARAMETRIZE { id = STRINGID_USEDMOVE; type = BATTLE_TYPE_RECORDED; automated = FALSE; }
    PARAMETRIZE { id = STRINGID_USEDMOVE; type = BATTLE_TYPE_RECORDED_LINK; automated = FALSE; }
    PARAMETRIZE { id = STRINGID_USEDMOVE; type = BATTLE_TYPE_CATCH_TUTORIAL; automated = FALSE; }
    PARAMETRIZE { id = STRINGID_USEDMOVE; type = BATTLE_TYPE_POKEDUDE; automated = FALSE; }
    PARAMETRIZE { id = STRINGID_USEDMOVE; type = 0; automated = TRUE; }
    gBattleTypeFlags = type;
    static const u8 text[] = _("Proceed?\p");
    StringCopy(gDisplayedStringBattle, text);
    EXPECT(!TestTacticaPrepareBattleMessage(0, id, automated));
    EXPECT_EQ(StringCompare(gDisplayedStringBattle, text), 0);
    gBattleTypeFlags = 0;
}

TEST("Tactica battle messages: terminal waits become one confirmation without stripping control arguments")
{
    static const u8 done[] = _("Done!");
    static const u8 scroll[] = _("Done!\p");
    static const u8 press[] = _("Done!{PAUSE_UNTIL_PRESS}");
    static const u8 both[] = _("Done!{PAUSE_UNTIL_PRESS}\p");
    static const u8 internal[] = _("Page one.\pPage two.");
    static const u8 argument[] = _("Done!{PAUSE 251}");
    const u8 *input, *expected;
    PARAMETRIZE_LABEL("%s", "scroll") { input = scroll; expected = done; }
    PARAMETRIZE_LABEL("%s", "press") { input = press; expected = done; }
    PARAMETRIZE_LABEL("%s", "both") { input = both; expected = done; }
    PARAMETRIZE_LABEL("%s", "internal") { input = internal; expected = internal; }
    PARAMETRIZE_LABEL("%s", "argument") { input = argument; expected = argument; }
    gBattleTypeFlags = 0;
    StringCopy(gDisplayedStringBattle, input);
    EXPECT(TestTacticaPrepareBattleMessage(0, STRINGID_TRAINER1LOSETEXT, FALSE));
    EXPECT_EQ(StringCompare(gDisplayedStringBattle, expected), 0);
    gDisplayedStringBattle[0] = EOS;
    EXPECT(!TestTacticaPrepareBattleMessage(0, STRINGID_USEDMOVE, FALSE));
}
TEST("Tactica battle messages: the real print handler dispatches to fresh-input confirmation")
{
    InitEventData();
    InitMessageWindow();
    struct BattleResources *savedResources = gBattleResources;
    struct BattleStruct *savedStruct = gBattleStruct;
    gBattleResources = AllocZeroed(sizeof(*gBattleResources));
    gBattleStruct = AllocZeroed(sizeof(*gBattleStruct));
    EXPECT(gBattleResources != NULL);
    EXPECT(gBattleStruct != NULL);
    enum BattlerId battler = 0;
    *(u16 *)&gBattleResources->bufferA[battler][2] = STRINGID_BUTITFAILED;
    struct BattleMsgData *data = (void *)&gBattleResources->bufferA[battler][4];
    for (u32 i = 0; i < 3; i++)
        data->textBuffs[i][0] = EOS;
    gSaveBlock2Ptr->optionsTextSpeed = OPTIONS_TEXT_SPEED_INSTANT;
    TestTacticaHandlePrintString(battler);
    gTextFlags.autoScroll = FALSE;
    EXPECT(gBattlerControllerFuncs[battler] != Controller_WaitForString);
    EXPECT(gDisplayedStringBattle[0] != EOS);
    gMain.heldKeys = gMain.newKeys = 0;
    for (u32 tick = 0; tick < 512; tick++)
    {
        RunTextPrinters();
        gBattlerControllerFuncs[battler](battler);
        EXPECT_EQ(sCompleted, 0);
    }
    EXPECT(!IsTextPrinterActiveOnWindow(B_WIN_MSG));
    gMain.newKeys = A_BUTTON;
    gBattlerControllerFuncs[battler](battler);
    EXPECT_EQ(sCompleted, 1);
    NativeSpeed_ClearInputEdges();
    Free(gBattleResources);
    Free(gBattleStruct);
    gBattleResources = savedResources;
    gBattleStruct = savedStruct;
    FreeAllWindowBuffers();
}
#endif
