#include "global.h"
#include "family_starter.h"
#include "data.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "event_data.h"
#include "item.h"
#include "daycare.h"
#include "constants/vars.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/opponents.h"
#include "constants/flags.h"
#include "test/test.h"

#if IS_HNS
static void InitFamilyTest(void)
{
    InitEventData();
    FamilyStarter_ClearPreview();
    ZeroPlayerPartyMons();
    ResetPokemonStorageSystem();
    ClearBag();
    memset(&gSaveBlock3Ptr->challengeSettings, 0, sizeof(struct ChallengeSettings));
    // Settings stores OFF as 31; zero is TYPE_NONE, an active restriction.
    gSaveBlock3Ptr->challengeSettings.tx_Challenges_OneTypeChallenge = 31;
    CreateRandomMon(&gPlayerParty[0], SPECIES_EEVEE, 5);
    gPlayerPartyCount = 1;
    gSpecialVar_0x8005 = SPECIES_CHARCADET;
    gSpecialVar_0x8006 = SPECIES_NONE;
    AddBagItem(ITEM_MYSTERY_EGG, 1);
}

TEST("Family starter: species menu reopens on the selected species id")
{
    InitFamilyTest();
    gSpecialVar_0x8004 = 2;
    gSpecialVar_Result = SPECIES_SNIVY;
    FamilyStarter_SaveSpeciesCursor();
    EXPECT_EQ(gSpecialVar_0x8007, SPECIES_SNIVY);

    gSpecialVar_Result = SPECIES_NONE;
    FamilyStarter_SaveSpeciesCursor();
    EXPECT_EQ(gSpecialVar_0x8007, 0xFFFF);
}

TEST("Family starter: evolution menu reopens on the selected species id")
{
    InitFamilyTest();
    gSpecialVar_0x8005 = SPECIES_EEVEE;
    gSpecialVar_Result = SPECIES_GLACEON;
    FamilyStarter_SaveEvolutionCursor();
    EXPECT_EQ(gSpecialVar_0x8008, SPECIES_GLACEON);

    gSpecialVar_0x8005 = SPECIES_CHARCADET;
    gSpecialVar_Result = SPECIES_CERULEDGE;
    FamilyStarter_SaveEvolutionCursor();
    EXPECT_EQ(gSpecialVar_0x8008, SPECIES_CERULEDGE);

    gSpecialVar_Result = SPECIES_NONE;
    FamilyStarter_SaveEvolutionCursor();
    EXPECT_EQ(gSpecialVar_0x8008, 0);
}

TEST("Family starter: previewed primary is the exact Pokemon received")
{
    u32 personality;
    bool32 isShiny;
    InitFamilyTest();
    ZeroPlayerPartyMons();
    gPlayerPartyCount = 0;
    VarSet(VAR_TEMP_2, SPECIES_EEVEE);
    FamilyStarter_PrepareStarterPreview(SPECIES_EEVEE);
    personality = FamilyStarter_GetPreviewPersonality(SPECIES_EEVEE);
    isShiny = FamilyStarter_IsPreviewShiny(SPECIES_EEVEE);
    FamilyStarter_GivePrimary();
    EXPECT_EQ(gSpecialVar_Result, TRUE);
    EXPECT_EQ(gPlayerPartyCount, 1);
    EXPECT_EQ(GetMonData(&gPlayerParty[0], MON_DATA_PERSONALITY), personality);
    EXPECT_EQ(GetMonData(&gPlayerParty[0], MON_DATA_IS_SHINY), isShiny);
    EXPECT_EQ(FamilyStarter_GetPreviewPersonality(SPECIES_EEVEE), 0);
}

TEST("Family starter: previewed egg keeps its exact personality")
{
    u32 personality;
    bool32 isShiny;
    InitFamilyTest();
    FlagSet(FLAG_SYS_POKEMON_GET);
    FamilyStarter_PreparePreview();
    personality = FamilyStarter_GetPreviewPersonality(SPECIES_CHARCADET);
    isShiny = FamilyStarter_IsPreviewShiny(SPECIES_CHARCADET);
    FamilyStarter_GiveEgg();
    EXPECT_EQ(gSpecialVar_Result, MON_GIVEN_TO_PARTY);
    EXPECT_EQ(GetMonData(&gPlayerParty[1], MON_DATA_PERSONALITY), personality);
    EXPECT_EQ(GetMonData(&gPlayerParty[1], MON_DATA_IS_SHINY), isShiny);
}

