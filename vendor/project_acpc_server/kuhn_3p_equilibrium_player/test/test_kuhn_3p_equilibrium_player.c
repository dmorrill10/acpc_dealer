#include <unity.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "test_helper.h"

#define CEXCEPTION_USE_CONFIG_FILE
#include "CException.h"

#include "kuhn_3p_equilibrium_player.h"

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

void test_init_private_info_throws_error_when_game_def_not_kuhn()
{
  Game game_def;

  CEXCEPTION_T e = 0;

  Try
  {
    init_private_info(&game_def, "0.1 0.1 0.1 0.1 0.1 12");
    TEST_FAIL_MESSAGE("Should Have Thrown An Error");
  }
  Catch(e)
  {
    TEST_ASSERT_EQUAL(1, e);
  }
}

void test_init_private_info_does_not_crash()
{
  Game game_def = init_kuhn_poker_game_def();

  CEXCEPTION_T e = 0;

  Try
  {
    kuhn_3p_equilibrium_player_t patient = init_private_info(&game_def, "0.1 0.1 0.1 0.1 0.1 12");
  }
  Catch(e)
  {
    TEST_FAIL_MESSAGE("Should Not Have Thrown An Error");
  }
}

void test_params_are_set_properly_for_family_2()
{
  Game game_def = init_kuhn_poker_game_def();
  TEST_IGNORE_MESSAGE("IMPLEMENT");
  return;
}

void test_params_are_set_properly_for_family_3()
{
  Game game_def = init_kuhn_poker_game_def();
  TEST_IGNORE_MESSAGE("IMPLEMENT");
}

void test_unspecified_params_can_be_specified_by_init_string()
{
  Game game_def = init_kuhn_poker_game_def();
  TEST_IGNORE_MESSAGE("IMPLEMENT");
}

void test_empty_param_string_throws_error()
{
  Game game_def = init_kuhn_poker_game_def();

  CEXCEPTION_T e = 0;

  Try
  {
    kuhn_3p_equilibrium_player_t patient = init_private_info(&game_def, "");
    TEST_FAIL_MESSAGE("Should Have Thrown An Error");
  }
  Catch(e)
  {
    TEST_ASSERT_EQUAL(1, e);
  }
}

void test_out_of_bounds_c11_throws_error()
{
  Game game_def = init_kuhn_poker_game_def();

  CEXCEPTION_T e = 0;

  Try
  {
    kuhn_3p_equilibrium_player_t patient = init_private_info(&game_def, "0.51 0.25 0.25 0.9375 0.0 12");
    TEST_FAIL_MESSAGE("Should Have Thrown An Error");
  }
  Catch(e)
  {
    TEST_ASSERT_EQUAL(1, e);
  }
}

void test_param_invalid_probability_throws_error()
{
  Game game_def = init_kuhn_poker_game_def();

  CEXCEPTION_T e = 0;

  char valid_params[] = "0.0 0.0 0.0"; // Sub-family 3 for simplicity
  char params[256];
  float invalid_param = -0.1;
  for (; invalid_param < 2.0; invalid_param += 2.0)
  {
    Try
    {
      memset(params, 0, strlen(valid_params) * sizeof(*params));
      sprintf(params, "%f3 %s", invalid_param, valid_params);
      kuhn_3p_equilibrium_player_t patient = init_private_info(&game_def, params);
      TEST_FAIL_MESSAGE("Should Have Thrown An Error");
    }
    Catch(e)
    {
      TEST_ASSERT_EQUAL(1, e);
    }
  }
}

