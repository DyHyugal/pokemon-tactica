#include "global.h"
#include "battle.h"
#include "battle_main.h"
#include "data.h"
#include "event_data.h"
#include "difficulty.h"
#include "caps.h"
#include "item.h"
#include "pokemon.h"
#include "tactica_progression.h"
#include "test/test.h"
#include "constants/hold_effects.h"
#include "constants/opponents.h"
#include "constants/flags.h"
#include "constants/vars.h"
#include "family_starter.h"

#if IS_HNS
static bool32 IsBossClass(u32 trainerClass)
{
    switch (trainerClass)
    {
    case TRAINER_CLASS_LEADER_HNS:
    case TRAINER_CLASS_LEADER_KANTO_HNS:
    case TRAINER_CLASS_RIVAL_HNS:
    case TRAINER_CLASS_ROCKET_ADMIN_HNS:
    case TRAINER_CLASS_ELITE_FOUR_HNS:
    case TRAINER_CLASS_CHAMPION_HNS:
    case TRAINER_CLASS_PKMN_TRAINER_1_HNS:
    case TRAINER_CLASS_MYSTERY_MAN_HNS:
    case TRAINER_CLASS_PROFESSOR_HNS:
    case TRAINER_CLASS_DEVELOPER_HNS:
    case TRAINER_CLASS_SALON_MAIDEN_HNS:
    case TRAINER_CLASS_DOME_ACE_HNS:
    case TRAINER_CLASS_PALACE_MAVEN_HNS:
    case TRAINER_CLASS_ARENA_TYCOON_HNS:
    case TRAINER_CLASS_FACTORY_HEAD_HNS:
    case TRAINER_CLASS_PIKE_QUEEN_HNS:
    case TRAINER_CLASS_PYRAMID_KING_HNS:
    case TRAINER_CLASS_KIMONO_GIRL_HNS:
        return TRUE;
    default:
        return FALSE;
    }
}

static u32 AuditBoss(u32 trainerId, u32 difficulty)
{
    const struct Trainer *trainer = &gTrainers[difficulty][trainerId];
    u32 errors = 0;
    for (u32 slot = 0; slot < trainer->partySize; slot++)
    {
        const struct TrainerMon *mon = &trainer->party[slot];
        bool32 legalAbility = mon->ability == ABILITY_NONE;
        EXPECT_LT(mon->species, NUM_SPECIES);
        EXPECT_NE(gSpeciesInfo[mon->species].baseHP, 0);
        EXPECT(!gSpeciesInfo[mon->species].isMegaEvolution);
        for (u32 a = 0; a < NUM_ABILITY_SLOTS; a++)
            legalAbility |= gSpeciesInfo[mon->species].abilities[a] == mon->ability;
        if (!legalAbility)
        {
            Test_MgbaPrintf("illegal ability: trainer=%d difficulty=%d slot=%d species=%d ability=%d", trainerId, difficulty, slot + 1, mon->species, mon->ability);
            errors++;
        }
        if (GetItemHoldEffect(mon->heldItem) == HOLD_EFFECT_MEGA_STONE)
        {
            struct FormChangeContext ctx = {
                .method = FORM_CHANGE_BATTLE_MEGA_EVOLUTION_ITEM,
                .currentSpecies = mon->species,
                .heldItem = mon->heldItem,
                .ability = mon->ability,
            };
            u32 target = GetFormChangeTargetSpecies_Internal(ctx);
            EXPECT_NE(target, SPECIES_NONE);
            EXPECT(gSpeciesInfo[target].isMegaEvolution);
            u32 abilityNum = 0;
            if (mon->ability != ABILITY_NONE)
                while (abilityNum < NUM_ABILITY_SLOTS && gSpeciesInfo[mon->species].abilities[abilityNum] != mon->ability)
                    abilityNum++;
            EXPECT_NE(GetAbilityBySpecies(target, abilityNum), ABILITY_NONE);
        }
    }
    return errors;
}

