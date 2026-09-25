#include "global.h"
#include "item.h"
#include "money.h"
#include "test/test.h"

#if IS_HNS
extern const u16 MegaShop_Items[];

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
#endif
