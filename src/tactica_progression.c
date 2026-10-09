#include "global.h"
#include "pokemon.h"
#include "move.h"
#include "tactica_progression.h"

u16 GetTacticaWildSpeciesAtLevel(u16 species, u8 level)
{
    u16 ancestor = species;
    for (u32 step = 0; step < 3; step++)
    {
        u16 parent = GetSpeciesPreEvolution(ancestor);
        if (parent == SPECIES_NONE)
            break;
        const struct Evolution *evolutions = GetSpeciesEvolutions(parent);
        for (u32 i = 0; evolutions[i].method != EVOLUTIONS_END; i++)
            if (evolutions[i].targetSpecies == ancestor
             && evolutions[i].method == EVO_LEVEL && evolutions[i].param > level)
                species = parent;
        ancestor = parent;
    }
    // Only deterministic level evolutions. Objects, friendship, gender and
    // other conditions remain the player's responsibility.
    for (u32 step = 0; step < 3; step++)
    {
        const struct Evolution *evolutions = GetSpeciesEvolutions(species);
        u16 target = species;
        for (u32 i = 0; evolutions[i].method != EVOLUTIONS_END; i++)
        {
            if (evolutions[i].method == EVO_LEVEL
             && evolutions[i].param > 0 && evolutions[i].param <= level
             && evolutions[i].params == NULL)
            {
                // Do not choose arbitrarily between alternative branches.
                if (target != species)
                    return species;
                target = evolutions[i].targetSpecies;
            }
        }
        if (target == species)
            break;
        species = target;
    }
    return species;
}

void EvolveTacticaWildMonAtLevel(struct Pokemon *mon)
{
    u8 level = GetMonData(mon, MON_DATA_LEVEL);
    for (u32 step = 0; step < 3; step++)
    {
        u16 species = GetMonData(mon, MON_DATA_SPECIES);
        const struct Evolution *evolutions = GetSpeciesEvolutions(species);
        u16 target = species;
        for (u32 i = 0; evolutions[i].method != EVOLUTIONS_END; i++)
        {
            bool32 canStop = TRUE;
            bool32 usesItem = FALSE;
            if (evolutions[i].method != EVO_LEVEL || evolutions[i].param == 0 || evolutions[i].param > level)
                continue;
            if (evolutions[i].params != NULL)
                for (u32 j = 0; evolutions[i].params[j].condition != CONDITIONS_END; j++)
                    if (evolutions[i].params[j].condition == IF_HOLD_ITEM || evolutions[i].params[j].condition == IF_BAG_ITEM_COUNT)
                        usesItem = TRUE;
            if (!usesItem && DoesMonMeetAdditionalConditions(mon, evolutions[i].params, NULL, PARTY_SIZE, &canStop, CHECK_EVO))
            {
                target = evolutions[i].targetSpecies;
                break;
            }
        }
        if (target == species)
            break;
        u32 exp = gExperienceTables[gSpeciesInfo[target].growthRate][level];
        u8 abilityNum = GetMonData(mon, MON_DATA_ABILITY_NUM);
        if (gSpeciesInfo[target].abilities[abilityNum] == ABILITY_NONE)
            abilityNum = 0;
        SetMonData(mon, MON_DATA_SPECIES, &target);
        SetMonData(mon, MON_DATA_EXP, &exp);
        SetMonData(mon, MON_DATA_ABILITY_NUM, &abilityNum);
        CalculateMonStats(mon);
    }
}

bool32 IsTacticaMoveLegalAtLevel(u16 species, u8 level, enum Move move)
{
    const struct LevelUpMove *learnset = GetSpeciesLevelUpLearnset(species);
    const u16 *eggMoves = GetSpeciesEggMoves(species);
    if (move == MOVE_NONE || move == MOVE_UNAVAILABLE)
        return FALSE;
    if (CanLearnTeachableMove(species, move))
        return TRUE;
    for (u32 i = 0; learnset[i].move != LEVEL_UP_MOVE_END; i++)
        if (learnset[i].move == move && learnset[i].level <= level)
            return TRUE;
    for (u32 i = 0; eggMoves[i] != MOVE_UNAVAILABLE; i++)
        if (eggMoves[i] == move)
            return TRUE;
    return FALSE;
}

static bool32 ContainsMove(const enum Move *moves, enum Move move)
{
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
        if (moves[i] == move)
            return TRUE;
    return FALSE;
}

void AdaptTacticaTrainerMoves(u16 species, u8 level, const enum Move *preferred, enum Move *moves)
{
    const struct LevelUpMove *learnset = GetSpeciesLevelUpLearnset(species);
    u32 count = 0;
    memset(moves, MOVE_NONE, sizeof(enum Move) * MAX_MON_MOVES);
    while (learnset[count].move != LEVEL_UP_MOVE_END)
        count++;
    for (u32 slot = 0; slot < MAX_MON_MOVES; slot++)
        if (IsTacticaMoveLegalAtLevel(species, level, preferred[slot]) && !ContainsMove(moves, preferred[slot]))
            moves[slot] = preferred[slot];
    for (u32 slot = 0; slot < MAX_MON_MOVES; slot++)
    {
        if (moves[slot] != MOVE_NONE)
            continue;
        // Prefer a legal substitute with the same type/category as the set,
        // then the same category, then another natural level-legal move.
        for (u32 pass = 0; pass < 3 && moves[slot] == MOVE_NONE; pass++)
            for (u32 i = count; i > 0; i--)
            {
                enum Move move = learnset[i - 1].move;
                if (learnset[i - 1].level > level || ContainsMove(moves, move))
                    continue;
                if (preferred[slot] != MOVE_NONE && GetMoveCategory(preferred[slot]) != DAMAGE_CATEGORY_STATUS && GetMoveCategory(move) == DAMAGE_CATEGORY_STATUS)
                    continue;
                if (pass < 2 && preferred[slot] != MOVE_NONE && GetMoveCategory(move) != GetMoveCategory(preferred[slot]))
                    continue;
                if (pass == 0 && preferred[slot] != MOVE_NONE && GetMoveType(move) != GetMoveType(preferred[slot]))
                    continue;
                moves[slot] = move;
                break;
            }
    }
}
