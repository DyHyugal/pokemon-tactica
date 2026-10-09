#include "global.h"
#include "test/battle.h"

AI_SINGLE_BATTLE_TEST("Tactica field support: Electric setter hands off before the ace without a knockout")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_BASIC_TRAINER | AI_FLAG_FIELD_SUPPORT | AI_FLAG_ACE_POKEMON);
        PLAYER(SPECIES_AZUMARILL) { HP(1000); MaxHP(1000); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_PINCURCHIN) { Ability(ABILITY_ELECTRIC_SURGE); Moves(MOVE_THUNDERBOLT); }
        OPPONENT(SPECIES_TOXTRICITY) { Moves(MOVE_ELECTRIC_TERRAIN, MOVE_THUNDERBOLT); }
        OPPONENT(SPECIES_RAICHU_ALOLA) { Ability(ABILITY_SURGE_SURFER); Moves(MOVE_THUNDERBOLT); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_SWITCH(opponent, 2); }
    }
}

AI_SINGLE_BATTLE_TEST("Tactica field support: immediate knockout is worth taking")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_BASIC_TRAINER | AI_FLAG_FIELD_SUPPORT);
        PLAYER(SPECIES_MAGIKARP) { HP(1); Moves(MOVE_SPLASH); }
        OPPONENT(SPECIES_PINCURCHIN) { Ability(ABILITY_ELECTRIC_SURGE); Moves(MOVE_THUNDERBOLT); }
        OPPONENT(SPECIES_TOXTRICITY) { Moves(MOVE_THUNDERBOLT); }
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); EXPECT_MOVE(opponent, MOVE_THUNDERBOLT); }
    }
}

AI_SINGLE_BATTLE_TEST("Tactica field support: restore Electric Terrain before sending another attacker")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_BASIC_TRAINER | AI_FLAG_FIELD_SUPPORT);
        PLAYER(SPECIES_BLISSEY) { HP(2000); MaxHP(2000); Moves(MOVE_CELEBRATE, MOVE_GRASSY_TERRAIN); }
        OPPONENT(SPECIES_PINCURCHIN) { Ability(ABILITY_ELECTRIC_SURGE); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_RAICHU_ALOLA) { Ability(ABILITY_SURGE_SURFER); Moves(MOVE_THUNDER_SHOCK); }
        OPPONENT(SPECIES_MAGNEZONE) { Moves(MOVE_THUNDER_SHOCK); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_SWITCH(opponent, 1); }
        TURN { MOVE(player, MOVE_GRASSY_TERRAIN); EXPECT_MOVE(opponent, MOVE_THUNDER_SHOCK); }
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_SWITCH(opponent, 0); }
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_SWITCH(opponent, 1); }
    }
}

AI_SINGLE_BATTLE_TEST("Tactica field support: manual Rain setter restores then hands off")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_BASIC_TRAINER | AI_FLAG_FIELD_SUPPORT);
        PLAYER(SPECIES_BLISSEY) { Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_VOLBEAT) { Moves(MOVE_RAIN_DANCE, MOVE_TACKLE); }
        OPPONENT(SPECIES_KINGDRA) { Ability(ABILITY_SWIFT_SWIM); Moves(MOVE_WATER_GUN); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_MOVE(opponent, MOVE_RAIN_DANCE); }
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_SWITCH(opponent, 1); }
    }
}

AI_SINGLE_BATTLE_TEST("Tactica field support: weather setters hand off to their beneficiaries")
{
    u32 setter, ability, receiver, receiverAbility;
    PARAMETRIZE { setter = SPECIES_PELIPPER; ability = ABILITY_DRIZZLE; receiver = SPECIES_KINGDRA; receiverAbility = ABILITY_SWIFT_SWIM; }
    PARAMETRIZE { setter = SPECIES_TORKOAL; ability = ABILITY_DROUGHT; receiver = SPECIES_VENUSAUR; receiverAbility = ABILITY_CHLOROPHYLL; }
    PARAMETRIZE { setter = SPECIES_TYRANITAR; ability = ABILITY_SAND_STREAM; receiver = SPECIES_EXCADRILL; receiverAbility = ABILITY_SAND_RUSH; }
    PARAMETRIZE { setter = SPECIES_NINETALES_ALOLA; ability = ABILITY_SNOW_WARNING; receiver = SPECIES_SANDSLASH_ALOLA; receiverAbility = ABILITY_SLUSH_RUSH; }
    GIVEN {
        AI_FLAGS(AI_FLAG_BASIC_TRAINER | AI_FLAG_FIELD_SUPPORT);
        PLAYER(SPECIES_BLISSEY) { Moves(MOVE_CELEBRATE); }
        OPPONENT(setter) { Ability(ability); Moves(MOVE_CELEBRATE); }
        OPPONENT(receiver) { Ability(receiverAbility); Moves(MOVE_TACKLE); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_SWITCH(opponent, 1); }
    }
}

