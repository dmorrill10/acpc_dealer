/*
Copyright (C) 2013 by the Computer Poker Research Group, University of Alberta
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "test_helper.h"

#include "kuhn_3p_equilibrium_player.h"

static int stderr_copy;
static int stderr_copy;
static FILE* dev_null = NULL;
void setUp()
{
  // Silence error output
  stderr_copy = dup(fileno(stderr));
  dev_null = freopen("/dev/null", "w", stderr);
}

void tearDown()
{
  if (dev_null) {
    // Restore error output
    fflush(stderr);
    dup2(stderr_copy, fileno(stderr));
    close(stderr_copy);
  }
}

void test_params_set_properly()
{
  Game game_def = init_kuhn_poker_game_def();

  double params[NUM_PARAMS] = {0};
  params[C11_INDEX] = 0.0;
  params[B11_INDEX] = 0.25;
  params[B21_INDEX] = 0.25;
  params[B32_INDEX] = 0.9375;
  params[C33_INDEX] = 0.0;
  params[C34_INDEX] = 1.0;

  CEXCEPTION_T e = 0;

  Try
  {
    Kuhn3pEquilibriumPlayer patient = new_kuhn_3p_equilibrium_player(
        &game_def,
        params,
        12
    );

    TEST_ASSERT_EQUAL_FLOAT(0.0, patient.params[C11_INDEX]);
    TEST_ASSERT_EQUAL_FLOAT(0.25, patient.params[B11_INDEX]);
    TEST_ASSERT_EQUAL_FLOAT(0.25, patient.params[B21_INDEX]);
    TEST_ASSERT_EQUAL_FLOAT(0.9375, patient.params[B32_INDEX]);
    TEST_ASSERT_EQUAL_FLOAT(0.0, patient.params[C33_INDEX]);
    TEST_ASSERT_EQUAL_FLOAT(1.0, patient.params[C34_INDEX]);
    TEST_ASSERT_EQUAL_FLOAT(12, patient.seed);

    TEST_ASSERT_EQUAL_FLOAT(0.0, patient.params[B23_INDEX]);
    TEST_ASSERT_EQUAL_FLOAT(
        (1 + 0.25 + 2 * 0.25) / 2.0,
        patient.params[B33_INDEX]
    );
    TEST_ASSERT_EQUAL_FLOAT(2 * 0.25 + 2 * 0.25, patient.params[B41_INDEX]);
    TEST_ASSERT_EQUAL_FLOAT(1/2.0, patient.params[C21_INDEX]);
  }
  Catch(e)
  {
    TEST_FAIL_MESSAGE("Should Not Have Thrown An Error");
  }
}

void test_b21_upper_bound()
{
  Game game_def = init_kuhn_poker_game_def();

  double params[NUM_PARAMS] = {0};
  params[C11_INDEX] = 0.0;
  params[B11_INDEX] = 0.25;
  params[B21_INDEX] = 0.26;
  params[B32_INDEX] = 0.9375;
  params[C33_INDEX] = 0.0;
  params[C34_INDEX] = 1.0;

  CEXCEPTION_T e = 0;

  Try
  {
    Kuhn3pEquilibriumPlayer patient = new_kuhn_3p_equilibrium_player(
        &game_def,
        params,
        12
    );
    TEST_FAIL_MESSAGE("Should Have Thrown An Error");
  }
  Catch(e)
  {
    TEST_ASSERT_EQUAL(1, e);
  }
}

void test_b11_upper_bound()
{
  Game game_def = init_kuhn_poker_game_def();

  double params[NUM_PARAMS] = {0};
  params[C11_INDEX] = 0.0;
  params[B11_INDEX] = 0.25;
  params[B21_INDEX] = 0.24;
  params[B32_INDEX] = 0.9375;
  params[C33_INDEX] = 0.0;
  params[C34_INDEX] = 1.0;

  CEXCEPTION_T e = 0;

  Try
  {
    Kuhn3pEquilibriumPlayer patient = new_kuhn_3p_equilibrium_player(
        &game_def,
        params,
        12
    );
    TEST_FAIL_MESSAGE("Should Have Thrown An Error");
      }
  Catch(e)
  {
    TEST_ASSERT_EQUAL(1, e);
  }
}

void test_b32_upper_bound()
{
  Game game_def = init_kuhn_poker_game_def();

  double params[NUM_PARAMS] = {0};
  params[C11_INDEX] = 0.0;
  params[B11_INDEX] = 0.25;
  params[B21_INDEX] = 0.25;
  params[B32_INDEX] = 0.9376;
  params[C33_INDEX] = 0.0;
  params[C34_INDEX] = 1.0;

  CEXCEPTION_T e = 0;

  Try
  {
    Kuhn3pEquilibriumPlayer patient = new_kuhn_3p_equilibrium_player(
        &game_def,
        params,
        12
    );
    TEST_FAIL_MESSAGE("Should Have Thrown An Error");
  }
  Catch(e)
  {
    TEST_ASSERT_EQUAL(1, e);
  }
}

void test_c33_lower_bound()
{
  Game game_def = init_kuhn_poker_game_def();

  double params[NUM_PARAMS] = {0};
  params[C11_INDEX] = 0.0;
  params[B11_INDEX] = 0.25;
  params[B21_INDEX] = 0.25;
  params[B32_INDEX] = 0.4;
  params[C33_INDEX] = 0.098;
  params[C34_INDEX] = 1.0;

  CEXCEPTION_T e = 0;

  Try
  {
    Kuhn3pEquilibriumPlayer patient = new_kuhn_3p_equilibrium_player(
        &game_def,
        params,
        12
    );
    TEST_FAIL_MESSAGE("Should Have Thrown An Error");
  }
  Catch(e)
  {
    TEST_ASSERT_EQUAL(1, e);
  }
}

void test_c33_upper_bound()
{
  Game game_def = init_kuhn_poker_game_def();

  double params[NUM_PARAMS] = {0};
  params[C11_INDEX] = 0.0;
  params[B11_INDEX] = 0.25;
  params[B21_INDEX] = 0.25;
  params[B32_INDEX] = 0.4;
  params[C33_INDEX] = 0.54;
  params[C34_INDEX] = 0.0;

  CEXCEPTION_T e = 0;

  Try
  {
    Kuhn3pEquilibriumPlayer patient = new_kuhn_3p_equilibrium_player(
        &game_def,
        params,
        12
    );
    TEST_FAIL_MESSAGE("Should Have Thrown An Error");
  }
  Catch(e)
  {
    TEST_ASSERT_EQUAL(1, e);
  }
}

void test_a24()
{
  Game game_def = init_kuhn_poker_game_def();

  double params[NUM_PARAMS] = {0};
  params[C11_INDEX] = 0.0;
  params[B11_INDEX] = 0.25;
  params[B21_INDEX] = 0.25;
  params[B32_INDEX] = 0.9375;
  params[C33_INDEX] = 0.0;
  params[C34_INDEX] = 1.0;

  CEXCEPTION_T e = 0;

  Try
  {
    Kuhn3pEquilibriumPlayer patient = new_kuhn_3p_equilibrium_player(
        &game_def,
        params,
        12
    );

    size_t num_actions = 3;

    double probs[NUM_ACTION_TYPES];
    memset(probs, -1, NUM_ACTION_TYPES * sizeof(*probs));

    enum ActionType actions[] = {a_call, a_raise, a_call};

    uint8_t card = QUEEN;

    action_probs(
        &patient,
        init_match_state(
            &game_def,
            A_POSITION,
            card,
            actions,
            num_actions
        ),
        probs
    );
    TEST_ASSERT_EQUAL_FLOAT(1.0, probs[a_fold]);
    TEST_ASSERT_EQUAL_FLOAT(0.0, probs[a_call]);
    TEST_ASSERT_EQUAL_FLOAT(0.0, probs[a_raise]);
  }
  Catch(e)
  {
    TEST_FAIL_MESSAGE("Should Not Have Thrown An Error");
  }
}

void test_a41()
{
  Game game_def = init_kuhn_poker_game_def();

  double params[NUM_PARAMS] = {0};
  params[C11_INDEX] = 0.0;
  params[B11_INDEX] = 0.25;
  params[B21_INDEX] = 0.25;
  params[B32_INDEX] = 0.9375;
  params[C33_INDEX] = 0.0;
  params[C34_INDEX] = 1.0;

  CEXCEPTION_T e = 0;

  Try
  {
    Kuhn3pEquilibriumPlayer patient = new_kuhn_3p_equilibrium_player(
        &game_def,
        params,
        12
    );

    double probs[NUM_ACTION_TYPES];
    memset(probs, -1, NUM_ACTION_TYPES * sizeof(*probs));

    size_t num_actions = 0;
    enum ActionType actions[] = {};

    uint8_t card = ACE;

    action_probs(
        &patient,
        init_match_state(
            &game_def,
            A_POSITION,
            card,
            actions,
            num_actions
        ),
        probs
    );

    TEST_ASSERT_EQUAL_FLOAT(0.0, probs[a_fold]);
    TEST_ASSERT_EQUAL_FLOAT(1.0, probs[a_call]);
    TEST_ASSERT_EQUAL_FLOAT(0.0, probs[a_raise]);
  }
  Catch(e)
  {
    TEST_FAIL_MESSAGE("Should Not Have Thrown An Error");
  }
}
