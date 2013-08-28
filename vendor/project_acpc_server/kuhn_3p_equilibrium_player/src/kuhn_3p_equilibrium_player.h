#ifndef KUHN_3P_EQUILIBRIUM_PLAYER_H
#define KUHN_3P_EQUILIBRIUM_PLAYER_H
/**
 * kuhn_3p_equilibrium_player.h
 * author:      Dustin Morrill (morrill@ualberta.ca)
 * date:        August 21, 2013
 * description: Player for 3-player Kuhn poker that plays according to an
 *              equilibrium component strategy specified by its given six
 *              parameters.
 */

#include "game.h"
#include "rng.h"
#include "net.h"

#define DEBUG 0

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
} parameter_index_t;

typedef struct {
  uint32_t seed;
  rng_state_t get_action_rng;
  const Game* game_def;
  double params[NUM_PARAMS];
} kuhn_3p_equilibrium_player_t;

typedef enum{JACK_RANK = 9, QUEEN_RANK, KING_RANK, ACE_RANK} card_rank_t;

typedef enum{A_POSITION = 0, B_POSITION, C_POSITION} position_t;

// Constants -------------
#define KUHN_SUIT 3
#define JACK makeCard(JACK_RANK, KUHN_SUIT)
#define QUEEN makeCard(QUEEN_RANK, KUHN_SUIT)
#define KING makeCard(KING_RANK, KUHN_SUIT)
#define ACE makeCard(ACE_RANK, KUHN_SUIT)

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

// Sub-family definitions
#define NUM_SUB_FAMILIES 3
#define SUB_FAMILY_DEFINING_PARAM_INDEX C11_INDEX
static const double SUB_FAMILY_DEFINING_PARAM_VALUES[] = {0.0, 1/2.0};

// Functions -----------------
double beta(const double b11, const double b21);
size_t sub_family_number(double c11);
kuhn_3p_equilibrium_player_t init_private_info(
    const Game const* game_def,
    const double const* params,
    uint32_t seed
);
Action action(
    kuhn_3p_equilibrium_player_t* player,
    MatchState view
);
void action_probs(
    const kuhn_3p_equilibrium_player_t const* player,
    MatchState view,
    double* probs
);

#endif
