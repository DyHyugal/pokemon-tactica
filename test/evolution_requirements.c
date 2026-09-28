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
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_USE, ITEM_DUSK_STONE, NULL, &canStopEvo, CHECK_EVO), SPECIES_NONE);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_USE, ITEM_AUSPICIOUS_ARMOR, NULL, &canStopEvo, CHECK_EVO), SPECIES_ARMAROUGE);

    InitEvolutionMon(&mon, SPECIES_APPLIN, 30, MON_MALE);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_USE, ITEM_LEAF_STONE, NULL, &canStopEvo, CHECK_EVO), SPECIES_NONE);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_USE, ITEM_SUN_STONE, NULL, &canStopEvo, CHECK_EVO), SPECIES_NONE);
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
    u8 friendship = (P_FRIENDSHIP_EVO_THRESHOLD >= GEN_8) ? 160 : 220;
    u8 previousFairyTypes = gSaveBlock3Ptr->challengeSettings.tx_Mode_Fairy_Types;
    u16 previousHour = SetTimeOfDay(DAY_HOUR_BEGIN);
    u32 i;

    gSaveBlock3Ptr->challengeSettings.tx_Mode_Fairy_Types = TRUE;
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
    gSaveBlock3Ptr->challengeSettings.tx_Mode_Fairy_Types = previousFairyTypes;
}


struct LinkingCordEvolutionCase
{
    u16 source;
    u16 target;
    u16 heldItem;
};

static const struct LinkingCordEvolutionCase sLinkingCordEvolutionCases[] =
{
    {SPECIES_POLIWHIRL, SPECIES_POLITOED, ITEM_KINGS_ROCK},
    {SPECIES_KADABRA, SPECIES_ALAKAZAM, ITEM_NONE},
    {SPECIES_MACHOKE, SPECIES_MACHAMP, ITEM_NONE},
    {SPECIES_GRAVELER, SPECIES_GOLEM, ITEM_NONE},
    {SPECIES_GRAVELER_ALOLA, SPECIES_GOLEM_ALOLA, ITEM_NONE},
    {SPECIES_SLOWPOKE, SPECIES_SLOWKING, ITEM_KINGS_ROCK},
    {SPECIES_HAUNTER, SPECIES_GENGAR, ITEM_NONE},
    {SPECIES_ONIX, SPECIES_STEELIX, ITEM_METAL_COAT},
    {SPECIES_RHYDON, SPECIES_RHYPERIOR, ITEM_PROTECTOR},
    {SPECIES_SEADRA, SPECIES_KINGDRA, ITEM_DRAGON_SCALE},
    {SPECIES_SCYTHER, SPECIES_SCIZOR, ITEM_METAL_COAT},
    {SPECIES_ELECTABUZZ, SPECIES_ELECTIVIRE, ITEM_ELECTIRIZER},
    {SPECIES_MAGMAR, SPECIES_MAGMORTAR, ITEM_MAGMARIZER},
    {SPECIES_PORYGON, SPECIES_PORYGON2, ITEM_UPGRADE},
    {SPECIES_PORYGON2, SPECIES_PORYGON_Z, ITEM_DUBIOUS_DISC},
    {SPECIES_FEEBAS, SPECIES_MILOTIC, ITEM_PRISM_SCALE},
    {SPECIES_DUSCLOPS, SPECIES_DUSKNOIR, ITEM_REAPER_CLOTH},
    {SPECIES_CLAMPERL, SPECIES_HUNTAIL, ITEM_DEEP_SEA_TOOTH},
    {SPECIES_CLAMPERL, SPECIES_GOREBYSS, ITEM_DEEP_SEA_SCALE},
    {SPECIES_BOLDORE, SPECIES_GIGALITH, ITEM_NONE},
    {SPECIES_GURDURR, SPECIES_CONKELDURR, ITEM_NONE},
    {SPECIES_KARRABLAST, SPECIES_ESCAVALIER, ITEM_NONE},
    {SPECIES_SHELMET, SPECIES_ACCELGOR, ITEM_NONE},
    {SPECIES_SPRITZEE, SPECIES_AROMATISSE, ITEM_SACHET},
    {SPECIES_SWIRLIX, SPECIES_SLURPUFF, ITEM_WHIPPED_DREAM},
    {SPECIES_PHANTUMP, SPECIES_TREVENANT, ITEM_NONE},
    {SPECIES_PUMPKABOO_AVERAGE, SPECIES_GOURGEIST_AVERAGE, ITEM_NONE},
    {SPECIES_PUMPKABOO_SMALL, SPECIES_GOURGEIST_SMALL, ITEM_NONE},
    {SPECIES_PUMPKABOO_LARGE, SPECIES_GOURGEIST_LARGE, ITEM_NONE},
    {SPECIES_PUMPKABOO_SUPER, SPECIES_GOURGEIST_SUPER, ITEM_NONE},
};

