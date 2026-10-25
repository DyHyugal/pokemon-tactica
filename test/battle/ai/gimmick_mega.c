#include "global.h"
#include "test/battle.h"
#include "battle_ai_util.h"
#include "data.h"

#if IS_HNS
AI_SINGLE_BATTLE_TEST("Red Lucario chooses Mega Evolution with its production Normal and Hard AI flags")
{
    u32 difficulty = DIFFICULTY_NORMAL;
    PARAMETRIZE { difficulty = DIFFICULTY_NORMAL; }
    PARAMETRIZE { difficulty = DIFFICULTY_HARD; }
    GIVEN {
        AI_FLAGS(gTrainers[difficulty][TRAINER_RED_HNS].aiFlags);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_LUCARIO) { Ability(ABILITY_JUSTIFIED); Item(ITEM_LUCARIONITE); Moves(MOVE_CLOSE_COMBAT); }
    } WHEN {
        TURN { EXPECT_MOVE(opponent, MOVE_CLOSE_COMBAT, gimmick: GIMMICK_MEGA); }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_MEGA_EVOLUTION, opponent);
    } THEN {
        EXPECT_EQ(opponent->species, SPECIES_LUCARIO_MEGA);
        EXPECT_EQ(opponent->ability, ABILITY_ADAPTABILITY);
    }
}
#endif

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
