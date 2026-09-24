#ifndef GUARD_FAMILY_SAFARI_H
#define GUARD_FAMILY_SAFARI_H

void FamilySafari_ResetRotation(void);
void FamilySafari_AdvanceRotation(void);
u8 FamilySafari_GetRotation(void);
u8 FamilySafari_GetPoolCount(u8 mapGroup, u8 mapNum);
bool32 FamilySafari_SelectEncounter(u8 mapGroup, u8 mapNum, u8 rotation, u8 roll,
                                   u16 *species, u8 *minLevel, u8 *maxLevel, u8 *slot);
bool32 FamilySafari_TryGetEncounter(u16 *species, u8 *level, u8 *slot);

#endif // GUARD_FAMILY_SAFARI_H
