/**
 * kuhn_3p_equilibrium_player.c
 * author:      Dustin Morrill (morrill@ualberta.ca)
 * date:        August 21, 2013
 * description: Player for 3-player Kuhn poker that plays according to an
 *              equilibrium component strategy specified by its given six
 *              parameters.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>

#include "CException.h"
void print_and_throw_error(const char const *message)
{
  fprintf(stderr, "ERROR: %s", message);
  Throw(1);
}
#define DEBUG_PRINT(...) \
    printf(__VA_ARGS__); \
    fflush(NULL);

#include "kuhn_3p_equilibrium_player.h"

bool is_3p_kuhn_poker_game(const Game const *game_def)
{
  assert(game_def);
  return (
      game_def->bettingType == limitBetting &&
      game_def->numRounds == 1 && game_def->maxRaises[0] == 1 &&
      game_def->numSuits == 1 && game_def->numRanks == 4 &&
      game_def->numHoleCards == 1 &&
      game_def->numBoardCards[0] == 0 &&
      game_def->numPlayers == 3
 );
}

double beta(double b11, double b21)
{
  return fmax(b11, b21);
}

size_t sub_family_number(double c11)
{
  for (size_t sub_family_index = 0; sub_family_index < NUM_SUB_FAMILIES - 1; ++sub_family_index)
  {
    if (c11 == SUB_FAMILY_DEFINING_PARAM_VALUES[sub_family_index])
    {
      // c11 is 0 or 1/2
      return sub_family_index + 1;
    }
  }

  if (c11 > SUB_FAMILY_DEFINING_PARAM_VALUES[NUM_SUB_FAMILIES - 2])
  {
    return NUM_SUB_FAMILIES + 1; // Illegal sub-family number
  }

  // c11 is between 0 and 1/2
  return NUM_SUB_FAMILIES;
}

void check_family_1_params(
    kuhn_3p_equilibrium_player_t* kuhn_3p_e_player
)
{
  assert(kuhn_3p_e_player);

  if (kuhn_3p_e_player->params[B21_INDEX] > 1/4.0)
  {
    print_and_throw_error(
        "kuhn_3p_equilibrium_player: b21 greater than 1/4\n"
    );
  }
  if (
      kuhn_3p_e_player->params[B11_INDEX] >
      kuhn_3p_e_player->params[B21_INDEX]
  )
  {
    print_and_throw_error(
        "kuhn_3p_equilibrium_player: b11 greater than b21\n"
    );
  }
  if (
      kuhn_3p_e_player->params[B32_INDEX] > (
          (
              2 + 3 * kuhn_3p_e_player->params[B11_INDEX] +
              4 * kuhn_3p_e_player->params[B21_INDEX]
          ) /
          4.0
      )
  )
  {
    print_and_throw_error(
        "kuhn_3p_equilibrium_player: b32 too large for any sub-family 1 "
        "equilibrium\n"
    );
  }
  if (
      kuhn_3p_e_player->params[C33_INDEX] < (
          (1/2.0) - kuhn_3p_e_player->params[B32_INDEX]
      )
  )
  {
    print_and_throw_error(
        "kuhn_3p_equilibrium_player: c33 too small for any sub-family 1 "
        "equilibrium\n"
    );
  }
  if (
      kuhn_3p_e_player->params[C33_INDEX] > (
          (1/2.0) - kuhn_3p_e_player->params[B32_INDEX] + (
              3 * kuhn_3p_e_player->params[B11_INDEX] +
              4 * kuhn_3p_e_player->params[B21_INDEX]
          ) / 4.0
      )
  )
  {
    print_and_throw_error(
        "kuhn_3p_equilibrium_player: c33 too large for any sub-family 1 "
        "equilibrium\n"
    );
  }

  kuhn_3p_e_player->params[B23_INDEX] = 0.0;
  kuhn_3p_e_player->params[B33_INDEX] = (
      1 +
      kuhn_3p_e_player->params[B11_INDEX] +
      2 * kuhn_3p_e_player->params[B21_INDEX]
  ) / 2.0;
  kuhn_3p_e_player->params[B41_INDEX] = (
      2 * kuhn_3p_e_player->params[B11_INDEX] +
      2 * kuhn_3p_e_player->params[B21_INDEX]
  );
  kuhn_3p_e_player->params[C21_INDEX] = 1/2.0;
  return;
}

void check_params(kuhn_3p_equilibrium_player_t* kuhn_3p_e_player)
{
  assert(kuhn_3p_e_player);

  switch (
      sub_family_number(
          kuhn_3p_e_player->params[SUB_FAMILY_DEFINING_PARAM_INDEX]
      )
  )
  {
  case 1:
    check_family_1_params(kuhn_3p_e_player);
    break;
  case 2:
//    check_family_2_params(kuhn_3p_e_player);
    break;
  case 3:
//    check_family_3_params(kuhn_3p_e_player);
    break;
  default:
    print_and_throw_error(
        "kuhn_3p_equilibrium_player: c11 parameter outside of range for any "
        "equilibrium sub-family\n"
    );
    return;
  }

  for (size_t i = 0; i < NUM_PARAMS; ++i) {
    if (
        kuhn_3p_e_player->params[i] < 0.0 ||
        kuhn_3p_e_player->params[i] > 1.0
    ) {
      print_and_throw_error(
         "kuhn_3p_equilibrium_player: strategy parameters must be in [0,1]\n"
      );
    }
  }
  return;
}

void action_probs_p0(uint8_t card, const State const* state, double* probs)
{
  assert(probs);
  assert(state);

  size_t situation_index = 0;

  if (0 == state->numActions[0]) // Situation 1
  {
    probs[a_fold] = 0.0;
    probs[a_call] = 1.0 - A[card][0];
    probs[a_raise] = A[card][0];
    return;

  } else if (a_call == state->action[0][1].type) // Situation 2
  {
    situation_index = 1;
  } else if (a_fold == state->action[0][2].type) // Situation 3
  {
    situation_index = 2;
  } else // Situation 4
  {
    situation_index = 3;
  }

  probs[a_fold] = 1.0 - A[card][situation_index];
  probs[a_call] = A[card][situation_index];
  probs[a_raise] = 0.0;

  return;
}

void action_probs_p1(
    const double const* params,
    uint8_t card,
    const State const* state,
    double* probs
)
{
  assert(params);
  assert(probs);
  assert(state);

  double param;

  // Situation 1 or 2
  if (1 == state->numActions[0])
  {
    if (a_call == state->action[0][0].type) // Situation 1
    {
      switch (card)
      {
      case JACK:
        param = params[B11_INDEX];
        break;
      case QUEEN:
        param = params[B21_INDEX];
        break;
      case KING:
        param = B31;
        break;
      default:
        param = params[B41_INDEX];
      }
      probs[a_fold] = 0.0;
      probs[a_call] = 1.0 - param;
      probs[a_raise] = param;
      return;
    } else // Situation 2
    {
      switch (card)
      {
      case JACK:
        param = B12;
        break;
      case QUEEN:
        param = B22;
        break;
      case KING:
        param = params[B32_INDEX];
        break;
      default:
        param = B42;
      }
    }
  } else // Situation 3 or 4
  {
    if (a_fold == state->action[0][3].type) // Situation 3
    {
      switch (card)
      {
      case JACK:
        param = B13;
        break;
      case QUEEN:
        param = params[B23_INDEX];
        break;
      case KING:
        param = params[B33_INDEX];
        break;
      default:
        param = B43;
      }
    } else // Situation 4
    {
      switch (card)
      {
      case JACK:
        param = B14;
        break;
      case QUEEN:
        param = B24;
        break;
      case KING:
        param = B34;
        break;
      default:
        param = B44;
      }
    }
  }

  probs[a_fold] = 1.0 - param;
  probs[a_call] = param;
  probs[a_raise] = 0.0;

  return;
}

void action_probs_p2(
    const double const* params,
    uint8_t card,
    const State const* state,
    double* probs
)
{
  assert(params);
  assert(probs);
  assert(state);

  double param;

  // Situation 1
  if (a_call == state->action[0][0].type)
  {
    if (a_call == state->action[0][1].type) // Situation 1
    {
      switch (card)
      {
      case JACK:
        param = params[C11_INDEX];
        break;
      case QUEEN:
        param = params[C21_INDEX];
        break;
      case KING:
        param = C31;
        break;
      default:
        param = C4[0];
      }
      probs[a_fold] = 0.0;
      probs[a_call] = 1.0 - param;
      probs[a_raise] = param;

      return;
    } else // Situation 2
    {
      switch (card)
      {
      case JACK:
        param = C12;
        break;
      case QUEEN:
        param = C22;
        break;
      case KING:
        param = C32;
        break;
      default:
        param = C4[1];
      }
    }
  } else // Situation 3 or 4
  {
    if (a_fold == state->action[0][1].type) // Situation 3
    {
      switch (card)
      {
      case JACK:
        param = C13;
        break;
      case QUEEN:
        param = C23;
        break;
      case KING:
        param = params[C33_INDEX];
        break;
      default:
        param = C4[2];
      }
    } else // Situation 4
    {
      switch (card)
      {
      case JACK:
        param = C14;
        break;
      case QUEEN:
        param = C24;
        break;
      case KING:
        param = params[C34_INDEX];
        break;
      default:
        param = C4[3];
      }
    }
  }

  probs[a_fold] = 1.0 - param;
  probs[a_call] = param;
  probs[a_raise] = 0.0;

  return;
}

/*********************************/
/****** Player Interface *********/
/*********************************/

