#include "global.h"
#include "family_starter.h"
#include "pokemon.h"
#include "daycare.h"
#include "pokemon_storage_system.h"
#include "starter_choose.h"
#include "script_pokemon_util.h"
#include "script_menu.h"
#include "list_menu.h"
#include "event_data.h"
#include "string_util.h"
#include "malloc.h"
#include "item.h"
#include "challenge_menu.h"
#include "randomizer.h"
#include "random.h"
#include "constants/vars.h"
#include "constants/flags.h"
#include "constants/items.h"
#include "constants/pokeball.h"

static EWRAM_DATA struct Pokemon sFamilyStarterPreview = {0};
static EWRAM_DATA u16 sFamilyStarterPreviewSpecies = SPECIES_NONE;
static EWRAM_DATA u16 sFamilyStarterPreviewPreference = SPECIES_NONE;
static EWRAM_DATA bool8 sFamilyStarterPreviewIsEgg = FALSE;
#define FAMILY_STARTER_MAX_PENDING_ITEMS 3

static EWRAM_DATA u16 sFamilyStarterPendingItems[FAMILY_STARTER_MAX_PENDING_ITEMS] = {0};
static EWRAM_DATA bool8 sFamilyStarterPendingItemsInPC[FAMILY_STARTER_MAX_PENDING_ITEMS] = {0};
static EWRAM_DATA u8 sFamilyStarterPendingItemCount = 0;
static EWRAM_DATA u8 sFamilyStarterPendingItemRead = 0;

static u16 ValidEvolutionPreference(u16 species, u16 preference);

// Menu specification only. Existing starter, species, evolution and item
// tables are deliberately not replaced or altered.
static const u16 sMenuSpecies[7][5] = {
    {SPECIES_CHARMANDER, SPECIES_TORCHIC, SPECIES_CHIMCHAR, SPECIES_CYNDAQUIL, SPECIES_CHARCADET},
    {SPECIES_MUDKIP, SPECIES_PIPLUP, SPECIES_FROAKIE, SPECIES_OSHAWOTT, SPECIES_HORSEA},
    {SPECIES_BULBASAUR, SPECIES_TREECKO, SPECIES_SNIVY, SPECIES_SPRIGATITO, SPECIES_GROOKEY},
    {SPECIES_ELEKID, SPECIES_MAGNEMITE, SPECIES_PAWMI, SPECIES_WATTREL, SPECIES_PICHU},
    {SPECIES_GLIGAR, SPECIES_DRILBUR, SPECIES_SANDILE, SPECIES_GOLETT, SPECIES_SANDYGAST},
    {SPECIES_VULPIX_ALOLA, SPECIES_SNORUNT, SPECIES_SWINUB, SPECIES_DARUMAKA_GALAR, SPECIES_VANILLITE},
    {SPECIES_EEVEE},
};

enum FamilyStarterCategory
{
    FAMILY_FIRE,
    FAMILY_WATER,
    FAMILY_GRASS,
    FAMILY_ELECTRIC,
    FAMILY_GROUND,
    FAMILY_ICE,
    FAMILY_EEVEE,
};

struct RivalCounterCategories
{
    u8 count;
    u8 categories[3];
};

static const struct RivalCounterCategories sRivalCounterCategories[] = {
    [FAMILY_FIRE] = {2, {FAMILY_WATER, FAMILY_GROUND}},
    [FAMILY_WATER] = {2, {FAMILY_GRASS, FAMILY_ELECTRIC}},
    [FAMILY_GRASS] = {2, {FAMILY_FIRE, FAMILY_ICE}},
    [FAMILY_ELECTRIC] = {1, {FAMILY_GROUND}},
    [FAMILY_GROUND] = {3, {FAMILY_WATER, FAMILY_GRASS, FAMILY_ICE}},
    [FAMILY_ICE] = {1, {FAMILY_FIRE}},
};

