#include "global.h"
#include "battle_setup.h"
#include "battle_main.h"
#include "caps.h"
#include "event_data.h"
#include "pokemon.h"
#include "training_npc.h"
#include "test/test.h"

#if IS_HNS
static u32 GetTestBossLevel(u16 trainerId, bool8 useLowestLevel)
{
    switch (trainerId)
    {
    case TRAINER_RIVAL_CHIKORITA_1_HNS:
    case TRAINER_RIVAL_CYNDAQUIL_1_HNS:
    case TRAINER_RIVAL_TOTODILE_1_HNS:
        return 17;
    case TRAINER_FALKNER_1_HNS:
        return useLowestLevel ? 14 : 17;
    case TRAINER_RIVAL_CHIKORITA_2_HNS:
    case TRAINER_RIVAL_CYNDAQUIL_2_HNS:
    case TRAINER_RIVAL_TOTODILE_2_HNS:
        return useLowestLevel ? 29 : 32;
    case TRAINER_BUGSY_1_HNS:
        return useLowestLevel ? 22 : 25;
    case TRAINER_WHITNEY_1_HNS:
        return useLowestLevel ? 29 : 32;
    case TRAINER_RIVAL_CHIKORITA_3_HNS:
    case TRAINER_RIVAL_CYNDAQUIL_3_HNS:
    case TRAINER_RIVAL_TOTODILE_3_HNS:
        return useLowestLevel ? 35 : 38;
    case TRAINER_RIVAL_CHIKORITA_4_HNS:
    case TRAINER_RIVAL_CYNDAQUIL_4_HNS:
    case TRAINER_RIVAL_TOTODILE_4_HNS:
        return useLowestLevel ? 61 : 64;
    case TRAINER_RIVAL_CHIKORITA_5_HNS:
    case TRAINER_RIVAL_CYNDAQUIL_5_HNS:
    case TRAINER_RIVAL_TOTODILE_5_HNS:
        return useLowestLevel ? 65 : 67;
    case TRAINER_RIVAL_CHIKORITA_6_HNS:
    case TRAINER_RIVAL_CYNDAQUIL_6_HNS:
    case TRAINER_RIVAL_TOTODILE_6_HNS:
        return 95;
    case TRAINER_MORTY_1_HNS:
        return useLowestLevel ? 35 : 38;
    case TRAINER_LTSURGE_HNS:
        return 75;
    case TRAINER_SABRINA_HNS:
    case TRAINER_ERIKA_HNS:
    case TRAINER_JANINE_HNS:
        return 80;
    case TRAINER_MISTY_HNS:
        return 85;
    case TRAINER_BROCK_HNS:
        return 90;
    case TRAINER_BLAINE_HNS:
        return 95;
    case TRAINER_BLUE_HNS:
    case TRAINER_WILL_2_HNS:
    case TRAINER_KOGA_2_HNS:
    case TRAINER_BRUNO_2_HNS:
    case TRAINER_KAREN_2_HNS:
    case TRAINER_LANCE_2_HNS:
        return 100;
    case TRAINER_RED_HNS:
        return useLowestLevel ? 77 : 93;
    default:
        return 1;
    }
}

static void SetLevelCapMode(u8 mode)
{
    SetLevelCapTrainerLevelGetterForTesting(GetTestBossLevel);
    gSaveBlock3Ptr->challengeSettings.tx_Challenges_LevelCap = mode;
}

TEST("Level cap: Falkner is the first HnS milestone")
{
    SetLevelCapMode(1);
    EXPECT_EQ(GetCurrentLevelCap(), 17);

    SetLevelCapMode(2);
    EXPECT_EQ(GetCurrentLevelCap(), 14);
}

TEST("Level cap: first Rocket boss escalates two levels from the previous cap")
{
    SetLevelCapMode(1);
    FlagSet(FLAG_DEFEATED_VIOLET_GYM);
    EXPECT_EQ(GetFamilyRocketTrainerLevel(TRAINER_PROTON_1_HNS), 19);
    EXPECT_EQ(GetCurrentLevelCap(), 19);

    SetLevelCapMode(2);
    EXPECT_EQ(GetFamilyRocketTrainerLevel(TRAINER_PROTON_1_HNS), 16);
    EXPECT_EQ(GetCurrentLevelCap(), 16);
}

