#include "global.h"
#include "pokemon.h"
#include "move.h"
#include "data.h"
#include "caps.h"
#include "tactica_progression.h"
#include "constants/maps.h"

#if IS_HNS
struct TacticaTrainerZoneFloor
{
    u16 map;
    u8 level;
    u8 ceiling;
};

#include "data/tactica_trainer_levels.h"
#endif

bool32 ResolveTacticaOrdinaryTrainerMon(u16 trainerId, u16 map, const struct Trainer *trainer, struct TrainerMon *mon)
{
#if IS_HNS
    u32 minimum = MAX_LEVEL;
    u32 floor = GetTacticaStoryTrainerFloor();
    u32 ceiling = floor;
    u16 oldSpecies = mon->species;
    u8 oldLevel = mon->lvl;
    enum Move moves[MAX_MON_MOVES];

    if (trainerId >= ARRAY_COUNT(sTacticaOrdinaryTrainers) || !sTacticaOrdinaryTrainers[trainerId]
     || trainer->party == NULL || trainer->partySize == 0)
        return FALSE;
    for (u32 i = 0; i < trainer->partySize; i++)
        minimum = min(minimum, trainer->party[i].lvl);
    for (u32 i = 0; i < ARRAY_COUNT(sTacticaTrainerZoneFloors); i++)
        if (sTacticaTrainerZoneFloors[i].map == map)
        {
            floor = max(floor, sTacticaTrainerZoneFloors[i].level);
            ceiling = max(ceiling, sTacticaTrainerZoneFloors[i].ceiling);
            break;
        }
    // Bound inherited wide spreads (e.g. Stephen's 57-75 party): raising the
    // lowest member must not manufacture level 93 before the level 80 Gyms.
    // Never lower an authored level that already exceeded the area's band.
    u32 raisedLevel = max(minimum, floor) + mon->lvl - minimum;
    mon->lvl = min(MAX_LEVEL, max(oldLevel, min(raisedLevel, max(ceiling, floor))));
    mon->species = GetTacticaWildSpeciesAtLevel(mon->species, mon->lvl);
    if (mon->lvl != oldLevel || mon->species != oldSpecies)
    {
        // Empty authored move slots mean the normal native level-up moves.
        if (mon->moves[0] != MOVE_NONE)
        {
            AdaptTacticaTrainerMoves(mon->species, mon->lvl, mon->moves, moves);
            memcpy(mon->moves, moves, sizeof(moves));
        }
        if (mon->species != oldSpecies && mon->ability != ABILITY_NONE)
        {
            bool32 legal = FALSE;
            for (u32 i = 0; i < NUM_ABILITY_SLOTS; i++)
                legal |= gSpeciesInfo[mon->species].abilities[i] == mon->ability;
            if (!legal)
                mon->ability = ABILITY_NONE;
        }
    }
    return TRUE;
#else
    return FALSE;
#endif
}

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