TEST("Family starter: all 31 menu entries have existing enabled species data")
{
    u32 category, index, count = 0;
    for (category = 0; category < 7; category++)
        for (index = 0; index < 5; index++)
        {
            u16 species = FamilyStarter_GetCandidate(category, index);
            if (species == SPECIES_NONE)
                continue;
            EXPECT(FamilyStarter_IsAvailable(species));
            count++;
        }
    EXPECT_EQ(count, 31);
    EXPECT_EQ(FamilyStarter_GetCandidate(7, 0), SPECIES_NONE);
    EXPECT_EQ(FamilyStarter_GetCandidate(0, 5), SPECIES_NONE);
    EXPECT(!FamilyStarter_IsAvailable(SPECIES_NONE));
    EXPECT(!FamilyStarter_IsAvailable(NUM_SPECIES));
}

TEST("Family starter: cancel or invalid choice preserves the mystery egg")
{
    InitFamilyTest();
    gSpecialVar_0x8005 = SPECIES_NONE;
    FamilyStarter_GiveEgg();
    EXPECT_EQ(gSpecialVar_Result, MON_CANT_GIVE);
    EXPECT(CheckBagHasItem(ITEM_MYSTERY_EGG, 1));
    EXPECT_EQ(gPlayerPartyCount, 1);
    EXPECT_EQ(VarGet(VAR_FAMILY_EGG_SPECIES), SPECIES_NONE);
    EXPECT(!FlagGet(FLAG_RECEIVED_TOGEPI_EGG));
}

TEST("Family starter: primary Eevee receives all three planned rewards")
{
    InitFamilyTest();
    gSpecialVar_0x8006 = SPECIES_VAPOREON;
    FamilyStarter_RecordPrimary();
    FlagSet(FLAG_SYS_POKEMON_GET);
    EXPECT_EQ(FamilyStarter_GetPrimarySpecies(), SPECIES_EEVEE);
    EXPECT_EQ(VarGet(VAR_FAMILY_STARTER_EVOLUTION), SPECIES_VAPOREON);
    EXPECT(CheckBagHasItem(ITEM_SILK_SCARF, 1));
    EXPECT(CheckBagHasItem(ITEM_MYSTIC_WATER, 1));
    EXPECT(CheckBagHasItem(ITEM_WATER_STONE, 1));
    gSpecialVar_0x8006 = SPECIES_CHARCADET;
    FamilyStarter_RecordPrimary();
    EXPECT_EQ(VarGet(VAR_FAMILY_STARTER_EVOLUTION), SPECIES_NONE);
}

TEST("Family starter: Eevee egg stores its existing stone evolution preference")
{
    InitFamilyTest();
    gSpecialVar_0x8005 = SPECIES_EEVEE;
    gSpecialVar_0x8006 = SPECIES_VAPOREON;
    FamilyStarter_GiveEgg();
    EXPECT_EQ(gSpecialVar_Result, MON_GIVEN_TO_PARTY);
    EXPECT_EQ(GetMonData(&gPlayerParty[1], MON_DATA_SPECIES), SPECIES_EEVEE);
    EXPECT_EQ(VarGet(VAR_FAMILY_EGG_EVOLUTION), SPECIES_VAPOREON);
    EXPECT(CheckBagHasItem(ITEM_SILK_SCARF, 1));
    EXPECT(CheckBagHasItem(ITEM_MYSTIC_WATER, 1));
    EXPECT(CheckBagHasItem(ITEM_WATER_STONE, 1));
}

TEST("Family starter: Sylveon plan uses Fairy Feather as its type booster")
{
    InitFamilyTest();
    gSpecialVar_0x8005 = SPECIES_EEVEE;
    gSpecialVar_0x8006 = SPECIES_SYLVEON;
    FamilyStarter_GiveEgg();
    EXPECT_EQ(gSpecialVar_Result, MON_GIVEN_TO_PARTY);
    EXPECT(CheckBagHasItem(ITEM_SILK_SCARF, 1));
    EXPECT(CheckBagHasItem(ITEM_FAIRY_FEATHER, 1));
    EXPECT(CheckBagHasItem(ITEM_SHINY_STONE, 1));
}

