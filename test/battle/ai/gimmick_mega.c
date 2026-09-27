#include "global.h"
#include "test/battle.h"
#include "battle_ai_util.h"

AI_SINGLE_BATTLE_TEST("AI uses Mega Evolution")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_OMNISCIENT );
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_VENUSAUR) { Item(ITEM_VENUSAURITE); Moves(MOVE_SLUDGE_BOMB); }
    } WHEN {
        TURN { EXPECT_MOVE(opponent, MOVE_SLUDGE_BOMB, gimmick: GIMMICK_MEGA); }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_MEGA_EVOLUTION, opponent);
    } THEN {
        EXPECT_EQ(opponent->species, SPECIES_VENUSAUR_MEGA);
    }
}

AI_SINGLE_BATTLE_TEST("Morty-style Gengar Mega Evolution replaces Cursed Body with Shadow Tag")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_OMNISCIENT);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_GENGAR) { Ability(ABILITY_CURSED_BODY); Item(ITEM_GENGARITE); Moves(MOVE_SHADOW_BALL); }
    } WHEN {
        TURN { EXPECT_MOVE(opponent, MOVE_SHADOW_BALL, gimmick: GIMMICK_MEGA); }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_MEGA_EVOLUTION, opponent);
    } THEN {
        EXPECT_EQ(opponent->species, SPECIES_GENGAR_MEGA);
        EXPECT_EQ(opponent->ability, ABILITY_SHADOW_TAG);
    }
}
