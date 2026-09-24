#ifndef GUARD_TRAINING_NPC_H
#define GUARD_TRAINING_NPC_H

#include "pokemon.h"

#define TRAINING_NATURE_COST 5000

enum TrainingStat
{
    TRAINING_STAT_HP,
    TRAINING_STAT_ATK,
    TRAINING_STAT_DEF,
    TRAINING_STAT_SPATK,
    TRAINING_STAT_SPDEF,
    TRAINING_STAT_SPEED,
    TRAINING_STAT_ALL,
};

enum TrainingEVAction
{
    TRAINING_EV_ADD_4,
    TRAINING_EV_ADD_16,
    TRAINING_EV_ADD_64,
    TRAINING_EV_MAX,
    TRAINING_EV_RESET_STAT,
    TRAINING_EV_RESET_ALL,
};

enum TrainingExpAction
{
    TRAINING_EXP_ADD_1,
    TRAINING_EXP_ADD_10,
    TRAINING_EXP_TO_CAP,
};

enum TrainingResult
{
    TRAINING_RESULT_INVALID,
    TRAINING_RESULT_SUCCESS,
    TRAINING_RESULT_NO_CHANGE,
    TRAINING_RESULT_NO_MONEY,
};

enum TrainingResult TrainingNpc_MaxIV(struct Pokemon *mon, u32 stat);
enum TrainingResult TrainingNpc_ChangeEV(struct Pokemon *mon, u32 stat, u32 action);
enum TrainingResult TrainingNpc_ChangeExp(struct Pokemon *mon, u32 action, u32 levelCap);
enum TrainingResult TrainingNpc_MaxFriendship(struct Pokemon *mon);
enum TrainingResult TrainingNpc_ChangeNature(struct Pokemon *mon, u32 nature, u32 *money);
enum TrainingResult TrainingNpc_ChangeAbility(struct Pokemon *mon, u32 abilityNum);
bool32 TrainingNpc_IsAbilityAvailable(struct Pokemon *mon, u32 abilityNum);

void TrainingNpc_ApplyIV(void);
void TrainingNpc_ApplyEV(void);
void TrainingNpc_ApplyExp(void);
void TrainingNpc_StartExpTraining(void);
void TrainingNpc_ApplyFriendship(void);
void TrainingNpc_BuildNatureMenu(void);
void TrainingNpc_BufferNatureName(void);
void TrainingNpc_ApplyNature(void);
void TrainingNpc_BuildAbilityMenu(void);
void TrainingNpc_BufferAbilityName(void);
void TrainingNpc_ApplyAbility(void);

#endif