TEST("Family starter: Charcadet branch grants its selected stone")
{
    InitFamilyTest();
    gSpecialVar_0x8006 = SPECIES_ARMAROUGE;
    FamilyStarter_GiveEgg();
    EXPECT_EQ(gSpecialVar_Result, MON_GIVEN_TO_PARTY);
    EXPECT_EQ(VarGet(VAR_FAMILY_EGG_EVOLUTION), SPECIES_ARMAROUGE);
    EXPECT(CheckBagHasItem(ITEM_CHARCOAL, 1));
    EXPECT(CheckBagHasItem(ITEM_AUSPICIOUS_ARMOR, 1));
    EXPECT(!CheckBagHasItem(ITEM_MALICIOUS_ARMOR, 1));
}

TEST("Family starter: linear species receive their automatic evolution item")
{
    static const u16 species[] = {
        SPECIES_HORSEA, SPECIES_ELEKID, SPECIES_MAGNEMITE, SPECIES_PICHU,
        SPECIES_GLIGAR, SPECIES_VULPIX_ALOLA, SPECIES_DARUMAKA_GALAR,
    };
    static const u16 items[] = {
        ITEM_DRAGON_SCALE, ITEM_ELECTIRIZER, ITEM_THUNDER_STONE, ITEM_THUNDER_STONE,
        ITEM_RAZOR_FANG, ITEM_ICE_STONE, ITEM_ICE_STONE,
    };
    static const u16 boosters[] = {
        ITEM_MYSTIC_WATER, ITEM_MAGNET, ITEM_MAGNET, ITEM_MAGNET,
        ITEM_SOFT_SAND, ITEM_NEVER_MELT_ICE, ITEM_NEVER_MELT_ICE,
    };
    u32 i;

    for (i = 0; i < ARRAY_COUNT(species); i++)
    {
        InitFamilyTest();
        CreateRandomMon(&gPlayerParty[0], species[i], 5);
        FamilyStarter_RecordPrimary();
        EXPECT(CheckBagHasItem(boosters[i], 1));
        EXPECT(CheckBagHasItem(items[i], 1));
        EXPECT_EQ(VarGet(VAR_FAMILY_STARTER_EVOLUTION), SPECIES_NONE);
    }
}

TEST("Family starter: Froslass preference makes the exact primary preview female")
{
    u32 personality;
    InitFamilyTest();
    ZeroPlayerPartyMons();
    gPlayerPartyCount = 0;
    gSpecialVar_0x8005 = SPECIES_SNORUNT;
    gSpecialVar_0x8006 = SPECIES_FROSLASS;
    VarSet(VAR_TEMP_2, SPECIES_SNORUNT);
    FamilyStarter_PreparePreview();
    personality = FamilyStarter_GetPreviewPersonality(SPECIES_SNORUNT);
    FamilyStarter_GivePrimary();
    EXPECT_EQ(gSpecialVar_Result, TRUE);
    EXPECT_EQ(GetMonData(&gPlayerParty[0], MON_DATA_PERSONALITY), personality);
    EXPECT_EQ(GetMonGender(&gPlayerParty[0]), MON_FEMALE);
    FamilyStarter_RecordPrimary();
    EXPECT_EQ(VarGet(VAR_FAMILY_STARTER_EVOLUTION), SPECIES_FROSLASS);
    EXPECT(CheckBagHasItem(ITEM_NEVER_MELT_ICE, 1));
    EXPECT(CheckBagHasItem(ITEM_DAWN_STONE, 1));
}

TEST("Family starter: Snorunt egg branches preserve gender and item choice")
{
    u32 personality;
    InitFamilyTest();
    FlagSet(FLAG_SYS_POKEMON_GET);
    gSpecialVar_0x8005 = SPECIES_SNORUNT;
    gSpecialVar_0x8006 = SPECIES_FROSLASS;
    FamilyStarter_PreparePreview();
    personality = FamilyStarter_GetPreviewPersonality(SPECIES_SNORUNT);
    FamilyStarter_GiveEgg();
    EXPECT_EQ(GetMonData(&gPlayerParty[1], MON_DATA_PERSONALITY), personality);
    EXPECT_EQ(GetMonGender(&gPlayerParty[1]), MON_FEMALE);
    EXPECT_EQ(VarGet(VAR_FAMILY_EGG_EVOLUTION), SPECIES_FROSLASS);
    EXPECT(CheckBagHasItem(ITEM_NEVER_MELT_ICE, 1));
    EXPECT(CheckBagHasItem(ITEM_DAWN_STONE, 1));

    InitFamilyTest();
    gSpecialVar_0x8005 = SPECIES_SNORUNT;
    gSpecialVar_0x8006 = SPECIES_GLALIE;
    FamilyStarter_GiveEgg();
    EXPECT_EQ(VarGet(VAR_FAMILY_EGG_EVOLUTION), SPECIES_GLALIE);
    EXPECT(!CheckBagHasItem(ITEM_DAWN_STONE, 1));
}

