#include "global.h"
#include "battle_setup.h"
#include "battle_main.h"
#include "battle.h"
#include "caps.h"
#include "data.h"
#include "difficulty.h"
#include "event_data.h"
#include "pokemon.h"
#include "tactica_progression.h"
#include "constants/maps.h"
#include "test/test.h"

#if IS_HNS
TEST("Tactica ordinary trainers: opening rival victory establishes the completed level floor")
{
    u32 rival = TRAINER_RIVAL_CHIKORITA_1_HNS;
    PARAMETRIZE { rival = TRAINER_RIVAL_CHIKORITA_1_HNS; }
    PARAMETRIZE { rival = TRAINER_RIVAL_CYNDAQUIL_1_HNS; }
    PARAMETRIZE { rival = TRAINER_RIVAL_TOTODILE_1_HNS; }
    InitEventData();
    EXPECT_EQ(GetTacticaStoryTrainerFloor(), 0);
    SetTrainerFlag(rival);
    EXPECT_EQ(GetTacticaStoryTrainerFloor(), 18);
    gSaveBlock3Ptr->challengeSettings.tx_Challenges_LevelCap = 1;
    EXPECT_EQ(GetCurrentLevelCap(), 18);
    gSaveBlock3Ptr->challengeSettings.tx_Challenges_LevelCap = 2;
    EXPECT_EQ(GetCurrentLevelCap(), 14);
    gSaveBlock3Ptr->challengeSettings.tx_Challenges_LevelCap = 0;
    // Lower-level Gym non-aces cannot undo a completed rival's floor.
    FlagSet(FLAG_DEFEATED_VIOLET_GYM);
    EXPECT_EQ(GetTacticaStoryTrainerFloor(), 18);
}

TEST("Tactica ordinary trainers: each central Kanto Gym victory raises the floor before the full group is complete")
{
    u32 trainer = TRAINER_SABRINA_HNS;
    PARAMETRIZE { trainer = TRAINER_SABRINA_HNS; }
    PARAMETRIZE { trainer = TRAINER_ERIKA_HNS; }
    PARAMETRIZE { trainer = TRAINER_JANINE_HNS; }
    InitEventData();
    FlagSet(FLAG_IS_CHAMPION);
    FlagSet(FLAG_DEFEATED_VERMILION_GYM);
    SetTrainerFlag(trainer);
    EXPECT_EQ(GetTacticaStoryTrainerFloor(), 80);
    gSaveBlock3Ptr->challengeSettings.tx_Challenges_LevelCap = 1;
    EXPECT_EQ(GetCurrentLevelCap(), 80);
    SetTrainerFlag(TRAINER_SABRINA_HNS);
    SetTrainerFlag(TRAINER_ERIKA_HNS);
    SetTrainerFlag(TRAINER_JANINE_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 85);
    gSaveBlock3Ptr->challengeSettings.tx_Challenges_LevelCap = 0;
}

