/*
Copyright (C) 2013 by the Computer Poker Research Group, University of Alberta
*/

#ifndef __PLAYER_CONFIG_H__
#define __PLAYER_CONFIG_H__

#include "dealer_connection.h"
#include "kuhn_3p_equilibrium_player.h"

typedef struct {
  DealerConnection dealer;
  Game *game;
  double params[NUM_PARAMS];
  uint32_t seed;
} PlayerConfig;

PlayerConfig new_player_config();
PlayerConfig new_player_config_from_argv(int argc, char** argv);

#endif
