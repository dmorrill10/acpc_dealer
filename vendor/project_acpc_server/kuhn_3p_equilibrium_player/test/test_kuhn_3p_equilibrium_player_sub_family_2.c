/*
Copyright (C) 2013 by the Computer Poker Research Group, University of Alberta
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "test_helper.h"

#include "kuhn_3p_equilibrium_player.h"

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
  params[C11_INDEX] = 0.5;
  params[B11_INDEX] = 0.25;
  params[B21_INDEX] = 1/2.0 - 2 * params[B11_INDEX];
  params[B23_INDEX] = (params[B11_INDEX] - params[B21_INDEX]) /
                      (2 * (1 - params[B21_INDEX]));
  params[B32_INDEX] = (2 + 4*params[B11_INDEX] + 3 * params[B21_INDEX]) / 4.0;
  params[C33_INDEX] = 1/2.0 - params[B32_INDEX] +
      (4 * params[B11_INDEX] + 3 * params[B21_INDEX]) / 4.0;
  params[C34_INDEX] = 3/4.0; // Unconstrained

  double b33 = (1.0 + params[B11_INDEX] + 2 * params[B21_INDEX]) / 2.0;
  double b41 = 2 * (params[B11_INDEX] + params[B21_INDEX]);

  CEXCEPTION_T e = 0;

  Try
  {
    Kuhn3pEquilibriumPlayer patient = new_kuhn_3p_equilibrium_player(
        &game_def,
        params,
        12
    );

    TEST_ASSERT_EQUAL_FLOAT(params[C11_INDEX], patient.params[C11_INDEX]);
    TEST_ASSERT_EQUAL_FLOAT(params[B11_INDEX], patient.params[B11_INDEX]);
    TEST_ASSERT_EQUAL_FLOAT(params[B21_INDEX], patient.params[B21_INDEX]);
    TEST_ASSERT_EQUAL_FLOAT(params[B23_INDEX], patient.params[B23_INDEX]);
    TEST_ASSERT_EQUAL_FLOAT(params[B32_INDEX], patient.params[B32_INDEX]);
    TEST_ASSERT_EQUAL_FLOAT(params[C33_INDEX], patient.params[C33_INDEX]);
    TEST_ASSERT_EQUAL_FLOAT(params[C34_INDEX], patient.params[C34_INDEX]);

    TEST_ASSERT_EQUAL_FLOAT(12, patient.seed);

    TEST_ASSERT_EQUAL_FLOAT(b33, patient.params[B33_INDEX]);
    TEST_ASSERT_EQUAL_FLOAT(b41, patient.params[B41_INDEX]);
    TEST_ASSERT_EQUAL_FLOAT(0.0, patient.params[C21_INDEX]);
  }
  Catch(e)
  {
    TEST_FAIL_MESSAGE("Should Not Have Thrown An Error");
  }
}

void test_b21()
{
  Game game_def = init_kuhn_poker_game_def();

  double params[NUM_PARAMS] = {0};
  params[C11_INDEX] = 0.5;
  params[B11_INDEX] = 1/4.0;
  params[B21_INDEX] = 1/2.0 - 2 * params[B11_INDEX] + 0.01;
  params[B23_INDEX] = (params[B11_INDEX] - params[B21_INDEX]) /
                      (2 * (1 - params[B21_INDEX]));
  params[B32_INDEX] = (2 + 4*params[B11_INDEX] + 3 * params[B21_INDEX]) / 4.0;
  params[C33_INDEX] = 1/2.0 - params[B32_INDEX] +
      (4 * params[B11_INDEX] + 3 * params[B21_INDEX]) / 4.0;

  CEXCEPTION_T e = 0;

  Try {
    Kuhn3pEquilibriumPlayer patient = new_kuhn_3p_equilibrium_player(
        &game_def,
        params,
        12
    );
    TEST_FAIL_MESSAGE("Should Have Thrown An Error");
  }
  Catch(e) {
    TEST_ASSERT_EQUAL(1, e);
  }

  params[B11_INDEX] = 1/6.0;
  params[B21_INDEX] = params[B11_INDEX] + 0.01;
  params[B23_INDEX] = (params[B11_INDEX] - params[B21_INDEX]) /
                      (2 * (1 - params[B21_INDEX]));
  params[B32_INDEX] = (2 + 4*params[B11_INDEX] + 3 * params[B21_INDEX]) / 4.0;
  params[C33_INDEX] = 1/2.0 - params[B32_INDEX] +
      (4 * params[B11_INDEX] + 3 * params[B21_INDEX]) / 4.0;

  Try {
    Kuhn3pEquilibriumPlayer patient = new_kuhn_3p_equilibrium_player(
        &game_def,
        params,
        12
    );
    TEST_FAIL_MESSAGE("Should Have Thrown An Error");
  }
  Catch(e) {
    TEST_ASSERT_EQUAL(1, e);
  }
}

void test_b11_upper_bound()
{
  Game game_def = init_kuhn_poker_game_def();

  double params[NUM_PARAMS] = {0};
  params[C11_INDEX] = 0.5;
  params[B11_INDEX] = 0.26;
  params[B21_INDEX] = 1/2.0 - 2 * params[B11_INDEX];
  params[B23_INDEX] = (params[B11_INDEX] - params[B21_INDEX]) /
                      (2 * (1 - params[B21_INDEX]));
  params[B32_INDEX] = (2 + 4*params[B11_INDEX] + 3 * params[B21_INDEX]) / 4.0;
  params[C33_INDEX] = 1/2.0 - params[B32_INDEX] +
      (4 * params[B11_INDEX] + 3 * params[B21_INDEX]) / 4.0;

  CEXCEPTION_T e = 0;

  Try {
    Kuhn3pEquilibriumPlayer patient = new_kuhn_3p_equilibrium_player(
        &game_def,
        params,
        12
    );
    TEST_FAIL_MESSAGE("Should Have Thrown An Error");
  }
  Catch(e) {
    TEST_ASSERT_EQUAL(1, e);
  }
}

void test_b23_upper_bound()
{
  Game game_def = init_kuhn_poker_game_def();

  double params[NUM_PARAMS] = {0};
  params[C11_INDEX] = 0.5;
  params[B11_INDEX] = 0.25;
  params[B21_INDEX] = 1/2.0 - 2 * params[B11_INDEX];
  params[B23_INDEX] = (params[B11_INDEX] - params[B21_INDEX]) /
                      (2 * (1 - params[B21_INDEX])) + 0.01;
  params[B32_INDEX] = (2 + 4*params[B11_INDEX] + 3 * params[B21_INDEX]) / 4.0;
  params[C33_INDEX] = 1/2.0 - params[B32_INDEX] +
      (4 * params[B11_INDEX] + 3 * params[B21_INDEX]) / 4.0;

  CEXCEPTION_T e = 0;

  Try {
    Kuhn3pEquilibriumPlayer patient = new_kuhn_3p_equilibrium_player(
        &game_def,
        params,
        12
    );
    TEST_FAIL_MESSAGE("Should Have Thrown An Error");
  }
  Catch(e) {
    TEST_ASSERT_EQUAL(1, e);
  }
}

void test_b32_upper_bound()
{
  Game game_def = init_kuhn_poker_game_def();

  double params[NUM_PARAMS] = {0};
  params[C11_INDEX] = 0.5;
  params[B11_INDEX] = 0.25;
  params[B21_INDEX] = 1/2.0 - 2 * params[B11_INDEX];
  params[B23_INDEX] = (params[B11_INDEX] - params[B21_INDEX]) /
                      (2 * (1 - params[B21_INDEX]));
  params[B32_INDEX] = (2 + 4*params[B11_INDEX] + 3 * params[B21_INDEX]) / 4.0 +
      0.01;
  params[C33_INDEX] = 1/2.0 - params[B32_INDEX] +
      (4 * params[B11_INDEX] + 3 * params[B21_INDEX]) / 4.0;

  CEXCEPTION_T e = 0;

  Try {
    Kuhn3pEquilibriumPlayer patient = new_kuhn_3p_equilibrium_player(
        &game_def,
        params,
        12
    );
    TEST_FAIL_MESSAGE("Should Have Thrown An Error");
  }
  Catch(e) {
    TEST_ASSERT_EQUAL(1, e);
  }
}

void test_c33()
{
  Game game_def = init_kuhn_poker_game_def();

  double params[NUM_PARAMS] = {0};
  params[C11_INDEX] = 0.5;
  params[B11_INDEX] = 0.25;
  params[B21_INDEX] = 1/2.0 - 2 * params[B11_INDEX];
  params[B23_INDEX] = (params[B11_INDEX] - params[B21_INDEX]) /
                      (2 * (1 - params[B21_INDEX]));
  params[B32_INDEX] = (2 + 4*params[B11_INDEX] + 3 * params[B21_INDEX]) / 4.0;

  // Check upper bound
  params[C33_INDEX] = 1/2.0 - params[B32_INDEX] +
      (4 * params[B11_INDEX] + 3 * params[B21_INDEX]) / 4.0 + 0.01;

  CEXCEPTION_T e = 0;

  Try {
    Kuhn3pEquilibriumPlayer patient = new_kuhn_3p_equilibrium_player(
        &game_def,
        params,
        12
    );
    TEST_FAIL_MESSAGE("Should Have Thrown An Error");
  }
  Catch(e) {
    TEST_ASSERT_EQUAL(1, e);
  }

  // Check lower bound
  params[C33_INDEX] = 1/2.0 - params[B32_INDEX] +
      (4 * params[B11_INDEX] + 3 * params[B21_INDEX]) / 4.0 - 0.01;

  Try {
    Kuhn3pEquilibriumPlayer patient = new_kuhn_3p_equilibrium_player(
        &game_def,
        params,
        12
    );
    TEST_FAIL_MESSAGE("Should Have Thrown An Error");
  }
  Catch(e) {
    TEST_ASSERT_EQUAL(1, e);
  }
}