AI_SINGLE_BATTLE_TEST("Tactica field support: Aurora Veil is set before handing off")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_BASIC_TRAINER | AI_FLAG_FIELD_SUPPORT | AI_FLAG_POWERFUL_STATUS);
        PLAYER(SPECIES_BLISSEY) { Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_NINETALES_ALOLA) { Ability(ABILITY_SNOW_WARNING); Moves(MOVE_AURORA_VEIL, MOVE_TACKLE); }
        OPPONENT(SPECIES_SANDSLASH_ALOLA) { Ability(ABILITY_SLUSH_RUSH); Moves(MOVE_TACKLE); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_MOVE(opponent, MOVE_AURORA_VEIL); }
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_SWITCH(opponent, 1); }
    }
}

AI_SINGLE_BATTLE_TEST("Tactica field support: no handoff without a beneficiary")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_BASIC_TRAINER | AI_FLAG_FIELD_SUPPORT);
        PLAYER(SPECIES_BLISSEY) { Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_PINCURCHIN) { Ability(ABILITY_ELECTRIC_SURGE); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_MEOWTH) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_MOVE(opponent, MOVE_CELEBRATE); }
    }
}

AI_SINGLE_BATTLE_TEST("Tactica field support: hidden player coverage does not change the handoff")
{
    u32 hidden;
    PARAMETRIZE { hidden = MOVE_EARTHQUAKE; }
    PARAMETRIZE { hidden = MOVE_CELEBRATE; }
    GIVEN {
        AI_FLAGS(AI_FLAG_BASIC_TRAINER | AI_FLAG_FIELD_SUPPORT);
        PLAYER(SPECIES_BLISSEY) { Moves(MOVE_CELEBRATE, hidden); }
        OPPONENT(SPECIES_PINCURCHIN) { Ability(ABILITY_ELECTRIC_SURGE); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_TOXTRICITY) { Moves(MOVE_THUNDER_SHOCK); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_SWITCH(opponent, 1); }
    }
}

AI_SINGLE_BATTLE_TEST("Tactica field support: ordinary AI keeps its current move behaviour")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_BASIC_TRAINER);
        PLAYER(SPECIES_BLISSEY) { Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_PINCURCHIN) { Ability(ABILITY_ELECTRIC_SURGE); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_TOXTRICITY) { Moves(MOVE_THUNDER_SHOCK); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_MOVE(opponent, MOVE_CELEBRATE); }
    }
}

AI_SINGLE_BATTLE_TEST("Tactica field support: backup Toxtricity restores terrain before the beneficiary")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_BASIC_TRAINER | AI_FLAG_FIELD_SUPPORT);
        PLAYER(SPECIES_BLISSEY) { HP(2000); MaxHP(2000); Moves(MOVE_CELEBRATE, MOVE_GRASSY_TERRAIN); }
        OPPONENT(SPECIES_RAICHU_ALOLA) { Ability(ABILITY_SURGE_SURFER); Moves(MOVE_THUNDER_SHOCK); }
        OPPONENT(SPECIES_TOXTRICITY) { Moves(MOVE_ELECTRIC_TERRAIN, MOVE_THUNDER_SHOCK); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_SWITCH(opponent, 1); }
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_MOVE(opponent, MOVE_ELECTRIC_TERRAIN); }
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_SWITCH(opponent, 0); }
    }
}

AI_SINGLE_BATTLE_TEST("Tactica field support: an ace is allowed when it is the only beneficiary")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_BASIC_TRAINER | AI_FLAG_FIELD_SUPPORT | AI_FLAG_ACE_POKEMON);
        PLAYER(SPECIES_BLISSEY) { Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_PINCURCHIN) { Ability(ABILITY_ELECTRIC_SURGE); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_RAICHU_ALOLA) { Ability(ABILITY_SURGE_SURFER); Moves(MOVE_THUNDER_SHOCK); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_SWITCH(opponent, 1); }
    }
}

