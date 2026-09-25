#include "global.h"
#include "pokemon.h"
#include "test/test.h"

#if IS_HNS
static bool32 HasLevelUpMove(u16 species, u16 level, enum Move move)
{
    const struct LevelUpMove *learnset = gSpeciesInfo[species].levelUpLearnset;

    for (; learnset->move != LEVEL_UP_MOVE_END; learnset++)
        if (learnset->level == level && learnset->move == move)
            return TRUE;
    return FALSE;
}

TEST("Tactica balance: corrected species identities are compiled")
{
    EXPECT_EQ(gSpeciesInfo[SPECIES_CHARIZARD].baseAttack, 110);
    EXPECT_EQ(gSpeciesInfo[SPECIES_CHARIZARD_MEGA_X].baseAttack, 156);
    EXPECT_EQ(gSpeciesInfo[SPECIES_CHARIZARD_MEGA_Y].baseAttack, 120);

    EXPECT_EQ(gSpeciesInfo[SPECIES_ELECTIVIRE].types[0], TYPE_ELECTRIC);
    EXPECT_EQ(gSpeciesInfo[SPECIES_ELECTIVIRE].types[1], TYPE_FIGHTING);
    EXPECT_EQ(gSpeciesInfo[SPECIES_ELECTIVIRE].abilities[2], ABILITY_SHEER_FORCE);

    EXPECT_EQ(gSpeciesInfo[SPECIES_HAXORUS].types[0], TYPE_DRAGON);
    EXPECT_EQ(gSpeciesInfo[SPECIES_HAXORUS].types[1], TYPE_STEEL);
    EXPECT_EQ(gSpeciesInfo[SPECIES_HAXORUS].baseSpeed, 110);

    EXPECT_EQ(gSpeciesInfo[SPECIES_SCEPTILE].baseAttack, 105);
    EXPECT_EQ(gSpeciesInfo[SPECIES_SCEPTILE].baseSpAttack, 85);
    EXPECT_EQ(gSpeciesInfo[SPECIES_SCEPTILE].types[1], TYPE_DRAGON);
}

TEST("Tactica balance: all fifteen custom moves are in the active learnsets")
{
    static const struct
    {
        u16 species;
        u16 level;
        enum Move move;
    } expected[] = {
        {SPECIES_ELECTABUZZ, 0, MOVE_BRICK_BREAK},
        {SPECIES_HAXORUS, 0, MOVE_IRON_HEAD},
        {SPECIES_CHARIZARD, 0, MOVE_DRAGON_PULSE},
        {SPECIES_SCEPTILE, 0, MOVE_DRAGON_CLAW},
        {SPECIES_SCEPTILE, 48, MOVE_DRAGON_DANCE},
        {SPECIES_TYPHLOSION, 0, MOVE_EARTH_POWER},
        {SPECIES_KINGDRA, 0, MOVE_LIQUIDATION},
        {SPECIES_KINGDRA, 52, MOVE_DRAGON_RUSH},
        {SPECIES_GRENINJA, 49, MOVE_DARK_PULSE},
        {SPECIES_HAXORUS, 0, MOVE_PSYCHO_CUT},
        {SPECIES_SCEPTILE, 52, MOVE_WOOD_HAMMER},
        {SPECIES_KINGDRA, 45, MOVE_WAVE_CRASH},
        {SPECIES_KINGDRA, 0, MOVE_DRAGON_CLAW},
        {SPECIES_KINGDRA, 52, MOVE_DRAGON_DANCE},
        {SPECIES_KINGDRA, 56, MOVE_FLIP_TURN},
    };

    for (u32 i = 0; i < ARRAY_COUNT(expected); i++)
        EXPECT(HasLevelUpMove(expected[i].species, expected[i].level, expected[i].move));
    EXPECT(!HasLevelUpMove(SPECIES_GRENINJA, 49, MOVE_EXTRASENSORY));
}
#endif