static const u16 sCategoryBoosters[] = {
    [FAMILY_FIRE] = ITEM_CHARCOAL,
    [FAMILY_WATER] = ITEM_MYSTIC_WATER,
    [FAMILY_GRASS] = ITEM_MIRACLE_SEED,
    [FAMILY_ELECTRIC] = ITEM_MAGNET,
    [FAMILY_GROUND] = ITEM_SOFT_SAND,
    [FAMILY_ICE] = ITEM_NEVER_MELT_ICE,
    [FAMILY_EEVEE] = ITEM_NONE,
};

struct EeveeTypeBooster
{
    u16 target;
    u16 item;
};

static const struct EeveeTypeBooster sEeveeTypeBoosters[] = {
    {SPECIES_VAPOREON, ITEM_MYSTIC_WATER},
    {SPECIES_JOLTEON, ITEM_MAGNET},
    {SPECIES_FLAREON, ITEM_CHARCOAL},
    {SPECIES_ESPEON, ITEM_TWISTED_SPOON},
    {SPECIES_UMBREON, ITEM_BLACK_GLASSES},
    {SPECIES_LEAFEON, ITEM_MIRACLE_SEED},
    {SPECIES_GLACEON, ITEM_NEVER_MELT_ICE},
    {SPECIES_SYLVEON, ITEM_FAIRY_FEATHER},
};

struct FamilyStarterEvolution
{
    u16 species;
    u16 target;
    u16 item;
};

// Selector-only rewards. These do not alter the global evolution tables.
static const struct FamilyStarterEvolution sStarterEvolutions[] = {
    {SPECIES_EEVEE, SPECIES_VAPOREON, ITEM_WATER_STONE},
    {SPECIES_EEVEE, SPECIES_JOLTEON, ITEM_THUNDER_STONE},
    {SPECIES_EEVEE, SPECIES_FLAREON, ITEM_FIRE_STONE},
    {SPECIES_EEVEE, SPECIES_ESPEON, ITEM_DAWN_STONE},
    {SPECIES_EEVEE, SPECIES_UMBREON, ITEM_DUSK_STONE},
    {SPECIES_EEVEE, SPECIES_LEAFEON, ITEM_LEAF_STONE},
    {SPECIES_EEVEE, SPECIES_GLACEON, ITEM_ICE_STONE},
    {SPECIES_EEVEE, SPECIES_SYLVEON, ITEM_SHINY_STONE},
    {SPECIES_CHARCADET, SPECIES_ARMAROUGE, ITEM_AUSPICIOUS_ARMOR},
    {SPECIES_CHARCADET, SPECIES_CERULEDGE, ITEM_MALICIOUS_ARMOR},
    {SPECIES_SNORUNT, SPECIES_GLALIE, ITEM_NONE},
    {SPECIES_SNORUNT, SPECIES_FROSLASS, ITEM_DAWN_STONE},
};

struct FamilyStarterAutomaticItem
{
    u16 species;
    u16 item;
};

// Linear evolution rewards do not ask a redundant evolution question.
static const struct FamilyStarterAutomaticItem sStarterAutomaticItems[] = {
    {SPECIES_HORSEA, ITEM_DRAGON_SCALE},
    {SPECIES_ELEKID, ITEM_ELECTIRIZER},
    {SPECIES_MAGNEMITE, ITEM_THUNDER_STONE},
    {SPECIES_PICHU, ITEM_THUNDER_STONE},
    {SPECIES_GLIGAR, ITEM_RAZOR_FANG},
    {SPECIES_VULPIX_ALOLA, ITEM_ICE_STONE},
    {SPECIES_DARUMAKA_GALAR, ITEM_ICE_STONE},
};

u16 FamilyStarter_GetCandidate(u32 category, u32 index)
{
    if (category >= ARRAY_COUNT(sMenuSpecies) || index >= ARRAY_COUNT(sMenuSpecies[0]))
        return SPECIES_NONE;
    return sMenuSpecies[category][index];
}

bool32 FamilyStarter_IsAvailable(u16 species)
{
    return species != SPECIES_NONE && species < NUM_SPECIES
        && gSpeciesInfo[species].baseHP != 0;
}

