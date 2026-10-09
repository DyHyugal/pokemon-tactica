#ifndef GUARD_TACTICA_PROGRESSION_H
#define GUARD_TACTICA_PROGRESSION_H

u16 GetTacticaWildSpeciesAtLevel(u16 species, u8 level);
struct Pokemon;
void EvolveTacticaWildMonAtLevel(struct Pokemon *mon);
bool32 IsTacticaMoveLegalAtLevel(u16 species, u8 level, enum Move move);
void AdaptTacticaTrainerMoves(u16 species, u8 level, const enum Move *preferred, enum Move *moves);

#endif
