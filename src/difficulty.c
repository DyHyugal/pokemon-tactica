#include "global.h"
#include "data.h"
#include "event_data.h"
#include "script.h"
#include "constants/battle.h"

enum DifficultyLevel GetCurrentDifficultyLevel(void)
{
#if IS_HNS && !TESTING
    // Tactica's existing Trainer IVs setting provides HARD (2) and native
    // NORMAL (0); SCALE (1) keeps the native trainer roster with scaled IVs.
    return gSaveBlock3Ptr->challengeSettings.tx_Challenges_TrainerScalingIVs == 2
        ? DIFFICULTY_HARD : DIFFICULTY_NORMAL;
#else
    if (!B_VAR_DIFFICULTY)
        return DIFFICULTY_NORMAL;

    return VarGet(B_VAR_DIFFICULTY);
#endif
}

void SetCurrentDifficultyLevel(enum DifficultyLevel desiredDifficulty)
{
#if IS_HNS && !TESTING
    gSaveBlock3Ptr->challengeSettings.tx_Challenges_TrainerScalingIVs =
        desiredDifficulty == DIFFICULTY_HARD ? 2 : 0;
#else
    if (!B_VAR_DIFFICULTY)
        return;

    if (desiredDifficulty > DIFFICULTY_MAX)
        desiredDifficulty = DIFFICULTY_MAX;

    VarSet(B_VAR_DIFFICULTY, desiredDifficulty);
#endif
}

enum DifficultyLevel GetBattlePartnerDifficultyLevel(u16 partnerId)
{
    enum DifficultyLevel difficulty = GetCurrentDifficultyLevel();

    if (partnerId > TRAINER_PARTNER(PARTNER_NONE))
        partnerId -= TRAINER_PARTNER(PARTNER_NONE);

    if (difficulty == DIFFICULTY_NORMAL)
        return DIFFICULTY_NORMAL;

    if (gBattlePartners[difficulty][partnerId].party == NULL)
        return DIFFICULTY_NORMAL;

    return difficulty;
}

enum DifficultyLevel GetTrainerDifficultyLevel(u16 trainerId)
{
    enum DifficultyLevel difficulty = GetCurrentDifficultyLevel();

    if (difficulty == DIFFICULTY_NORMAL)
        return DIFFICULTY_NORMAL;

    if (gTrainers[difficulty][trainerId].party == NULL)
        return DIFFICULTY_NORMAL;

    return difficulty;
}

void Script_IncreaseDifficulty(void)
{
    enum DifficultyLevel currentDifficulty;

    if (!B_VAR_DIFFICULTY)
        return;

    currentDifficulty = GetCurrentDifficultyLevel();

    if (currentDifficulty++ > DIFFICULTY_MAX)
        return;

    Script_RequestEffects(SCREFF_V1);
    Script_RequestWriteVar(B_VAR_DIFFICULTY);

    SetCurrentDifficultyLevel(currentDifficulty);
}

void Script_DecreaseDifficulty(void)
{
    enum DifficultyLevel currentDifficulty;

    if (!B_VAR_DIFFICULTY)
        return;

    currentDifficulty = GetCurrentDifficultyLevel();

    if (!currentDifficulty)
        return;

    Script_RequestEffects(SCREFF_V1);
    Script_RequestWriteVar(B_VAR_DIFFICULTY);

    SetCurrentDifficultyLevel(--currentDifficulty);
}

void Script_GetDifficulty(void)
{
    Script_RequestEffects(SCREFF_V1);
    gSpecialVar_Result = GetCurrentDifficultyLevel();
}

void Script_SetDifficulty(struct ScriptContext *ctx)
{
    enum DifficultyLevel desiredDifficulty = ScriptReadByte(ctx);

    Script_RequestEffects(SCREFF_V1);
    Script_RequestWriteVar(B_VAR_DIFFICULTY);

    SetCurrentDifficultyLevel(desiredDifficulty);
}
