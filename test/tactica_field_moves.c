#include "global.h"
#include "test/test.h"
#include "field_move.h"
#include "pokemon.h"
#include "party_menu.h"
#include "event_data.h"
#include "item.h"
#include "script.h"
#include "field_player_avatar.h"
#include "overworld.h"
#include "region_map.h"
#include "follower_npc.h"
#include "constants/map_types.h"
#include "constants/party_menu.h"
#include "constants/region_map_sections.h"
#include "text.h"
#include "window.h"

extern bool8 ScrCmd_checkfieldmove(struct ScriptContext *ctx);
extern bool8 ScrCmd_checkpartymove(struct ScriptContext *ctx);

#if IS_HNS
TEST("Tactica traversal: each HM depends on its story gate, not moves or machines")
{
    u32 field, badge;
    PARAMETRIZE { field = FIELD_MOVE_CUT; badge = FLAG_BADGE02_GET; }
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
extern u32 TestTacticaPartyFieldActions(struct Pokemon *mons, u8 slotId);

TEST("Tactica traversal: Flash is automatic from the beginning, including saved darkness")
{
    InitEventData();
    EXPECT(IsFieldMoveUnlocked(FIELD_MOVE_FLASH));
    u32 cave, oldLevel;
    PARAMETRIZE { cave = TRUE; oldLevel = 0; }
    PARAMETRIZE { cave = TRUE; oldLevel = 4; }
    PARAMETRIZE { cave = FALSE; oldLevel = 4; }
    bool8 savedCave = gMapHeader.cave;
    gMapHeader.cave = cave;
    SetFlashLevel(oldLevel);
    ASSUME(GetFlashLevel() == oldLevel);
    SetDefaultFlashLevel();
    EXPECT_EQ(GetFlashLevel(), cave ? 1 : 0);
    EXPECT(!FlagGet(FLAG_BADGE01_GET));
    gMapHeader.cave = savedCave;
}

TEST("Tactica traversal: learned HMs never appear in party actions, ordinary Dig remains")
{
    InitEventData();
    ZeroPlayerPartyMons();
    CreateRandomMon(&gPlayerParty[0], SPECIES_TORKOAL, 20);
    u32 field;
    PARAMETRIZE { field = FIELD_MOVE_CUT; }
    PARAMETRIZE { field = FIELD_MOVE_FLY; }
    PARAMETRIZE { field = FIELD_MOVE_SURF; }
    PARAMETRIZE { field = FIELD_MOVE_STRENGTH; }
    PARAMETRIZE { field = FIELD_MOVE_FLASH; }
    PARAMETRIZE { field = FIELD_MOVE_ROCK_SMASH; }
    PARAMETRIZE { field = FIELD_MOVE_WATERFALL; }
    PARAMETRIZE { field = FIELD_MOVE_DIVE; }
    SetMonMoveSlot(&gPlayerParty[0], FieldMove_GetMoveId(field), 0);
    SetMonMoveSlot(&gPlayerParty[0], MOVE_DIG, 1);
    SetMonMoveSlot(&gPlayerParty[0], MOVE_NONE, 2);
    SetMonMoveSlot(&gPlayerParty[0], MOVE_NONE, 3);
    u32 moves = TestTacticaPartyFieldActions(gPlayerParty, 0);
    EXPECT_EQ(moves & (1u << field), 0);
    EXPECT(moves & (1u << FIELD_MOVE_DIG));
}

TEST("Tactica traversal: map Fly requires badge, visited destination, outdoors and escort permission")
{
    InitEventData();
    ClearBag();
    ZeroPlayerPartyMons();
    ClearFollowerNPCData();
    struct RegionMap map = {0};
    u8 savedType = gMapHeader.mapType;
    gMapHeader.mapType = MAP_TYPE_ROUTE;
    map.mapSecType = MAPSECTYPE_CITY_CANFLY;
    EXPECT(!CanFlyFromRegionMap(&map));
    FlagSet(FLAG_BADGE05_GET);
    EXPECT(CanFlyFromRegionMap(&map)); // No party, Fly move, HM or Poke Rider.
    map.mapSecType = MAPSECTYPE_CITY_CANTFLY;
    EXPECT(!CanFlyFromRegionMap(&map));
    map.mapSecType = MAPSECTYPE_ROUTE;
    EXPECT(!CanFlyFromRegionMap(&map));
    map.mapSecType = MAPSECTYPE_CITY_CANFLY;
    gMapHeader.mapType = MAP_TYPE_INDOOR;
    EXPECT(!CanFlyFromRegionMap(&map));
    gMapHeader.mapType = MAP_TYPE_ROUTE;
#if FNPC_ENABLE_NPC_FOLLOWERS
    SetFollowerNPCData(FNPC_DATA_IN_PROGRESS, TRUE);
    SetFollowerNPCData(FNPC_DATA_FOLLOWER_FLAGS, 0);
    EXPECT(!CanFlyFromRegionMap(&map));
    SetFollowerNPCData(FNPC_DATA_FOLLOWER_FLAGS, FOLLOWER_NPC_FLAG_CAN_LEAVE_ROUTE);
    EXPECT(CanFlyFromRegionMap(&map));
#else
    EXPECT(!PlayerHasFollowerNPC());
#endif
    ClearFollowerNPCData();
    gMapHeader.mapType = savedType;
}
extern u32 TestTacticaMapsecType(mapsec_u16_t mapSecId);
extern void TestTacticaMapPanels(const struct RegionMap *map);

TEST("Tactica traversal: real Johto and Kanto visit flags select the matching Fly arrival")
{
    u32 section, visit;
    PARAMETRIZE { section = MAPSEC_CHERRYGROVE_CITY; visit = FLAG_VISITED_CHERRYGROVE_CITY; }
    PARAMETRIZE { section = MAPSEC_SAFFRON_CITY; visit = FLAG_VISITED_SAFFRON_CITY; }
    InitEventData();
    EXPECT_EQ(TestTacticaMapsecType(section), MAPSECTYPE_CITY_CANTFLY);
    FlagSet(visit);
    EXPECT_EQ(TestTacticaMapsecType(section), MAPSECTYPE_CITY_CANFLY);
    struct RegionMap map = {.mapSecId = section};
    SetFlyDestination(&map);
    EXPECT_EQ(GetDestinationWarpMapHeader()->regionMapSectionId, section);
}

static u32 MapPixel(const u8 *tiles, u32 x, u32 y)
{
    u32 index = (y / 8 * 30 + x / 8) * 32 + y % 8 * 4 + x % 8 / 2;
    return (tiles[index] >> (x % 2 * 4)) & 15;
}

TEST("Tactica traversal: map panels render destination, Fly and hints without overlap")
{
    u32 section;
    PARAMETRIZE { section = MAPSEC_CHERRYGROVE_CITY; }
    PARAMETRIZE { section = MAPSEC_BLACKTHORN_CITY; }
    PARAMETRIZE { section = MAPSEC_VERMILION_CITY; }
    InitEventData();
    ClearFollowerNPCData();
    FlagSet(FLAG_BADGE05_GET);
    u8 savedType = gMapHeader.mapType;
    gMapHeader.mapType = MAP_TYPE_ROUTE;
    struct RegionMap map = {.mapSecId = section, .mapSecType = MAPSECTYPE_CITY_CANFLY};
    FlagSet(FLAG_VISITED_KANTO);
    GetMapName(map.mapSecName, section, MAP_NAME_LENGTH);
    SetDefaultFontsPointer();
    TestTacticaMapPanels(&map);
    EXPECT_EQ(GetWindowAttribute(0, WINDOW_WIDTH), 30);
    EXPECT_EQ(GetWindowAttribute(0, WINDOW_HEIGHT), 3);
    const u8 *pixels = (const u8 *)GetWindowAttribute(0, WINDOW_TILE_DATA);
    u32 nameRight = 0, actionLeft = 240, hintPixels = 0;
    for (u32 x = 0; x < 240; x++)
        for (u32 y = 2; y < 24; y++)
        {
            u32 color = MapPixel(pixels, x, y);
            if (y < 14 && color == 2)
                nameRight = max(nameRight, x);
            if (y < 14 && color == 4)
                actionLeft = min(actionLeft, x);
            if (y >= 16 && color == 5)
                hintPixels++;
        }
    EXPECT(nameRight > 8);
    EXPECT(actionLeft < 232);
    EXPECT_GT(actionLeft, nameRight + 8);
    EXPECT(hintPixels > 10);
    FreeAllWindowBuffers();
    gMapHeader.mapType = savedType;
}
#endif