TEST("Tactica boss legality: all production boss sets have legal entry abilities")
{
    u32 errors = 0;
    InitEventData();
    memset(&gSaveBlock3Ptr->challengeSettings, 0, sizeof(struct ChallengeSettings));
    gSaveBlock3Ptr->challengeSettings.tx_Challenges_OneTypeChallenge = 31;
    for (u32 id = 15; id < TRAINERS_COUNT; id++)
        for (u32 d = 0; d < DIFFICULTY_COUNT; d++)
            if (gTrainers[d][id].party != NULL && IsBossClass(gTrainers[d][id].trainerClass))
                errors += AuditBoss(id, d);
    EXPECT_EQ(errors, 0);
}
TEST("Tactica boss legality: Major Bob party creation is safe in Normal and Hard")
{
    u32 difficulty;
    PARAMETRIZE { difficulty = DIFFICULTY_NORMAL; }
    PARAMETRIZE { difficulty = DIFFICULTY_HARD; }
    InitEventData();
    memset(&gSaveBlock3Ptr->challengeSettings, 0, sizeof(struct ChallengeSettings));
    gSaveBlock3Ptr->challengeSettings.tx_Challenges_OneTypeChallenge = 31;
    EXPECT_EQ(CreateNPCTrainerPartyFromTrainer(gEnemyParty, &gTrainers[difficulty][TRAINER_LTSURGE_HNS], TRUE, BATTLE_TYPE_TRAINER), 6);
    EXPECT_EQ(GetMonData(&gEnemyParty[5], MON_DATA_SPECIES), SPECIES_MANECTRIC);
    EXPECT_EQ(GetMonAbility(&gEnemyParty[5]), ABILITY_LIGHTNING_ROD);
    EXPECT_EQ(GetMonData(&gEnemyParty[5], MON_DATA_MOVE4), MOVE_SIGNAL_BEAM);
    EXPECT_EQ(GetMonData(&gEnemyParty[1], MON_DATA_SPECIES), SPECIES_RAICHU);
    EXPECT_EQ(GetMonData(&gEnemyParty[1], MON_DATA_HELD_ITEM), ITEM_RAICHUNITE_Y);
}

