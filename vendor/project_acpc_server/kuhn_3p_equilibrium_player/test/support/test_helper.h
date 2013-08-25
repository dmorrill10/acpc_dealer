#ifndef __TEST_HELPER__
#define __TEST_HELPER__

#include <stdio.h>
#include "game.h"

Game init_kuhn_poker_game_def();
MatchState init_match_state(
    const Game const* game_def,
    uint8_t position,
    uint8_t card,
    Action* actions,
    uint num_actions
);

#define DEBUG_PRINT(...) \
    printf(__VA_ARGS__); \
    fflush(NULL);


#endif
