#include "global.h"
#include "test/test.h"
#include "field_move.h"
#include "pokemon.h"
#include "party_menu.h"
#include "event_data.h"
#include "item.h"
#include "script.h"
#include "field_player_avatar.h"

extern bool8 ScrCmd_checkfieldmove(struct ScriptContext *ctx);
extern bool8 ScrCmd_checkpartymove(struct ScriptContext *ctx);

#if IS_HNS
TEST("Tactica traversal: each HM depends on its story gate, not moves or machines")
{
    u32 field, badge;
    PARAMETRIZE { field = FIELD_MOVE_CUT; badge = FLAG_BADGE02_GET; }
    PARAMETRIZE { field = FIELD_MOVE_FLASH; badge = FLAG_BADGE01_GET; }
    PARAMETRIZE { field = FIELD_MOVE_ROCK_SMASH; badge = FLAG_BADGE01_GET; }
    PARAMETRIZE { field = FIELD_MOVE_STRENGTH; badge = FLAG_BADGE03_GET; }
    PARAMETRIZE { field = FIELD_MOVE_SURF; badge = FLAG_BADGE04_GET; }
    PARAMETRIZE { field = FIELD_MOVE_FLY; badge = FLAG_BADGE05_GET; }
    PARAMETRIZE { field = FIELD_MOVE_DIVE; badge = FLAG_BADGE07_GET; }
    PARAMETRIZE { field = FIELD_MOVE_WATERFALL; badge = FLAG_BADGE08_GET; }
    InitEventData();
    ClearBag();
    ZeroPlayerPartyMons();
    CreateRandomMon(&gPlayerParty[0], SPECIES_MAGIKARP, 5);
    SetMonMoveSlot(&gPlayerParty[0], MOVE_SPLASH, 0);
    u32 move = FieldMove_GetMoveId(field);
    ASSUME(!CanLearnTeachableMove(SPECIES_MAGIKARP, move));
    u8 checkField[] = {field, TRUE};
    u8 checkParty[] = {move & 255, move >> 8};
    struct ScriptContext ctx = {0};
    EXPECT_EQ(GetTacticaTraversalUser(field), PARTY_SIZE);
    ctx.scriptPtr = checkField;
    ScrCmd_checkfieldmove(&ctx);
    EXPECT_EQ(gSpecialVar_Result, PARTY_SIZE);
    FlagSet(badge);
    if (field == FIELD_MOVE_CUT || field == FIELD_MOVE_ROCK_SMASH)
    {
        EXPECT_EQ(GetTacticaTraversalUser(field), PARTY_SIZE);
        FlagSet(field == FIELD_MOVE_CUT ? FLAG_RECEIVED_HM_CUT : FLAG_RECEIVED_HM_ROCK_SMASH);
    }
    ctx.scriptPtr = checkField;
    ScrCmd_checkfieldmove(&ctx);
    EXPECT_EQ(gSpecialVar_Result, 0);
    EXPECT_EQ(gSpecialVar_0x8004, SPECIES_MAGIKARP);
    ctx.scriptPtr = checkParty;
    ScrCmd_checkpartymove(&ctx);
    EXPECT_EQ(gSpecialVar_Result, 0);
    EXPECT(!MonKnowsMove(&gPlayerParty[0], move));
}

TEST("Tactica traversal: eggs are skipped for animation and no empty-party actor is returned")
{
    InitEventData();
    FlagSet(FLAG_BADGE02_GET);
    FlagSet(FLAG_RECEIVED_HM_CUT);
    ZeroPlayerPartyMons();
    EXPECT_EQ(GetTacticaTraversalUser(FIELD_MOVE_CUT), PARTY_SIZE);
    CreateRandomMon(&gPlayerParty[0], SPECIES_MAGIKARP, 5);
    bool8 egg = TRUE;
    SetMonData(&gPlayerParty[0], MON_DATA_IS_EGG, &egg);
    CreateRandomMon(&gPlayerParty[1], SPECIES_MAGIKARP, 5);
    EXPECT_EQ(GetTacticaTraversalUser(FIELD_MOVE_CUT), 1);
}

TEST("Tactica traversal: non-HM field moves retain their ordinary rules")
{
    EXPECT(!IsTacticaTraversalMove(FIELD_MOVE_DIG));
    EXPECT(!IsTacticaTraversalMove(FIELD_MOVE_TELEPORT));
    EXPECT_EQ(GetTacticaTraversalUserForMove(MOVE_HEADBUTT), PARTY_SIZE);
}
#endif
