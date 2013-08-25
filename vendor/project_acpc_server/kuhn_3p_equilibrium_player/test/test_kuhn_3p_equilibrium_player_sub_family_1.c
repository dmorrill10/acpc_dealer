#include <unity.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define CEXCEPTION_USE_CONFIG_FILE
#include "CException.h"

#include "kuhn_3p_equilibrium_player.h"

#define DEBUG_PRINT(...) \
    printf(__VA_ARGS__); \
    fflush(NULL);

static int stderr_copy;
void setUp()
{
  // Silence error output
  stderr_copy = dup(fileno(stderr));
  freopen("/dev/null", "w", stderr);
}

void tearDown()
{
  // Restore error output
  fflush(stderr);
  dup2(stderr_copy, fileno(stderr));
  close(stderr_copy);
}

//player_view_t init_player_view(
//    const game_def_t const* game_def,
//    position_t position,
//    card_t card,
//    action_t* actions,
//    int num_actions
//)
//{
//  player_view_t view;
//
//  view.player = position;
//  view.state.player = position;
//  view.state.num_actions[0] = num_actions;
//
//  for (int i = 0; i < num_actions; ++i)
//  {
//    view.state.actions[0][i] = actions[i];
//  }
//  view.state.pc[position][0] = card;
//
//  return view;
//}
//
//void test_params_set_properly()
//{
//  game_def_t game_def = init_kuhn_poker_game_def();
//
//  CEXCEPTION_T e;
//
//  Try
//  {
//    kuhn_3p_equilibrium_player_t* patient = init_private_info(
//        &game_def,
//        "0 0.25 0.25 0.9375 0.0 1 12"
//    );
//    TEST_ASSERT_NOT_NULL(patient);
//
//    TEST_ASSERT_EQUAL_FLOAT(0.0, patient->params[C11_INDEX]);
//    TEST_ASSERT_EQUAL_FLOAT(0.25, patient->params[B11_INDEX]);
//    TEST_ASSERT_EQUAL_FLOAT(0.25, patient->params[B21_INDEX]);
//    TEST_ASSERT_EQUAL_FLOAT(0.9375, patient->params[B32_INDEX]);
//    TEST_ASSERT_EQUAL_FLOAT(0.0, patient->params[C33_INDEX]);
//    TEST_ASSERT_EQUAL_FLOAT(1.0, patient->params[C34_INDEX]);
//    TEST_ASSERT_EQUAL_FLOAT(12, patient->seed);
//
//    TEST_ASSERT_EQUAL_FLOAT(0.0, patient->params[B23_INDEX]);
//    TEST_ASSERT_EQUAL_FLOAT(
//        (1 + 0.25 + 2 * 0.25) / 2.0,
//        patient->params[B33_INDEX]
//    );
//    TEST_ASSERT_EQUAL_FLOAT(2 * 0.25 + 2 * 0.25, patient->params[B41_INDEX]);
//    TEST_ASSERT_EQUAL_FLOAT(1/2.0, patient->params[C21_INDEX]);
//
//    free(patient);
//  }
//  Catch(e)
//  {
//    TEST_FAIL_MESSAGE("Should Not Have Thrown An Error");
//  }
//}
//
//void test_b21_upper_bound()
//{
//  game_def_t game_def = init_kuhn_poker_game_def();
//
//  CEXCEPTION_T e;
//
//  Try
//  {
//    kuhn_3p_equilibrium_player_t* patient = init_private_info(
//        &game_def,
//        "0 0.25 0.26 0.9375 0.0 12"
//    );
//    TEST_FAIL_MESSAGE("Should Have Thrown An Error");
//    free(patient);
//  }
//  Catch(e)
//  {
//    TEST_ASSERT_EQUAL(-1, e);
//  }
//}
//
//void test_b11_upper_bound()
//{
//  game_def_t game_def = init_kuhn_poker_game_def();
//
//  CEXCEPTION_T e;
//
//  Try
//  {
//    kuhn_3p_equilibrium_player_t* patient = init_private_info(
//        &game_def,
//        "0 0.25 0.24 0.9375 0.0 12"
//    );
//    TEST_FAIL_MESSAGE("Should Have Thrown An Error");
//    free(patient);
//  }
//  Catch(e)
//  {
//    TEST_ASSERT_EQUAL(-1, e);
//  }
//}
//
//void test_b32_upper_bound()
//{
//  game_def_t game_def = init_kuhn_poker_game_def();
//
//  CEXCEPTION_T e;
//
//  Try
//  {
//    kuhn_3p_equilibrium_player_t* patient = init_private_info(
//        &game_def,
//        "0 0.25 0.25 0.9376 0.0 12"
//    );
//    TEST_FAIL_MESSAGE("Should Have Thrown An Error");
//    free(patient);
//  }
//  Catch(e)
//  {
//    TEST_ASSERT_EQUAL(-1, e);
//  }
//}
//
//void test_c33_lower_bound()
//{
//  game_def_t game_def = init_kuhn_poker_game_def();
//
//  CEXCEPTION_T e;
//
//  Try
//  {
//    kuhn_3p_equilibrium_player_t* patient = init_private_info(
//        &game_def,
//        "0 0.25 0.25 0.4 0.098 12"
//    );
//    TEST_FAIL_MESSAGE("Should Have Thrown An Error");
//    free(patient);
//  }
//  Catch(e)
//  {
//    TEST_ASSERT_EQUAL(-1, e);
//  }
//}
//
//void test_c33_upper_bound()
//{
//  game_def_t game_def = init_kuhn_poker_game_def();
//
//  CEXCEPTION_T e;
//
//  Try
//  {
//    kuhn_3p_equilibrium_player_t* patient = init_private_info(
//        &game_def,
//        "0 0.25 0.25 0.4 0.54 12"
//    );
//    TEST_FAIL_MESSAGE("Should Have Thrown An Error");
//    free(patient);
//  }
//  Catch(e)
//  {
//    TEST_ASSERT_EQUAL(-1, e);
//  }
//}
//
//// @todo Move to general tests
//void test_action_probs_in_position_A()
//{
//  game_def_t game_def = init_kuhn_poker_game_def();
//
//  CEXCEPTION_T e;
//
//  Try
//  {
//    kuhn_3p_equilibrium_player_t* patient = init_private_info(
//        &game_def,
//        "0 0.25 0.25 0.9375 0.0 1 12"
//    );
//    TEST_ASSERT_NOT_NULL(patient);
//
//    double probs[game_def.NUM_ACTIONS];
//    int num_actions = 0;
//    player_view_t view;
//
//    action_t actions1[] = {};
//    action_t actions2[] = {ACTION_CALL, ACTION_CALL, ACTION_RAISE};
//    action_t actions3[] = {ACTION_CALL, ACTION_RAISE, ACTION_FOLD};
//    action_t actions4[] = {ACTION_CALL, ACTION_RAISE, ACTION_CALL};
//
//    for (card_t card = JACK; card <= ACE; ++card)
//    {
//      // Situation 1
//      memset(probs, -1, game_def.NUM_ACTIONS * sizeof(*probs));
//      num_actions = 0;
//      view = init_player_view(
//          &game_def,
//          A_POSITION,
//          card,
//          actions1,
//          num_actions
//      );
//      get_action_probs(&view, 0, patient, probs);
//      TEST_ASSERT_EQUAL_FLOAT(0.0, probs[ACTION_FOLD]);
//      TEST_ASSERT_EQUAL_FLOAT(1 - A[card][0], probs[ACTION_CALL]);
//      TEST_ASSERT_EQUAL_FLOAT(A[card][0], probs[ACTION_RAISE]);
//
//      // Situation 2
//      memset(probs, -1, game_def.NUM_ACTIONS * sizeof(*probs));
//      num_actions = 3;
//      view = init_player_view(
//          &game_def,
//          A_POSITION,
//          card,
//          actions2,
//          num_actions
//      );
//      get_action_probs(&view, 0, patient, probs);
//      TEST_ASSERT_EQUAL_FLOAT(1 - A[card][1], probs[ACTION_FOLD]);
//      TEST_ASSERT_EQUAL_FLOAT(A[card][1], probs[ACTION_CALL]);
//      TEST_ASSERT_EQUAL_FLOAT(0.0, probs[ACTION_RAISE]);
//
//      // Situation 3
//      memset(probs, -1, game_def.NUM_ACTIONS * sizeof(*probs));
//      num_actions = 3;
//      view = init_player_view(
//          &game_def,
//          A_POSITION,
//          card,
//          actions3,
//          num_actions
//      );
//      get_action_probs(&view, 0, patient, probs);
//      TEST_ASSERT_EQUAL_FLOAT(1 - A[card][2], probs[ACTION_FOLD]);
//      TEST_ASSERT_EQUAL_FLOAT(A[card][2], probs[ACTION_CALL]);
//      TEST_ASSERT_EQUAL_FLOAT(0.0, probs[ACTION_RAISE]);
//
//      // Situation 4
//      memset(probs, -1, game_def.NUM_ACTIONS * sizeof(*probs));
//      num_actions = 3;
//      view = init_player_view(
//          &game_def,
//          A_POSITION,
//          card,
//          actions4,
//          num_actions
//      );
//      get_action_probs(&view, 0, patient, probs);
//      TEST_ASSERT_EQUAL_FLOAT(1 - A[card][3], probs[ACTION_FOLD]);
//      TEST_ASSERT_EQUAL_FLOAT(A[card][3], probs[ACTION_CALL]);
//      TEST_ASSERT_EQUAL_FLOAT(0.0, probs[ACTION_RAISE]);
//    }
//
//    free(patient);
//  }
//  Catch(e)
//  {
//    TEST_FAIL_MESSAGE("Should Not Have Thrown An Error");
//  }
//}
//
//void test_action_probs_in_position_B()
//{
//  game_def_t game_def = init_kuhn_poker_game_def();
//
//  CEXCEPTION_T e;
//
//  Try
//  {
//    kuhn_3p_equilibrium_player_t* patient = init_private_info(
//        &game_def,
//        "0 0.25 0.25 0.9375 0.0 1 12"
//    );
//    TEST_ASSERT_NOT_NULL(patient);
//
//    double probs[game_def.NUM_ACTIONS];
//    int num_actions = 0;
//    player_view_t view;
//
//    action_t actions1[] = {ACTION_CALL};
//    action_t actions2[] = {ACTION_RAISE};
//    action_t actions3[] = {
//        ACTION_CALL, ACTION_CALL, ACTION_RAISE, ACTION_FOLD
//    };
//    action_t actions4[] = {
//        ACTION_CALL, ACTION_CALL, ACTION_RAISE, ACTION_CALL
//    };
//    double param;
//
//    for (card_t card = JACK; card <= ACE; ++card)
//    {
//      // Situation 1
//      memset(probs, -1, game_def.NUM_ACTIONS * sizeof(*probs));
//      num_actions = 1;
//      view = init_player_view(
//          &game_def,
//          B_POSITION,
//          card,
//          actions1,
//          num_actions
//      );
//      switch (card)
//      {
//      case JACK:
//        param = patient->params[B11_INDEX];
//        break;
//      case QUEEN:
//        param = patient->params[B21_INDEX];
//        break;
//      case KING:
//        param = B31;
//        break;
//      default:
//        param = patient->params[B41_INDEX];
//      }
//      get_action_probs(&view, 0, patient, probs);
//
//      TEST_ASSERT_EQUAL_FLOAT(0.0, probs[ACTION_FOLD]);
//      TEST_ASSERT_EQUAL_FLOAT(1 - param, probs[ACTION_CALL]);
//      TEST_ASSERT_EQUAL_FLOAT(param, probs[ACTION_RAISE]);
//
//      // Situation 2
//      memset(probs, -1, game_def.NUM_ACTIONS * sizeof(*probs));
//      num_actions = 1;
//      view = init_player_view(
//          &game_def,
//          B_POSITION,
//          card,
//          actions2,
//          num_actions
//      );
//      switch (card)
//      {
//      case JACK:
//        param = B12;
//        break;
//      case QUEEN:
//        param = B22;
//        break;
//      case KING:
//        param = patient->params[B32_INDEX];
//        break;
//      default:
//        param = B42;
//      }
//      get_action_probs(&view, 0, patient, probs);
//
//      TEST_ASSERT_EQUAL_FLOAT(1 - param, probs[ACTION_FOLD]);
//      TEST_ASSERT_EQUAL_FLOAT(param, probs[ACTION_CALL]);
//      TEST_ASSERT_EQUAL_FLOAT(0.0, probs[ACTION_RAISE]);
//
//      // Situation 3
//      memset(probs, -1, game_def.NUM_ACTIONS * sizeof(*probs));
//      num_actions = 4;
//      view = init_player_view(
//          &game_def,
//          B_POSITION,
//          card,
//          actions3,
//          num_actions
//      );
//      switch (card)
//      {
//      case JACK:
//        param = B13;
//        break;
//      case QUEEN:
//        param = patient->params[B23_INDEX];
//        break;
//      case KING:
//        param = patient->params[B33_INDEX];
//        break;
//      default:
//        param = B43;
//      }
//      get_action_probs(&view, 0, patient, probs);
//
//      TEST_ASSERT_EQUAL_FLOAT(1 - param, probs[ACTION_FOLD]);
//      TEST_ASSERT_EQUAL_FLOAT(param, probs[ACTION_CALL]);
//      TEST_ASSERT_EQUAL_FLOAT(0.0, probs[ACTION_RAISE]);
//
//      // Situation 4
//      memset(probs, -1, game_def.NUM_ACTIONS * sizeof(*probs));
//      num_actions = 4;
//      view = init_player_view(
//          &game_def,
//          B_POSITION,
//          card,
//          actions4,
//          num_actions
//      );
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
//      get_action_probs(&view, 0, patient, probs);
//
//      TEST_ASSERT_EQUAL_FLOAT(1 - param, probs[ACTION_FOLD]);
//      TEST_ASSERT_EQUAL_FLOAT(param, probs[ACTION_CALL]);
//      TEST_ASSERT_EQUAL_FLOAT(0.0, probs[ACTION_RAISE]);
//    }
//
//    free(patient);
//  }
//  Catch(e)
//  {
//    TEST_FAIL_MESSAGE("Should Not Have Thrown An Error");
//  }
//}
//
//void test_action_probs_in_position_C()
//{
//  game_def_t game_def = init_kuhn_poker_game_def();
//
//  CEXCEPTION_T e;
//
//  Try
//  {
//    kuhn_3p_equilibrium_player_t* patient = init_private_info(
//        &game_def,
//        "0 0.25 0.25 0.9375 0.0 1 12"
//    );
//    TEST_ASSERT_NOT_NULL(patient);
//
//    double probs[game_def.NUM_ACTIONS];
//    int num_actions = 2;
//    player_view_t view;
//
//    action_t actions1[] = {ACTION_CALL, ACTION_CALL};
//    action_t actions2[] = {ACTION_CALL, ACTION_RAISE};
//    action_t actions3[] = {ACTION_RAISE, ACTION_FOLD};
//    action_t actions4[] = {ACTION_RAISE, ACTION_CALL};
//    double param;
//
//    for (card_t card = JACK; card <= ACE; ++card)
//    {
//      // Situation 1
//      memset(probs, -1, game_def.NUM_ACTIONS * sizeof(*probs));
//      view = init_player_view(
//          &game_def,
//          C_POSITION,
//          card,
//          actions1,
//          num_actions
//      );
//      switch (card)
//      {
//      case JACK:
//        param = patient->params[C11_INDEX];
//        break;
//      case QUEEN:
//        param = patient->params[C21_INDEX];
//        break;
//      case KING:
//        param = C31;
//        break;
//      default:
//        param = C4[0];
//      }
//      get_action_probs(&view, 0, patient, probs);
//
//      TEST_ASSERT_EQUAL_FLOAT(0.0, probs[ACTION_FOLD]);
//      TEST_ASSERT_EQUAL_FLOAT(1 - param, probs[ACTION_CALL]);
//      TEST_ASSERT_EQUAL_FLOAT(param, probs[ACTION_RAISE]);
//
//      // Situation 2
//      memset(probs, -1, game_def.NUM_ACTIONS * sizeof(*probs));
//      view = init_player_view(
//          &game_def,
//          C_POSITION,
//          card,
//          actions2,
//          num_actions
//      );
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
//      get_action_probs(&view, 0, patient, probs);
//
//      TEST_ASSERT_EQUAL_FLOAT(1 - param, probs[ACTION_FOLD]);
//      TEST_ASSERT_EQUAL_FLOAT(param, probs[ACTION_CALL]);
//      TEST_ASSERT_EQUAL_FLOAT(0.0, probs[ACTION_RAISE]);
//
//      // Situation 3
//      memset(probs, -1, game_def.NUM_ACTIONS * sizeof(*probs));
//      view = init_player_view(
//          &game_def,
//          C_POSITION,
//          card,
//          actions3,
//          num_actions
//      );
//      switch (card)
//      {
//      case JACK:
//        param = C13;
//        break;
//      case QUEEN:
//        param = C23;
//        break;
//      case KING:
//        param = patient->params[C33_INDEX];
//        break;
//      default:
//        param = C4[2];
//      }
//      get_action_probs(&view, 0, patient, probs);
//
//      TEST_ASSERT_EQUAL_FLOAT(1 - param, probs[ACTION_FOLD]);
//      TEST_ASSERT_EQUAL_FLOAT(param, probs[ACTION_CALL]);
//      TEST_ASSERT_EQUAL_FLOAT(0.0, probs[ACTION_RAISE]);
//
//      // Situation 4
//      memset(probs, -1, game_def.NUM_ACTIONS * sizeof(*probs));
//      view = init_player_view(
//          &game_def,
//          C_POSITION,
//          card,
//          actions4,
//          num_actions
//      );
//      switch (card)
//      {
//      case JACK:
//        param = C14;
//        break;
//      case QUEEN:
//        param = C24;
//        break;
//      case KING:
//        param = patient->params[C34_INDEX];
//        break;
//      default:
//        param = C4[3];
//      }
//      get_action_probs(&view, 0, patient, probs);
//
//      TEST_ASSERT_EQUAL_FLOAT(1 - param, probs[ACTION_FOLD]);
//      TEST_ASSERT_EQUAL_FLOAT(param, probs[ACTION_CALL]);
//      TEST_ASSERT_EQUAL_FLOAT(0.0, probs[ACTION_RAISE]);
//    }
//
//    free(patient);
//  }
//  Catch(e)
//  {
//    TEST_FAIL_MESSAGE("Should Not Have Thrown An Error");
//  }
//}
