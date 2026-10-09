#include "global.h"
#include "test/test.h"
#include "wild_encounter.h"
#include "pokemon.h"
#include "tactica_progression.h"
#include "event_data.h"
#include "family_safari.h"
#include "constants/maps.h"
#include "constants/wild_encounter.h"

#if IS_HNS
TEST("Tactica encounters: every standard method uses four real slots")
{
    EXPECT_EQ(LAND_WILD_COUNT, 4);
    EXPECT_EQ(WATER_WILD_COUNT, 4);
    EXPECT_EQ(ROCK_WILD_COUNT, 4);
    EXPECT_EQ(FISH_WILD_COUNT, 4);
}

TEST("Tactica encounters: slot boundaries are 30 30 30 10")
{
    static const u8 rolls[] = {0, 29, 30, 59, 60, 89, 90, 99};
    static const u8 expected[] = {0, 0, 1, 1, 2, 2, 3, 3};

    for (u32 i = 0; i < ARRAY_COUNT(rolls); i++)
        EXPECT_EQ(ChooseTacticaEncounterSlot(rolls[i]), expected[i]);
}
TEST("Tactica encounters: actual wild creation respects exact evolution thresholds")
{
    static const struct {u16 source; u8 level; u16 expected;} cases[] = {
        {SPECIES_MAGIKARP, 19, SPECIES_MAGIKARP},
        {SPECIES_MAGIKARP, 20, SPECIES_GYARADOS},
        {SPECIES_MAGIKARP, 50, SPECIES_GYARADOS},
        {SPECIES_VOLCARONA, 38, SPECIES_LARVESTA},
        {SPECIES_LARVESTA, 58, SPECIES_LARVESTA},
        {SPECIES_LARVESTA, 59, SPECIES_VOLCARONA},
        {SPECIES_STARAVIA, 33, SPECIES_STARAVIA},
        {SPECIES_STARAVIA, 34, SPECIES_STARAPTOR},
        {SPECIES_TYNAMO, 38, SPECIES_TYNAMO},
        {SPECIES_TYNAMO, 39, SPECIES_EELEKTRIK},
        {SPECIES_MUDKIP, 18, SPECIES_MARSHTOMP},
        {SPECIES_MUDKIP, 45, SPECIES_SWAMPERT},
        // Owner: object / Linking Cord evolutions remain player decisions.
        {SPECIES_SEADRA, 60, SPECIES_SEADRA},
        {SPECIES_EEVEE, 50, SPECIES_EEVEE},
        {SPECIES_EELEKTRIK, 50, SPECIES_EELEKTRIK},
    };
    u32 index = 0;
    for (u32 i = 0; i < ARRAY_COUNT(cases); i++)
        PARAMETRIZE { index = i; }
    gSaveBlock3Ptr->challengeSettings.tx_Mode_Modern_Moves = 1;
    CreateWildMon(cases[index].source, cases[index].level);
    EXPECT_EQ(GetMonData(&gEnemyParty[0], MON_DATA_SPECIES), cases[index].expected);
    EXPECT_EQ(GetMonData(&gEnemyParty[0], MON_DATA_LEVEL), cases[index].level);
    EXPECT_NE(GetMonData(&gEnemyParty[0], MON_DATA_MOVE1), MOVE_NONE);
}

TEST("Tactica encounters: a Safari pool uses evolved level stages while retaining item evolutions")
{
    u16 species;
    u8 low, high, slot;
    EXPECT(FamilySafari_SelectEncounter(MAP_GROUP(MAP_SAFARI_ZONE_TOP_LEFT_HNS), MAP_NUM(MAP_SAFARI_ZONE_TOP_LEFT_HNS), 1, 65, &species, &low, &high, &slot));
    EXPECT_EQ(species, SPECIES_LEDIAN);
    CreateWildMon(species, low);
    EXPECT_EQ(GetMonData(&gEnemyParty[0], MON_DATA_SPECIES), SPECIES_LEDIAN);
    EXPECT(FamilySafari_SelectEncounter(MAP_GROUP(MAP_SAFARI_ZONE_TOP_LEFT_HNS), MAP_NUM(MAP_SAFARI_ZONE_TOP_LEFT_HNS), 3, 35, &species, &low, &high, &slot));
    EXPECT_EQ(species, SPECIES_KARRABLAST);
    CreateWildMon(species, high);
    EXPECT_EQ(GetMonData(&gEnemyParty[0], MON_DATA_SPECIES), SPECIES_KARRABLAST);
}
#endif
