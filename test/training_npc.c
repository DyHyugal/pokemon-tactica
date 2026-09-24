#include "global.h"
#include "money.h"
#include "pokemon.h"
#include "training_npc.h"
#include "test/test.h"

#if IS_HNS
static void InitTrainingMon(struct Pokemon *mon)
{
    CreateRandomMon(mon, SPECIES_BULBASAUR, 20);
}

TEST("Training NPC: IV service caps one stat or every stat at 31")
{
    struct Pokemon mon;
    u32 value = 0;
    u32 i;

    InitTrainingMon(&mon);
    SetMonData(&mon, MON_DATA_HP_IV, &value);
    EXPECT_EQ(TrainingNpc_MaxIV(&mon, TRAINING_STAT_HP), TRAINING_RESULT_SUCCESS);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_HP_IV), 31);
    for (i = 0; i < NUM_STATS; i++)
        SetMonData(&mon, MON_DATA_HP_IV + i, &value);
    EXPECT_EQ(TrainingNpc_MaxIV(&mon, TRAINING_STAT_ALL), TRAINING_RESULT_SUCCESS);
    for (i = 0; i < NUM_STATS; i++)
        EXPECT_EQ(GetMonData(&mon, MON_DATA_HP_IV + i), 31);
}

TEST("Training NPC: EV additions obey the stat and total caps exactly")
{
    struct Pokemon mon;
    u8 value;

    InitTrainingMon(&mon);
    value = 250;
    SetMonData(&mon, MON_DATA_HP_EV, &value);
    EXPECT_EQ(TrainingNpc_ChangeEV(&mon, TRAINING_STAT_HP, TRAINING_EV_ADD_16), TRAINING_RESULT_SUCCESS);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_HP_EV), 252);
    EXPECT_EQ(TrainingNpc_ChangeEV(&mon, TRAINING_STAT_HP, TRAINING_EV_ADD_4), TRAINING_RESULT_NO_CHANGE);

    value = 0;
    SetMonData(&mon, MON_DATA_HP_EV, &value);
    value = 252;
    SetMonData(&mon, MON_DATA_ATK_EV, &value);
    SetMonData(&mon, MON_DATA_DEF_EV, &value);
    value = 0;
    SetMonData(&mon, MON_DATA_SPEED_EV, &value);
    SetMonData(&mon, MON_DATA_SPATK_EV, &value);
    SetMonData(&mon, MON_DATA_SPDEF_EV, &value);
    EXPECT_EQ(TrainingNpc_ChangeEV(&mon, TRAINING_STAT_HP, TRAINING_EV_MAX), TRAINING_RESULT_SUCCESS);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_HP_EV), 6);
    EXPECT_EQ(TrainingNpc_ChangeEV(&mon, TRAINING_STAT_SPEED, TRAINING_EV_ADD_4), TRAINING_RESULT_NO_CHANGE);
}

TEST("Training NPC: EV reset clears one stat or the full spread")
{
    struct Pokemon mon;
    u8 value = 64;
    u32 i;

    InitTrainingMon(&mon);
    for (i = 0; i < NUM_STATS; i++)
        SetMonData(&mon, MON_DATA_HP_EV + i, &value);
    EXPECT_EQ(TrainingNpc_ChangeEV(&mon, TRAINING_STAT_SPATK, TRAINING_EV_RESET_STAT), TRAINING_RESULT_SUCCESS);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_SPATK_EV), 0);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_HP_EV), 64);
    EXPECT_EQ(TrainingNpc_ChangeEV(&mon, TRAINING_STAT_HP, TRAINING_EV_RESET_ALL), TRAINING_RESULT_SUCCESS);
    for (i = 0; i < NUM_STATS; i++)
        EXPECT_EQ(GetMonData(&mon, MON_DATA_HP_EV + i), 0);
}