TEST("Level cap: consecutive Rocket bosses share the last canonical milestone plus two")
{
    SetLevelCapMode(1);
    EXPECT_EQ(GetFamilyRocketTrainerLevel(TRAINER_PETREL_1_HNS), 40);
    EXPECT_EQ(GetFamilyRocketTrainerLevel(TRAINER_ARIANA_1_HNS), 40);
}

TEST("Level cap: Proton 1 immediately advances preparation to Bugsy")
{
    SetLevelCapMode(1);
    FlagSet(FLAG_DEFEATED_VIOLET_GYM);
    SetTrainerFlag(TRAINER_PROTON_1_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 25);

    SetLevelCapMode(2);
    EXPECT_EQ(GetCurrentLevelCap(), 22);

    FlagSet(FLAG_DEFEATED_AZALEA_TOWN_GYM);
    EXPECT_EQ(GetCurrentLevelCap(), 29);
}

TEST("Level cap: rival fights use the next Gym cap")
{
    SetLevelCapMode(1);
    FlagSet(FLAG_DEFEATED_VIOLET_GYM);
    SetTrainerFlag(TRAINER_PROTON_1_HNS);
    FlagSet(FLAG_DEFEATED_AZALEA_TOWN_GYM);
    EXPECT_EQ(GetCurrentLevelCap(), 32);

    SetTrainerFlag(TRAINER_RIVAL_CYNDAQUIL_2_HNS);
    FlagSet(FLAG_DEFEATED_GOLDENROD_CITY_GYM);
    EXPECT_EQ(GetCurrentLevelCap(), 38);

    SetLevelCapMode(2);
    EXPECT_EQ(GetCurrentLevelCap(), 35);
}

TEST("Level cap: later rival fights follow the next major boss")
{
    EXPECT_EQ(GetTestBossLevel(TRAINER_RIVAL_CYNDAQUIL_4_HNS, FALSE), 64);
    EXPECT_EQ(GetTestBossLevel(TRAINER_RIVAL_CYNDAQUIL_4_HNS, TRUE), 61);
    EXPECT_EQ(GetTestBossLevel(TRAINER_RIVAL_CYNDAQUIL_5_HNS, FALSE), 67);
    EXPECT_EQ(GetTestBossLevel(TRAINER_RIVAL_CYNDAQUIL_5_HNS, TRUE), 65);
    EXPECT_EQ(GetTestBossLevel(TRAINER_RIVAL_CYNDAQUIL_6_HNS, FALSE), 95);
}

TEST("Level cap: Rocket encounters follow actual story stages")
{
    SetLevelCapMode(1);
    EXPECT_EQ(GetFamilyRocketTrainerLevel(TRAINER_PROTON_1_HNS), 19);
    EXPECT_EQ(GetFamilyRocketTrainerLevel(TRAINER_PETREL_1_HNS), 40);
    EXPECT_EQ(GetFamilyRocketTrainerLevel(TRAINER_ARIANA_1_HNS), 40);
    EXPECT_EQ(GetFamilyRocketTrainerLevel(TRAINER_PETREL_2_HNS), 40);
    EXPECT_EQ(GetFamilyRocketTrainerLevel(TRAINER_PROTON_2_HNS), 66);
    EXPECT_EQ(GetFamilyRocketTrainerLevel(TRAINER_ARIANA_2_HNS), 66);
    EXPECT_EQ(GetFamilyRocketTrainerLevel(TRAINER_ARCHER_HNS), 66);
}