TEST("Evolution requirements: canonical trade families expose no legacy trade route")
{
    const struct LinkingCordEvolutionCase *testCase = NULL;
    const struct Evolution *evos;
    u32 i;

    for (i = 0; i < ARRAY_COUNT(sLinkingCordEvolutionCases); i++)
        PARAMETRIZE { testCase = &sLinkingCordEvolutionCases[i]; }

    evos = GetSpeciesEvolutions(testCase->source);
    EXPECT_NE(evos, NULL);
    for (i = 0; evos[i].method != EVOLUTIONS_END; i++)
        EXPECT_NE(evos[i].method, EVO_TRADE);
}

TEST("Evolution requirements: all canonical trade families use Linking Cord")
{
    const struct LinkingCordEvolutionCase *testCase = NULL;
    struct Pokemon mon;
    bool32 canStopEvo = TRUE;
    u16 heldItem;
    u32 i;

    for (i = 0; i < ARRAY_COUNT(sLinkingCordEvolutionCases); i++)
        PARAMETRIZE { testCase = &sLinkingCordEvolutionCases[i]; }

    heldItem = testCase->heldItem;
    InitEvolutionMon(&mon, testCase->source, 50, MON_GENDER_RANDOM);

    if (heldItem != ITEM_NONE)
    {
        EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_USE, ITEM_LINKING_CORD, NULL, &canStopEvo, CHECK_EVO), SPECIES_NONE);
        SetMonData(&mon, MON_DATA_HELD_ITEM, &heldItem);
        EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_USE, heldItem, NULL, &canStopEvo, CHECK_EVO), SPECIES_NONE);
    }

    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_USE, ITEM_LINKING_CORD, NULL, &canStopEvo, CHECK_EVO), testCase->target);
}


struct ImpossibleEvolutionCase
{
    u16 source;
    u16 target;
};

static const struct ImpossibleEvolutionCase sImpossibleEvolutionCases[] =
{
    {SPECIES_INKAY, SPECIES_MALAMAR},
    {SPECIES_FINIZEN, SPECIES_PALAFIN_ZERO},
    {SPECIES_YAMASK_GALAR, SPECIES_RUNERIGUS},
    {SPECIES_URSARING, SPECIES_URSALUNA},
    {SPECIES_MELTAN, SPECIES_MELMETAL},
};

TEST("Evolution requirements: unsupported official mechanics use Linking Cord")
{
    const struct ImpossibleEvolutionCase *testCase = NULL;
    struct Pokemon mon;
    bool32 canStopEvo = TRUE;
    u32 i;

    for (i = 0; i < ARRAY_COUNT(sImpossibleEvolutionCases); i++)
        PARAMETRIZE { testCase = &sImpossibleEvolutionCases[i]; }

    InitEvolutionMon(&mon, testCase->source, 60, MON_GENDER_RANDOM);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_NORMAL, ITEM_NONE, NULL, &canStopEvo, CHECK_EVO), SPECIES_NONE);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_USE, ITEM_LINKING_CORD, NULL, &canStopEvo, CHECK_EVO), testCase->target);
}

TEST("Evolution requirements: supported atypical mechanics remain native")
{
    const struct Evolution *evos;

    evos = GetSpeciesEvolutions(SPECIES_FARFETCHD_GALAR);
    EXPECT_EQ(evos[0].method, EVO_BATTLE_END);
    EXPECT_EQ(evos[0].targetSpecies, SPECIES_SIRFETCHD);

    evos = GetSpeciesEvolutions(SPECIES_PAWMO);
    EXPECT_EQ(evos[0].method, EVO_LEVEL);
    EXPECT_EQ(evos[0].targetSpecies, SPECIES_PAWMOT);

    evos = GetSpeciesEvolutions(SPECIES_BRAMBLIN);
    EXPECT_EQ(evos[0].method, EVO_LEVEL);
    EXPECT_EQ(evos[0].targetSpecies, SPECIES_BRAMBLEGHAST);

    evos = GetSpeciesEvolutions(SPECIES_RELLOR);
    EXPECT_EQ(evos[0].method, EVO_LEVEL);
    EXPECT_EQ(evos[0].targetSpecies, SPECIES_RABSCA);

    evos = GetSpeciesEvolutions(SPECIES_PRIMEAPE);
    EXPECT_EQ(evos[0].method, EVO_LEVEL);
    EXPECT_EQ(evos[0].targetSpecies, SPECIES_ANNIHILAPE);
}

TEST("Evolution requirements: existing level evolutions still work")
{
    struct Pokemon mon;
    bool32 canStopEvo = TRUE;

    InitEvolutionMon(&mon, SPECIES_BULBASAUR, 15, MON_MALE);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_NORMAL, ITEM_NONE, NULL, &canStopEvo, CHECK_EVO), SPECIES_NONE);
    InitEvolutionMon(&mon, SPECIES_BULBASAUR, 16, MON_MALE);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_NORMAL, ITEM_NONE, NULL, &canStopEvo, CHECK_EVO), SPECIES_IVYSAUR);

}
#endif
