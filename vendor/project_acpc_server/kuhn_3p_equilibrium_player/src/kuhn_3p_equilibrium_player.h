/*
Copyright (C) 2013 by the Computer Poker Research Group, University of Alberta
*/

#ifndef __KUHN_3P_EQUILIBRIUM_PLAYER_H__
#define __KUHN_3P_EQUILIBRIUM_PLAYER_H__

#include <game.h>
#include <rng.h>
#include <net.h>

// Types ----------------
typedef enum {
  C11_INDEX = 0,
  B11_INDEX,
  B21_INDEX,
  B23_INDEX,
  B32_INDEX,
  B33_INDEX,
  B41_INDEX,
  C21_INDEX,
  C33_INDEX,
  C34_INDEX,
  NUM_PARAMS
} ParameterIndex;

typedef struct {
  uint32_t seed;
  rng_state_t get_action_rng;
  const Game* game_def;
  double params[NUM_PARAMS];
} Kuhn3pEquilibriumPlayer;

typedef enum{JACK_RANK = 9, QUEEN_RANK, KING_RANK, ACE_RANK} CardRank;

typedef enum{A_POSITION = 0, B_POSITION, C_POSITION, NUM_PLAYERS} PlayerPosition;

// Constants -------------
#define KUHN_SUIT 3
#define JACK makeCard(JACK_RANK, KUHN_SUIT)
#define QUEEN makeCard(QUEEN_RANK, KUHN_SUIT)
#define KING makeCard(KING_RANK, KUHN_SUIT)
#define ACE makeCard(ACE_RANK, KUHN_SUIT)
#define NUM_CARDS 4

#define KAPPA (1/24.0)

static const int SUB_FAMILY_1_INDEPENDENT_PARAMS[] = {
    B11_INDEX, B21_INDEX, B32_INDEX, C33_INDEX, C34_INDEX
};

static const int SUB_FAMILY_2_INDEPENDENT_PARAMS[] = {
    B11_INDEX, B21_INDEX, B23_INDEX, B32_INDEX, C33_INDEX, C34_INDEX
};

static const int SUB_FAMILY_3_INDEPENDENT_PARAMS[] = {
    B11_INDEX, B32_INDEX, C33_INDEX, C34_INDEX
};

// Necessary params for P1
static const double A[][4] = {
    {0.0, 0.0, 0.0,   0.0},
    {0.0, 0.0, 0.0,   0.0},
    {0.0, 0.0, 1/2.0, 0.0},
    {0.0, 1.0, 1.0,   1.0}
};

// Necessary params for P2
#define B12 0.0
#define B13 0.0
#define B14 0.0
#define B22 0.0
#define B24 0.0
#define B31 0.0
#define B34 0.0
#define B42 1.0
#define B43 1.0
#define B44 1.0

// Necessary params for P3
#define C12 0.0
#define C13 0.0
#define C14 0.0
#define C22 0.0
#define C23 0.0
#define C24 0.0
#define C31 0.0
#define C32 0.0
static const double C4[] = {1.0, 1.0, 1.0, 1.0};

// Functions -----------------
double beta(const Kuhn3pEquilibriumPlayer* kuhn_3p_e_player);
Kuhn3pEquilibriumPlayer new_kuhn_3p_equilibrium_player(
    const Game* game_def,
    const double* params,
    uint32_t seed
);
Action action(
    Kuhn3pEquilibriumPlayer* player,
    MatchState view
);
void action_probs(
    const Kuhn3pEquilibriumPlayer* player,
    MatchState view,
    double* probs
);

#endif