TEST("Family starter: Froslass egg is regenerated female if its preview was lost")
{
    InitFamilyTest();
    gSpecialVar_0x8005 = SPECIES_SNORUNT;
    gSpecialVar_0x8006 = SPECIES_FROSLASS;
    FamilyStarter_ClearPreview();
    FamilyStarter_GiveEgg();
    EXPECT_EQ(gSpecialVar_Result, MON_GIVEN_TO_PARTY);
    EXPECT_EQ(GetMonData(&gPlayerParty[1], MON_DATA_SPECIES), SPECIES_SNORUNT);
    EXPECT_EQ(GetMonGender(&gPlayerParty[1]), MON_FEMALE);
    EXPECT_EQ(VarGet(VAR_FAMILY_EGG_EVOLUTION), SPECIES_FROSLASS);
}

TEST("Family starter: cumulative item feedback is queued in reward order")
{
    InitFamilyTest();
    gSpecialVar_0x8006 = SPECIES_CERULEDGE;
    FamilyStarter_GiveEgg();
    EXPECT(CheckBagHasItem(ITEM_CHARCOAL, 1));
    EXPECT(CheckBagHasItem(ITEM_MALICIOUS_ARMOR, 1));

    FamilyStarter_PreparePendingItemMessage();
    EXPECT_EQ(gSpecialVar_Result, 1);
    EXPECT_EQ(gSpecialVar_0x8000, ITEM_CHARCOAL);
    FamilyStarter_PreparePendingItemMessage();
    EXPECT_EQ(gSpecialVar_Result, 1);
    EXPECT_EQ(gSpecialVar_0x8000, ITEM_MALICIOUS_ARMOR);
    FamilyStarter_PreparePendingItemMessage();
    EXPECT_EQ(gSpecialVar_Result, FALSE);

    FamilyStarter_GiveEgg();
    EXPECT_EQ(gSpecialVar_Result, MON_CANT_GIVE);
    EXPECT(CheckBagHasItem(ITEM_CHARCOAL, 1));
    EXPECT(CheckBagHasItem(ITEM_MALICIOUS_ARMOR, 1));
    EXPECT(!CheckBagHasItem(ITEM_MALICIOUS_ARMOR, 2));
}

TEST("Family starter: a full bag sends every reward to item storage")
{
    struct BagPocket *pocket;
    u32 i;
    InitFamilyTest();
    gSpecialVar_0x8005 = SPECIES_HORSEA;
    pocket = &gBagPockets[GetItemPocket(ITEM_DRAGON_SCALE)];
    for (i = 0; i < pocket->capacity; i++)
    {
        struct ItemSlot slot = {.itemId = ITEM_POTION, .quantity = 1};
        BagPocket_SetSlotData(pocket, i, slot);
    }
    FamilyStarter_GiveEgg();
    EXPECT(CheckPCHasItem(ITEM_MYSTIC_WATER, 1));
    EXPECT(CheckPCHasItem(ITEM_DRAGON_SCALE, 1));

    FamilyStarter_PreparePendingItemMessage();
    EXPECT_EQ(gSpecialVar_Result, 2);
    EXPECT_EQ(gSpecialVar_0x8000, ITEM_MYSTIC_WATER);
    FamilyStarter_PreparePendingItemMessage();
    EXPECT_EQ(gSpecialVar_Result, 2);
    EXPECT_EQ(gSpecialVar_0x8000, ITEM_DRAGON_SCALE);
    FamilyStarter_PreparePendingItemMessage();
    EXPECT_EQ(gSpecialVar_Result, FALSE);
}

