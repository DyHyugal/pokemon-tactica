#include "global.h"
#include "training_npc.h"
#include "caps.h"
#include "event_data.h"
#include "malloc.h"
#include "money.h"
#include "party_menu.h"
#include "script.h"
#include "script_menu.h"
#include "string_util.h"
#include "constants/pokemon.h"

static const s32 sIVFields[] =
{
    MON_DATA_HP_IV,
    MON_DATA_ATK_IV,
    MON_DATA_DEF_IV,
    MON_DATA_SPATK_IV,
    MON_DATA_SPDEF_IV,
    MON_DATA_SPEED_IV,
};

static const s32 sEVFields[] =
{
    MON_DATA_HP_EV,
    MON_DATA_ATK_EV,
    MON_DATA_DEF_EV,
    MON_DATA_SPATK_EV,
    MON_DATA_SPDEF_EV,
    MON_DATA_SPEED_EV,
};

static bool32 IsSelectedMonValid(void)
{
    if (gSpecialVar_0x8004 >= gPlayerPartyCount)
        return FALSE;
    return GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_SPECIES) != SPECIES_EGG;
}

static void PushChoice(const u8 *text, u32 id)
{
    struct ListMenuItem item;
    u8 *name = Alloc(StringLength(text) + 1);

    if (name == NULL)
        return;
    StringCopy(name, text);
    item.name = name;
    item.id = id;
    MultichoiceDynamic_PushElement(item);
}

static u32 GetTotalEVs(struct Pokemon *mon)
{
    u32 total = 0;
    u32 i;

    for (i = 0; i < ARRAY_COUNT(sEVFields); i++)
        total += GetMonData(mon, sEVFields[i]);
    return total;
}

enum TrainingResult TrainingNpc_MaxIV(struct Pokemon *mon, u32 stat)
{
    u32 value = MAX_PER_STAT_IVS;
    bool32 changed = FALSE;
    u32 i;

    if (stat > TRAINING_STAT_ALL)
        return TRAINING_RESULT_INVALID;
    for (i = 0; i < ARRAY_COUNT(sIVFields); i++)
    {
        if (stat != TRAINING_STAT_ALL && stat != i)
            continue;
        if (GetMonData(mon, sIVFields[i]) != value)
        {
            SetMonData(mon, sIVFields[i], &value);
            changed = TRUE;
        }
    }
    if (changed)
        CalculateMonStats(mon);
    return changed ? TRAINING_RESULT_SUCCESS : TRAINING_RESULT_NO_CHANGE;
}

enum TrainingResult TrainingNpc_ChangeEV(struct Pokemon *mon, u32 stat, u32 action)
{
    u32 i;
    u32 current;
    u32 total;
    u32 amount;
    u8 value;

    if (action > TRAINING_EV_RESET_ALL)
        return TRAINING_RESULT_INVALID;
    if (action == TRAINING_EV_RESET_ALL)
    {
        bool32 changed = FALSE;
        value = 0;
        for (i = 0; i < ARRAY_COUNT(sEVFields); i++)
        {
            if (GetMonData(mon, sEVFields[i]) != 0)
            {
                SetMonData(mon, sEVFields[i], &value);
                changed = TRUE;
            }
        }
        if (changed)
            CalculateMonStats(mon);
        return changed ? TRAINING_RESULT_SUCCESS : TRAINING_RESULT_NO_CHANGE;
    }
    if (stat >= TRAINING_STAT_ALL)
        return TRAINING_RESULT_INVALID;

    current = GetMonData(mon, sEVFields[stat]);
    if (action == TRAINING_EV_RESET_STAT)
    {
        if (current == 0)
            return TRAINING_RESULT_NO_CHANGE;
        value = 0;
        SetMonData(mon, sEVFields[stat], &value);
        CalculateMonStats(mon);
        return TRAINING_RESULT_SUCCESS;
    }

    total = GetTotalEVs(mon);
    if (current >= MAX_PER_STAT_EVS || total >= MAX_TOTAL_EVS)
        return TRAINING_RESULT_NO_CHANGE;
    switch (action)
    {
    case TRAINING_EV_ADD_4:  amount = 4; break;
    case TRAINING_EV_ADD_16: amount = 16; break;
    case TRAINING_EV_ADD_64: amount = 64; break;
    case TRAINING_EV_MAX:    amount = MAX_PER_STAT_EVS; break;
    default:                 return TRAINING_RESULT_INVALID;
    }
    amount = min(amount, MAX_PER_STAT_EVS - current);
    amount = min(amount, MAX_TOTAL_EVS - total);
    if (amount == 0)
        return TRAINING_RESULT_NO_CHANGE;
    value = current + amount;
    SetMonData(mon, sEVFields[stat], &value);
    CalculateMonStats(mon);
    return TRAINING_RESULT_SUCCESS;
}

