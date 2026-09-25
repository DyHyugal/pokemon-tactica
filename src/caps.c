#include "global.h"
#include "battle.h"
#include "battle_setup.h"
#include "data.h"
#include "event_data.h"
#include "caps.h"
#include "pokemon.h"

static u8 GetCurrentBadgeCount(void)
{
    u16 i;
    u8 badgeCount = 0;
    for (i = FLAG_BADGE01_GET; i < FLAG_BADGE01_GET + NUM_BADGES; i++)
    {
        if (FlagGet(i))
            badgeCount++;
    }
    return badgeCount;
}

static const u8 sLevelCapTable_Normal[] =
{
    [0] = 11,  // no badges
    [1] = 16,  // badge 1
    [2] = 21,  // badge 2
    [3] = 25,  // badge 3
    [4] = 31,  // badge 4
    [5] = 36,  // badge 5
    [6] = 38,  // badge 6
    [7] = 45,  // badge 7
    [8] = 56,  // badge 8
};

static const u8 sLevelCapTable_Hard[] =
{
    [0] = 8,
    [1] = 15,
    [2] = 20,
    [3] = 23,
    [4] = 29,
    [5] = 33,
    [6] = 37,
    [7] = 42,
    [8] = 54,
};

#if IS_HNS
struct BossLevelCapMilestone
{
    u16 trainerIds[3];
    u16 completionFlag;
    u16 variantFlags[2];
};

#define BOSS_MILESTONE(trainerId) \
    { .trainerIds = {trainerId} }
#define BOSS_MILESTONE_FLAG(trainerId, flag) \
    { .trainerIds = {trainerId}, .completionFlag = flag }
#define BOSS_MILESTONE_VARIANTS(trainerId1, trainerId2, trainerId3) \
    { .trainerIds = {trainerId1, trainerId2, trainerId3} }
#define BOSS_MILESTONE_FLEXIBLE(trainerId1, trainerId2, trainerId3, flag, variantFlag1, variantFlag2) \
    { .trainerIds = {trainerId1, trainerId2, trainerId3}, .completionFlag = flag, .variantFlags = {variantFlag1, variantFlag2} }

