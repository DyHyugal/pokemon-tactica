#ifndef GUARD_CONSTANTS_MOVE_RELEARNER_H
#define GUARD_CONSTANTS_MOVE_RELEARNER_H

// Max number of moves shown by the move relearner and Tactica move shop.
// The largest generated Tactica compatibility list currently has 372 moves.
#define MAX_RELEARNER_MOVES 400
#define TACTICA_MOVE_SHOP_PRICE 3000

// Move Relearner menu change constants
enum MoveRelearnerStates
{
    MOVE_RELEARNER_LEVEL_UP_MOVES,
    MOVE_RELEARNER_EGG_MOVES,
    MOVE_RELEARNER_TM_MOVES,
    MOVE_RELEARNER_TUTOR_MOVES,
    MOVE_RELEARNER_COUNT,
    // Kept outside MOVE_RELEARNER_COUNT so the regular relearner UI never
    // treats paid shop lessons as free relearnable moves.
    MOVE_RELEARNER_TACTICA_SHOP_MOVES = MOVE_RELEARNER_COUNT,
    MOVE_RELEARNER_STATE_COUNT,
};

enum RelearnMode
{
    RELEARN_MODE_NONE = 0,
    RELEARN_MODE_SCRIPT = 1,                     // Relearning moves through an event script
    // These two must stay 2 and 3, they are tied to the summary screen pages
    RELEARN_MODE_PSS_PAGE_BATTLE_MOVES = 2,      // Relearning moves through the summary screen's battle moves page
    RELEARN_MODE_PSS_PAGE_CONTEST_MOVES = 3,     // Relearning moves through the summary screen's contest moves page (defaults to contest page on relearner screen)
    RELEARN_MODE_PARTY_MENU = 4,                 // Relearning moves through the party menu's moves submenu
    RELEARN_MODE_BOX_PSS_PAGE_BATTLE_MOVES = 5,
    RELEARN_MODE_BOX_PSS_PAGE_CONTEST_MOVES = 6,
};

#endif // GUARD_CONSTANTS_MOVE_RELEARNER_H