void test_action_probs_in_position_A()
{
  TEST_IGNORE_MESSAGE("ADJUST DELTA");
  return;

  Game game_def = init_kuhn_poker_game_def();

  CEXCEPTION_T e = 0;

  Try
  {
    // @todo Loop with a string (or strings) from each sub-family
    kuhn_3p_equilibrium_player_t patient = init_private_info(
        &game_def,
        "0 0.25 0.25 0.9375 0.0 1 12"
    );

    uint num_actions = 0;

    double* probs;

    Action actions1[] = {};
    Action actions2[] = {a_call, a_call, a_raise};
    Action actions3[] = {a_call, a_raise, a_fold};
    Action actions4[] = {a_call, a_raise, a_call};

    uint8_t card = JACK;
    for (; card <= ACE; ++card)
    {
      // Situation 1
      num_actions = 0;

      probs = action_probs(
          patient,
          init_match_state(
              &game_def,
              A_POSITION,
              card,
              actions1,
              num_actions
          )
      );

      DEBUG_PRINT("fold: %lf, call: %lf, raise: %lf\n", probs[a_fold], probs[a_call], probs[a_raise]);

      TEST_ASSERT_EQUAL_FLOAT(0.0, probs[a_fold]);
      TEST_ASSERT_EQUAL_FLOAT(1 - A[card][0], probs[a_call]);
      TEST_ASSERT_EQUAL_FLOAT(A[card][0], probs[a_raise]);

      // Situation 2
      num_actions = 3;

      probs = action_probs(
          patient,
          init_match_state(
              &game_def,
              A_POSITION,
              card,
              actions2,
              num_actions
          )
      );
      TEST_ASSERT_EQUAL_FLOAT(1 - A[card][1], probs[a_fold]);
      TEST_ASSERT_EQUAL_FLOAT(A[card][1], probs[a_call]);
      TEST_ASSERT_EQUAL_FLOAT(0.0, probs[a_raise]);

      // Situation 3
      num_actions = 3;

      probs = action_probs(
          patient,
          init_match_state(
              &game_def,
              A_POSITION,
              card,
              actions3,
              num_actions
          )
      );
      TEST_ASSERT_EQUAL_FLOAT(1 - A[card][2], probs[a_fold]);
      TEST_ASSERT_EQUAL_FLOAT(A[card][2], probs[a_call]);
      TEST_ASSERT_EQUAL_FLOAT(0.0, probs[a_raise]);

      // Situation 4
      num_actions = 3;

      probs = action_probs(
          patient,
          init_match_state(
              &game_def,
              A_POSITION,
              card,
              actions4,
              num_actions
          )
      );
      TEST_ASSERT_EQUAL_FLOAT(1 - A[card][3], probs[a_fold]);
      TEST_ASSERT_EQUAL_FLOAT(A[card][3], probs[a_call]);
      TEST_ASSERT_EQUAL_FLOAT(0.0, probs[a_raise]);
    }
  }
  Catch(e)
  {
    TEST_FAIL_MESSAGE("Should Not Have Thrown An Error");
  }
}

void test_action_probs_in_position_B()
{
  TEST_IGNORE_MESSAGE("ADJUST DELTA");
  return;

  Game game_def = init_kuhn_poker_game_def();

  CEXCEPTION_T e = 0;

  Try
  {
    // @todo Loop with a string (or strings) from each sub-family
    kuhn_3p_equilibrium_player_t patient = init_private_info(
        &game_def,
        "0 0.25 0.25 0.9375 0.0 1 12"
    );

    double *probs;
    uint num_actions = 0;
    MatchState view;

    Action actions1[] = {a_call};
    Action actions2[] = {a_raise};
    Action actions3[] = {
        a_call, a_call, a_raise, a_fold
    };
    Action actions4[] = {
        a_call, a_call, a_raise, a_call
    };
    double param;

    uint8_t card = JACK;
    for (; card <= ACE; ++card)
    {
      // Situation 1
      num_actions = 1;

      switch (card)
      {
      case JACK:
        param = patient.params[B11_INDEX];
        break;
      case QUEEN:
        param = patient.params[B21_INDEX];
        break;
      case KING:
        param = B31;
        break;
      default:
        param = patient.params[B41_INDEX];
      }
      probs = action_probs(
          patient,
          init_match_state(
              &game_def,
              B_POSITION,
              card,
              actions1,
              num_actions
          )
      );

      TEST_ASSERT_EQUAL_FLOAT(0.0, probs[a_fold]);
      TEST_ASSERT_EQUAL_FLOAT(1 - param, probs[a_call]);
      TEST_ASSERT_EQUAL_FLOAT(param, probs[a_raise]);

      // Situation 2
      num_actions = 1;

      switch (card)
      {
      case JACK:
        param = B12;
        break;
      case QUEEN:
        param = B22;
        break;
      case KING:
        param = patient.params[B32_INDEX];
        break;
      default:
        param = B42;
      }
      probs = action_probs(
          patient,
          init_match_state(
              &game_def,
              B_POSITION,
              card,
              actions2,
              num_actions
          )
      );

      TEST_ASSERT_EQUAL_FLOAT(1 - param, probs[a_fold]);
      TEST_ASSERT_EQUAL_FLOAT(param, probs[a_call]);
      TEST_ASSERT_EQUAL_FLOAT(0.0, probs[a_raise]);

      // Situation 3
      num_actions = 4;
      switch (card)
      {
      case JACK:
        param = B13;
        break;
      case QUEEN:
        param = patient.params[B23_INDEX];
        break;
      case KING:
        param = patient.params[B33_INDEX];
        break;
      default:
        param = B43;
      }
      probs = action_probs(
          patient,
          init_match_state(
              &game_def,
              B_POSITION,
              card,
              actions3,
              num_actions
          )
      );

      TEST_ASSERT_EQUAL_FLOAT(1 - param, probs[a_fold]);
      TEST_ASSERT_EQUAL_FLOAT(param, probs[a_call]);
      TEST_ASSERT_EQUAL_FLOAT(0.0, probs[a_raise]);

      // Situation 4
      num_actions = 4;

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
      probs = action_probs(
          patient,
          init_match_state(
              &game_def,
              B_POSITION,
              card,
              actions4,
              num_actions
          )
      );

      TEST_ASSERT_EQUAL_FLOAT(1 - param, probs[a_fold]);
      TEST_ASSERT_EQUAL_FLOAT(param, probs[a_call]);
      TEST_ASSERT_EQUAL_FLOAT(0.0, probs[a_raise]);
    }
  }
  Catch(e)
  {
    TEST_FAIL_MESSAGE("Should Not Have Thrown An Error");
  }
}