// Independent expected levels: these literals describe approved outcomes,
// not a second call to the scaling helper or its generated table.
TEST("Tactica ordinary trainers: real party levels match independent route cave interior and Kanto examples")
{
    static const struct {
        u16 trainer, map;
        u8 state, count, levels[PARTY_SIZE];
    } cases[] = {
        {TRAINER_WADE_HNS, MAP_ROUTE31_HNS, 0, 2, {7, 8}},
        {TRAINER_RUSSELL_HNS, MAP_UNION_CAVE_1F_HNS, 0, 2, {17, 16}},
        {TRAINER_RUSSELL_HNS, MAP_UNION_CAVE_1F_HNS, 1, 2, {19, 18}},
        {TRAINER_JOEY_HNS, MAP_ROUTE30_HNS, 1, 1, {18}},
        {TRAINER_WADE_HNS, MAP_ROUTE31_HNS, 1, 2, {18, 18}},
        {TRAINER_SAMUEL_HNS, MAP_ROUTE34_HNS, 2, 3, {29, 29, 29}},
        {TRAINER_CARRIE_HNS, MAP_GOLDENROD_CITY_GYM_HNS, 2, 1, {29}},
        {TRAINER_STEPHEN_HNS, MAP_ROUTE12_HNS, 3, 4, {75, 75, 75, 75}},
        {TRAINER_STEPHEN_HNS, MAP_ROUTE12_HNS, 4, 4, {80, 80, 80, 80}},
        {TRAINER_WADE_HNS, MAP_ROUTE31_HNS, 5, 2, {100, 100}},
        {TRAINER_JOEY_5_HNS, MAP_ROUTE30_HNS, 1, 1, {65}},
    };
    u32 index = 0, difficulty = DIFFICULTY_NORMAL, cap = 0;
    for (u32 i = 0; i < ARRAY_COUNT(cases); i++)
        for (u32 d = DIFFICULTY_NORMAL; d <= DIFFICULTY_HARD; d++)
            for (u32 c = 0; c < 3; c++)
                PARAMETRIZE { index = i; difficulty = d; cap = c; }
    u32 savedDifficulty = GetCurrentDifficultyLevel();
    u32 savedFlags = gBattleTypeFlags;
    InitEventData();
    memset(&gSaveBlock3Ptr->challengeSettings, 0, sizeof(struct ChallengeSettings));
    gSaveBlock3Ptr->challengeSettings.tx_Challenges_OneTypeChallenge = 31;
    gSaveBlock3Ptr->challengeSettings.tx_Mode_Modern_Moves = 1;
    gSaveBlock3Ptr->challengeSettings.tx_Challenges_LevelCap = cap;
    SetCurrentDifficultyLevel(difficulty);
    if (cases[index].state >= 1)
        SetTrainerFlag(TRAINER_RIVAL_CYNDAQUIL_1_HNS);
    if (cases[index].state == 2)
        SetTrainerFlag(TRAINER_RIVAL_CYNDAQUIL_2_HNS);
    if (cases[index].state == 3 || cases[index].state == 4)
    {
        FlagSet(FLAG_IS_CHAMPION);
        FlagSet(FLAG_DEFEATED_VERMILION_GYM);
    }
    if (cases[index].state == 4)
        SetTrainerFlag(TRAINER_SABRINA_HNS);
    if (cases[index].state == 5)
        FlagSet(FLAG_IS_KANTO_CHAMPION);
    gSaveBlock1Ptr->location.mapGroup = MAP_GROUP(cases[index].map);
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(cases[index].map);
    gBattleTypeFlags = BATTLE_TYPE_TRAINER;
    EXPECT_EQ(CreateNPCTrainerParty(gEnemyParty, cases[index].trainer, TRUE), cases[index].count);
    for (u32 slot = 0; slot < cases[index].count; slot++)
        EXPECT_EQ(GetMonData(&gEnemyParty[slot], MON_DATA_LEVEL), cases[index].levels[slot]);
    gBattleTypeFlags = savedFlags;
    SetCurrentDifficultyLevel(savedDifficulty);
}

static const struct ScalingCase
{
    u16 map;
    u16 trainer;
    u8 zoneFloor;
    u8 zoneCeiling;
} sScalingCases[] = {
#include "tactica_trainer_scaling_cases.h"
};

