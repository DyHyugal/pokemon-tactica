#include "global.h"
#include "family_safari.h"
#include "test/test.h"
#include "constants/map_groups.h"
#include "constants/species.h"

#if IS_HNS
TEST("Family Safari: admissions reset to pool A and encounters advance the session")
{
    FamilySafari_ResetRotation();
    EXPECT_EQ(FamilySafari_GetRotation(), 0);
    FamilySafari_AdvanceRotation();
    FamilySafari_AdvanceRotation();
    EXPECT_EQ(FamilySafari_GetRotation(), 2);
    FamilySafari_ResetRotation();
    EXPECT_EQ(FamilySafari_GetRotation(), 0);
}

TEST("Family Safari: Johto and Kanto sectors expose their authored pool counts")
{
    EXPECT_EQ(FamilySafari_GetPoolCount(MAP_GROUP(MAP_SAFARI_ZONE_TOP_LEFT_HNS),
                                       MAP_NUM(MAP_SAFARI_ZONE_TOP_LEFT_HNS)), 5);
    EXPECT_EQ(FamilySafari_GetPoolCount(MAP_GROUP(MAP_FUCHSIA_CITY_SAFARI_ZONE_BEACH_HNS),
                                       MAP_NUM(MAP_FUCHSIA_CITY_SAFARI_ZONE_BEACH_HNS)), 6);
    EXPECT_EQ(FamilySafari_GetPoolCount(MAP_GROUP(MAP_NEW_BARK_TOWN_HNS),
                                       MAP_NUM(MAP_NEW_BARK_TOWN_HNS)), 0);
}

TEST("Family Safari: selection keeps 30 30 30 10 weights")
{
    u16 species;
    u8 minLevel, maxLevel, slot;
    u8 rolls[] = {0, 29, 30, 59, 60, 89, 90, 99};
    u8 expected[] = {0, 0, 1, 1, 2, 2, 3, 3};

    for (u32 i = 0; i < ARRAY_COUNT(rolls); i++)
    {
        EXPECT(FamilySafari_SelectEncounter(MAP_GROUP(MAP_SAFARI_ZONE_TOP_LEFT_HNS),
                                            MAP_NUM(MAP_SAFARI_ZONE_TOP_LEFT_HNS),
                                            0, rolls[i], &species, &minLevel, &maxLevel, &slot));
        EXPECT_EQ(slot, expected[i]);
        EXPECT_EQ(minLevel, 37);
        EXPECT_EQ(maxLevel, 40);
    }
}

TEST("Family Safari: one session counter rotates independently modulo each sector")
{
    u16 speciesA, speciesWrapped;
    u8 minLevel, maxLevel, slot;

    EXPECT(FamilySafari_SelectEncounter(MAP_GROUP(MAP_SAFARI_ZONE_TOP_LEFT_HNS),
                                        MAP_NUM(MAP_SAFARI_ZONE_TOP_LEFT_HNS),
                                        0, 0, &speciesA, &minLevel, &maxLevel, &slot));
    EXPECT(FamilySafari_SelectEncounter(MAP_GROUP(MAP_SAFARI_ZONE_TOP_LEFT_HNS),
                                        MAP_NUM(MAP_SAFARI_ZONE_TOP_LEFT_HNS),
                                        5, 0, &speciesWrapped, &minLevel, &maxLevel, &slot));
    EXPECT_EQ(speciesA, speciesWrapped);

    EXPECT(FamilySafari_SelectEncounter(MAP_GROUP(MAP_FUCHSIA_CITY_SAFARI_ZONE_BEACH_HNS),
                                        MAP_NUM(MAP_FUCHSIA_CITY_SAFARI_ZONE_BEACH_HNS),
                                        5, 0, &speciesWrapped, &minLevel, &maxLevel, &slot));
    EXPECT_NE(speciesA, speciesWrapped);
    EXPECT_EQ(minLevel, 68);
    EXPECT_EQ(maxLevel, 72);
}
#endif
