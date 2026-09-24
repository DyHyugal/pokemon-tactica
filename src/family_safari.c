#include "global.h"
#include "family_safari.h"
#include "overworld.h"
#include "random.h"
#include "constants/map_groups.h"
#include "constants/species.h"

struct FamilySafariPool
{
    u8 minLevel;
    u8 maxLevel;
    u16 species[4];
};

struct FamilySafariSector
{
    u8 mapGroup;
    u8 mapNum;
    u8 poolCount;
    const struct FamilySafariPool *pools;
};

#include "data/family_remix_safari.h"

EWRAM_DATA static u8 sFamilySafariRotation = 0;

static const struct FamilySafariSector *GetFamilySafariSector(u8 mapGroup, u8 mapNum)
{
    u32 i;

    for (i = 0; i < ARRAY_COUNT(sFamilySafariSectors); i++)
    {
        if (sFamilySafariSectors[i].mapGroup == mapGroup
         && sFamilySafariSectors[i].mapNum == mapNum)
            return &sFamilySafariSectors[i];
    }

    return NULL;
}

void FamilySafari_ResetRotation(void)
{
    sFamilySafariRotation = 0;
}

void FamilySafari_AdvanceRotation(void)
{
    sFamilySafariRotation++;
}

u8 FamilySafari_GetRotation(void)
{
    return sFamilySafariRotation;
}

u8 FamilySafari_GetPoolCount(u8 mapGroup, u8 mapNum)
{
    const struct FamilySafariSector *sector = GetFamilySafariSector(mapGroup, mapNum);

    return sector == NULL ? 0 : sector->poolCount;
}

bool32 FamilySafari_SelectEncounter(u8 mapGroup, u8 mapNum, u8 rotation, u8 roll,
                                   u16 *species, u8 *minLevel, u8 *maxLevel, u8 *slot)
{
    const struct FamilySafariSector *sector = GetFamilySafariSector(mapGroup, mapNum);
    const struct FamilySafariPool *pool;
    u8 selectedSlot;

    if (sector == NULL)
        return FALSE;

    pool = &sector->pools[rotation % sector->poolCount];
    if (roll < 30)
        selectedSlot = 0;
    else if (roll < 60)
        selectedSlot = 1;
    else if (roll < 90)
        selectedSlot = 2;
    else
        selectedSlot = 3;

    *species = pool->species[selectedSlot];
    *minLevel = pool->minLevel;
    *maxLevel = pool->maxLevel;
    *slot = selectedSlot;
    return TRUE;
}

bool32 FamilySafari_TryGetEncounter(u16 *species, u8 *level, u8 *slot)
{
    u8 minLevel;
    u8 maxLevel;

    if (!FamilySafari_SelectEncounter(gSaveBlock1Ptr->location.mapGroup,
                                      gSaveBlock1Ptr->location.mapNum,
                                      sFamilySafariRotation,
                                      Random() % 100,
                                      species, &minLevel, &maxLevel, slot))
        return FALSE;

    *level = minLevel + Random() % (maxLevel - minLevel + 1);
    return TRUE;
}