enum TrainingResult TrainingNpc_ChangeExp(struct Pokemon *mon, u32 action, u32 levelCap)
{
    u32 species = GetMonData(mon, MON_DATA_SPECIES);
    u32 currentLevel = GetMonData(mon, MON_DATA_LEVEL);
    u32 targetLevel;
    u32 exp;

    if (action > TRAINING_EXP_TO_CAP || levelCap == 0)
        return TRAINING_RESULT_INVALID;

    levelCap = min(levelCap, MAX_LEVEL);
    if (currentLevel >= levelCap)
        return TRAINING_RESULT_NO_CHANGE;

    switch (action)
    {
    case TRAINING_EXP_ADD_1:
        targetLevel = currentLevel + 1;
        break;
    case TRAINING_EXP_ADD_10:
        targetLevel = currentLevel + 10;
        break;
    case TRAINING_EXP_TO_CAP:
        targetLevel = levelCap;
        break;
    default:
        return TRAINING_RESULT_INVALID;
    }

    targetLevel = min(targetLevel, levelCap);
    exp = gExperienceTables[gSpeciesInfo[species].growthRate][targetLevel];
    SetMonData(mon, MON_DATA_EXP, &exp);
    CalculateMonStats(mon);
    return TRAINING_RESULT_SUCCESS;
}

static enum TrainingResult GetExpTargetLevel(struct Pokemon *mon, u32 action, u32 levelCap, u8 *targetLevel)
{
    u32 currentLevel = GetMonData(mon, MON_DATA_LEVEL);
    u32 target;

    if (action > TRAINING_EXP_TO_CAP || levelCap == 0)
        return TRAINING_RESULT_INVALID;
    levelCap = min(levelCap, MAX_LEVEL);
    if (currentLevel >= levelCap)
        return TRAINING_RESULT_NO_CHANGE;

    if (action == TRAINING_EXP_ADD_1)
        target = currentLevel + 1;
    else if (action == TRAINING_EXP_ADD_10)
        target = currentLevel + 10;
    else
        target = levelCap;

    *targetLevel = min(target, levelCap);
    return TRAINING_RESULT_SUCCESS;
}

enum TrainingResult TrainingNpc_MaxFriendship(struct Pokemon *mon)
{
    u8 value = MAX_FRIENDSHIP;

    if (GetMonData(mon, MON_DATA_FRIENDSHIP) == MAX_FRIENDSHIP)
        return TRAINING_RESULT_NO_CHANGE;
    SetMonData(mon, MON_DATA_FRIENDSHIP, &value);
    return TRAINING_RESULT_SUCCESS;
}

enum TrainingResult TrainingNpc_ChangeNature(struct Pokemon *mon, u32 nature, u32 *money)
{
    if (nature >= NUM_NATURES)
        return TRAINING_RESULT_INVALID;
    if (GetMonData(mon, MON_DATA_HIDDEN_NATURE) == nature)
        return TRAINING_RESULT_NO_CHANGE;
    if (!IsEnoughMoney(money, TRAINING_NATURE_COST))
        return TRAINING_RESULT_NO_MONEY;
    SetMonData(mon, MON_DATA_HIDDEN_NATURE, &nature);
    CalculateMonStats(mon);
    RemoveMoney(money, TRAINING_NATURE_COST);
    return TRAINING_RESULT_SUCCESS;
}

bool32 TrainingNpc_IsAbilityAvailable(struct Pokemon *mon, u32 abilityNum)
{
    u32 species;
    u32 i;
    enum Ability ability;

    if (abilityNum >= NUM_ABILITY_SLOTS)
        return FALSE;
    species = GetMonData(mon, MON_DATA_SPECIES);
    ability = gSpeciesInfo[species].abilities[abilityNum];
    if (ability == ABILITY_NONE)
        return FALSE;
    for (i = 0; i < abilityNum; i++)
        if (gSpeciesInfo[species].abilities[i] == ability)
            return FALSE;
    return TRUE;
}

enum TrainingResult TrainingNpc_ChangeAbility(struct Pokemon *mon, u32 abilityNum)
{
    if (!TrainingNpc_IsAbilityAvailable(mon, abilityNum))
        return TRAINING_RESULT_INVALID;
    if (GetMonData(mon, MON_DATA_ABILITY_NUM) == abilityNum)
        return TRAINING_RESULT_NO_CHANGE;
    SetMonData(mon, MON_DATA_ABILITY_NUM, &abilityNum);
    return TRAINING_RESULT_SUCCESS;
}