void test_action_probs_in_position_C()
{
  TEST_IGNORE_MESSAGE("ADJUST DELTA");
  return;

  Game game_def = init_kuhn_poker_game_def();

  CEXCEPTION_T e = 0;

  Try
  {
    // @todo Loop with a string (or strings) from each sub-family
    kuhn_3p_equilibrium_player_t patient = init_private_info(
        &game_def,
        "0 0.25 0.25 0.9375 0.0 1 12"
    );
    //
    double *probs;
    uint num_actions = 2;
    MatchState view;

    Action actions1[] = {a_call, a_call};
    Action actions2[] = {a_call, a_raise};
    Action actions3[] = {a_raise, a_fold};
    Action actions4[] = {a_raise, a_call};
    double param;

    uint8_t card = JACK;
    for (; card <= ACE; ++card)
    {
      // Situation 1
      switch (card)
      {
      case JACK:
        param = patient.params[C11_INDEX];
        break;
      case QUEEN:
        param = patient.params[C21_INDEX];
        break;
      case KING:
        param = C31;
        break;
      default:
        param = C4[0];
      }
      probs = action_probs(
          patient,
          init_match_state(
              &game_def,
              C_POSITION,
              card,
              actions1,
              num_actions
          )
      );

      TEST_ASSERT_EQUAL_FLOAT(0.0, probs[a_fold]);
      TEST_ASSERT_EQUAL_FLOAT(1 - param, probs[a_call]);
      TEST_ASSERT_EQUAL_FLOAT(param, probs[a_raise]);

      // Situation 2
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
      probs = action_probs(
          patient,
          init_match_state(
              &game_def,
              C_POSITION,
              card,
              actions2,
              num_actions
          )
      );

      TEST_ASSERT_EQUAL_FLOAT(1 - param, probs[a_fold]);
      TEST_ASSERT_EQUAL_FLOAT(param, probs[a_call]);
      TEST_ASSERT_EQUAL_FLOAT(0.0, probs[a_raise]);

      // Situation 3
      switch (card)
      {
      case JACK:
        param = C13;
        break;
      case QUEEN:
        param = C23;
        break;
      case KING:
        param = patient.params[C33_INDEX];
        break;
      default:
        param = C4[2];
      }
      probs = action_probs(
          patient,
          init_match_state(
              &game_def,
              C_POSITION,
              card,
              actions3,
              num_actions
          )
      );

      TEST_ASSERT_EQUAL_FLOAT(1 - param, probs[a_fold]);
      TEST_ASSERT_EQUAL_FLOAT(param, probs[a_call]);
      TEST_ASSERT_EQUAL_FLOAT(0.0, probs[a_raise]);

      // Situation 4
      switch (card)
      {
      case JACK:
        param = C14;
        break;
      case QUEEN:
        param = C24;
        break;
      case KING:
        param = patient.params[C34_INDEX];
        break;
      default:
        param = C4[3];
      }
      probs = action_probs(
          patient,
          init_match_state(
              &game_def,
              C_POSITION,
              card,
              actions4,
              num_actions
          )
      );

      TEST_ASSERT_EQUAL_FLOAT(1 - param, probs[a_fold]);
      TEST_ASSERT_EQUAL_FLOAT(param, probs[a_call]);
      TEST_ASSERT_EQUAL_FLOAT(0.0, probs[a_raise]);
    }
  }
  Catch(e)
  {
    TEST_FAIL_MESSAGE("Should Not Have Thrown An Error");
  }
}
