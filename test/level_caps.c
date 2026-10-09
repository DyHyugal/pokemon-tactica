#include "global.h"
#include "battle_setup.h"
#include "battle_main.h"
#include "caps.h"
#include "event_data.h"
#include "pokemon.h"
#include "training_npc.h"
#include "data.h"
#include "difficulty.h"
#include "test/test.h"

#if IS_HNS
static void SetLevelCapMode(u8 mode)
{
    gSaveBlock3Ptr->challengeSettings.tx_Challenges_LevelCap = mode;
}

TEST("Level cap: Falkner is the first HnS milestone")
{
    SetLevelCapMode(1);
    EXPECT_EQ(GetCurrentLevelCap(), 18);

    SetLevelCapMode(2);
    EXPECT_EQ(GetCurrentLevelCap(), 14);
}

TEST("Level cap: first Rocket boss escalates two levels from the previous cap")
{
    SetLevelCapMode(1);
    FlagSet(FLAG_DEFEATED_VIOLET_GYM);
    EXPECT_EQ(GetFamilyRocketTrainerLevel(TRAINER_PROTON_1_HNS), 20);
    EXPECT_EQ(GetCurrentLevelCap(), 20);

    SetLevelCapMode(2);
    EXPECT_EQ(GetFamilyRocketTrainerLevel(TRAINER_PROTON_1_HNS), 20);
    EXPECT_EQ(GetCurrentLevelCap(), 16);
}