TEST("Level cap: Rocket parties use legal evolution stages at their resolved level")
{
    EXPECT_EQ(GetFamilyRocketLegalSpecies(SPECIES_CROBAT, 19), SPECIES_ZUBAT);
    EXPECT_EQ(GetFamilyRocketLegalSpecies(SPECIES_WEEZING, 19), SPECIES_KOFFING);
    EXPECT_EQ(GetFamilyRocketLegalSpecies(SPECIES_RATICATE, 19), SPECIES_RATTATA);
    EXPECT_EQ(GetFamilyRocketLegalSpecies(SPECIES_SCOLIPEDE, 19), SPECIES_VENIPEDE);
    EXPECT_EQ(GetFamilyRocketLegalSpecies(SPECIES_TOXICROAK, 19), SPECIES_CROAGUNK);
    EXPECT_EQ(GetFamilyRocketLegalSpecies(SPECIES_MUK_ALOLA, 19), SPECIES_GRIMER_ALOLA);

    EXPECT_EQ(GetFamilyRocketLegalSpecies(SPECIES_CROBAT, 23), SPECIES_CROBAT);
    EXPECT_EQ(GetFamilyRocketLegalSpecies(SPECIES_SCOLIPEDE, 29), SPECIES_WHIRLIPEDE);
    EXPECT_EQ(GetFamilyRocketLegalSpecies(SPECIES_SCOLIPEDE, 30), SPECIES_SCOLIPEDE);
    EXPECT_EQ(GetFamilyRocketLegalSpecies(SPECIES_TOXICROAK, 37), SPECIES_TOXICROAK);
    EXPECT_EQ(GetFamilyRocketLegalSpecies(SPECIES_MUK_ALOLA, 38), SPECIES_MUK_ALOLA);
}

TEST("Level cap: Rocket runtime applies the dynamic level and legal species")
{
    SetLevelCapMode(1);
    CreateRandomMon(&gEnemyParty[0], SPECIES_CROBAT, 1);

    ApplyFamilyRocketPartyLevel(gEnemyParty, 1, TRAINER_PROTON_1_HNS);

    EXPECT_EQ(GetMonData(&gEnemyParty[0], MON_DATA_LEVEL), 19);
    EXPECT_EQ(GetMonData(&gEnemyParty[0], MON_DATA_SPECIES), SPECIES_ZUBAT);
}

TEST("Level cap: hard mode uses the next boss party's lowest level")
{
    SetLevelCapMode(2);
    SetTrainerFlag(TRAINER_RIVAL_CYNDAQUIL_1_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 14);

    FlagSet(FLAG_DEFEATED_VIOLET_GYM);
    EXPECT_EQ(GetCurrentLevelCap(), 16);
}

TEST("Level cap: league transitions use Kanto, Red, then the engine maximum")
{
    SetLevelCapMode(1);
    FlagSet(FLAG_IS_CHAMPION);
    EXPECT_EQ(GetCurrentLevelCap(), 75);

    FlagSet(FLAG_IS_KANTO_CHAMPION);
    EXPECT_EQ(GetCurrentLevelCap(), 93);

    FlagSet(FLAG_DEFEATED_RED);
    EXPECT_EQ(GetCurrentLevelCap(), MAX_LEVEL);
}

TEST("Level cap: Kanto scales by five and parallel central Gyms share level 80")
{
    SetLevelCapMode(1);
    FlagSet(FLAG_IS_CHAMPION);
    EXPECT_EQ(GetCurrentLevelCap(), 75);

    FlagSet(FLAG_DEFEATED_VERMILION_GYM);
    EXPECT_EQ(GetCurrentLevelCap(), 80);

    SetTrainerFlag(TRAINER_SABRINA_HNS);
    SetTrainerFlag(TRAINER_ERIKA_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 80);

    SetTrainerFlag(TRAINER_JANINE_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 85);

    FlagSet(FLAG_DEFEATED_CERULEAN_GYM);
    EXPECT_EQ(GetCurrentLevelCap(), 90);

    FlagSet(FLAG_DEFEATED_PEWTER_GYM);
    EXPECT_EQ(GetCurrentLevelCap(), 95);

    SetTrainerFlag(TRAINER_RIVAL_CYNDAQUIL_6_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 100);

    FlagSet(FLAG_DEFEATED_CINNABAR_ISLAND_GYM);
    EXPECT_EQ(GetCurrentLevelCap(), 100);
}

TEST("Level cap: EXP Training reads the live boss cap")
{
    SetLevelCapMode(1);
    ZeroPlayerPartyMons();
    CreateRandomMon(&gPlayerParty[0], SPECIES_BULBASAUR, 2);
    gPlayerPartyCount = 1;
    gSpecialVar_0x8004 = 0;
    gSpecialVar_0x8005 = TRAINING_EXP_TO_CAP;

    TrainingNpc_ApplyExp();

    EXPECT_EQ(gSpecialVar_Result, TRAINING_RESULT_SUCCESS);
    EXPECT_EQ(GetMonData(&gPlayerParty[0], MON_DATA_LEVEL), 17);
}
#endif