TEST("Family starter: rival uses an allowed fixed counter starter against Electric")
{
    u16 rival;
    InitFamilyTest();
    CreateRandomMon(&gPlayerParty[0], SPECIES_ELEKID, 5);
    FamilyStarter_RecordPrimary();
    rival = VarGet(VAR_FAMILY_RIVAL_SPECIES);
    EXPECT(rival == SPECIES_DRILBUR || rival == SPECIES_TREECKO);
    EXPECT_EQ(FamilyStarter_GetRivalSpecies(SPECIES_CHIKORITA), rival);
    EXPECT(FamilyStarter_IsAvailable(FamilyStarter_GetRivalSpecies(SPECIES_BAYLEEF)));
    VarSet(VAR_FAMILY_RIVAL_SPECIES, SPECIES_NONE);
    EXPECT_EQ(FamilyStarter_GetRivalSpecies(SPECIES_CHIKORITA), SPECIES_CHIKORITA);
}

TEST("Family starter: rival category draw follows the canonical counter matrix")
{
    static const u8 expected[][3] = {
        {1, 4, 1},
        {2, 3, 2},
        {0, 5, 0},
        {4, 2, 4},
        {1, 5, 1},
        {0, 1, 0},
    };

    for (u32 playerCategory = 0; playerCategory < ARRAY_COUNT(expected); playerCategory++)
        for (u32 roll = 0; roll < ARRAY_COUNT(expected[0]); roll++)
            EXPECT_EQ(FamilyStarter_GetRivalCounterCategory(playerCategory, roll), expected[playerCategory][roll]);
}

TEST("Family starter: every rival archetype has one fixed starter")
{
    static const u16 expected[] = {
        SPECIES_TORCHIC,
        SPECIES_MUDKIP,
        SPECIES_TREECKO,
        SPECIES_ELEKID,
        SPECIES_DRILBUR,
        SPECIES_DARUMAKA_GALAR,
    };

    for (u32 category = 0; category < ARRAY_COUNT(expected); category++)
        EXPECT_EQ(FamilyStarter_GetRivalFixedStarter(category), expected[category]);
    EXPECT_EQ(FamilyStarter_GetRivalFixedStarter(ARRAY_COUNT(expected)), SPECIES_NONE);
}

TEST("Family starter: rival draw is persisted once")
{
    u16 rival;

    InitFamilyTest();
    CreateRandomMon(&gPlayerParty[0], SPECIES_ELEKID, 5);
    FamilyStarter_RecordPrimary();
    rival = VarGet(VAR_FAMILY_RIVAL_SPECIES);
    EXPECT_NE(rival, SPECIES_NONE);

    SetMonData(&gPlayerParty[0], MON_DATA_SPECIES, &(u16){SPECIES_CYNDAQUIL});
    FamilyStarter_RecordPrimary();
    EXPECT_EQ(VarGet(VAR_FAMILY_RIVAL_SPECIES), rival);
}