static bool32 UNUSED IsMenuSpecies(u16 species)
{
    u32 category, index;
    if (!FamilyStarter_IsAvailable(species))
        return FALSE;
    for (category = 0; category < ARRAY_COUNT(sMenuSpecies); category++)
        for (index = 0; index < ARRAY_COUNT(sMenuSpecies[0]); index++)
            if (sMenuSpecies[category][index] == species)
                return TRUE;
    return FALSE;
}

static bool32 UNUSED IsFamilyStarterMode(void)
{
    if (!IS_HNS || IsOneTypeChallengeActive())
        return FALSE;
#if RANDOMIZER_AVAILABLE
    if (RandomizerFeatureEnabled(RANDOMIZE_STARTER_AND_GIFT_MON))
        return FALSE;
#endif
    return TRUE;
}

void FamilyStarter_ClearPreview(void)
{
    ZeroMonData(&sFamilyStarterPreview);
    sFamilyStarterPreviewSpecies = SPECIES_NONE;
    sFamilyStarterPreviewPreference = SPECIES_NONE;
    sFamilyStarterPreviewIsEgg = FALSE;
    memset(sFamilyStarterPendingItems, 0, sizeof(sFamilyStarterPendingItems));
    memset(sFamilyStarterPendingItemsInPC, 0, sizeof(sFamilyStarterPendingItemsInPC));
    sFamilyStarterPendingItemCount = 0;
    sFamilyStarterPendingItemRead = 0;
}

static void PreparePreview(u16 species, u16 preference, bool32 isEgg)
{
    u8 value;
    bool32 forceFemale = species == SPECIES_SNORUNT && preference == SPECIES_FROSLASS;

    if (!FamilyStarter_IsAvailable(species))
    {
        FamilyStarter_ClearPreview();
        return;
    }
    if (sFamilyStarterPreviewSpecies == species
     && sFamilyStarterPreviewPreference == preference
     && sFamilyStarterPreviewIsEgg == isEgg)
        return;

    do
    {
        if (isEgg)
        {
            CreateEgg(&sFamilyStarterPreview, species, TRUE);
            value = TRUE;
            SetMonData(&sFamilyStarterPreview, MON_DATA_IS_EGG, &value);
        }
        else
        {
            CreateRandomMon(&sFamilyStarterPreview, species, 5);
            value = BALL_POKE;
            SetMonData(&sFamilyStarterPreview, MON_DATA_POKEBALL, &value);
        }
    }
    while (forceFemale && GetMonGender(&sFamilyStarterPreview) != MON_FEMALE);
    sFamilyStarterPreviewSpecies = species;
    sFamilyStarterPreviewPreference = preference;
    sFamilyStarterPreviewIsEgg = isEgg;
}

void FamilyStarter_PreparePreview(void)
{
    PreparePreview(gSpecialVar_0x8005, gSpecialVar_0x8006, FlagGet(FLAG_SYS_POKEMON_GET));
}

void FamilyStarter_PrepareStarterPreview(u16 species)
{
    PreparePreview(species, SPECIES_NONE, FALSE);
}

bool32 FamilyStarter_IsPreviewShiny(u16 species)
{
    return sFamilyStarterPreviewSpecies == species
        && GetMonData(&sFamilyStarterPreview, MON_DATA_IS_SHINY);
}

u32 FamilyStarter_GetPreviewPersonality(u16 species)
{
    if (sFamilyStarterPreviewSpecies != species)
        return 0;
    return GetMonData(&sFamilyStarterPreview, MON_DATA_PERSONALITY);
}

