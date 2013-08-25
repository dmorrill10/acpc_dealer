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
