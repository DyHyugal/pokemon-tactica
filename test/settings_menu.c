#include "global.h"
#include "challenge_menu.h"
#include "load_save.h"
#include "main.h"
#include "new_game.h"
#include "option_menu.h"
#include "palette.h"
#include "pokemon.h"
#include "test/test.h"

static bool8 sReturnedFromSetup;

static void SetupReturned(void)
{
    sReturnedFromSetup = TRUE;
}

static void PumpSetupFrame(u16 keys)
{
    gMain.newKeys = keys;
    gMain.newAndRepeatedKeys = keys;
    gMain.heldKeys = keys;
    gMain.callback2();
    if (gMain.vblankCallback)
        gMain.vblankCallback();
}

TEST("Settings shiny rate: extended odds use the existing shiny threshold")
{
    struct ChallengeSettings saved = gSaveblock3.challengeSettings;
    const u32 expected[] = {8, 16, 32, 64, 128, 655, 6554, 65536};
    u32 i;
    for (i = 0; i < ARRAY_COUNT(expected); i++)
    {
        gSaveblock3.challengeSettings.tx_Features_ShinyChance = i;
        EXPECT_EQ(GetShinyOdds(), expected[i]);
    }
    gSaveblock3.challengeSettings = saved;
}

TEST("Settings recommended: competitive IVs and scaled Trainer EVs are enabled")
{
    struct ChallengeSettings saved = gSaveblock3.challengeSettings;
    struct Pokemon mon;
    u32 stat;

    memset(&gSaveblock3.challengeSettings, 0, sizeof(gSaveblock3.challengeSettings));
    SetDefaultChallengeSettings();
    EXPECT_EQ((u32)gSaveblock3.challengeSettings.tx_Challenges_MaxPartyIVs, 1);
    EXPECT_EQ((u32)gSaveblock3.challengeSettings.tx_Challenges_TrainerScalingIVs, 2);
    EXPECT_EQ((u32)gSaveblock3.challengeSettings.tx_Challenges_TrainerScalingEVs, 1);
    EXPECT_EQ((u32)gSaveblock3.challengeSettings.tx_Challenges_NoEVs, 0);
    EXPECT_EQ(GetCurrentTrainerIVs(), MAX_PER_STAT_IVS);

    CreateRandomMon(&mon, SPECIES_BULBASAUR, 5);
    for (stat = 0; stat < NUM_STATS; stat++)
        EXPECT_EQ(GetMonData(&mon, MON_DATA_HP_IV + stat), MAX_PER_STAT_IVS);

    gSaveblock3.challengeSettings = saved;
}

TEST("Settings setup: Recommended A and R bypass all challenge tabs")
{
    u32 shortcut, frame;
    MainCallback original = gMain.callback2;
    MainCallback savedCallback = gMain.savedCallback;
    IntrCallback vblank = gMain.vblankCallback;
    struct ChallengeSettings saved = gSaveblock3.challengeSettings;
    const u16 shortcuts[] = {A_BUTTON, R_BUTTON};
    for (shortcut = 0; shortcut < ARRAY_COUNT(shortcuts); shortcut++)
    {
        memset(&gSaveblock3.challengeSettings, 0, sizeof(gSaveblock3.challengeSettings));
        SetDefaultChallengeSettings();
        ChallengeMenu_SetInitialSetup(TRUE);
        sReturnedFromSetup = FALSE;
        gMain.savedCallback = SetupReturned;
        SetMainCallback2(CB2_InitChallengeMenu);
        for (frame = 0; frame < 64; frame++)
            PumpSetupFrame(0);
        PumpSetupFrame(shortcuts[shortcut]);
        for (frame = 0; frame < 64; frame++)
            PumpSetupFrame(0);
        EXPECT(sReturnedFromSetup);
        EXPECT_EQ((u32)gSaveblock3.challengeSettings.tx_Challenges_Nuzlocke, 0);
        EXPECT_EQ((u32)gSaveblock3.challengeSettings.tx_Random_Starter, 0);
        EXPECT_EQ((u32)gSaveblock3.challengeSettings.tx_Mode_Modern_Moves, 1);
    }
    gSaveblock3.challengeSettings = saved;
    gMain.savedCallback = savedCallback;
    SetVBlankCallback(vblank);
    SetMainCallback2(original);
}

TEST("Settings setup: Custom edits are discarded when returning to Recommended")
{
    u32 frame;
    MainCallback original = gMain.callback2;
    MainCallback savedCallback = gMain.savedCallback;
    IntrCallback vblank = gMain.vblankCallback;
    struct ChallengeSettings saved = gSaveblock3.challengeSettings;
    memset(&gSaveblock3.challengeSettings, 0, sizeof(gSaveblock3.challengeSettings));
    SetDefaultChallengeSettings();
    ChallengeMenu_SetInitialSetup(TRUE);
    sReturnedFromSetup = FALSE;
    gMain.savedCallback = SetupReturned;
    SetMainCallback2(CB2_InitChallengeMenu);
    for (frame = 0; frame < 64; frame++)
        PumpSetupFrame(0);
    PumpSetupFrame(DPAD_RIGHT); // Custom
    PumpSetupFrame(R_BUTTON); // Features
    PumpSetupFrame(DPAD_RIGHT); // Change RTC type
    EXPECT(!sReturnedFromSetup);
    PumpSetupFrame(L_BUTTON); // Mode
    PumpSetupFrame(DPAD_LEFT); // Recommended
    PumpSetupFrame(A_BUTTON);
    for (frame = 0; frame < 64; frame++)
        PumpSetupFrame(0);
    EXPECT(sReturnedFromSetup);
    EXPECT_EQ((u32)gSaveblock3.challengeSettings.tx_Features_RTCType, 0);
    gSaveblock3.challengeSettings = saved;
    gMain.savedCallback = savedCallback;
    SetVBlankCallback(vblank);
    SetMainCallback2(original);
}

TEST("Settings options: challenge save and cancel return through Options")
{
    u32 save, frame, row;
    MainCallback original = gMain.callback2;
    MainCallback savedCallback = gMain.savedCallback;
    IntrCallback vblank = gMain.vblankCallback;
    struct ChallengeSettings saved = gSaveblock3.challengeSettings;
    for (save = 0; save < 2; save++)
    {
        sReturnedFromSetup = FALSE;
        gMain.savedCallback = SetupReturned;
        SetMainCallback2(CB2_InitOptionMenu);
        for (frame = 0; frame < 64; frame++)
            PumpSetupFrame(0);
        for (row = 0; row < 13; row++)
            PumpSetupFrame(DPAD_DOWN); // Challenge Settings, last row of Main
        PumpSetupFrame(A_BUTTON);
        for (frame = 0; frame < 64; frame++)
            PumpSetupFrame(0);
        EXPECT(!sReturnedFromSetup);
        if (save)
        {
            for (row = 0; row < 6; row++)
                PumpSetupFrame(R_BUTTON);
            PumpSetupFrame(0); // Build final confirmation
            PumpSetupFrame(A_BUTTON);
        }
        else
            PumpSetupFrame(B_BUTTON);
        for (frame = 0; frame < 64; frame++)
            PumpSetupFrame(0);
        EXPECT(!sReturnedFromSetup); // Still in Options
        PumpSetupFrame(B_BUTTON);
        for (frame = 0; frame < 64; frame++)
            PumpSetupFrame(0);
        EXPECT(sReturnedFromSetup); // Original caller, not a callback loop
    }
    gSaveblock3.challengeSettings = saved;
    gMain.savedCallback = savedCallback;
    SetVBlankCallback(vblank);
    SetMainCallback2(original);
}