static void CheckMappedParties(u32 chunk, bool32 exhaustive)
{
    u32 caseId = 0, difficulty = DIFFICULTY_NORMAL, story = 0;
    static const u8 floors[] = {0, 18, 29, MAX_LEVEL};
    for (u32 i = chunk; i < ARRAY_COUNT(sScalingCases); i += 4)
    {
        if (exhaustive)
        {
            for (u32 d = DIFFICULTY_NORMAL; d <= DIFFICULTY_HARD; d++)
                for (u32 stage = 0; stage < ARRAY_COUNT(floors); stage++)
                    PARAMETRIZE { caseId = i; difficulty = d; story = stage; }
        }
        else
        {
            // Each placed team remains exercised; vary context across teams.
            PARAMETRIZE { caseId = i; difficulty = DIFFICULTY_NORMAL + i % 2; story = i / 2 % ARRAY_COUNT(floors); }
        }
    }
    const struct ScalingCase *area = &sScalingCases[caseId];
    u32 savedDifficulty = GetCurrentDifficultyLevel();
    u32 savedBattleFlags = gBattleTypeFlags;
    InitEventData();
    memset(&gSaveBlock3Ptr->challengeSettings, 0, sizeof(struct ChallengeSettings));
    gSaveBlock3Ptr->challengeSettings.tx_Challenges_OneTypeChallenge = 31;
    gSaveBlock3Ptr->challengeSettings.tx_Mode_Modern_Moves = 1;
    gSaveBlock3Ptr->challengeSettings.tx_Challenges_LevelCap = (story + difficulty) % 3;
    SetCurrentDifficultyLevel(difficulty);
    if (story >= 1)
        SetTrainerFlag(TRAINER_RIVAL_CYNDAQUIL_1_HNS);
    if (story >= 2)
        FlagSet(FLAG_DEFEATED_GOLDENROD_CITY_GYM);
    if (story >= 3)
        FlagSet(FLAG_IS_KANTO_CHAMPION);
    gSaveBlock1Ptr->location.mapGroup = MAP_GROUP(area->map);
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(area->map);
    gBattleTypeFlags = BATTLE_TYPE_TRAINER;
    const struct Trainer *trainer = GetTrainerStructFromId(area->trainer);
    EXPECT_GT(trainer->partySize, 0);
    EXPECT_LE(trainer->partySize, PARTY_SIZE);
    EXPECT_EQ(GetTacticaStoryTrainerFloor(), floors[story]);
    EXPECT_EQ(CreateNPCTrainerParty(gEnemyParty, area->trainer, TRUE), trainer->partySize);
    for (u32 slot = 0; slot < trainer->partySize; slot++)
    {
        u32 actual = GetMonData(&gEnemyParty[slot], MON_DATA_LEVEL);
        u32 minimum = max(area->zoneFloor, floors[story]);
        u32 maximum = max(area->zoneCeiling, floors[story]);
        if (actual < minimum || actual < trainer->party[slot].lvl || actual > max(maximum, trainer->party[slot].lvl))
            Test_MgbaPrintf("Scaling mismatch: map=%d trainer=%d difficulty=%d stage=%d slot=%d level=%d floor=%d ceiling=%d", area->map, area->trainer, difficulty, story, slot, actual, minimum, maximum);
        EXPECT_GE(actual, minimum);
        EXPECT_GE(actual, trainer->party[slot].lvl);
        EXPECT_LE(actual, min(MAX_LEVEL, max(maximum, trainer->party[slot].lvl)));
    }
    SetCurrentDifficultyLevel(savedDifficulty);
    gBattleTypeFlags = savedBattleFlags;
}
TEST("Tactica ordinary trainers: all mapped parties chunk 1/4") { CheckMappedParties(0, FALSE); }
TEST("Tactica ordinary trainers: all mapped parties chunk 2/4") { CheckMappedParties(1, FALSE); }
TEST("Tactica ordinary trainers: all mapped parties chunk 3/4") { CheckMappedParties(2, FALSE); }
TEST("Tactica ordinary trainers: all mapped parties chunk 4/4") { CheckMappedParties(3, FALSE); }
// Explicit diagnostic audit, excluded from regular CI file selection.
TEST("Tactica exhaustive trainer audit: chunk 1/4") { CheckMappedParties(0, TRUE); }
TEST("Tactica exhaustive trainer audit: chunk 2/4") { CheckMappedParties(1, TRUE); }
TEST("Tactica exhaustive trainer audit: chunk 3/4") { CheckMappedParties(2, TRUE); }
TEST("Tactica exhaustive trainer audit: chunk 4/4") { CheckMappedParties(3, TRUE); }
#endif
