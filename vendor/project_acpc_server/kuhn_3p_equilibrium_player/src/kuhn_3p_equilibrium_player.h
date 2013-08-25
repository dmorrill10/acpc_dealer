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

// Types ----------------
typedef struct {
  uint32_t seed;
  rng_state_t get_action_rng;
  Game *game_def;
  double params[ 10 ];
} kuhn_3p_equilibrium_player_t;

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

typedef enum{JACK = 0, QUEEN, KING, ACE} card_t;

typedef enum{A_POSITION = 0, B_POSITION, C_POSITION} position_t;

// Constants -------------
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
static const float A[][4] = {
    {0, 0, 0,     0},
    {0, 0, 0,     0},
    {0, 0, 1/2.0, 0},
    {0, 1, 1,     1}
};

// Necessary params for P2
#define B12 0
#define B13 0
#define B14 0
#define B22 0
#define B24 0
#define B31 0
#define B34 0
#define B42 1
#define B43 1
#define B44 1

// Necessary params for P3
#define C12 0
#define C13 0
#define C14 0
#define C22 0
#define C23 0
#define C24 0
#define C31 0
#define C32 0
static const float C4[] = {1, 1, 1, 1};

// Sub-family definitions
#define NUM_SUB_FAMILIES 3
#define SUB_FAMILY_DEFINING_PARAM_INDEX C11_INDEX
static const float SUB_FAMILY_DEFINING_PARAM_VALUES[] = {0.0, 1/2.0};

// Functions -----------------
double beta(const double b11, const double b21);
kuhn_3p_equilibrium_player_t init_private_info(const Game const* game_def, const char const* arg_string);

#endif