/* create any private space needed for future calls
   game_def is a private copy allocated by the dealer for the player's use */
kuhn_3p_equilibrium_player_t init_private_info(
    const Game const* game_def,
    const double const* params,
    uint32_t seed
)
{
  assert(game_def);
  assert(params);

  kuhn_3p_equilibrium_player_t kuhn_3p_e_player;

  /* This player cannot be used outside of Kuhn poker */
  if(!is_3p_kuhn_poker_game(game_def))
  {
    print_and_throw_error(
        "kuhn_3p_equilibrium_player used in non-Kuhn game\n"
    );
    return kuhn_3p_e_player;
  }

  kuhn_3p_e_player.game_def = game_def;

  memset(
      kuhn_3p_e_player.params,
      0,
      NUM_PARAMS * sizeof(*kuhn_3p_e_player.params)
  );
  for (size_t i = 0; i < NUM_PARAMS; ++i)
  {
    kuhn_3p_e_player.params[i] = params[i];
  }

  kuhn_3p_e_player.seed = seed;

  CEXCEPTION_T e = 0;
  Try
  {
    check_params(&kuhn_3p_e_player);
  }
  Catch(e)
  {
    Throw(e);
  }

  /* Create our random number generators */
  init_genrand(&kuhn_3p_e_player.get_action_rng, kuhn_3p_e_player.seed);

  return kuhn_3p_e_player;
}