void TrainingNpc_ApplyIV(void)
{
    gSpecialVar_Result = IsSelectedMonValid()
        ? TrainingNpc_MaxIV(&gPlayerParty[gSpecialVar_0x8004], gSpecialVar_0x8005)
        : TRAINING_RESULT_INVALID;
}

void TrainingNpc_ApplyEV(void)
{
    gSpecialVar_Result = IsSelectedMonValid()
        ? TrainingNpc_ChangeEV(&gPlayerParty[gSpecialVar_0x8004], gSpecialVar_0x8005, gSpecialVar_0x8006)
        : TRAINING_RESULT_INVALID;
}

void TrainingNpc_ApplyExp(void)
{
    gSpecialVar_Result = IsSelectedMonValid()
        ? TrainingNpc_ChangeExp(&gPlayerParty[gSpecialVar_0x8004], gSpecialVar_0x8005, GetCurrentLevelCap())
        : TRAINING_RESULT_INVALID;
    if (gSpecialVar_Result == TRAINING_RESULT_SUCCESS)
        ConvertIntToDecimalStringN(gStringVar1, GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_LEVEL), STR_CONV_MODE_LEFT_ALIGN, 3);
}

void TrainingNpc_StartExpTraining(void)
{
    u8 targetLevel = 0;

    gSpecialVar_Result = IsSelectedMonValid()
        ? GetExpTargetLevel(&gPlayerParty[gSpecialVar_0x8004], gSpecialVar_0x8005, GetCurrentLevelCap(), &targetLevel)
        : TRAINING_RESULT_INVALID;
    if (gSpecialVar_Result == TRAINING_RESULT_SUCCESS)
        StartTrainingNpcLevelUp(gSpecialVar_0x8004, targetLevel);
}

void TrainingNpc_ApplyFriendship(void)
{
    gSpecialVar_Result = IsSelectedMonValid()
        ? TrainingNpc_MaxFriendship(&gPlayerParty[gSpecialVar_0x8004])
        : TRAINING_RESULT_INVALID;
}

void TrainingNpc_BuildNatureMenu(void)
{
    u32 nature;

    for (nature = 0; nature < NUM_NATURES; nature++)
        PushChoice(gNaturesInfo[nature].name, nature);
}

void TrainingNpc_BufferNatureName(void)
{
    if (gSpecialVar_0x8005 < NUM_NATURES)
        StringCopy(gStringVar1, gNaturesInfo[gSpecialVar_0x8005].name);
}

void TrainingNpc_ApplyNature(void)
{
    gSpecialVar_Result = IsSelectedMonValid()
        ? TrainingNpc_ChangeNature(&gPlayerParty[gSpecialVar_0x8004], gSpecialVar_0x8005, &gSaveBlock1Ptr->money)
        : TRAINING_RESULT_INVALID;
}

void TrainingNpc_BuildAbilityMenu(void)
{
    struct Pokemon *mon;
    u32 species;
    u32 abilityNum;

    if (!IsSelectedMonValid())
        return;
    mon = &gPlayerParty[gSpecialVar_0x8004];
    species = GetMonData(mon, MON_DATA_SPECIES);
    for (abilityNum = 0; abilityNum < NUM_ABILITY_SLOTS; abilityNum++)
        if (TrainingNpc_IsAbilityAvailable(mon, abilityNum))
            PushChoice(gAbilitiesInfo[gSpeciesInfo[species].abilities[abilityNum]].name, abilityNum);
}

void TrainingNpc_BufferAbilityName(void)
{
    struct Pokemon *mon;
    u32 species;

    if (!IsSelectedMonValid() || !TrainingNpc_IsAbilityAvailable(&gPlayerParty[gSpecialVar_0x8004], gSpecialVar_0x8005))
        return;
    mon = &gPlayerParty[gSpecialVar_0x8004];
    species = GetMonData(mon, MON_DATA_SPECIES);
    StringCopy(gStringVar1, gAbilitiesInfo[gSpeciesInfo[species].abilities[gSpecialVar_0x8005]].name);
}

void TrainingNpc_ApplyAbility(void)
{
    gSpecialVar_Result = IsSelectedMonValid()
        ? TrainingNpc_ChangeAbility(&gPlayerParty[gSpecialVar_0x8004], gSpecialVar_0x8005)
        : TRAINING_RESULT_INVALID;
}