// The order follows the mandatory HnS story. When the middle Johto gyms or
// Kanto gyms are completed out of order, completed milestones establish a
// floor so that the cap can never move backwards.
static const struct BossLevelCapMilestone sJohtoBossMilestones[] =
{
    BOSS_MILESTONE_FLAG(TRAINER_FALKNER_1_HNS, FLAG_DEFEATED_VIOLET_GYM),
    BOSS_MILESTONE(TRAINER_PROTON_1_HNS),
    BOSS_MILESTONE_VARIANTS(TRAINER_RIVAL_CHIKORITA_2_HNS, TRAINER_RIVAL_CYNDAQUIL_2_HNS, TRAINER_RIVAL_TOTODILE_2_HNS),
    BOSS_MILESTONE_FLAG(TRAINER_BUGSY_1_HNS, FLAG_DEFEATED_AZALEA_TOWN_GYM),
    BOSS_MILESTONE_FLAG(TRAINER_WHITNEY_1_HNS, FLAG_DEFEATED_GOLDENROD_CITY_GYM),
    BOSS_MILESTONE_VARIANTS(TRAINER_RIVAL_CHIKORITA_3_HNS, TRAINER_RIVAL_CYNDAQUIL_3_HNS, TRAINER_RIVAL_TOTODILE_3_HNS),
    BOSS_MILESTONE_FLAG(TRAINER_MORTY_1_HNS, FLAG_DEFEATED_ECRUTEAK_CITY_GYM),
    BOSS_MILESTONE(TRAINER_PETREL_1_HNS),
    BOSS_MILESTONE(TRAINER_ARIANA_1_HNS),
    BOSS_MILESTONE_FLEXIBLE(TRAINER_CHUCK_1_HNS, TRAINER_CHUCK_1_2_HNS, TRAINER_CHUCK_1_3_HNS, FLAG_DEFEATED_CIANWOOD_GYM, FLAG_DEFEATED_OLIVINE_CITY_GYM, FLAG_DEFEATED_MAHOGANY_TOWN_GYM),
    BOSS_MILESTONE_FLEXIBLE(TRAINER_PRYCE_1_HNS, TRAINER_PRYCE_1_2_HNS, TRAINER_PRYCE_1_3_HNS, FLAG_DEFEATED_MAHOGANY_TOWN_GYM, FLAG_DEFEATED_CIANWOOD_GYM, FLAG_DEFEATED_OLIVINE_CITY_GYM),
    BOSS_MILESTONE_FLEXIBLE(TRAINER_JASMINE_1_HNS, TRAINER_JASMINE_1_2_HNS, TRAINER_JASMINE_1_3_HNS, FLAG_DEFEATED_OLIVINE_CITY_GYM, FLAG_DEFEATED_CIANWOOD_GYM, FLAG_DEFEATED_MAHOGANY_TOWN_GYM),
    BOSS_MILESTONE(TRAINER_PETREL_2_HNS),
    BOSS_MILESTONE_VARIANTS(TRAINER_RIVAL_CHIKORITA_4_HNS, TRAINER_RIVAL_CYNDAQUIL_4_HNS, TRAINER_RIVAL_TOTODILE_4_HNS),
    BOSS_MILESTONE(TRAINER_PROTON_2_HNS),
    BOSS_MILESTONE(TRAINER_ARIANA_2_HNS),
    BOSS_MILESTONE(TRAINER_ARCHER_HNS),
    BOSS_MILESTONE_FLAG(TRAINER_CLAIR_1_HNS, FLAG_DEFEATED_BLACKTHORN_GYM),
    BOSS_MILESTONE_VARIANTS(TRAINER_RIVAL_CHIKORITA_5_HNS, TRAINER_RIVAL_CYNDAQUIL_5_HNS, TRAINER_RIVAL_TOTODILE_5_HNS),
    BOSS_MILESTONE(TRAINER_WILL_1_HNS),
    BOSS_MILESTONE(TRAINER_KOGA_1_HNS),
    BOSS_MILESTONE(TRAINER_BRUNO_1_HNS),
    BOSS_MILESTONE(TRAINER_KAREN_1_HNS),
    BOSS_MILESTONE(TRAINER_LANCE_1_HNS),
};

static const struct BossLevelCapMilestone sKantoBossMilestones[] =
{
    BOSS_MILESTONE_FLAG(TRAINER_LTSURGE_HNS, FLAG_DEFEATED_VERMILION_GYM),
    BOSS_MILESTONE_FLAG(TRAINER_ERIKA_HNS, FLAG_DEFEATED_CELADON_GYM),
    BOSS_MILESTONE_FLAG(TRAINER_MISTY_HNS, FLAG_DEFEATED_CERULEAN_GYM),
    BOSS_MILESTONE_VARIANTS(TRAINER_RIVAL_CHIKORITA_6_HNS, TRAINER_RIVAL_CYNDAQUIL_6_HNS, TRAINER_RIVAL_TOTODILE_6_HNS),
    BOSS_MILESTONE_FLAG(TRAINER_JANINE_HNS, FLAG_DEFEATED_FUCHSIA_GYM),
    BOSS_MILESTONE_FLAG(TRAINER_SABRINA_HNS, FLAG_DEFEATED_SAFFRON_GYM),
    BOSS_MILESTONE_FLAG(TRAINER_BROCK_HNS, FLAG_DEFEATED_PEWTER_GYM),
    BOSS_MILESTONE_FLAG(TRAINER_BLAINE_HNS, FLAG_DEFEATED_CINNABAR_ISLAND_GYM),
    BOSS_MILESTONE_FLAG(TRAINER_BLUE_HNS, FLAG_DEFEATED_VIRIDIAN_GYM),
    BOSS_MILESTONE(TRAINER_WILL_2_HNS),
    BOSS_MILESTONE(TRAINER_KOGA_2_HNS),
    BOSS_MILESTONE(TRAINER_BRUNO_2_HNS),
    BOSS_MILESTONE(TRAINER_KAREN_2_HNS),
    BOSS_MILESTONE(TRAINER_LANCE_2_HNS),
};

#if TESTING
static LevelCapTrainerLevelGetter sLevelCapTrainerLevelGetter;