void FamilyStarter_GivePrimary(void)
{
    u16 species = VarGet(VAR_TEMP_2);
    u16 preference = ValidEvolutionPreference(species, gSpecialVar_0x8006);

    gSpecialVar_Result = FALSE;
    if (sFamilyStarterPreviewSpecies != species
     || sFamilyStarterPreviewPreference != preference
     || sFamilyStarterPreviewIsEgg)
        PreparePreview(species, preference, FALSE);
    if (sFamilyStarterPreviewSpecies != species
     || sFamilyStarterPreviewPreference != preference
     || sFamilyStarterPreviewIsEgg
     || gPlayerPartyCount != 0
     || GetMaxPartySize() == 0)
        return;
    if (GiveScriptedMonToPlayer(&sFamilyStarterPreview, PARTY_SIZE) == MON_GIVEN_TO_PARTY)
    {
        FamilyStarter_ClearPreview();
        gSpecialVar_Result = TRUE;
    }
}

void FamilyStarter_UseMenu(void)
{
    gSpecialVar_0x8006 = SPECIES_NONE;
    gSpecialVar_Result = IsFamilyStarterMode();
}

static u32 GetMenuCategory(u16 species)
{
    u32 category, index;
    for (category = 0; category < ARRAY_COUNT(sMenuSpecies); category++)
        for (index = 0; index < ARRAY_COUNT(sMenuSpecies[0]); index++)
            if (sMenuSpecies[category][index] == species)
                return category;
    return FAMILY_EEVEE;
}

u32 FamilyStarter_GetRivalCounterCategory(u32 playerCategory, u32 roll)
{
    const struct RivalCounterCategories *candidates;

    if (playerCategory >= FAMILY_EEVEE)
        return roll % FAMILY_EEVEE;
    candidates = &sRivalCounterCategories[playerCategory];
    return candidates->categories[roll % candidates->count];
}

static void SelectRivalStarter(u16 playerSpecies)
{
    u32 playerCategory = GetMenuCategory(playerSpecies);
    u32 counterCategory;
    u16 savedSpecies = VarGet(VAR_FAMILY_RIVAL_SPECIES);

    if (IsMenuSpecies(savedSpecies))
        return;

    counterCategory = FamilyStarter_GetRivalCounterCategory(playerCategory, Random());
    u16 rivalSpecies = sMenuSpecies[counterCategory][Random() % ARRAY_COUNT(sMenuSpecies[0])];
    VarSet(VAR_FAMILY_RIVAL_SPECIES, rivalSpecies);
}

static void PushChoice(const u8 *text, u16 id)
{
    struct ListMenuItem item;
    u8 *name = Alloc(StringLength(text) + 1);
    if (name == NULL)
        return;
    StringCopy(name, text);
    item.name = name;
    item.id = id;
    // The existing dynamic menu takes ownership of this copied string.
    MultichoiceDynamic_PushElement(item);
}

void FamilyStarter_BuildCategoryMenu(void)
{
    PushChoice(COMPOUND_STRING("Feu"), FAMILY_FIRE);
    PushChoice(COMPOUND_STRING("Eau"), FAMILY_WATER);
    PushChoice(COMPOUND_STRING("Plante"), FAMILY_GRASS);
    PushChoice(COMPOUND_STRING("Électrik"), FAMILY_ELECTRIC);
    PushChoice(COMPOUND_STRING("Sol"), FAMILY_GROUND);
    PushChoice(COMPOUND_STRING("Glace"), FAMILY_ICE);
    PushChoice(COMPOUND_STRING("Évoli"), FAMILY_EEVEE);
}

void FamilyStarter_BuildSpeciesMenu(void)
{
    u32 index;
    for (index = 0; index < ARRAY_COUNT(sMenuSpecies[0]); index++)
    {
        u16 species = FamilyStarter_GetCandidate(gSpecialVar_0x8004, index);
        if (FamilyStarter_IsAvailable(species))
            PushChoice(GetSpeciesName(species), species);
    }
    PushChoice(COMPOUND_STRING("Retour"), SPECIES_NONE);
}

void FamilyStarter_SaveSpeciesCursor(void)
{
    u32 index;

    gSpecialVar_0x8007 = 0;
    for (index = 0; index < ARRAY_COUNT(sMenuSpecies[0]); index++)
    {
        if (FamilyStarter_GetCandidate(gSpecialVar_0x8004, index) == gSpecialVar_Result)
        {
            gSpecialVar_0x8007 = index;
            break;
        }
    }
}