/* return a (valid!) action at the information state described by view */
Action action(
    kuhn_3p_equilibrium_player_t* player,
    MatchState view
)
{
  assert(player);

  double probs[NUM_ACTION_TYPES];
  memset(probs, 0, NUM_ACTION_TYPES * sizeof(*probs));

  action_probs(player, view, probs);

  double r = genrand_real2(&player->get_action_rng);
  enum ActionType i = a_fold;
  for(; i < NUM_ACTION_TYPES; i++) {
    if(r <= probs[i]) {
      break;
    }
    r -= probs[i];
  }

  Action a = {a.type = i, a.size = 0};

  return a;
}

/* Returns a distribution of actions in action_triple
   This is an extra function, and does _NOT_ need to be implemented
   to be considered a valid player.h interface. */
void action_probs(
    const kuhn_3p_equilibrium_player_t const* player,
    MatchState view,
    double* probs
)
{
  assert(player);
  assert(probs);

  uint8_t card = view.state.holeCards[view.viewingPlayer][0];

  switch (view.viewingPlayer)
  {
  case 0:
    action_probs_p0(card, &view.state, probs);
    return;
  case 1:
    action_probs_p1(player->params, card, &view.state, probs);
    return;
  case 2:
    action_probs_p2(player->params, card, &view.state, probs);
    return;
  default:
    print_and_throw_error(
        "kuhn_3p_equilibrium_player: action probabilities "
        "requested for an out of bounds seat.\n"
    );
    return;
  }
  return;

//  if(view.viewingPlayer == 0)
//  {
//    if(view.state->num_actions[ 0 ] == 0) {
//      /* We are in the case of <card><unknown_card> no actions */
//      if(card == JACK) {
//        action_probs[a_fold] = 0.0;
//        action_probs[a_call] = 1.0 - uplayer>params[ ALPHA ];
//        action_probs[a_raise] = uplayer>params[ALPHA];
//      } else if(card == QUEEN) {
//        action_probs[a_fold] = 0.0;
//        action_probs[a_call] = 1.0;
//        action_probs[a_raise] = 0.0;
//      } else if(card == KING) {
//        action_probs[a_fold] = 0.0;
//        action_probs[a_call] = 1.0 - uplayer>params[ GAMMA ];
//        action_probs[a_raise] = uplayer>params[ GAMMA ];
//      } else {
//        fprintf(stderr, "IMPOSSIBLE! Unhanded case\n");
//        exit(-1);
//      }
//    } else {
//      /* We are in the case of <card><unknown_card> pb */
//      if(card == JACK) {
//        action_probs[a_fold] = 1.0;
//        action_probs[a_call] = 0.0;
//        action_probs[a_raise] = 0.0;
//      } else if(card == QUEEN) {
//        action_probs[a_fold] = 1.0 - uplayer>params[ BETA ];
//        action_probs[a_call] = uplayer>params[ BETA ];
//        action_probs[a_raise] = 0.0;
//      } else if(card == KING) {
//        action_probs[a_fold] = 0.0;
//        action_probs[a_call] = 1.0;
//        action_probs[a_raise] = 0.0;
//      } else {
//        fprintf(stderr, "IMPOSSIBLE! Unhanded case\n");
//        exit(-1);
//      }
//    }
//  } else {
//    /* Player 2 action probs*/
//    /* We are in <unknown_card><card> pass */
//    if(view.state->action[ 0 ][ 0 ] == a_call) {
//      if(card == JACK) {
//        action_probs[a_fold] = 0.0;
//        action_probs[a_call] = 1.0 - uplayer>params[ XI ];
//        action_probs[a_raise] = uplayer>params[ XI ];
//      } else if(card == QUEEN) {
//        action_probs[a_fold] = 0.0;
//        action_probs[a_call] = 1.0;
//        action_probs[a_raise] = 0.0;
//      } else if(card == KING) {
//        action_probs[a_fold] = 0.0;
//        action_probs[a_call] = 0.0;
//        action_probs[a_raise] = 1.0;
//      } else {
//        fprintf(stderr, "IMPOSSIBLE! Unhanded case\n");
//        exit(-1);
//      }
//    } else {
//      /* We are in <unknown_card><card> bet */
//      if(card == JACK) {
//        action_probs[a_fold] = 1.0;
//        action_probs[a_call] = 0.0;
//        action_probs[a_raise] = 0.0;
//      } else if(card == QUEEN) {
//        action_probs[a_fold] = 1.0 - uplayer>params[ ETA ];
//        action_probs[a_call] = uplayer>params[ ETA ];
//        action_probs[a_raise] = 0.0;
//      } else if(card == KING) {
//        action_probs[a_fold] = 0.0;
//        action_probs[a_call] = 1.0;
//        action_probs[a_raise] = 0.0;
//      } else {
//        fprintf(stderr, "IMPOSSIBLE! Unhanded case\n");
//        exit(-1);
//      }
//    }
//  }
}

///* player view of the game in the final state */
//void game_over(MatchState view, uint32_t game_id, void *private_info)
//{
//}