void SetLevelCapTrainerLevelGetterForTesting(LevelCapTrainerLevelGetter getter)
{
    sLevelCapTrainerLevelGetter = getter;
}
#endif

static u32 GetTrainerLevelCap(u16 trainerId, bool8 useLowestLevel)
{
    const struct Trainer *trainer;
    const struct TrainerMon *party;
    u32 levelCap = useLowestLevel ? MAX_LEVEL : 0;
    u32 i;

#if TESTING
    if (sLevelCapTrainerLevelGetter != NULL)
        return sLevelCapTrainerLevelGetter(trainerId, useLowestLevel);
#endif

    trainerId = SanitizeTrainerId(trainerId);
    trainer = &gTrainers[GetTrainerDifficultyLevel(trainerId)][trainerId];
    party = trainer->party;
    if (party == NULL || trainer->partySize == 0)
        return 0;

    for (i = 0; i < trainer->partySize; i++)
    {
        if ((!useLowestLevel && party[i].lvl > levelCap)
         || (useLowestLevel && party[i].lvl < levelCap))
            levelCap = party[i].lvl;
    }

    return levelCap;
}

static u16 GetMilestoneTrainerId(const struct BossLevelCapMilestone *milestone)
{
    if (milestone->variantFlags[0] != 0)
    {
        u32 variant = FlagGet(milestone->variantFlags[0]) + FlagGet(milestone->variantFlags[1]);
        return milestone->trainerIds[variant];
    }

    return milestone->trainerIds[0];
}

static bool32 IsFamilyRocketTrainer(u16 trainerId)
{
    switch (trainerId)
    {
    case TRAINER_PROTON_1_HNS:
    case TRAINER_PROTON_2_HNS:
    case TRAINER_PETREL_1_HNS:
    case TRAINER_PETREL_2_HNS:
    case TRAINER_ARIANA_1_HNS:
    case TRAINER_ARIANA_2_HNS:
    case TRAINER_ARCHER_HNS:
        return TRUE;
    default:
        return FALSE;
    }
}

static u32 GetMilestoneLevelCap(const struct BossLevelCapMilestone *milestone, bool8 useLowestLevel)
{
    u32 levelCap = 0;
    u32 i;

    if (milestone->variantFlags[0] != 0)
        return GetTrainerLevelCap(GetMilestoneTrainerId(milestone), useLowestLevel);

    for (i = 0; i < ARRAY_COUNT(milestone->trainerIds); i++)
    {
        u32 trainerCap;

        if (milestone->trainerIds[i] == 0)
            break;
        trainerCap = GetTrainerLevelCap(milestone->trainerIds[i], useLowestLevel);
        if (trainerCap > levelCap)
            levelCap = trainerCap;
    }

    return levelCap;
}

static bool32 IsMilestoneComplete(const struct BossLevelCapMilestone *milestone)
{
    u32 i;

    if (milestone->completionFlag != 0 && FlagGet(milestone->completionFlag))
        return TRUE;

    for (i = 0; i < ARRAY_COUNT(milestone->trainerIds); i++)
    {
        if (milestone->trainerIds[i] == 0)
            break;
        if (HasTrainerBeenFought(milestone->trainerIds[i]))
            return TRUE;
    }

    return FALSE;
}

static u32 GetBossProgressionLevelCap(const struct BossLevelCapMilestone *milestones, u32 count, bool8 useLowestLevel)
{
    u32 nextBossCap = 0;
    u32 progressionFloor = 0;
    u32 progressionReference = 0;
    u32 i;

    for (i = 0; i < count; i++)
    {
        u16 trainerId = GetMilestoneTrainerId(&milestones[i]);
        u32 milestoneCap = IsFamilyRocketTrainer(trainerId)
            ? min(progressionReference + 2, MAX_LEVEL)
            : GetMilestoneLevelCap(&milestones[i], useLowestLevel);

        if (!IsFamilyRocketTrainer(trainerId))
            progressionReference = max(progressionReference, milestoneCap);
        if (IsMilestoneComplete(&milestones[i]))
        {
            if (milestoneCap > progressionFloor)
                progressionFloor = milestoneCap;
        }
        else if (nextBossCap == 0)
        {
            nextBossCap = milestoneCap;
        }
    }

    return max(nextBossCap, progressionFloor);
}

