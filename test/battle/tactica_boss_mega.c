#include "global.h"
#include "test/battle.h"

#if IS_HNS
SINGLE_BATTLE_TEST("Tactica boss legality: production boss Megas update ability and both types")
{
    u32 species, ability, item, mega, megaAbility, type1, type2;
#include "tactica_boss_mega_cases.h"
    GIVEN {
        // Multitype cannot be traced: observe the Mega's own Trace ability.
        PLAYER(SPECIES_ARCEUS) { Ability(ABILITY_MULTITYPE); }
        OPPONENT(species) { Ability(ability); Item(item); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_CELEBRATE, gimmick: GIMMICK_MEGA); }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_MEGA_EVOLUTION, opponent);
    } THEN {
        EXPECT_EQ(opponent->species, mega);
        EXPECT_EQ(opponent->ability, megaAbility);
        EXPECT_EQ(opponent->types[0], type1);
        EXPECT_EQ(opponent->types[1], type2);
        EXPECT_EQ(GetMonData(&gEnemyParty[0], MON_DATA_SPECIES), mega);
    }
}
SINGLE_BATTLE_TEST("Tactica boss legality: same Mega forms revert safely for the player")
{
    u32 species, ability, item, mega, megaAbility, type1, type2;
#include "tactica_boss_mega_cases.h"
    GIVEN {
        PLAYER(species) { Ability(ability); Item(item); }
        OPPONENT(SPECIES_ARCEUS) { Ability(ABILITY_MULTITYPE); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE, gimmick: GIMMICK_MEGA); MOVE(opponent, MOVE_CELEBRATE); }
    } THEN {
        EXPECT_EQ(player->species, mega);
        EXPECT_EQ(player->ability, megaAbility);
        EXPECT_EQ(player->types[0], type1);
        EXPECT_EQ(player->types[1], type2);
        EXPECT_EQ(GetMonData(&gPlayerParty[0], MON_DATA_SPECIES), species);
        EXPECT_EQ(GetMonAbility(&gPlayerParty[0]), ability);
    }
}
#endif
