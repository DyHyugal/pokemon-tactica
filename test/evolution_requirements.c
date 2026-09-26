#include "global.h"
#include "constants/rtc.h"
#include "overworld.h"
#include "pokemon.h"
#include "test/test.h"

#if IS_HNS
static void InitEvolutionMon(struct Pokemon *mon, u16 species, u8 level, u8 gender)
{
    u32 personality = GetMonPersonality(species, gender, NATURE_HARDY, 0);
    CreateMon(mon, species, level, personality, OTID_STRUCT_PLAYER_ID);
}

TEST("Evolution requirements: item evolutions reject levels below their family threshold")
{
    struct Pokemon mon;
    bool32 canStopEvo = TRUE;
    u16 previousHour = SetTimeOfDay(DAY_HOUR_BEGIN);

    InitEvolutionMon(&mon, SPECIES_HAPPINY, 15, MON_FEMALE);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_USE, ITEM_OVAL_STONE, NULL, &canStopEvo, CHECK_EVO), SPECIES_NONE);
    InitEvolutionMon(&mon, SPECIES_VULPIX_ALOLA, 29, MON_FEMALE);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_USE, ITEM_ICE_STONE, NULL, &canStopEvo, CHECK_EVO), SPECIES_NONE);
    InitEvolutionMon(&mon, SPECIES_ELECTABUZZ, 35, MON_MALE);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_USE, ITEM_ELECTIRIZER, NULL, &canStopEvo, CHECK_EVO), SPECIES_NONE);

    SetTimeOfDay(previousHour);
}

TEST("Evolution requirements: item evolutions work at levels 16, 30, and 36")
{
    struct Pokemon mon;
    bool32 canStopEvo = TRUE;
    u16 previousHour = SetTimeOfDay(DAY_HOUR_BEGIN);

    InitEvolutionMon(&mon, SPECIES_HAPPINY, 16, MON_FEMALE);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_USE, ITEM_OVAL_STONE, NULL, &canStopEvo, CHECK_EVO), SPECIES_CHANSEY);
    InitEvolutionMon(&mon, SPECIES_VULPIX_ALOLA, 30, MON_FEMALE);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_USE, ITEM_ICE_STONE, NULL, &canStopEvo, CHECK_EVO), SPECIES_NINETALES_ALOLA);
    InitEvolutionMon(&mon, SPECIES_ELECTABUZZ, 36, MON_MALE);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_USE, ITEM_ELECTIRIZER, NULL, &canStopEvo, CHECK_EVO), SPECIES_ELECTIVIRE);

    SetTimeOfDay(previousHour);
}

TEST("Evolution requirements: branched item evolutions keep the selected branch")
{
    struct Pokemon mon;
    bool32 canStopEvo = TRUE;

    InitEvolutionMon(&mon, SPECIES_CHARCADET, 30, MON_MALE);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_USE, ITEM_AUSPICIOUS_ARMOR, NULL, &canStopEvo, CHECK_EVO), SPECIES_ARMAROUGE);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_USE, ITEM_MALICIOUS_ARMOR, NULL, &canStopEvo, CHECK_EVO), SPECIES_CERULEDGE);

    InitEvolutionMon(&mon, SPECIES_SNORUNT, 30, MON_FEMALE);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_USE, ITEM_DAWN_STONE, NULL, &canStopEvo, CHECK_EVO), SPECIES_FROSLASS);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_NORMAL, ITEM_NONE, NULL, &canStopEvo, CHECK_EVO), SPECIES_GLALIE);
}

TEST("Evolution requirements: existing level and trade replacement evolutions still work")
{
    struct Pokemon mon;
    bool32 canStopEvo = TRUE;

    InitEvolutionMon(&mon, SPECIES_BULBASAUR, 15, MON_MALE);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_NORMAL, ITEM_NONE, NULL, &canStopEvo, CHECK_EVO), SPECIES_NONE);
    InitEvolutionMon(&mon, SPECIES_BULBASAUR, 16, MON_MALE);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_NORMAL, ITEM_NONE, NULL, &canStopEvo, CHECK_EVO), SPECIES_IVYSAUR);

    InitEvolutionMon(&mon, SPECIES_KADABRA, 41, MON_MALE);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_NORMAL, ITEM_NONE, NULL, &canStopEvo, CHECK_EVO), SPECIES_NONE);
    InitEvolutionMon(&mon, SPECIES_KADABRA, 42, MON_MALE);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_NORMAL, ITEM_NONE, NULL, &canStopEvo, CHECK_EVO), SPECIES_ALAKAZAM);
}
#endif