TEST("Family starter: saved rival category resolves every authored party phase")
{
    struct TrainerMon mon = {0};

    InitFamilyTest();
    VarSet(VAR_FAMILY_RIVAL_SPECIES, SPECIES_TORCHIC);

    mon.lvl = 17;
    EXPECT(FamilyStarter_ResolveRivalMon(TRAINER_RIVAL_CHIKORITA_1_HNS, 0, &mon));
    EXPECT_EQ(mon.species, SPECIES_COMBUSKEN);
    EXPECT_EQ(mon.moves[0], MOVE_NONE);

    mon = (struct TrainerMon){.lvl = 15};
    EXPECT(FamilyStarter_ResolveRivalMon(TRAINER_RIVAL_CHIKORITA_2_HNS, 0, &mon));
    EXPECT_EQ(mon.species, SPECIES_TORKOAL);
    EXPECT_EQ(mon.moves[0], MOVE_EMBER);
    EXPECT_EQ(mon.heldItem, ITEM_NONE);
    EXPECT_EQ(mon.ev, NULL);

    mon = (struct TrainerMon){.lvl = 16};
    EXPECT(FamilyStarter_ResolveRivalMon(TRAINER_RIVAL_CHIKORITA_2_HNS, 1, &mon));
    EXPECT_EQ(mon.species, SPECIES_COMBUSKEN);
    EXPECT_EQ(mon.moves[0], MOVE_NONE);

    mon = (struct TrainerMon){.lvl = 24};
    EXPECT(FamilyStarter_ResolveRivalMon(TRAINER_RIVAL_CHIKORITA_3_HNS, 2, &mon));
    EXPECT_EQ(mon.species, SPECIES_IVYSAUR);
    EXPECT_EQ(mon.moves[0], MOVE_MEGA_DRAIN);

    mon = (struct TrainerMon){.lvl = 24};
    EXPECT(FamilyStarter_ResolveRivalMon(TRAINER_RIVAL_CHIKORITA_3_HNS, 1, &mon));
    EXPECT_EQ(mon.heldItem, ITEM_NONE);
    mon = (struct TrainerMon){.lvl = 39};
    EXPECT(FamilyStarter_ResolveRivalMon(TRAINER_RIVAL_CHIKORITA_4_HNS, 1, &mon));
    EXPECT_EQ(mon.heldItem, ITEM_BLAZIKENITE);

    mon = (struct TrainerMon){.lvl = 68};
    EXPECT(FamilyStarter_ResolveRivalMon(TRAINER_RIVAL_CHIKORITA_7_HNS, 5, &mon));
    EXPECT_EQ(mon.species, SPECIES_GREAT_TUSK);
    EXPECT_EQ(mon.moves[0], MOVE_HEADLONG_RUSH);
    EXPECT_EQ(mon.heldItem, ITEM_ASSAULT_VEST);

    VarSet(VAR_FAMILY_RIVAL_SPECIES, SPECIES_DRILBUR);
    mon = (struct TrainerMon){.lvl = 40};
    EXPECT(FamilyStarter_ResolveRivalMon(TRAINER_RIVAL_CHIKORITA_4_HNS, 3, &mon));
    EXPECT_EQ(mon.species, SPECIES_GABITE);
    mon.lvl = 48;
    EXPECT(FamilyStarter_ResolveRivalMon(TRAINER_RIVAL_CHIKORITA_5_HNS, 3, &mon));
    EXPECT_EQ(mon.species, SPECIES_GARCHOMP);

    VarSet(VAR_FAMILY_RIVAL_SPECIES, SPECIES_MUDKIP);
    mon = (struct TrainerMon){.lvl = 39};
    EXPECT(FamilyStarter_ResolveRivalMon(TRAINER_RIVAL_CHIKORITA_4_HNS, 3, &mon));
    EXPECT_EQ(mon.species, SPECIES_POLITOED);

    VarSet(VAR_FAMILY_RIVAL_SPECIES, SPECIES_ELEKID);
    mon = (struct TrainerMon){.lvl = 18};
    EXPECT(FamilyStarter_ResolveRivalMon(TRAINER_RIVAL_CHIKORITA_2_HNS, 1, &mon));
    EXPECT_EQ(mon.species, SPECIES_ELEKID);
    mon.lvl = 40;
    EXPECT(FamilyStarter_ResolveRivalMon(TRAINER_RIVAL_CHIKORITA_4_HNS, 1, &mon));
    EXPECT_EQ(mon.species, SPECIES_ELECTIVIRE);
    EXPECT_EQ(mon.heldItem, ITEM_EXPERT_BELT);

    EXPECT(!FamilyStarter_ResolveRivalMon(TRAINER_YOUNGSTER_CALVIN, 0, &mon));
}

TEST("Family starter: monotype keeps the historical rival selection")
{
    InitFamilyTest();
    gSaveBlock3Ptr->challengeSettings.tx_Challenges_OneTypeChallenge = TYPE_FIRE;
    CreateRandomMon(&gPlayerParty[0], SPECIES_CYNDAQUIL, 5);
    FamilyStarter_RecordPrimary();
    EXPECT_EQ(VarGet(VAR_FAMILY_RIVAL_SPECIES), SPECIES_NONE);
    EXPECT_EQ(FamilyStarter_GetRivalSpecies(SPECIES_CHIKORITA), SPECIES_CHIKORITA);
}