void FamilyStarter_SaveEvolutionCursor(void)
{
    u32 i;
    u32 row = 0;

    gSpecialVar_0x8008 = 0;
    for (i = 0; i < ARRAY_COUNT(sStarterEvolutions); i++)
    {
        if (sStarterEvolutions[i].species != gSpecialVar_0x8005
         || !FamilyStarter_IsAvailable(sStarterEvolutions[i].target))
            continue;
        if (sStarterEvolutions[i].target == gSpecialVar_Result)
        {
            gSpecialVar_0x8008 = row;
            return;
        }
        row++;
    }
}

void FamilyStarter_HasEvolutionChoices(void)
{
    u32 i;
    gSpecialVar_Result = FALSE;
    for (i = 0; i < ARRAY_COUNT(sStarterEvolutions); i++)
        if (sStarterEvolutions[i].species == gSpecialVar_0x8005
         && FamilyStarter_IsAvailable(sStarterEvolutions[i].target))
            gSpecialVar_Result = TRUE;
}

void FamilyStarter_BuildEvolutionMenu(void)
{
    u32 i;
    for (i = 0; i < ARRAY_COUNT(sStarterEvolutions); i++)
        if (sStarterEvolutions[i].species == gSpecialVar_0x8005
         && FamilyStarter_IsAvailable(sStarterEvolutions[i].target))
            PushChoice(GetSpeciesName(sStarterEvolutions[i].target), sStarterEvolutions[i].target);
}

static u16 UNUSED ValidEvolutionPreference(u16 species, u16 preference)
{
    u32 i;
    for (i = 0; i < ARRAY_COUNT(sStarterEvolutions); i++)
        if (sStarterEvolutions[i].species == species
         && sStarterEvolutions[i].target == preference
         && FamilyStarter_IsAvailable(preference))
            return preference;
    return SPECIES_NONE;
}

static u16 GetEvolutionItem(u16 species, u16 preference)
{
    u32 i;
    for (i = 0; i < ARRAY_COUNT(sStarterEvolutions); i++)
        if (sStarterEvolutions[i].species == species
         && sStarterEvolutions[i].target == preference)
            return sStarterEvolutions[i].item;
    for (i = 0; i < ARRAY_COUNT(sStarterAutomaticItems); i++)
        if (sStarterAutomaticItems[i].species == species)
            return sStarterAutomaticItems[i].item;
    return ITEM_NONE;
}

static void ResetPendingItems(void)
{
    memset(sFamilyStarterPendingItems, 0, sizeof(sFamilyStarterPendingItems));
    memset(sFamilyStarterPendingItemsInPC, 0, sizeof(sFamilyStarterPendingItemsInPC));
    sFamilyStarterPendingItemCount = 0;
    sFamilyStarterPendingItemRead = 0;
}

static void QueuePendingItem(u16 item)
{
    bool8 inPC;

    if (item == ITEM_NONE || sFamilyStarterPendingItemCount >= FAMILY_STARTER_MAX_PENDING_ITEMS)
        return;

    if (AddBagItem(item, 1))
        inPC = FALSE;
    else if (AddPCItem(item, 1))
        inPC = TRUE;
    else
        return;

    sFamilyStarterPendingItems[sFamilyStarterPendingItemCount] = item;
    sFamilyStarterPendingItemsInPC[sFamilyStarterPendingItemCount] = inPC;
    sFamilyStarterPendingItemCount++;
}

static u16 GetEeveeTypeBooster(u16 preference)
{
    u32 i;

    for (i = 0; i < ARRAY_COUNT(sEeveeTypeBoosters); i++)
        if (sEeveeTypeBoosters[i].target == preference)
            return sEeveeTypeBoosters[i].item;

    return ITEM_NONE;
}

