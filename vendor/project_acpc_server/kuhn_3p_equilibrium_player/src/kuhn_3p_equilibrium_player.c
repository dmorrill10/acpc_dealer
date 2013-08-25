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

#define CEXCEPTION_USE_CONFIG_FILE
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

int sub_family_number(const kuhn_3p_equilibrium_player_t const* kuhn_3p_e_player)
{
  assert(kuhn_3p_e_player);

  int sub_family_index = 0;
  for (; sub_family_index < NUM_SUB_FAMILIES - 1; ++sub_family_index)
  {
    if (
        kuhn_3p_e_player->params[SUB_FAMILY_DEFINING_PARAM_INDEX] ==
        SUB_FAMILY_DEFINING_PARAM_VALUES[sub_family_index]
    )
    {
      return sub_family_index + 1;
    }
  }

  if (
      kuhn_3p_e_player->params[SUB_FAMILY_DEFINING_PARAM_INDEX] >
      SUB_FAMILY_DEFINING_PARAM_VALUES[NUM_SUB_FAMILIES - 2]
  )
  {
    return NUM_SUB_FAMILIES + 1; // Illegal sub-family number
  }

  return NUM_SUB_FAMILIES;
}

void extract_family_1_params(
    kuhn_3p_equilibrium_player_t* kuhn_3p_e_player,
    const char const* arg_string
)
{
  assert(kuhn_3p_e_player);
  assert(arg_string);
  if (
      sscanf(
          arg_string,
          "%lf %lf %lf %lf %lf %lf %"SCNu32,
          &(kuhn_3p_e_player->params[C11_INDEX]),
          &(kuhn_3p_e_player->params[SUB_FAMILY_1_INDEPENDENT_PARAMS[0]]),
          &(kuhn_3p_e_player->params[SUB_FAMILY_1_INDEPENDENT_PARAMS[1]]),
          &(kuhn_3p_e_player->params[SUB_FAMILY_1_INDEPENDENT_PARAMS[2]]),
          &(kuhn_3p_e_player->params[SUB_FAMILY_1_INDEPENDENT_PARAMS[3]]),
          &(kuhn_3p_e_player->params[SUB_FAMILY_1_INDEPENDENT_PARAMS[4]]),
          &(kuhn_3p_e_player->seed)
      ) != 7
     )
  {
    print_and_throw_error(
        "kuhn_3p_equilibrium_player: not enough parameters found for a "
        "sub-family 1 equilibrium\n"
    );
  }
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

void parse_arg_string(
    kuhn_3p_equilibrium_player_t* kuhn_3p_e_player,
    const char const* arg_string
)
{
  assert(kuhn_3p_e_player);
  assert(arg_string);
  if (!(sscanf(arg_string, "%lf", &(kuhn_3p_e_player->params[C11_INDEX])) == 1))
  {
    print_and_throw_error(
        "kuhn_3p_equilibrium_player: c11 parameter not found in argument "
        "string\n"
    );
  }

  switch (sub_family_number(kuhn_3p_e_player))
  {
  case 1:
    extract_family_1_params(kuhn_3p_e_player, arg_string);
    break;
  case 2:
//    extract_family_2_params(kuhn_3p_e_player, arg_string);
    break;
  case 3:
//    extract_family_3_params(kuhn_3p_e_player, arg_string);
    break;
  default:
    print_and_throw_error(
        "kuhn_3p_equilibrium_player: c11 parameter outside of range for any "
        "equilibrium sub-family\n"
    );
    return;
  }

  uint i = 0;
  for (; i < NUM_PARAMS; ++i) {
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

//void get_action_probs_p0(
//    int card,
//    const game_state_t const * state,
//    double* action_probs
//)
//{
//  uint situation_index = 0;
//  if (0 == state->num_actions[0]) // Situation 1
//  {
//    action_probs[ACTION_FOLD] = 0.0;
//    action_probs[ACTION_CALL] = 1.0 - A[card][0];
//    action_probs[ACTION_RAISE] = A[card][0];
//    return;
//  } else if (ACTION_CALL == state->actions[0][1]) // Situation 2
//  {
//    situation_index = 1;
//  } else if (ACTION_FOLD == state->actions[0][2]) // Situation 3
//  {
//    situation_index = 2;
//  } else // Situation 4
//  {
//    situation_index = 3;
//  }
//  action_probs[ACTION_FOLD] = 1.0 - A[card][situation_index];
//  action_probs[ACTION_CALL] = A[card][situation_index];
//  action_probs[ACTION_RAISE] = 0.0;
//  return;
//}
//
//void get_action_probs_p1(
//    const double const* params,
//    int card,
//    const game_state_t const * state,
//    double* action_probs
//)
//{
//  double param;
//  // Situation 1 or 2
//  if (1 == state->num_actions[0])
//  {
//    if (ACTION_CALL == state->actions[0][0]) // Situation 1
//    {
//      switch (card)
//      {
//      case JACK:
//        param = params[B11_INDEX];
//        break;
//      case QUEEN:
//        param = params[B21_INDEX];
//        break;
//      case KING:
//        param = B31;
//        break;
//      default:
//        param = params[B41_INDEX];
//      }
//      action_probs[ACTION_FOLD] = 0.0;
//      action_probs[ACTION_CALL] = 1.0 - param;
//      action_probs[ACTION_RAISE] = param;
//      return;
//    } else // Situation 2
//    {
//      switch (card)
//      {
//      case JACK:
//        param = B12;
//        break;
//      case QUEEN:
//        param = B22;
//        break;
//      case KING:
//        param = params[B32_INDEX];
//        break;
//      default:
//        param = B42;
//      }
//    }
//  } else // Situation 3 or 4
//  {
//    if (ACTION_FOLD == state->actions[0][3]) // Situation 3
//    {
//      switch (card)
//      {
//      case JACK:
//        param = B13;
//        break;
//      case QUEEN:
//        param = params[B23_INDEX];
//        break;
//      case KING:
//        param = params[B33_INDEX];
//        break;
//      default:
//        param = B43;
//      }
//    } else // Situation 4
//    {
//      switch (card)
//      {
//      case JACK:
//        param = B14;
//        break;
//      case QUEEN:
//        param = B24;
//        break;
//      case KING:
//        param = B34;
//        break;
//      default:
//        param = B44;
//      }
//    }
//  }
//
//  action_probs[ACTION_FOLD] = 1.0 - param;
//  action_probs[ACTION_CALL] = param;
//  action_probs[ACTION_RAISE] = 0.0;
//
//  return;
//}
//
//void get_action_probs_p2(
//    const double const* params,
//    int card,
//    const game_state_t const * state,
//    double* action_probs
//)
//{
//  double param;
//  // Situation 1
//  if (ACTION_CALL == state->actions[0][0])
//  {
//    if (ACTION_CALL == state->actions[0][1]) // Situation 1
//    {
//      switch (card)
//      {
//      case JACK:
//        param = params[C11_INDEX];
//        break;
//      case QUEEN:
//        param = params[C21_INDEX];
//        break;
//      case KING:
//        param = C31;
//        break;
//      default:
//        param = C4[0];
//      }
//      action_probs[ACTION_FOLD] = 0.0;
//      action_probs[ACTION_CALL] = 1.0 - param;
//      action_probs[ACTION_RAISE] = param;
//      return;
//    } else // Situation 2
//    {
//      switch (card)
//      {
//      case JACK:
//        param = C12;
//        break;
//      case QUEEN:
//        param = C22;
//        break;
//      case KING:
//        param = C32;
//        break;
//      default:
//        param = C4[1];
//      }
//    }
//  } else // Situation 3 or 4
//  {
//    if (ACTION_FOLD == state->actions[0][1]) // Situation 3
//    {
//      switch (card)
//      {
//      case JACK:
//        param = C13;
//        break;
//      case QUEEN:
//        param = C23;
//        break;
//      case KING:
//        param = params[C33_INDEX];
//        break;
//      default:
//        param = C4[2];
//      }
//    } else // Situation 4
//    {
//      switch (card)
//      {
//      case JACK:
//        param = C14;
//        break;
//      case QUEEN:
//        param = C24;
//        break;
//      case KING:
//        param = params[C34_INDEX];
//        break;
//      default:
//        param = C4[3];
//      }
//    }
//  }
//
//  action_probs[ACTION_FOLD] = 1.0 - param;
//  action_probs[ACTION_CALL] = param;
//  action_probs[ACTION_RAISE] = 0.0;
//
//  return;
//}
//
/*********************************/
/****** Player Interface *********/
/*********************************/

/* create any private space needed for future calls
   game_def is a private copy allocated by the dealer for the player's use */
kuhn_3p_equilibrium_player_t init_private_info(const Game const* game_def, const char const* arg_string)
{
  assert(game_def);
  assert(arg_string);

  /* This player cannot be used outside of Kuhn poker */
  if(!is_3p_kuhn_poker_game(game_def))
  {
    print_and_throw_error(
        "kuhn_3p_equilibrium_player used in non-Kuhn game\n"
    );
  }

  kuhn_3p_equilibrium_player_t kuhn_3p_e_player;
  kuhn_3p_e_player.game_def = game_def;

  CEXCEPTION_T e;
  Try
  {
    parse_arg_string(&kuhn_3p_e_player, arg_string);
  }
  Catch(e)
  {
    Throw(e);
  }

  /* Create our random number generators */
  init_genrand(&kuhn_3p_e_player.get_action_rng, kuhn_3p_e_player.seed);

  return kuhn_3p_e_player;
}

///* a new tournament is starting - re-initialise private state if necessary */
//void new_tournament(void *private_info)
//{
//}
//
///* return a (valid!) action at the information state described by view */
//int get_action(player_view_t *view, uint32_t game_id, void *private_info)
//{
//  int i;
//  double r;
//  kuhn_3p_equilibrium_player_t * ukuhn_player =
//      (kuhn_3p_equilibrium_player_t *) private_info;
//  double action_probs[ ukuhn_player->game_def->NUM_ACTIONS ];
//
//  get_action_probs(view, game_id, private_info, action_probs);
//
//  r = genrand_real2(&ukuhn_player->get_action_rng);
//  for(i = 0; i < ukuhn_player->game_def->NUM_ACTIONS - 1; i++) {
//    if(r <= action_probs[ i ]) {
//      break;
//    }
//    r -= action_probs[ i ];
//  }
//
//  return i;
//}
//
///* Returns a distribution of actions in action_triple
//   This is an extra function, and does _NOT_ need to be implemented
//   to be considered a valid player.h interface. */
//void get_action_probs(
//    player_view_t *view,
//    uint32_t game_id,
//    void *private_info,
//    double action_probs[]
//)
//{
//  kuhn_3p_equilibrium_player_t* kuhn_player =
//      (kuhn_3p_equilibrium_player_t*) private_info;
//
//  card_t card = view->state.pc[view->player][0];
//
//  switch (view->player)
//  {
//  case 0:
//    get_action_probs_p0(card, &view->state, action_probs);
//    return;
//  case 1:
//    get_action_probs_p1(kuhn_player->params, card, &view->state, action_probs);
//    return;
//  case 2:
//    get_action_probs_p2(kuhn_player->params, card, &view->state, action_probs);
//    return;
//  default:
//    print_and_throw_error(
//        "kuhn_3p_equilibrium_player: action probabilities "
//        "requested for an out of bounds seat.\n"
//    );
//  }
//
////  if(view->player == 0)
////  {
////    if(view->state.num_actions[ 0 ] == 0) {
////      /* We are in the case of <card><unknown_card> no actions */
////      if(card == JACK) {
////        action_probs[ACTION_FOLD] = 0.0;
////        action_probs[ACTION_CALL] = 1.0 - ukuhn_player->params[ ALPHA ];
////        action_probs[ACTION_RAISE] = ukuhn_player->params[ALPHA];
////      } else if(card == QUEEN) {
////        action_probs[ACTION_FOLD] = 0.0;
////        action_probs[ACTION_CALL] = 1.0;
////        action_probs[ACTION_RAISE] = 0.0;
////      } else if(card == KING) {
////        action_probs[ACTION_FOLD] = 0.0;
////        action_probs[ACTION_CALL] = 1.0 - ukuhn_player->params[ GAMMA ];
////        action_probs[ACTION_RAISE] = ukuhn_player->params[ GAMMA ];
////      } else {
////        fprintf(stderr, "IMPOSSIBLE! Unhanded case\n");
////        exit(-1);
////      }
////    } else {
////      /* We are in the case of <card><unknown_card> pb */
////      if(card == JACK) {
////        action_probs[ACTION_FOLD] = 1.0;
////        action_probs[ACTION_CALL] = 0.0;
////        action_probs[ACTION_RAISE] = 0.0;
////      } else if(card == QUEEN) {
////        action_probs[ACTION_FOLD] = 1.0 - ukuhn_player->params[ BETA ];
////        action_probs[ACTION_CALL] = ukuhn_player->params[ BETA ];
////        action_probs[ACTION_RAISE] = 0.0;
////      } else if(card == KING) {
////        action_probs[ACTION_FOLD] = 0.0;
////        action_probs[ACTION_CALL] = 1.0;
////        action_probs[ACTION_RAISE] = 0.0;
////      } else {
////        fprintf(stderr, "IMPOSSIBLE! Unhanded case\n");
////        exit(-1);
////      }
////    }
////  } else {
////    /* Player 2 action probs*/
////    /* We are in <unknown_card><card> pass */
////    if(view->state.actions[ 0 ][ 0 ] == ACTION_CALL) {
////      if(card == JACK) {
////        action_probs[ACTION_FOLD] = 0.0;
////        action_probs[ACTION_CALL] = 1.0 - ukuhn_player->params[ XI ];
////        action_probs[ACTION_RAISE] = ukuhn_player->params[ XI ];
////      } else if(card == QUEEN) {
////        action_probs[ACTION_FOLD] = 0.0;
////        action_probs[ACTION_CALL] = 1.0;
////        action_probs[ACTION_RAISE] = 0.0;
////      } else if(card == KING) {
////        action_probs[ACTION_FOLD] = 0.0;
////        action_probs[ACTION_CALL] = 0.0;
////        action_probs[ACTION_RAISE] = 1.0;
////      } else {
////        fprintf(stderr, "IMPOSSIBLE! Unhanded case\n");
////        exit(-1);
////      }
////    } else {
////      /* We are in <unknown_card><card> bet */
////      if(card == JACK) {
////        action_probs[ACTION_FOLD] = 1.0;
////        action_probs[ACTION_CALL] = 0.0;
////        action_probs[ACTION_RAISE] = 0.0;
////      } else if(card == QUEEN) {
////        action_probs[ACTION_FOLD] = 1.0 - ukuhn_player->params[ ETA ];
////        action_probs[ACTION_CALL] = ukuhn_player->params[ ETA ];
////        action_probs[ACTION_RAISE] = 0.0;
////      } else if(card == KING) {
////        action_probs[ACTION_FOLD] = 0.0;
////        action_probs[ACTION_CALL] = 1.0;
////        action_probs[ACTION_RAISE] = 0.0;
////      } else {
////        fprintf(stderr, "IMPOSSIBLE! Unhanded case\n");
////        exit(-1);
////      }
////    }
////  }
//}
//
///* player view of the game in the final state */
//void game_over(player_view_t *view, uint32_t game_id, void *private_info)
//{
//}