TEST("Level cap: consecutive Rocket bosses share the last canonical milestone plus two")
{
    SetLevelCapMode(1);
    FlagSet(FLAG_DEFEATED_ECRUTEAK_CITY_GYM);
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

TEST("Level cap: Whitney remains the next cap after the equal-level rival milestone")
{
    SetLevelCapMode(1);
    FlagSet(FLAG_DEFEATED_VIOLET_GYM);
    SetTrainerFlag(TRAINER_PROTON_1_HNS);
    FlagSet(FLAG_DEFEATED_AZALEA_TOWN_GYM);
    SetTrainerFlag(TRAINER_RIVAL_CYNDAQUIL_2_HNS);

    EXPECT_EQ(GetCurrentLevelCap(), 32);

    SetLevelCapMode(2);
    EXPECT_EQ(GetCurrentLevelCap(), 29);
}

TEST("Level cap: later rival levels come from the actual trainer parties")
{
    const struct Trainer *trainer;
    static const u16 ids[] = {TRAINER_RIVAL_CYNDAQUIL_4_HNS, TRAINER_RIVAL_CYNDAQUIL_5_HNS, TRAINER_RIVAL_CYNDAQUIL_6_HNS};
    static const u8 minima[] = {61, 65, 95};
    static const u8 maxima[] = {64, 67, 95};
    for (u32 i = 0; i < ARRAY_COUNT(ids); i++)
    {
        u32 low = MAX_LEVEL, high = 0;
        trainer = &gTrainers[GetTrainerDifficultyLevel(ids[i])][ids[i]];
        EXPECT_EQ(trainer->partySize, PARTY_SIZE);
        for (u32 j = 0; j < trainer->partySize; j++)
        {
            low = min(low, trainer->party[j].lvl);
            high = max(high, trainer->party[j].lvl);
        }
        EXPECT_EQ(low, minima[i]);
        EXPECT_EQ(high, maxima[i]);
    }
}

TEST("Level cap: all Rocket fights use defeated Gyms and ignore cap mode and rival flags")
{
    static const u16 ids[] = {TRAINER_PROTON_1_HNS, TRAINER_PROTON_2_HNS, TRAINER_PETREL_1_HNS, TRAINER_PETREL_2_HNS, TRAINER_ARIANA_1_HNS, TRAINER_ARIANA_2_HNS, TRAINER_ARCHER_HNS};
    FlagSet(FLAG_DEFEATED_GOLDENROD_CITY_GYM);
    SetTrainerFlag(TRAINER_RIVAL_CYNDAQUIL_4_HNS);
    for (u32 mode = 0; mode <= 2; mode++)
    {
        SetLevelCapMode(mode);
        for (u32 i = 0; i < ARRAY_COUNT(ids); i++)
        {
            EXPECT_EQ(GetFamilyRocketTrainerLevel(ids[i]), 34);
            EXPECT_EQ(GetFamilyRocketTrainerMonLevel(ids[i], 0, 6), 31);
            EXPECT_EQ(GetFamilyRocketTrainerMonLevel(ids[i], 5, 6), 34);
        }
    }
    // Completing a later Gym changes the common reference, not just one boss.
    FlagSet(FLAG_DEFEATED_OLIVINE_CITY_GYM);
    for (u32 i = 0; i < ARRAY_COUNT(ids); i++)
    {
        EXPECT_EQ(GetFamilyRocketTrainerMonLevel(ids[i], 0, 6), 50);
        EXPECT_EQ(GetFamilyRocketTrainerMonLevel(ids[i], 5, 6), 54);
    }
    EXPECT_EQ(GetFamilyRocketTrainerLevel(TRAINER_FALKNER_1_HNS), 0);
}

TEST("Level cap: every Johto milestone follows the complete NORMAL progression")
{
    SetLevelCapMode(1);
    EXPECT_EQ(GetCurrentLevelCap(), 18);

    FlagSet(FLAG_DEFEATED_VIOLET_GYM);
    EXPECT_EQ(GetCurrentLevelCap(), 20);
    SetTrainerFlag(TRAINER_PROTON_1_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 25);
    FlagSet(FLAG_DEFEATED_AZALEA_TOWN_GYM);
    EXPECT_EQ(GetCurrentLevelCap(), 32);
    SetTrainerFlag(TRAINER_RIVAL_CYNDAQUIL_2_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 32);
    FlagSet(FLAG_DEFEATED_GOLDENROD_CITY_GYM);
    EXPECT_EQ(GetCurrentLevelCap(), 38);
    SetTrainerFlag(TRAINER_RIVAL_CYNDAQUIL_3_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 38);
    FlagSet(FLAG_DEFEATED_ECRUTEAK_CITY_GYM);
    EXPECT_EQ(GetCurrentLevelCap(), 45);
    FlagSet(FLAG_DEFEATED_CIANWOOD_GYM);
    EXPECT_EQ(GetCurrentLevelCap(), 52);
    FlagSet(FLAG_DEFEATED_OLIVINE_CITY_GYM);
    EXPECT_EQ(GetCurrentLevelCap(), 54);
    SetTrainerFlag(TRAINER_PETREL_1_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 54);
    SetTrainerFlag(TRAINER_ARIANA_1_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 57);
    FlagSet(FLAG_DEFEATED_MAHOGANY_TOWN_GYM);
    EXPECT_EQ(GetCurrentLevelCap(), 59);
    SetTrainerFlag(TRAINER_PETREL_2_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 64);
    SetTrainerFlag(TRAINER_RIVAL_CYNDAQUIL_4_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 64);
    SetTrainerFlag(TRAINER_PROTON_2_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 64);
    SetTrainerFlag(TRAINER_ARIANA_2_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 64);
    SetTrainerFlag(TRAINER_ARCHER_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 64);
    FlagSet(FLAG_DEFEATED_BLACKTHORN_GYM);
    EXPECT_EQ(GetCurrentLevelCap(), 67);
    SetTrainerFlag(TRAINER_RIVAL_CYNDAQUIL_5_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 67);
    SetTrainerFlag(TRAINER_WILL_1_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 67);
    SetTrainerFlag(TRAINER_KOGA_1_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 67);
    SetTrainerFlag(TRAINER_BRUNO_1_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 67);
    SetTrainerFlag(TRAINER_KAREN_1_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 70);
}

TEST("Level cap: every Johto milestone follows the complete HARD progression")
{
    SetLevelCapMode(2);
    EXPECT_EQ(GetCurrentLevelCap(), 14);

    FlagSet(FLAG_DEFEATED_VIOLET_GYM);
    EXPECT_EQ(GetCurrentLevelCap(), 16);
    SetTrainerFlag(TRAINER_PROTON_1_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 22);
    FlagSet(FLAG_DEFEATED_AZALEA_TOWN_GYM);
    EXPECT_EQ(GetCurrentLevelCap(), 29);
    SetTrainerFlag(TRAINER_RIVAL_CYNDAQUIL_2_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 29);
    FlagSet(FLAG_DEFEATED_GOLDENROD_CITY_GYM);
    EXPECT_EQ(GetCurrentLevelCap(), 35);
    SetTrainerFlag(TRAINER_RIVAL_CYNDAQUIL_3_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 35);
    FlagSet(FLAG_DEFEATED_ECRUTEAK_CITY_GYM);
    EXPECT_EQ(GetCurrentLevelCap(), 42);
    FlagSet(FLAG_DEFEATED_CIANWOOD_GYM);
    EXPECT_EQ(GetCurrentLevelCap(), 48);
    FlagSet(FLAG_DEFEATED_OLIVINE_CITY_GYM);
    EXPECT_EQ(GetCurrentLevelCap(), 50);
    SetTrainerFlag(TRAINER_PETREL_1_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 50);
    SetTrainerFlag(TRAINER_ARIANA_1_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 55);
    FlagSet(FLAG_DEFEATED_MAHOGANY_TOWN_GYM);
    EXPECT_EQ(GetCurrentLevelCap(), 57);
    SetTrainerFlag(TRAINER_PETREL_2_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 61);
    SetTrainerFlag(TRAINER_RIVAL_CYNDAQUIL_4_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 61);
    SetTrainerFlag(TRAINER_PROTON_2_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 61);
    SetTrainerFlag(TRAINER_ARIANA_2_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 61);
    SetTrainerFlag(TRAINER_ARCHER_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 61);
    FlagSet(FLAG_DEFEATED_BLACKTHORN_GYM);
    EXPECT_EQ(GetCurrentLevelCap(), 65);
    SetTrainerFlag(TRAINER_RIVAL_CYNDAQUIL_5_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 65);
    SetTrainerFlag(TRAINER_WILL_1_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 65);
    SetTrainerFlag(TRAINER_KOGA_1_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 65);
    SetTrainerFlag(TRAINER_BRUNO_1_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 65);
    SetTrainerFlag(TRAINER_KAREN_1_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 68);
}

TEST("Level cap: Rocket parties use legal evolution stages at their resolved level")
{
    EXPECT_EQ(GetFamilyRocketLegalSpecies(SPECIES_CROBAT, 20), SPECIES_ZUBAT);
    EXPECT_EQ(GetFamilyRocketLegalSpecies(SPECIES_WEEZING, 20), SPECIES_KOFFING);
    EXPECT_EQ(GetFamilyRocketLegalSpecies(SPECIES_RATICATE, 20), SPECIES_RATICATE);
    EXPECT_EQ(GetFamilyRocketLegalSpecies(SPECIES_SCOLIPEDE, 20), SPECIES_VENIPEDE);
    EXPECT_EQ(GetFamilyRocketLegalSpecies(SPECIES_TOXICROAK, 20), SPECIES_CROAGUNK);
    EXPECT_EQ(GetFamilyRocketLegalSpecies(SPECIES_MUK_ALOLA, 20), SPECIES_GRIMER_ALOLA);

    EXPECT_EQ(GetFamilyRocketLegalSpecies(SPECIES_CROBAT, 23), SPECIES_CROBAT);
    EXPECT_EQ(GetFamilyRocketLegalSpecies(SPECIES_SCOLIPEDE, 29), SPECIES_WHIRLIPEDE);
    EXPECT_EQ(GetFamilyRocketLegalSpecies(SPECIES_SCOLIPEDE, 30), SPECIES_SCOLIPEDE);
    EXPECT_EQ(GetFamilyRocketLegalSpecies(SPECIES_TOXICROAK, 37), SPECIES_TOXICROAK);
    EXPECT_EQ(GetFamilyRocketLegalSpecies(SPECIES_MUK_ALOLA, 38), SPECIES_MUK_ALOLA);
}

TEST("Level cap: Rocket runtime applies the dynamic level and legal species")
{
    SetLevelCapMode(1);
    FlagSet(FLAG_DEFEATED_VIOLET_GYM);
    CreateRandomMon(&gEnemyParty[0], SPECIES_CROBAT, 1);

    ApplyFamilyRocketPartyLevel(gEnemyParty, 1, TRAINER_PROTON_1_HNS);

    EXPECT_EQ(GetMonData(&gEnemyParty[0], MON_DATA_LEVEL), 20);
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
    EXPECT_EQ(GetCurrentLevelCap(), MAX_LEVEL);

    SetLevelCapMode(2);
    EXPECT_EQ(GetCurrentLevelCap(), MAX_LEVEL);

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
    EXPECT_EQ(GetCurrentLevelCap(), 95);

    FlagSet(FLAG_DEFEATED_CINNABAR_ISLAND_GYM);
    EXPECT_EQ(GetCurrentLevelCap(), 100);

    FlagSet(FLAG_DEFEATED_VIRIDIAN_GYM);
    SetTrainerFlag(TRAINER_WILL_2_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 100);
    SetTrainerFlag(TRAINER_KOGA_2_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 100);
    SetTrainerFlag(TRAINER_BRUNO_2_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 100);
    SetTrainerFlag(TRAINER_KAREN_2_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 100);
    SetTrainerFlag(TRAINER_LANCE_2_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 100);
}

TEST("Level cap: optional Gym orders and Rocket victories never decrease an acquired cap")
{
    static const u16 gyms[] = {FLAG_DEFEATED_VIOLET_GYM, FLAG_DEFEATED_AZALEA_TOWN_GYM, FLAG_DEFEATED_GOLDENROD_CITY_GYM, FLAG_DEFEATED_ECRUTEAK_CITY_GYM, FLAG_DEFEATED_CIANWOOD_GYM, FLAG_DEFEATED_OLIVINE_CITY_GYM, FLAG_DEFEATED_MAHOGANY_TOWN_GYM, FLAG_DEFEATED_BLACKTHORN_GYM};
    static const u16 rockets[] = {TRAINER_PROTON_1_HNS, TRAINER_PETREL_1_HNS, TRAINER_ARIANA_1_HNS, TRAINER_PETREL_2_HNS, TRAINER_PROTON_2_HNS, TRAINER_ARIANA_2_HNS, TRAINER_ARCHER_HNS};
    u32 mode = 1, reverse = FALSE;
    PARAMETRIZE { mode = 1; reverse = FALSE; }
    PARAMETRIZE { mode = 2; reverse = FALSE; }
    PARAMETRIZE { mode = 1; reverse = TRUE; }
    PARAMETRIZE { mode = 2; reverse = TRUE; }
    SetLevelCapMode(mode);
    u32 previous = GetCurrentLevelCap();
    for (u32 step = 0; step < ARRAY_COUNT(gyms); step++)
    {
        FlagSet(gyms[reverse ? ARRAY_COUNT(gyms) - 1 - step : step]);
        u32 current = GetCurrentLevelCap();
        EXPECT_GE(current, previous);
        previous = current;
        if (step < ARRAY_COUNT(rockets))
        {
            SetTrainerFlag(rockets[step]);
            current = GetCurrentLevelCap();
            EXPECT_GE(current, previous);
            previous = current;
        }
    }
    FlagSet(FLAG_IS_CHAMPION);
    EXPECT_GE(GetCurrentLevelCap(), previous);
    FlagSet(FLAG_DEFEATED_VIRIDIAN_GYM);
    EXPECT_EQ(GetCurrentLevelCap(), MAX_LEVEL);
    FlagSet(FLAG_IS_KANTO_CHAMPION);
    EXPECT_EQ(GetCurrentLevelCap(), MAX_LEVEL);
    FlagSet(FLAG_DEFEATED_RED);
    EXPECT_EQ(GetCurrentLevelCap(), MAX_LEVEL);
}

TEST("Level cap: a chain of Rocket admins shares one cap and one team range")
{
    SetLevelCapMode(1);
    FlagSet(FLAG_DEFEATED_VIOLET_GYM);
    FlagSet(FLAG_DEFEATED_AZALEA_TOWN_GYM);
    FlagSet(FLAG_DEFEATED_GOLDENROD_CITY_GYM);
    FlagSet(FLAG_DEFEATED_ECRUTEAK_CITY_GYM);
    FlagSet(FLAG_DEFEATED_CIANWOOD_GYM);
    FlagSet(FLAG_DEFEATED_OLIVINE_CITY_GYM);
    SetTrainerFlag(TRAINER_PROTON_1_HNS);
    SetTrainerFlag(TRAINER_RIVAL_CYNDAQUIL_2_HNS);
    SetTrainerFlag(TRAINER_RIVAL_CYNDAQUIL_3_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 54);
    SetTrainerFlag(TRAINER_PETREL_1_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 54);
    EXPECT_EQ(GetFamilyRocketTrainerLevel(TRAINER_ARIANA_1_HNS), 54);
    SetTrainerFlag(TRAINER_ARIANA_1_HNS);
    EXPECT_EQ(GetCurrentLevelCap(), 57);
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
    EXPECT_EQ(GetMonData(&gPlayerParty[0], MON_DATA_LEVEL), 18);
}
#endif