TEST("Family starter: the actual selected egg is granted exactly once")
{
    u32 isEgg = FALSE;
    InitFamilyTest();
    FamilyStarter_GiveEgg();
    EXPECT_EQ(gSpecialVar_Result, MON_GIVEN_TO_PARTY);
    EXPECT_EQ(GetMonData(&gPlayerParty[1], MON_DATA_SPECIES), SPECIES_CHARCADET);
    EXPECT(GetMonData(&gPlayerParty[1], MON_DATA_IS_EGG));
    EXPECT_EQ(VarGet(VAR_FAMILY_EGG_SPECIES), SPECIES_CHARCADET);
    EXPECT_EQ(GetMonData(&gPlayerParty[1], MON_DATA_FRIENDSHIP), 0);
    EXPECT_EQ(gSaveBlock1Ptr->daycare.stepCounter, 255);
    EXPECT(!CheckBagHasItem(ITEM_MYSTERY_EGG, 1));
    EXPECT(FlagGet(FLAG_RECEIVED_TOGEPI_EGG));
    FamilyStarter_HasHatchedEgg();
    EXPECT_EQ(gSpecialVar_Result, FALSE);
    FamilyStarter_GiveEgg();
    EXPECT_EQ(gSpecialVar_Result, MON_CANT_GIVE);
    EXPECT_EQ(gPlayerPartyCount, 2);
    SetMonData(&gPlayerParty[1], MON_DATA_IS_EGG, &isEgg);
    FamilyStarter_HasHatchedEgg();
    EXPECT_EQ(gSpecialVar_Result, TRUE);
}

TEST("Family starter: Elm egg cannot duplicate the primary starter")
{
    InitFamilyTest();
    FlagSet(FLAG_SYS_POKEMON_GET);
    VarSet(VAR_FAMILY_STARTER_SPECIES, SPECIES_CHARCADET);
    FamilyStarter_GiveEgg();
    EXPECT_EQ(gSpecialVar_Result, MON_CANT_GIVE);
    EXPECT_EQ(gPlayerPartyCount, 1);
    EXPECT(CheckBagHasItem(ITEM_MYSTERY_EGG, 1));
    EXPECT_EQ(VarGet(VAR_FAMILY_EGG_SPECIES), SPECIES_NONE);
}

TEST("Family starter: selected party egg is ready to hatch on the next step")
{
    InitFamilyTest();
    FamilyStarter_GiveEgg();
    EXPECT(ShouldEggHatch());
    EXPECT_EQ(gSpecialVar_0x8004, 1);
}

TEST("Family starter: party limit one sends the egg to the PC")
{
    struct BoxPokemon *egg;
    InitFamilyTest();
    gSaveBlock3Ptr->challengeSettings.tx_Challenges_PartyLimit = 5;
    FamilyStarter_GiveEgg();
    EXPECT_EQ(gSpecialVar_Result, MON_GIVEN_TO_PC);
    EXPECT_EQ(gPlayerPartyCount, 1);
    egg = GetBoxedMonPtr(gSpecialVar_MonBoxId, gSpecialVar_MonBoxPos);
    EXPECT_EQ(GetBoxMonData(egg, MON_DATA_SPECIES), SPECIES_CHARCADET);
    EXPECT(GetBoxMonData(egg, MON_DATA_IS_EGG));
    EXPECT_EQ(GetBoxMonData(egg, MON_DATA_FRIENDSHIP), 0);
}

TEST("Family starter: monotype restriction sends an incompatible egg to the PC")
{
    InitFamilyTest();
    gSaveBlock3Ptr->challengeSettings.tx_Challenges_OneTypeChallenge = TYPE_WATER;
    FamilyStarter_GiveEgg();
    EXPECT_EQ(gSpecialVar_Result, MON_GIVEN_TO_PC);
    EXPECT_EQ(gPlayerPartyCount, 1);
    EXPECT_EQ(VarGet(VAR_FAMILY_EGG_SPECIES), SPECIES_CHARCADET);
}

TEST("Family starter: full party and PC leave the egg and progression unchanged")
{
    u32 box, slot;
    InitFamilyTest();
    gSaveBlock3Ptr->challengeSettings.tx_Challenges_PartyLimit = 5;
    for (box = 0; box < TOTAL_BOXES_COUNT; box++)
        for (slot = 0; slot < IN_BOX_COUNT; slot++)
            *GetBoxedMonPtr(box, slot) = gPlayerParty[0].box;
    FamilyStarter_GiveEgg();
    EXPECT_EQ(gSpecialVar_Result, MON_CANT_GIVE);
    EXPECT(CheckBagHasItem(ITEM_MYSTERY_EGG, 1));
    EXPECT_EQ(VarGet(VAR_FAMILY_EGG_SPECIES), SPECIES_NONE);
    EXPECT(!FlagGet(FLAG_RECEIVED_TOGEPI_EGG));
    EXPECT(!CheckBagHasItem(ITEM_AUSPICIOUS_ARMOR, 1));
    FamilyStarter_PreparePendingItemMessage();
    EXPECT_EQ(gSpecialVar_Result, FALSE);
}
#endif
