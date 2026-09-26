#ifndef GUARD_FAMILY_STARTER_H
#define GUARD_FAMILY_STARTER_H

struct TrainerMon;

u16 FamilyStarter_GetCandidate(u32 category, u32 index);
bool32 FamilyStarter_IsAvailable(u16 species);
void FamilyStarter_UseMenu(void);
void FamilyStarter_BuildCategoryMenu(void);
void FamilyStarter_BuildSpeciesMenu(void);
void FamilyStarter_SaveSpeciesCursor(void);
void FamilyStarter_SaveEvolutionCursor(void);
void FamilyStarter_HasEvolutionChoices(void);
void FamilyStarter_BuildEvolutionMenu(void);
void FamilyStarter_PreparePreview(void);
void FamilyStarter_PrepareStarterPreview(u16 species);
bool32 FamilyStarter_IsPreviewShiny(u16 species);
u32 FamilyStarter_GetPreviewPersonality(u16 species);
void FamilyStarter_ClearPreview(void);
void FamilyStarter_GivePrimary(void);
void FamilyStarter_RecordPrimary(void);
void FamilyStarter_PreparePendingItemMessage(void);
void FamilyStarter_GiveEgg(void);
void FamilyStarter_HasHatchedEgg(void);
u16 FamilyStarter_GetPrimarySpecies(void);
u16 FamilyStarter_GetRivalSpecies(u16 originalSpecies);
u32 FamilyStarter_GetRivalCounterCategory(u32 playerCategory, u32 roll);
u16 FamilyStarter_GetRivalFixedStarter(u32 rivalCategory);
bool32 FamilyStarter_ResolveRivalMon(u16 trainerId, u32 slot, struct TrainerMon *mon);

#endif