u32 GetFamilyRocketTrainerLevel(u16 trainerId)
{
    bool8 useLowestLevel = gSaveBlock3Ptr->challengeSettings.tx_Challenges_LevelCap == 2;
    u32 progressionReference = 0;
    u32 i;

    if (!IsFamilyRocketTrainer(trainerId))
        return 0;

    for (i = 0; i < ARRAY_COUNT(sJohtoBossMilestones); i++)
    {
        u16 milestoneTrainerId = GetMilestoneTrainerId(&sJohtoBossMilestones[i]);
        u32 milestoneCap = IsFamilyRocketTrainer(milestoneTrainerId)
            ? min(progressionReference + 2, MAX_LEVEL)
            : GetMilestoneLevelCap(&sJohtoBossMilestones[i], useLowestLevel);

        if (!IsFamilyRocketTrainer(milestoneTrainerId))
            progressionReference = max(progressionReference, milestoneCap);
        if (milestoneTrainerId == trainerId)
            return milestoneCap;
    }

    return 0;
}

u16 GetFamilyRocketLegalSpecies(u16 species, u32 level)
{
    switch (species)
    {
    case SPECIES_CROBAT:
        if (level < 22)
            return SPECIES_ZUBAT;
        if (level < 23)
            return SPECIES_GOLBAT;
        break;
    case SPECIES_WEEZING:
        if (level < 35)
            return SPECIES_KOFFING;
        break;
    case SPECIES_RATICATE:
        if (level < 20)
            return SPECIES_RATTATA;
        break;
    case SPECIES_SCOLIPEDE:
        if (level < 22)
            return SPECIES_VENIPEDE;
        if (level < 30)
            return SPECIES_WHIRLIPEDE;
        break;
    case SPECIES_TOXICROAK:
        if (level < 37)
            return SPECIES_CROAGUNK;
        break;
    case SPECIES_MUK_ALOLA:
        if (level < 38)
            return SPECIES_GRIMER_ALOLA;
        break;
    case SPECIES_ELECTRODE:
        if (level < 30)
            return SPECIES_VOLTORB;
        break;
    case SPECIES_MUK:
        if (level < 38)
            return SPECIES_GRIMER;
        break;
    case SPECIES_ZOROARK_HISUI:
        if (level < 30)
            return SPECIES_ZORUA_HISUI;
        break;
    }

    return species;
}

static u32 GetHnsLevelCap(u8 challengeLevelCap)
{
    bool8 useLowestLevel = challengeLevelCap == 2;

    if (!FlagGet(FLAG_IS_CHAMPION))
        return GetBossProgressionLevelCap(sJohtoBossMilestones, ARRAY_COUNT(sJohtoBossMilestones), useLowestLevel);

    if (!FlagGet(FLAG_IS_KANTO_CHAMPION))
        return GetBossProgressionLevelCap(sKantoBossMilestones, ARRAY_COUNT(sKantoBossMilestones), useLowestLevel);

    if (!FlagGet(FLAG_DEFEATED_RED))
        return GetTrainerLevelCap(TRAINER_RED_HNS, useLowestLevel);

    return MAX_LEVEL;
}
#endif

