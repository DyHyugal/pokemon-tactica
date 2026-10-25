#include "global.h"
#include "data.h"
#include "pokemon.h"
#include "event_data.h"
#include "battle_setup.h"
#include "caps.h"
#include "difficulty.h"
#include "tactica_progression.h"
#include "constants/maps.h"
#include "test/test.h"

#if IS_HNS
TEST("Tactica ordinary trainers: route floors preserve the authored party spread")
{
    const struct Trainer *trainer = &gTrainers[DIFFICULTY_NORMAL][TRAINER_WADE_HNS];
    struct TrainerMon first = trainer->party[0];
    struct TrainerMon second = trainer->party[1];
    InitEventData();
    EXPECT_EQ(GetTacticaStoryTrainerFloor(), 0);
    EXPECT_EQ(first.lvl, 4);
    EXPECT_EQ(second.lvl, 5);
    EXPECT(ResolveTacticaOrdinaryTrainerMon(TRAINER_WADE_HNS, MAP_ROUTE31_HNS, trainer, &first));
    EXPECT(ResolveTacticaOrdinaryTrainerMon(TRAINER_WADE_HNS, MAP_ROUTE31_HNS, trainer, &second));
    EXPECT_EQ(first.lvl, 7);
    EXPECT_EQ(second.lvl - first.lvl, trainer->party[1].lvl - trainer->party[0].lvl);
}

TEST("Tactica ordinary trainers: story raises old routes independently of player caps")
{
    const struct Trainer *trainer = &gTrainers[DIFFICULTY_NORMAL][TRAINER_WADE_HNS];
    InitEventData();
    FlagSet(FLAG_DEFEATED_GOLDENROD_CITY_GYM);
    for (u32 capMode = 0; capMode <= 2; capMode++)
    {
        struct TrainerMon mon = trainer->party[0];
        gSaveBlock3Ptr->challengeSettings.tx_Challenges_LevelCap = capMode;
        EXPECT(ResolveTacticaOrdinaryTrainerMon(TRAINER_WADE_HNS, MAP_ROUTE30_HNS, trainer, &mon));
        EXPECT_EQ(mon.lvl, 29);
        EXPECT_EQ(mon.species, SPECIES_BEEDRILL);
    }
}

TEST("Tactica ordinary trainers: cave floors apply before a new Gym victory")
{
    const struct Trainer *trainer = &gTrainers[DIFFICULTY_NORMAL][TRAINER_WADE_HNS];
    struct TrainerMon mon = trainer->party[0];
    InitEventData();
    EXPECT(ResolveTacticaOrdinaryTrainerMon(TRAINER_WADE_HNS, MAP_UNION_CAVE_1F_HNS, trainer, &mon));
    EXPECT_EQ(mon.lvl, 16);
    EXPECT_EQ(mon.species, SPECIES_BEEDRILL);
}

TEST("Tactica ordinary trainers: completed Rocket does not compound a new Gym plus two")
{
    InitEventData();
    FlagSet(FLAG_DEFEATED_BLACKTHORN_GYM);
    SetTrainerFlag(TRAINER_PROTON_1_HNS);
    SetTrainerFlag(TRAINER_ARIANA_2_HNS);
    EXPECT_EQ(GetTacticaStoryTrainerFloor(), 61);
}

TEST("Tactica ordinary trainers: wide inherited spreads do not overlevel Kanto preparation")
{
    const struct Trainer *trainer = &gTrainers[DIFFICULTY_NORMAL][TRAINER_STEPHEN_HNS];
    InitEventData();
    FlagSet(FLAG_IS_CHAMPION);
    FlagSet(FLAG_DEFEATED_VERMILION_GYM);
    EXPECT_EQ(GetTacticaStoryTrainerFloor(), 75);
    for (u32 i = 0; i < trainer->partySize; i++)
    {
        struct TrainerMon mon = trainer->party[i];
        EXPECT(ResolveTacticaOrdinaryTrainerMon(TRAINER_STEPHEN_HNS, MAP_ROUTE12_HNS, trainer, &mon));
        EXPECT_EQ(mon.lvl, 75);
    }
}

TEST("Tactica ordinary trainers: protected bosses are never altered")
{
    static const u16 ids[] = {
        TRAINER_FALKNER_1_HNS, TRAINER_PROTON_1_HNS, TRAINER_RIVAL_CHIKORITA_1_HNS,
        TRAINER_WILL_1_HNS, TRAINER_LANCE_1_HNS, TRAINER_RED_HNS,
    };
    InitEventData();
    FlagSet(FLAG_IS_KANTO_CHAMPION);
    for (u32 i = 0; i < ARRAY_COUNT(ids); i++)
    {
        const struct Trainer *trainer = &gTrainers[DIFFICULTY_NORMAL][ids[i]];
        struct TrainerMon mon = trainer->party[0];
        EXPECT(!ResolveTacticaOrdinaryTrainerMon(ids[i], MAP_ROUTE26_HNS, trainer, &mon));
        EXPECT_EQ(mon.lvl, trainer->party[0].lvl);
        EXPECT_EQ(mon.species, trainer->party[0].species);
    }
}

TEST("Tactica ordinary trainers: NORMAL and HARD share levels, and late parties stop at 100")
{
    enum DifficultyLevel savedDifficulty = GetCurrentDifficultyLevel();
    InitEventData();
    FlagSet(FLAG_IS_KANTO_CHAMPION);
    for (u32 difficulty = DIFFICULTY_NORMAL; difficulty <= DIFFICULTY_HARD; difficulty++)
    {
        SetCurrentDifficultyLevel(difficulty);
        const struct Trainer *trainer = GetTrainerStructFromId(TRAINER_WADE_HNS);
        for (u32 i = 0; i < trainer->partySize; i++)
        {
            struct TrainerMon mon = trainer->party[i];
            EXPECT(ResolveTacticaOrdinaryTrainerMon(TRAINER_WADE_HNS, MAP_ROUTE30_HNS, trainer, &mon));
            EXPECT_EQ(mon.lvl, MAX_LEVEL);
        }
    }
    SetCurrentDifficultyLevel(savedDifficulty);
}
TEST("Tactica ordinary trainers: move eligibility rejects impossible moves instead of validating itself")
{
    gSaveBlock3Ptr->challengeSettings.tx_Mode_Modern_Moves = 1;
    EXPECT(IsTacticaMoveLegalAtLevel(SPECIES_MAGIKARP, 5, MOVE_SPLASH));
    EXPECT(!IsTacticaMoveLegalAtLevel(SPECIES_MAGIKARP, 5, MOVE_FLAMETHROWER));
    EXPECT(!IsTacticaMoveLegalAtLevel(SPECIES_MAGIKARP, 5, MOVE_NONE));
}
#endif