TEST("Training NPC: EXP service obeys increments and the current level cap")
{
    struct Pokemon mon;

    InitTrainingMon(&mon);
    EXPECT_EQ(TrainingNpc_ChangeExp(&mon, TRAINING_EXP_ADD_1, 25), TRAINING_RESULT_SUCCESS);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_LEVEL), 21);
    EXPECT_EQ(TrainingNpc_ChangeExp(&mon, TRAINING_EXP_ADD_10, 25), TRAINING_RESULT_SUCCESS);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_LEVEL), 25);
    EXPECT_EQ(TrainingNpc_ChangeExp(&mon, TRAINING_EXP_TO_CAP, 25), TRAINING_RESULT_NO_CHANGE);

    InitTrainingMon(&mon);
    EXPECT_EQ(TrainingNpc_ChangeExp(&mon, TRAINING_EXP_TO_CAP, 1000), TRAINING_RESULT_SUCCESS);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_LEVEL), MAX_LEVEL);
    EXPECT_EQ(TrainingNpc_ChangeExp(&mon, TRAINING_EXP_ADD_1, MAX_LEVEL), TRAINING_RESULT_NO_CHANGE);
    EXPECT_EQ(TrainingNpc_ChangeExp(&mon, TRAINING_EXP_TO_CAP + 1, 25), TRAINING_RESULT_INVALID);
}

TEST("Training NPC: friendship reaches the native maximum only")
{
    struct Pokemon mon;
    u8 value = 1;

    InitTrainingMon(&mon);
    SetMonData(&mon, MON_DATA_FRIENDSHIP, &value);
    EXPECT_EQ(TrainingNpc_MaxFriendship(&mon), TRAINING_RESULT_SUCCESS);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_FRIENDSHIP), MAX_FRIENDSHIP);
    EXPECT_EQ(TrainingNpc_MaxFriendship(&mon), TRAINING_RESULT_NO_CHANGE);
}

TEST("Training NPC: Nature costs 5000 only after a valid change")
{
    struct Pokemon mon;
    u32 nature = NATURE_HARDY;
    u32 personality;
    u32 ability;

    InitTrainingMon(&mon);
    SetMonData(&mon, MON_DATA_HIDDEN_NATURE, &nature);
    personality = GetMonData(&mon, MON_DATA_PERSONALITY);
    ability = GetMonData(&mon, MON_DATA_ABILITY_NUM);
    SetMoney(&gSaveBlock1Ptr->money, TRAINING_NATURE_COST);
    EXPECT_EQ(TrainingNpc_ChangeNature(&mon, NATURE_ADAMANT, &gSaveBlock1Ptr->money), TRAINING_RESULT_SUCCESS);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_HIDDEN_NATURE), NATURE_ADAMANT);
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), 0);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_PERSONALITY), personality);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_ABILITY_NUM), ability);

    SetMoney(&gSaveBlock1Ptr->money, TRAINING_NATURE_COST - 1);
    EXPECT_EQ(TrainingNpc_ChangeNature(&mon, NATURE_BOLD, &gSaveBlock1Ptr->money), TRAINING_RESULT_NO_MONEY);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_HIDDEN_NATURE), NATURE_ADAMANT);
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), TRAINING_NATURE_COST - 1);
    EXPECT_EQ(TrainingNpc_ChangeNature(&mon, NUM_NATURES, &gSaveBlock1Ptr->money), TRAINING_RESULT_INVALID);
}

TEST("Training NPC: only real species abilities can be selected")
{
    struct Pokemon mon;
    u32 abilityNum = 0;
    u32 personality;

    InitTrainingMon(&mon);
    SetMonData(&mon, MON_DATA_ABILITY_NUM, &abilityNum);
    personality = GetMonData(&mon, MON_DATA_PERSONALITY);
    EXPECT(TrainingNpc_IsAbilityAvailable(&mon, 0));
    EXPECT(!TrainingNpc_IsAbilityAvailable(&mon, 1));
    EXPECT(TrainingNpc_IsAbilityAvailable(&mon, 2));
    EXPECT(!TrainingNpc_IsAbilityAvailable(&mon, NUM_ABILITY_SLOTS));
    EXPECT_EQ(TrainingNpc_ChangeAbility(&mon, 1), TRAINING_RESULT_INVALID);
    EXPECT_EQ(TrainingNpc_ChangeAbility(&mon, 2), TRAINING_RESULT_SUCCESS);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_ABILITY_NUM), 2);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_PERSONALITY), personality);
}
#endif
