#ifndef GUARD_MOVE_RELEARNER_H
#define GUARD_MOVE_RELEARNER_H

#include "constants/move_relearner.h"

void TeachMoveRelearnerMove(void);
void MoveRelearnerShowHideHearts(s32 move);
void MoveRelearnerShowHideCategoryIcon(s32);
void CB2_InitLearnMove(void);
bool32 CanBoxMonRelearnAnyMove(struct BoxPokemon *boxMon);
bool32 CanBoxMonRelearnMoves(struct BoxPokemon *boxMon, enum MoveRelearnerStates state);
bool32 CanSpeciesLearnTacticaShopMove(u16 species, enum Move move);
u32 GetTacticaMoveShopMoves(struct BoxPokemon *boxMon, u16 *moves, u32 capacity);
bool32 CanAffordTacticaMoveShopPurchase(void);
bool32 PurchaseTacticaMoveShopMove(void);

extern enum MoveRelearnerStates gMoveRelearnerState;
extern enum RelearnMode gRelearnMode;

#endif //GUARD_MOVE_RELEARNER_H
