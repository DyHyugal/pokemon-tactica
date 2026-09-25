#include "global.h"
#include "test/test.h"
#include "wild_encounter.h"
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
#endif
