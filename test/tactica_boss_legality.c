#include "global.h"
#include "battle.h"
#include "battle_main.h"
#include "data.h"
#include "event_data.h"
#include "difficulty.h"
#include "caps.h"
#include "item.h"
#include "pokemon.h"
#include "tactica_progression.h"
#include "test/test.h"
#include "constants/hold_effects.h"
#include "constants/opponents.h"
#include "constants/flags.h"
#include "constants/vars.h"
#include "family_starter.h"

#if IS_HNS
TEST("Tactica boss legality: both Red encounters create the approved six level-100 sets")
{
    static const struct {
        u16 species, item, ability;
        u8 nature;
        u16 moves[MAX_MON_MOVES];
        u8 hardEVs[NUM_STATS]; // HP, Atk, Def, SpA, SpD, Speed.
    } expected[] = {
        {SPECIES_PIKACHU_STARTER, ITEM_LIGHT_BALL, ABILITY_LIGHTNING_ROD, NATURE_NAIVE,
         {MOVE_VOLT_TACKLE, MOVE_SURF, MOVE_IRON_TAIL, MOVE_FAKE_OUT}, {0, 252, 0, 4, 0, 252}},
        {SPECIES_LUCARIO, ITEM_LUCARIONITE, ABILITY_JUSTIFIED, NATURE_JOLLY,
         {MOVE_SWORDS_DANCE, MOVE_CLOSE_COMBAT, MOVE_METEOR_MASH, MOVE_BULLET_PUNCH}, {0, 252, 0, 0, 4, 252}},
        {SPECIES_SERPERIOR, ITEM_LEFTOVERS, ABILITY_CONTRARY, NATURE_TIMID,
         {MOVE_LEAF_STORM, MOVE_DRAGON_PULSE, MOVE_GLARE, MOVE_SUBSTITUTE}, {4, 0, 0, 252, 0, 252}},
        {SPECIES_DRACOVISH, ITEM_CHOICE_SCARF, ABILITY_STRONG_JAW, NATURE_JOLLY,
         {MOVE_FISHIOUS_REND, MOVE_PSYCHIC_FANGS, MOVE_CRUNCH, MOVE_DRAGON_CLAW}, {0, 252, 0, 0, 4, 252}},
        {SPECIES_GLISCOR, ITEM_TOXIC_ORB, ABILITY_POISON_HEAL, NATURE_IMPISH,
         {MOVE_EARTHQUAKE, MOVE_ROOST, MOVE_STEALTH_ROCK, MOVE_KNOCK_OFF}, {252, 0, 252, 0, 4, 0}},
        {SPECIES_ROTOM_HEAT, ITEM_HEAVY_DUTY_BOOTS, ABILITY_LEVITATE, NATURE_BOLD,
         {MOVE_OVERHEAT, MOVE_VOLT_SWITCH, MOVE_WILL_O_WISP, MOVE_DEFOG}, {248, 0, 168, 0, 0, 92}},
    };
    static const u8 evFields[] = {MON_DATA_HP_EV, MON_DATA_ATK_EV, MON_DATA_DEF_EV,
                                  MON_DATA_SPATK_EV, MON_DATA_SPDEF_EV, MON_DATA_SPEED_EV};
    u32 trainer = TRAINER_RED_HNS, difficulty = DIFFICULTY_NORMAL;
    PARAMETRIZE { trainer = TRAINER_RED_HNS; difficulty = DIFFICULTY_NORMAL; }
    PARAMETRIZE { trainer = TRAINER_RED_HNS; difficulty = DIFFICULTY_HARD; }
    PARAMETRIZE { trainer = TRAINER_RED_POSTOBC_HNS; difficulty = DIFFICULTY_NORMAL; }
    PARAMETRIZE { trainer = TRAINER_RED_POSTOBC_HNS; difficulty = DIFFICULTY_HARD; }
    u32 savedDifficulty = GetCurrentDifficultyLevel();
    u32 savedBattleFlags = gBattleTypeFlags;
    InitEventData();
    memset(&gSaveBlock3Ptr->challengeSettings, 0, sizeof(struct ChallengeSettings));
    gSaveBlock3Ptr->challengeSettings.tx_Challenges_OneTypeChallenge = 31;
    gSaveBlock3Ptr->challengeSettings.tx_Mode_Modern_Moves = 1;
    SetCurrentDifficultyLevel(difficulty);
    gBattleTypeFlags = BATTLE_TYPE_TRAINER;
    EXPECT_EQ(CreateNPCTrainerParty(gEnemyParty, trainer, TRUE), PARTY_SIZE);
    for (u32 slot = 0; slot < PARTY_SIZE; slot++)
    {
        struct Pokemon *mon = &gEnemyParty[slot];
        EXPECT_EQ(GetMonData(mon, MON_DATA_SPECIES), expected[slot].species);
        EXPECT_EQ(GetMonData(mon, MON_DATA_LEVEL), 100);
        EXPECT_EQ(GetMonData(mon, MON_DATA_HELD_ITEM), expected[slot].item);
        EXPECT_EQ(GetMonAbility(mon), expected[slot].ability);
        EXPECT_EQ(GetNature(mon), expected[slot].nature);
        for (u32 move = 0; move < MAX_MON_MOVES; move++)
            EXPECT_EQ(GetMonData(mon, MON_DATA_MOVE1 + move), expected[slot].moves[move]);
        for (u32 stat = 0; stat < NUM_STATS; stat++)
            EXPECT_EQ(GetMonData(mon, evFields[stat]), difficulty == DIFFICULTY_HARD ? expected[slot].hardEVs[stat] : 0);
    }
    SetCurrentDifficultyLevel(savedDifficulty);
    gBattleTypeFlags = savedBattleFlags;
}

