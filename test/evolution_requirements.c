#include "global.h"
#include "constants/rtc.h"
#include "constants/moves.h"
#include "overworld.h"
#include "pokemon.h"
#include "test/test.h"

#if IS_HNS
static void InitEvolutionMon(struct Pokemon *mon, u16 species, u8 level, u8 gender)
{
    u32 personality = GetMonPersonality(species, gender, NATURE_HARDY, 0);
    CreateMon(mon, species, level, personality, OTID_STRUCT_PLAYER_ID);
}

TEST("Evolution requirements: item evolutions have no artificial family-stage minimum")
{
    struct Pokemon mon;
    bool32 canStopEvo = TRUE;

    InitEvolutionMon(&mon, SPECIES_VULPIX_ALOLA, 1, MON_FEMALE);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_USE, ITEM_ICE_STONE, NULL, &canStopEvo, CHECK_EVO), SPECIES_NINETALES_ALOLA);

    InitEvolutionMon(&mon, SPECIES_EELEKTRIK, 1, MON_MALE);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_USE, ITEM_THUNDER_STONE, NULL, &canStopEvo, CHECK_EVO), SPECIES_EELEKTROSS);
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
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_NORMAL, ITEM_NONE, NULL, &canStopEvo, CHECK_EVO), SPECIES_NONE);
    InitEvolutionMon(&mon, SPECIES_SNORUNT, 42, MON_MALE);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_NORMAL, ITEM_NONE, NULL, &canStopEvo, CHECK_EVO), SPECIES_GLALIE);
}

TEST("Evolution requirements: obsolete convenience items no longer bypass official methods")
{
    struct Pokemon mon;
    bool32 canStopEvo = TRUE;

    InitEvolutionMon(&mon, SPECIES_CHARCADET, 30, MON_MALE);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_USE, ITEM_FIRE_STONE, NULL, &canStopEvo, CHECK_EVO), SPECIES_NONE);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_USE, ITEM_AUSPICIOUS_ARMOR, NULL, &canStopEvo, CHECK_EVO), SPECIES_ARMAROUGE);

    InitEvolutionMon(&mon, SPECIES_APPLIN, 30, MON_MALE);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_USE, ITEM_LEAF_STONE, NULL, &canStopEvo, CHECK_EVO), SPECIES_NONE);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_USE, ITEM_TART_APPLE, NULL, &canStopEvo, CHECK_EVO), SPECIES_FLAPPLE);

    InitEvolutionMon(&mon, SPECIES_DURALUDON, 30, MON_MALE);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_USE, ITEM_UP_GRADE, NULL, &canStopEvo, CHECK_EVO), SPECIES_NONE);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_USE, ITEM_METAL_ALLOY, NULL, &canStopEvo, CHECK_EVO), SPECIES_ARCHALUDON);

    InitEvolutionMon(&mon, SPECIES_BISHARP, 60, MON_MALE);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_USE, ITEM_KINGS_ROCK, NULL, &canStopEvo, CHECK_EVO), SPECIES_NONE);
}

TEST("Evolution requirements: Eevee uses current friendship and move conditions")
{
    struct Pokemon mon;
    bool32 canStopEvo = TRUE;
    u8 friendship = FRIENDSHIP_EVO_THRESHOLD;
    u16 previousHour = SetTimeOfDay(DAY_HOUR_BEGIN);
    u32 i;

    InitEvolutionMon(&mon, SPECIES_EEVEE, 30, MON_FEMALE);
    SetMonData(&mon, MON_DATA_FRIENDSHIP, &friendship);
    for (i = 0; i < MAX_MON_MOVES; i++)
        SetMonMoveSlot(&mon, MOVE_NONE, i);
    SetMonMoveSlot(&mon, MOVE_BABY_DOLL_EYES, 0);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_NORMAL, ITEM_NONE, NULL, &canStopEvo, CHECK_EVO), SPECIES_SYLVEON);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_USE, ITEM_SHINY_STONE, NULL, &canStopEvo, CHECK_EVO), SPECIES_NONE);

    InitEvolutionMon(&mon, SPECIES_EEVEE, 30, MON_FEMALE);
    SetMonData(&mon, MON_DATA_FRIENDSHIP, &friendship);
    for (i = 0; i < MAX_MON_MOVES; i++)
        SetMonMoveSlot(&mon, MOVE_NONE, i);
    SetMonMoveSlot(&mon, MOVE_TACKLE, 0);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_NORMAL, ITEM_NONE, NULL, &canStopEvo, CHECK_EVO), SPECIES_ESPEON);

    SetTimeOfDay(NIGHT_HOUR_BEGIN);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_NORMAL, ITEM_NONE, NULL, &canStopEvo, CHECK_EVO), SPECIES_UMBREON);

    SetTimeOfDay(previousHour);
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
