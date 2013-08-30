/*
Copyright (C) 2013 by the Computer Poker Research Group, University of Alberta
*/

#ifndef __TEST_HELPER__
#define __TEST_HELPER__

#include <stdio.h>
#include "game.h"

#include <unity.h>
#include <CException.h>

Game init_kuhn_poker_game_def();
MatchState init_match_state(
    const Game const* game_def,
    uint8_t position,
    uint8_t card,
    const enum ActionType const* actions,
    size_t num_actions
);

#define DEBUG_PRINT(...) \
    printf(__VA_ARGS__); \
    fflush(NULL);

#endif