TEST("Tactica boss legality: Major Bob party creation is safe in Normal and Hard")
{
    u32 difficulty;
    PARAMETRIZE { difficulty = DIFFICULTY_NORMAL; }
    PARAMETRIZE { difficulty = DIFFICULTY_HARD; }
    u32 savedDifficulty = GetCurrentDifficultyLevel();
    u32 savedBattleFlags = gBattleTypeFlags;
    InitEventData();
    SetCurrentDifficultyLevel(difficulty);
    gBattleTypeFlags = BATTLE_TYPE_TRAINER;
    memset(&gSaveBlock3Ptr->challengeSettings, 0, sizeof(struct ChallengeSettings));
    gSaveBlock3Ptr->challengeSettings.tx_Challenges_OneTypeChallenge = 31;
    EXPECT_EQ(CreateNPCTrainerParty(gEnemyParty, TRAINER_LTSURGE_HNS, TRUE), 6);
    EXPECT_EQ(GetMonData(&gEnemyParty[5], MON_DATA_SPECIES), SPECIES_MANECTRIC);
    EXPECT_EQ(GetMonAbility(&gEnemyParty[5]), ABILITY_LIGHTNING_ROD);
    EXPECT_EQ(GetMonData(&gEnemyParty[5], MON_DATA_MOVE4), MOVE_SIGNAL_BEAM);
    EXPECT_EQ(GetMonData(&gEnemyParty[1], MON_DATA_SPECIES), SPECIES_RAICHU);
    EXPECT_EQ(GetMonData(&gEnemyParty[1], MON_DATA_HELD_ITEM), ITEM_RAICHUNITE_Y);
    SetCurrentDifficultyLevel(savedDifficulty);
    gBattleTypeFlags = savedBattleFlags;
}

TEST("Tactica boss legality: dynamic Rocket parties keep legal abilities and adapted moves")
{
    static const u16 trainers[] = {TRAINER_PROTON_1_HNS, TRAINER_PROTON_2_HNS, TRAINER_PETREL_1_HNS, TRAINER_PETREL_2_HNS, TRAINER_ARIANA_1_HNS, TRAINER_ARIANA_2_HNS, TRAINER_ARCHER_HNS};
    static const u16 flags[] = {FLAG_DEFEATED_VIOLET_GYM, FLAG_DEFEATED_AZALEA_TOWN_GYM, FLAG_DEFEATED_GOLDENROD_CITY_GYM, FLAG_DEFEATED_ECRUTEAK_CITY_GYM, FLAG_DEFEATED_CIANWOOD_GYM, FLAG_DEFEATED_OLIVINE_CITY_GYM, FLAG_DEFEATED_MAHOGANY_TOWN_GYM, FLAG_DEFEATED_BLACKTHORN_GYM};
    // Canonical non-ace / ace levels after each completed Gym, including no badge.
    static const u8 lows[] = {2, 16, 24, 31, 37, 44, 50, 57, 63};
    static const u8 highs[] = {2, 20, 27, 34, 40, 47, 54, 59, 66};
    u32 difficulty = DIFFICULTY_NORMAL, stage = 0, trainerId = TRAINER_PROTON_1_HNS;
    for (u32 d = DIFFICULTY_NORMAL; d <= DIFFICULTY_HARD; d++)
        for (u32 f = 0; f <= ARRAY_COUNT(flags); f++)
            for (u32 t = 0; t < ARRAY_COUNT(trainers); t++)
                PARAMETRIZE { difficulty = d; stage = f; trainerId = trainers[t]; }
    u32 savedDifficulty = GetCurrentDifficultyLevel();
    u32 savedBattleFlags = gBattleTypeFlags;
    InitEventData();
    gBattleTypeFlags = BATTLE_TYPE_TRAINER;
    memset(&gSaveBlock3Ptr->challengeSettings, 0, sizeof(struct ChallengeSettings));
    gSaveBlock3Ptr->challengeSettings.tx_Challenges_OneTypeChallenge = 31;
    gSaveBlock3Ptr->challengeSettings.tx_Mode_Modern_Moves = 1;
    SetCurrentDifficultyLevel(difficulty);
    for (u32 f = 0; f < stage; f++)
        FlagSet(flags[f]);
    const struct Trainer *trainer = &gTrainers[GetTrainerDifficultyLevel(trainerId)][trainerId];
    u32 count = CreateNPCTrainerParty(gEnemyParty, trainerId, TRUE);
    EXPECT_GT(count, 0);
    EXPECT_EQ(count, trainer->partySize);
    for (u32 slot = 0; slot < count; slot++)
    {
        u32 species = GetMonData(&gEnemyParty[slot], MON_DATA_SPECIES);
        u32 ability = GetMonAbility(&gEnemyParty[slot]);
        u32 level = GetMonData(&gEnemyParty[slot], MON_DATA_LEVEL);
        bool32 legal = FALSE;
        for (u32 a = 0; a < NUM_ABILITY_SLOTS; a++)
            legal |= gSpeciesInfo[species].abilities[a] == ability;
        EXPECT(legal);
        EXPECT_EQ(level, slot == count - 1 ? highs[stage] : lows[stage]);
        for (u32 move = 0; move < MAX_MON_MOVES; move++)
        {
            u32 moveId = GetMonData(&gEnemyParty[slot], MON_DATA_MOVE1 + move);
            if (moveId != MOVE_NONE)
                EXPECT(IsTacticaMoveLegalAtLevel(species, level, moveId));
        }
    }
    SetCurrentDifficultyLevel(savedDifficulty);
    gBattleTypeFlags = savedBattleFlags;
}
#endif
