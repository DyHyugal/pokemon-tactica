#include "global.h"
#include "test/battle.h"

AI_SINGLE_BATTLE_TEST("Prefer Baton Pass hands off after two boosts against a Fighting type")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_DOUBLE_TEAM) == EFFECT_EVASION_UP);
        ASSUME(GetMoveEffect(MOVE_BATON_PASS) == EFFECT_BATON_PASS);
        AI_FLAGS(AI_FLAG_BASIC_TRAINER | AI_FLAG_PREFER_BATON_PASS);
        PLAYER(SPECIES_MACHOKE) { Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_EEVEE) { Moves(MOVE_DOUBLE_TEAM, MOVE_BATON_PASS); }
        OPPONENT(SPECIES_URSARING) { Moves(MOVE_TACKLE); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_MOVE(opponent, MOVE_DOUBLE_TEAM); }
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_MOVE(opponent, MOVE_DOUBLE_TEAM); }
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_MOVE(opponent, MOVE_BATON_PASS); EXPECT_SEND_OUT(opponent, 1); }
    }
}

AI_SINGLE_BATTLE_TEST("Prefer Baton Pass allows a third boost only at full HP in a safe matchup")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_DOUBLE_TEAM) == EFFECT_EVASION_UP);
        ASSUME(GetMoveEffect(MOVE_BATON_PASS) == EFFECT_BATON_PASS);
        AI_FLAGS(AI_FLAG_BASIC_TRAINER | AI_FLAG_PREFER_BATON_PASS);
        PLAYER(SPECIES_BLISSEY) { Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_EEVEE) { Moves(MOVE_DOUBLE_TEAM, MOVE_BATON_PASS); }
        OPPONENT(SPECIES_MILTANK) { Moves(MOVE_TACKLE); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_MOVE(opponent, MOVE_DOUBLE_TEAM); }
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_MOVE(opponent, MOVE_DOUBLE_TEAM); }
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_MOVE(opponent, MOVE_DOUBLE_TEAM); }
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_MOVE(opponent, MOVE_BATON_PASS); EXPECT_SEND_OUT(opponent, 1); }
    }
}

AI_SINGLE_BATTLE_TEST("Smart setup users attack after gaining two positive stages")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_SWORDS_DANCE) == EFFECT_ATTACK_UP_2);
        AI_FLAGS(AI_FLAG_BASIC_TRAINER | AI_FLAG_POWERFUL_STATUS);
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_MIMIKYU) { Moves(MOVE_SWORDS_DANCE, MOVE_PLAY_ROUGH); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_MOVE(opponent, MOVE_SWORDS_DANCE); }
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_MOVE(opponent, MOVE_PLAY_ROUGH); }
    }
}