TEST("Tactica boss legality: dynamic rival entries keep legal base abilities")
{
    static const u16 starters[] = {SPECIES_TORCHIC, SPECIES_MUDKIP, SPECIES_TREECKO, SPECIES_ELEKID, SPECIES_DRILBUR, SPECIES_DARUMAKA_GALAR};
    static const u8 counts[] = {1, 4, 6, 6, 6, 6, 6};
    static const u8 levels[] = {18, 32, 38, 64, 67, 95, 95};
    u32 savedDifficulty = GetCurrentDifficultyLevel();
    for (u32 d = DIFFICULTY_NORMAL; d <= DIFFICULTY_HARD; d++)
        for (u32 c = 0; c < ARRAY_COUNT(starters); c++)
            for (u32 f = 0; f < ARRAY_COUNT(counts); f++)
            {
                InitEventData();
                memset(&gSaveBlock3Ptr->challengeSettings, 0, sizeof(struct ChallengeSettings));
                gSaveBlock3Ptr->challengeSettings.tx_Challenges_OneTypeChallenge = 31;
                gSaveBlock3Ptr->challengeSettings.tx_Mode_Modern_Moves = 1;
                SetCurrentDifficultyLevel(d);
                VarSet(VAR_FAMILY_RIVAL_SPECIES, starters[c]);
                for (u32 slot = 0; slot < counts[f]; slot++)
                {
                    struct TrainerMon mon = {.lvl = levels[f]};
                    EXPECT(FamilyStarter_ResolveRivalMon(TRAINER_RIVAL_CYNDAQUIL_1_HNS + f, slot, &mon));
                    bool32 legal = mon.ability == ABILITY_NONE;
                    for (u32 a = 0; a < NUM_ABILITY_SLOTS; a++)
                        legal |= gSpeciesInfo[mon.species].abilities[a] == mon.ability;
                    EXPECT(legal);
                    if (GetItemHoldEffect(mon.heldItem) == HOLD_EFFECT_MEGA_STONE)
                    {
                        struct FormChangeContext ctx = {
                            .method = FORM_CHANGE_BATTLE_MEGA_EVOLUTION_ITEM,
                            .currentSpecies = mon.species,
                            .heldItem = mon.heldItem,
                            .ability = mon.ability,
                        };
                        u32 target = GetFormChangeTargetSpecies_Internal(ctx);
                        EXPECT_NE(target, SPECIES_NONE);
                        EXPECT(gSpeciesInfo[target].isMegaEvolution);
                    }
                }
            }
    SetCurrentDifficultyLevel(savedDifficulty);
}
TEST("Tactica boss legality: dynamic Rocket parties keep legal abilities and adapted moves")
{
    static const u16 trainers[] = {TRAINER_PROTON_1_HNS, TRAINER_PROTON_2_HNS, TRAINER_PETREL_1_HNS, TRAINER_PETREL_2_HNS, TRAINER_ARIANA_1_HNS, TRAINER_ARIANA_2_HNS, TRAINER_ARCHER_HNS};
    static const u16 flags[] = {FLAG_DEFEATED_VIOLET_GYM, FLAG_DEFEATED_AZALEA_TOWN_GYM, FLAG_DEFEATED_GOLDENROD_CITY_GYM, FLAG_DEFEATED_ECRUTEAK_CITY_GYM, FLAG_DEFEATED_CIANWOOD_GYM, FLAG_DEFEATED_OLIVINE_CITY_GYM, FLAG_DEFEATED_MAHOGANY_TOWN_GYM, FLAG_DEFEATED_BLACKTHORN_GYM};
    u32 difficulty = DIFFICULTY_NORMAL, stage = 0, trainerId = TRAINER_PROTON_1_HNS;
    for (u32 d = DIFFICULTY_NORMAL; d <= DIFFICULTY_HARD; d++)
        for (u32 f = 0; f <= ARRAY_COUNT(flags); f++)
            for (u32 t = 0; t < ARRAY_COUNT(trainers); t++)
                PARAMETRIZE { difficulty = d; stage = f; trainerId = trainers[t]; }
    u32 savedDifficulty = GetCurrentDifficultyLevel();
    InitEventData();
    memset(&gSaveBlock3Ptr->challengeSettings, 0, sizeof(struct ChallengeSettings));
    gSaveBlock3Ptr->challengeSettings.tx_Challenges_OneTypeChallenge = 31;
    gSaveBlock3Ptr->challengeSettings.tx_Mode_Modern_Moves = 1;
    SetCurrentDifficultyLevel(difficulty);
    for (u32 f = 0; f < stage; f++)
        FlagSet(flags[f]);
    const struct Trainer *trainer = &gTrainers[GetTrainerDifficultyLevel(trainerId)][trainerId];
    u32 count = CreateNPCTrainerPartyFromTrainer(gEnemyParty, trainer, TRUE, BATTLE_TYPE_TRAINER);
    ApplyFamilyRocketPartyLevel(gEnemyParty, count, trainerId);
    for (u32 slot = 0; slot < count; slot++)
    {
        u32 species = GetMonData(&gEnemyParty[slot], MON_DATA_SPECIES);
        u32 ability = GetMonAbility(&gEnemyParty[slot]);
        u32 level = GetMonData(&gEnemyParty[slot], MON_DATA_LEVEL);
        bool32 legal = FALSE;
        for (u32 a = 0; a < NUM_ABILITY_SLOTS; a++)
            legal |= gSpeciesInfo[species].abilities[a] == ability;
        EXPECT(legal);
        EXPECT_EQ(level, GetFamilyRocketTrainerMonLevel(trainerId, slot, count));
        for (u32 move = 0; move < MAX_MON_MOVES; move++)
        {
            u32 moveId = GetMonData(&gEnemyParty[slot], MON_DATA_MOVE1 + move);
            if (moveId != MOVE_NONE)
                EXPECT(IsTacticaMoveLegalAtLevel(species, level, moveId));
        }
    }
    SetCurrentDifficultyLevel(savedDifficulty);
}
#endif