static void QueueStarterRewards(u16 species, u16 preference)
{
    u16 evolutionItem;
    u32 category;

    ResetPendingItems();

    if (species == SPECIES_EEVEE)
    {
        // Eevee intentionally receives three rewards:
        // Normal STAB booster + planned Eeveelution STAB booster + evolution stone.
        QueuePendingItem(ITEM_SILK_SCARF);
        QueuePendingItem(GetEeveeTypeBooster(preference));
        QueuePendingItem(GetEvolutionItem(species, preference));
        return;
    }

    category = GetMenuCategory(species);
    if (category < FAMILY_EEVEE)
        QueuePendingItem(sCategoryBoosters[category]);

    // Evolution helpers are cumulative with the category booster.
    evolutionItem = GetEvolutionItem(species, preference);
    QueuePendingItem(evolutionItem);
}

void FamilyStarter_PreparePendingItemMessage(void)
{
    u16 item;
    bool8 inPC;

    gSpecialVar_Result = FALSE;
    if (sFamilyStarterPendingItemRead >= sFamilyStarterPendingItemCount)
    {
        ResetPendingItems();
        return;
    }

    item = sFamilyStarterPendingItems[sFamilyStarterPendingItemRead];
    inPC = sFamilyStarterPendingItemsInPC[sFamilyStarterPendingItemRead];
    sFamilyStarterPendingItemRead++;

    gSpecialVar_0x8000 = item;
    gSpecialVar_0x8001 = 1;
    gSpecialVar_0x8007 = TRUE;
    gSpecialVar_Result = inPC ? 2 : 1;
}

void FamilyStarter_RecordPrimary(void)
{
#if IS_HNS
    u16 species = GetMonData(&gPlayerParty[0], MON_DATA_SPECIES);
    if (IsFamilyStarterMode() && IsMenuSpecies(species))
    {
        u16 preference = ValidEvolutionPreference(species, gSpecialVar_0x8006);
        VarSet(VAR_FAMILY_STARTER_SPECIES, species);
        VarSet(VAR_FAMILY_STARTER_EVOLUTION, preference);
        SelectRivalStarter(species);
        QueueStarterRewards(species, preference);
    }
#endif
}

static u32 GetJohtoRivalStarterStage(u16 species)
{
    switch (species)
    {
    case SPECIES_CHIKORITA:
    case SPECIES_CYNDAQUIL:
    case SPECIES_TOTODILE:
        return 0;
    case SPECIES_BAYLEEF:
    case SPECIES_QUILAVA:
    case SPECIES_CROCONAW:
        return 1;
    case SPECIES_MEGANIUM:
    case SPECIES_TYPHLOSION:
    case SPECIES_FERALIGATR:
        return 2;
    default:
        return 3;
    }
}

static u16 GetFirstEvolution(u16 species)
{
    const struct Evolution *evolutions = GetSpeciesEvolutions(species);
    u32 i;
    if (evolutions != NULL)
        for (i = 0; evolutions[i].method != EVOLUTIONS_END; i++)
            if (FamilyStarter_IsAvailable(evolutions[i].targetSpecies))
                return evolutions[i].targetSpecies;
    return species;
}

u16 FamilyStarter_GetRivalSpecies(u16 originalSpecies)
{
#if IS_HNS
    u32 stage = GetJohtoRivalStarterStage(originalSpecies);
    u16 species = VarGet(VAR_FAMILY_RIVAL_SPECIES);
    u32 i;
    if (stage < 3 && IsMenuSpecies(species))
    {
        for (i = 0; i < stage; i++)
            species = GetFirstEvolution(species);
        return species;
    }
#endif
    return originalSpecies;
}

u16 FamilyStarter_GetPrimarySpecies(void)
{
#if IS_HNS
    u16 species = VarGet(VAR_FAMILY_STARTER_SPECIES);
    if (FlagGet(FLAG_SYS_POKEMON_GET) && IsMenuSpecies(species))
        return species;
#endif
    return GetStarterPokemon(VarGet(VAR_STARTER_MON));
}

