#include "global.h"
#include "item.h"
#include "money.h"
#include "move_relearner.h"
#include "party_menu.h"
#include "pokemon.h"
#include "test/test.h"

#if IS_HNS
extern const u16 MegaShop_Items[];
extern const u16 ItemShop_Items_Stones[];
extern const u16 ItemShop_Items_TradeHeld[];
extern const u16 ItemShop_Items_SpecialRegional[];
extern const u16 ItemShop_Items_TypeBoosters[];
extern const u16 ItemShop_Items_ChoiceOffensive[];
extern const u16 ItemShop_Items_DefenseSustain[];
extern const u16 ItemShop_Items_WeatherTerrain[];
extern const u16 ItemShop_Items_ActivationUtility[];
extern const u16 ItemShop_Items_CompetitiveBerries[];
extern const u16 ItemShop_Items_Training[];
extern const u16 ItemShop_Items_BreedingFriendship[];
extern const u16 ItemShop_Items_Useful[];

static const u16 *const sStrategicItemShopLists[] =
{
    ItemShop_Items_Stones,
    ItemShop_Items_TradeHeld,
    ItemShop_Items_SpecialRegional,
    ItemShop_Items_TypeBoosters,
    ItemShop_Items_ChoiceOffensive,
    ItemShop_Items_DefenseSustain,
    ItemShop_Items_WeatherTerrain,
    ItemShop_Items_ActivationUtility,
    ItemShop_Items_CompetitiveBerries,
    ItemShop_Items_Training,
    ItemShop_Items_BreedingFriendship,
    ItemShop_Items_Useful,
};

TEST("Tactica shops: every playable Mega Stone is sold for 3000")
{
    u32 count;

    for (count = 0; MegaShop_Items[count] != ITEM_NONE; count++)
    {
        EXPECT_LT(count, 92);
        EXPECT_EQ(GetItemPrice(MegaShop_Items[count]), 3000);
    }
    EXPECT_EQ(count, 92);
}

TEST("Tactica shops: a Mega Stone purchase adds the item and removes its price")
{
    enum Item item = MegaShop_Items[0];
    u32 price = GetItemPrice(item);

    ClearBag();
    SetMoney(&gSaveBlock1Ptr->money, price * 2);
    EXPECT(AddBagItem(item, 1));
    RemoveMoney(&gSaveBlock1Ptr->money, price);

    EXPECT(CheckBagHasItem(item, 1));
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), price);
}

TEST("Tactica shops: strategic stock is unique, priced, and exhaustive")
{
    bool8 seen[ITEMS_COUNT] = {0};
    u32 count = 0;

    for (u32 list = 0; list < ARRAY_COUNT(sStrategicItemShopLists); list++)
    {
        for (u32 index = 0; sStrategicItemShopLists[list][index] != ITEM_NONE; index++)
        {
            enum Item item = sStrategicItemShopLists[list][index];

            EXPECT_LT(item, ITEMS_COUNT);
            EXPECT(!seen[item]);
            EXPECT_GT(GetItemPrice(item), 0);
            seen[item] = TRUE;
            count++;
        }
    }

    EXPECT_EQ(count, 259);
    EXPECT(seen[ITEM_FLAME_PLATE]);
    EXPECT(seen[ITEM_FIRE_MEMORY]);
    EXPECT(seen[ITEM_NORMAL_GEM]);
    EXPECT(seen[ITEM_LIGHT_BALL]);
    EXPECT(seen[ITEM_ELECTRIC_SEED]);
    EXPECT(seen[ITEM_ABSORB_BULB]);
    EXPECT(seen[ITEM_GRIP_CLAW]);
    EXPECT(seen[ITEM_MICLE_BERRY]);
    EXPECT(seen[ITEM_MARANGA_BERRY]);
}

TEST("Tactica shops: move lessons expose only compatible playable moves")
{
    struct Pokemon mon;
    u16 moves[MAX_RELEARNER_MOVES];
    bool8 hasFlareBlitz = FALSE;

    CreateMon(&mon, SPECIES_CHARIZARD, 5, 0, OTID_STRUCT_PLAYER_ID);
    u32 count = GetTacticaMoveShopMoves(&mon.box, moves, ARRAY_COUNT(moves));

    EXPECT_GT(count, 0);
    EXPECT_LT(count, MAX_RELEARNER_MOVES);
    for (u32 i = 0; i < count; i++)
    {
        EXPECT_LT(moves[i], MOVES_COUNT);
        EXPECT_NE(moves[i], MOVE_STRUGGLE);
        EXPECT(CanSpeciesLearnTacticaShopMove(SPECIES_CHARIZARD, moves[i]));
        if (moves[i] == MOVE_FLARE_BLITZ)
            hasFlareBlitz = TRUE;
    }

    EXPECT(hasFlareBlitz);
    EXPECT(CanSpeciesLearnTacticaShopMove(SPECIES_DRAGONITE, MOVE_OUTRAGE));
    EXPECT(!CanSpeciesLearnTacticaShopMove(SPECIES_MAGIKARP, MOVE_FLARE_BLITZ));

    CreateMon(&mon, SPECIES_MEW, 5, 0, OTID_STRUCT_PLAYER_ID);
    count = GetTacticaMoveShopMoves(&mon.box, moves, ARRAY_COUNT(moves));
    EXPECT_GT(count, 350);
    EXPECT_LT(count, MAX_RELEARNER_MOVES);
}

TEST("Tactica shops: a confirmed move lesson charges once and teaches")
{
    struct Pokemon mon;

    CreateMon(&mon, SPECIES_CHARIZARD, 5, 0, OTID_STRUCT_PLAYER_ID);
    SetMoney(&gSaveBlock1Ptr->money, TACTICA_MOVE_SHOP_PRICE * 2);

    EXPECT(CanAffordTacticaMoveShopPurchase());
    EXPECT(PurchaseTacticaMoveShopMove());
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), TACTICA_MOVE_SHOP_PRICE);
    EXPECT_NE(GiveMoveToBoxMon(&mon.box, MOVE_FLARE_BLITZ), MON_HAS_MAX_MOVES);
    EXPECT(BoxMonKnowsMove(&mon.box, MOVE_FLARE_BLITZ));

    SetMoney(&gSaveBlock1Ptr->money, TACTICA_MOVE_SHOP_PRICE - 1);
    EXPECT(!CanAffordTacticaMoveShopPurchase());
    EXPECT(!PurchaseTacticaMoveShopMove());
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), TACTICA_MOVE_SHOP_PRICE - 1);
}
#endif