AI_SINGLE_BATTLE_TEST("Tactica field support: trapped setters cannot hand off")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_BASIC_TRAINER | AI_FLAG_FIELD_SUPPORT);
        PLAYER(SPECIES_DUGTRIO) { Ability(ABILITY_ARENA_TRAP); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_PINCURCHIN) { Ability(ABILITY_ELECTRIC_SURGE); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_RAICHU_ALOLA) { Ability(ABILITY_SURGE_SURFER); Moves(MOVE_THUNDER_SHOCK); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_MOVE(opponent, MOVE_CELEBRATE); }
    }
}

AI_SINGLE_BATTLE_TEST("Tactica field support: after a KO the backup setter precedes another attacker")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_BASIC_TRAINER | AI_FLAG_FIELD_SUPPORT);
        PLAYER(SPECIES_BLISSEY) { Speed(1000); Moves(MOVE_BLOCK, MOVE_GRASSY_TERRAIN, MOVE_DRAGON_RAGE); }
        OPPONENT(SPECIES_PINCURCHIN) { Speed(100); HP(1); Ability(ABILITY_ELECTRIC_SURGE); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_RAICHU_ALOLA) { Speed(100); HP(1); Ability(ABILITY_SURGE_SURFER); Moves(MOVE_THUNDER_SHOCK); }
        OPPONENT(SPECIES_TOXTRICITY) { Speed(100); Moves(MOVE_ELECTRIC_TERRAIN, MOVE_THUNDER_SHOCK); }
        OPPONENT(SPECIES_ELECTIVIRE) { Speed(100); Moves(MOVE_THUNDER_SHOCK); }
    } WHEN {
        TURN { MOVE(player, MOVE_BLOCK); EXPECT_SWITCH(opponent, 1); }
        TURN { MOVE(player, MOVE_GRASSY_TERRAIN); EXPECT_MOVE(opponent, MOVE_THUNDER_SHOCK); }
        TURN { MOVE(player, MOVE_DRAGON_RAGE); EXPECT_SEND_OUT(opponent, 2); }
    }
}

AI_SINGLE_BATTLE_TEST("Tactica field support: restore expired terrain before the next attack")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_BASIC_TRAINER | AI_FLAG_FIELD_SUPPORT);
        PLAYER(SPECIES_BLISSEY) { HP(2000); MaxHP(2000); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_PINCURCHIN) { Ability(ABILITY_ELECTRIC_SURGE); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_RAICHU_ALOLA) { Ability(ABILITY_SURGE_SURFER); Moves(MOVE_THUNDER_SHOCK); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_SWITCH(opponent, 1); }
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_MOVE(opponent, MOVE_THUNDER_SHOCK); }
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_MOVE(opponent, MOVE_THUNDER_SHOCK); }
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_MOVE(opponent, MOVE_THUNDER_SHOCK); }
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_MOVE(opponent, MOVE_THUNDER_SHOCK); }
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_SWITCH(opponent, 0); }
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_SWITCH(opponent, 1); }
    }
}

AI_SINGLE_BATTLE_TEST("Tactica field support: no handoff into lethal known entry hazards")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_BASIC_TRAINER | AI_FLAG_FIELD_SUPPORT | AI_FLAG_POWERFUL_STATUS);
        PLAYER(SPECIES_BLISSEY) { Moves(MOVE_STEALTH_ROCK, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_NINETALES_ALOLA) { Ability(ABILITY_SNOW_WARNING); Moves(MOVE_AURORA_VEIL, MOVE_TACKLE); }
        OPPONENT(SPECIES_SANDSLASH_ALOLA) { HP(1); Ability(ABILITY_SLUSH_RUSH); Moves(MOVE_TACKLE); }
    } WHEN {
        TURN { MOVE(player, MOVE_STEALTH_ROCK); EXPECT_MOVE(opponent, MOVE_AURORA_VEIL); }
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_MOVE(opponent, MOVE_TACKLE); }
    }
}

AI_SINGLE_BATTLE_TEST("Tactica field support: Cloud Nine prevents pointless weather handoffs")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_BASIC_TRAINER | AI_FLAG_FIELD_SUPPORT);
        PLAYER(SPECIES_GOLDUCK) { Ability(ABILITY_CLOUD_NINE); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_PELIPPER) { Ability(ABILITY_DRIZZLE); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_KINGDRA) { Ability(ABILITY_SWIFT_SWIM); Moves(MOVE_TACKLE); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_MOVE(opponent, MOVE_CELEBRATE); }
    }
}
