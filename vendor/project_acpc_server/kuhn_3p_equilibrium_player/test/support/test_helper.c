/*
Copyright (C) 2013 by the Computer Poker Research Group, University of Alberta
*/

#include "test_helper.h"

Game init_kuhn_poker_game_def()
{
  Game kuhn_def;

  kuhn_def.bettingType = limitBetting;
  kuhn_def.numRounds = 1;
  kuhn_def.maxRaises[ 0 ] = 1;
  kuhn_def.numSuits = 1;
  kuhn_def.numRanks = 4;
  kuhn_def.numHoleCards = 1;
  kuhn_def.numBoardCards[ 0 ] = 0;
  kuhn_def.numPlayers = 3;

  return kuhn_def;
}

MatchState init_match_state(
    const Game const* game_def,
    uint8_t position,
    uint8_t card,
    const enum ActionType const* actions,
    size_t num_actions
)
{
  MatchState view;

  view.viewingPlayer = position;
  view.state.actingPlayer[0][num_actions -1] = position;
  view.state.numActions[0] = num_actions;

  for (size_t i = 0; i < num_actions; ++i)
  {
    view.state.action[0][i].type = actions[i];
  }
  view.state.holeCards[position][0] = card;
  view.state.round = 0;

  return view;
}