u32 GetCurrentLevelCap(void)
{
    static const u32 sLevelCapFlagMap[][2] =
    {
        {FLAG_BADGE01_GET, 15},
        {FLAG_BADGE02_GET, 19},
        {FLAG_BADGE03_GET, 24},
        {FLAG_BADGE04_GET, 29},
        {FLAG_BADGE05_GET, 31},
        {FLAG_BADGE06_GET, 33},
        {FLAG_BADGE07_GET, 42},
        {FLAG_BADGE08_GET, 46},
        {FLAG_IS_CHAMPION, 58},
    };

    u8 challengeLevelCap = gSaveBlock3Ptr->challengeSettings.tx_Challenges_LevelCap;

    if (challengeLevelCap != 0)
    {
        u8 badgeCount = GetCurrentBadgeCount();
        if (badgeCount > 8)
            badgeCount = 8;

#if IS_HNS
        return GetHnsLevelCap(challengeLevelCap);
#else
        if (FlagGet(FLAG_IS_CHAMPION))
            return MAX_LEVEL;
#endif

        if (challengeLevelCap == 1)
            return sLevelCapTable_Normal[badgeCount];
        else
            return sLevelCapTable_Hard[badgeCount];
    }

    u32 i;

    if (B_LEVEL_CAP_TYPE == LEVEL_CAP_FLAG_LIST)
    {
        for (i = 0; i < ARRAY_COUNT(sLevelCapFlagMap); i++)
        {
            if (!FlagGet(sLevelCapFlagMap[i][0]))
                return sLevelCapFlagMap[i][1];
        }
    }
    else if (B_LEVEL_CAP_TYPE == LEVEL_CAP_VARIABLE)
    {
        return VarGet(B_LEVEL_CAP_VARIABLE);
    }

    return MAX_LEVEL;
}

u32 GetSoftLevelCapExpValue(u32 level, u32 expValue)
{
    static const u32 sExpScalingDown[5] = { 4, 8, 16, 32, 64 };
    static const u32 sExpScalingUp[5]   = { 16, 8, 4, 2, 1 };

    u32 levelDifference;
    u32 currentLevelCap = GetCurrentLevelCap();

    if (B_EXP_CAP_TYPE == EXP_CAP_NONE)
        return expValue;

    if (level < currentLevelCap)
    {
        if (B_LEVEL_CAP_EXP_UP)
        {
            levelDifference = currentLevelCap - level;
            if (levelDifference > ARRAY_COUNT(sExpScalingUp) - 1)
                return expValue + (expValue / sExpScalingUp[ARRAY_COUNT(sExpScalingUp) - 1]);
            else
                return expValue + (expValue / sExpScalingUp[levelDifference]);
        }
        else
        {
            return expValue;
        }
    }
    else if (B_EXP_CAP_TYPE == EXP_CAP_HARD)
    {
        return 0;
    }
    else if (B_EXP_CAP_TYPE == EXP_CAP_SOFT)
    {
        levelDifference = level - currentLevelCap;
        if (levelDifference > ARRAY_COUNT(sExpScalingDown) - 1)
            return expValue / sExpScalingDown[ARRAY_COUNT(sExpScalingDown) - 1];
        else
            return expValue / sExpScalingDown[levelDifference];
    }
    else
    {
       return expValue;
    }
}

u32 GetCurrentEVCap(void)
{
    static const u16 sEvCapFlagMap[][2] = {
        // Define EV caps for each milestone
        {FLAG_BADGE01_GET, MAX_TOTAL_EVS *  1 / 17},
        {FLAG_BADGE02_GET, MAX_TOTAL_EVS *  3 / 17},
        {FLAG_BADGE03_GET, MAX_TOTAL_EVS *  5 / 17},
        {FLAG_BADGE04_GET, MAX_TOTAL_EVS *  7 / 17},
        {FLAG_BADGE05_GET, MAX_TOTAL_EVS *  9 / 17},
        {FLAG_BADGE06_GET, MAX_TOTAL_EVS * 11 / 17},
        {FLAG_BADGE07_GET, MAX_TOTAL_EVS * 13 / 17},
        {FLAG_BADGE08_GET, MAX_TOTAL_EVS * 15 / 17},
        {FLAG_IS_CHAMPION, MAX_TOTAL_EVS},
    };

    if (B_EV_CAP_TYPE == EV_CAP_FLAG_LIST)
    {
        for (u32 evCap = 0; evCap < ARRAY_COUNT(sEvCapFlagMap); evCap++)
        {
            if (!FlagGet(sEvCapFlagMap[evCap][0]))
                return sEvCapFlagMap[evCap][1];
        }
    }
    else if (B_EV_CAP_TYPE == EV_CAP_VARIABLE)
    {
        return VarGet(B_EV_CAP_VARIABLE);
    }
    else if (B_EV_CAP_TYPE == EV_CAP_NO_GAIN)
    {
        return 0;
    }

    return MAX_TOTAL_EVS;
}
