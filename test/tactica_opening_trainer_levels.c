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

TEST("Tactica ordinary trainers: completed floors follow the whole Johto Kanto and League journey")
{
    static const struct
    {
        u16 trainer;
        u16 flag;
        bool8 rocket;
    } journey[] = {
        {TRAINER_RIVAL_CYNDAQUIL_1_HNS},
        {TRAINER_FALKNER_1_HNS, FLAG_DEFEATED_VIOLET_GYM},
        {TRAINER_PROTON_1_HNS, 0, TRUE},
        {TRAINER_BUGSY_1_HNS, FLAG_DEFEATED_AZALEA_TOWN_GYM},
        {TRAINER_RIVAL_CYNDAQUIL_2_HNS},
        {TRAINER_WHITNEY_1_HNS, FLAG_DEFEATED_GOLDENROD_CITY_GYM},
        {TRAINER_RIVAL_CYNDAQUIL_3_HNS},
        {TRAINER_MORTY_1_HNS, FLAG_DEFEATED_ECRUTEAK_CITY_GYM},
        {TRAINER_CHUCK_1_HNS, FLAG_DEFEATED_CIANWOOD_GYM},
        {TRAINER_JASMINE_1_HNS, FLAG_DEFEATED_OLIVINE_CITY_GYM},
        {TRAINER_PETREL_1_HNS, 0, TRUE},
        {TRAINER_ARIANA_1_HNS, 0, TRUE},
        {TRAINER_PRYCE_1_HNS, FLAG_DEFEATED_MAHOGANY_TOWN_GYM},
        {TRAINER_PETREL_2_HNS, 0, TRUE},
        {TRAINER_RIVAL_CYNDAQUIL_4_HNS},
        {TRAINER_PROTON_2_HNS, 0, TRUE},
        {TRAINER_ARIANA_2_HNS, 0, TRUE},
        {TRAINER_ARCHER_HNS, 0, TRUE},
        {TRAINER_CLAIR_1_HNS, FLAG_DEFEATED_BLACKTHORN_GYM},
        {TRAINER_RIVAL_CYNDAQUIL_5_HNS},
        {TRAINER_WILL_1_HNS}, {TRAINER_KOGA_1_HNS}, {TRAINER_BRUNO_1_HNS}, {TRAINER_KAREN_1_HNS},
        {TRAINER_LANCE_1_HNS, FLAG_IS_CHAMPION},
        {TRAINER_LTSURGE_HNS, FLAG_DEFEATED_VERMILION_GYM},
        {TRAINER_SABRINA_HNS}, {TRAINER_ERIKA_HNS}, {TRAINER_JANINE_HNS},
        {TRAINER_MISTY_HNS, FLAG_DEFEATED_CERULEAN_GYM},
        {TRAINER_BROCK_HNS, FLAG_DEFEATED_PEWTER_GYM},
        {TRAINER_RIVAL_CYNDAQUIL_6_HNS},
        {TRAINER_BLAINE_HNS, FLAG_DEFEATED_CINNABAR_ISLAND_GYM},
        {TRAINER_BLUE_HNS, FLAG_DEFEATED_VIRIDIAN_GYM},
        {TRAINER_WILL_2_HNS}, {TRAINER_KOGA_2_HNS}, {TRAINER_BRUNO_2_HNS}, {TRAINER_KAREN_2_HNS},
        {TRAINER_LANCE_2_HNS, FLAG_IS_KANTO_CHAMPION},
        {TRAINER_RIVAL_CYNDAQUIL_7_HNS}, {TRAINER_RED_HNS},
    };
    u32 difficulty = DIFFICULTY_NORMAL;
    PARAMETRIZE { difficulty = DIFFICULTY_NORMAL; }
    PARAMETRIZE { difficulty = DIFFICULTY_HARD; }
    u32 savedDifficulty = GetCurrentDifficultyLevel();
    InitEventData();
    SetCurrentDifficultyLevel(difficulty);
    u32 expected = 0;
    for (u32 i = 0; i < ARRAY_COUNT(journey); i++)
    {
        SetTrainerFlag(journey[i].trainer);
        if (journey[i].flag != 0)
            FlagSet(journey[i].flag);
        if (!journey[i].rocket)
        {
            const struct Trainer *trainer = GetTrainerStructFromId(journey[i].trainer);
            u32 minimum = MAX_LEVEL;
            for (u32 slot = 0; slot < trainer->partySize; slot++)
                minimum = min(minimum, trainer->party[slot].lvl);
            expected = max(expected, minimum);
        }
        EXPECT_EQ(GetTacticaStoryTrainerFloor(), expected);
    }
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

static void CheckMappedParties(u32 chunk)
{
    u32 caseId = 0, difficulty = DIFFICULTY_NORMAL, story = 0;
    static const u8 floors[] = {0, 18, 29, MAX_LEVEL};
    for (u32 i = chunk; i < ARRAY_COUNT(sScalingCases); i += 4)
        for (u32 d = DIFFICULTY_NORMAL; d <= DIFFICULTY_HARD; d++)
            for (u32 stage = 0; stage < ARRAY_COUNT(floors); stage++)
                PARAMETRIZE { caseId = i; difficulty = d; story = stage; }
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
TEST("Tactica ordinary trainers: all mapped parties chunk 1/4") { CheckMappedParties(0); }
TEST("Tactica ordinary trainers: all mapped parties chunk 2/4") { CheckMappedParties(1); }
TEST("Tactica ordinary trainers: all mapped parties chunk 3/4") { CheckMappedParties(2); }
TEST("Tactica ordinary trainers: all mapped parties chunk 4/4") { CheckMappedParties(3); }
#endif