void FamilyStarter_GiveEgg(void)
{
#if IS_HNS
    u16 species = gSpecialVar_0x8005;
    u16 preference = ValidEvolutionPreference(species, gSpecialVar_0x8006);
    u32 hatchCycles = 0;
    u32 personality;
    u8 result;
    gSpecialVar_Result = MON_CANT_GIVE;
    if (VarGet(VAR_FAMILY_EGG_SPECIES) != SPECIES_NONE
     || !CheckBagHasItem(ITEM_MYSTERY_EGG, 1)
     || !IsMenuSpecies(species))
        return;
    if (sFamilyStarterPreviewSpecies != species
     || sFamilyStarterPreviewPreference != preference
     || !sFamilyStarterPreviewIsEgg)
        PreparePreview(species, preference, TRUE);
    if (sFamilyStarterPreviewSpecies == species
     && sFamilyStarterPreviewPreference == preference
     && sFamilyStarterPreviewIsEgg)
    {
        if (GetMaxPartySize() == 1)
        {
            SetMonData(&sFamilyStarterPreview, MON_DATA_OT_NAME, gSaveBlock2Ptr->playerName);
            SetMonData(&sFamilyStarterPreview, MON_DATA_OT_GENDER, &gSaveBlock2Ptr->playerGender);
            SetMonData(&sFamilyStarterPreview, MON_DATA_OT_ID, gSaveBlock2Ptr->playerTrainerId);
            result = CopyMonToPC(&sFamilyStarterPreview);
        }
        else
        {
            result = GiveCapturedMonToPlayer(&sFamilyStarterPreview);
        }
    }
    else
    {
        result = ScriptGiveEgg(species);
    }
    if (result == MON_CANT_GIVE)
        return;
    FamilyStarter_ClearPreview();
    if (result == MON_GIVEN_TO_PC)
    {
        struct BoxPokemon *egg = GetBoxedMonPtr(gSpecialVar_MonBoxId, gSpecialVar_MonBoxPos);
        SetBoxMonData(egg, MON_DATA_FRIENDSHIP, &hatchCycles);
        personality = GetBoxMonData(egg, MON_DATA_PERSONALITY);
    }
    else
    {
        struct Pokemon *egg = &gPlayerParty[gPlayerPartyCount - 1];
        SetMonData(egg, MON_DATA_FRIENDSHIP, &hatchCycles);
        personality = GetMonData(egg, MON_DATA_PERSONALITY);
        // Gen 3 checks Eggs every 256 steps. The next real player step now
        // enters the existing hatch script; PID/shininess is already fixed.
        gSaveBlock1Ptr->daycare.stepCounter = 255;
    }
    // Conversion is committed only after the real egg has a party/PC slot.
    RemoveBagItem(ITEM_MYSTERY_EGG, 1);
    VarSet(VAR_FAMILY_EGG_SPECIES, species);
    VarSet(VAR_FAMILY_EGG_PERSONALITY_LO, personality);
    VarSet(VAR_FAMILY_EGG_PERSONALITY_HI, personality >> 16);
    VarSet(VAR_FAMILY_EGG_EVOLUTION, preference);
    FlagSet(FLAG_RECEIVED_TOGEPI_EGG);
    FlagClear(FLAG_HIDE_NEWBARKTOWN_LAB_AIDE);
    QueueStarterRewards(species, preference);
    gSpecialVar_Result = result;
#else
    gSpecialVar_Result = MON_CANT_GIVE;
#endif
}

void FamilyStarter_HasHatchedEgg(void)
{
    u32 i;
    u32 personality = VarGet(VAR_FAMILY_EGG_PERSONALITY_LO)
        | ((u32)VarGet(VAR_FAMILY_EGG_PERSONALITY_HI) << 16);
    gSpecialVar_Result = FALSE;
    if (VarGet(VAR_FAMILY_EGG_SPECIES) == SPECIES_NONE)
        return;
    for (i = 0; i < gPlayerPartyCount; i++)
        if (!GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG)
         && GetMonData(&gPlayerParty[i], MON_DATA_PERSONALITY) == personality
         && GetMonData(&gPlayerParty[i], MON_DATA_OT_ID) == READ_OTID_FROM_SAVE)
            gSpecialVar_Result = TRUE;
}
