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
  params[C11_INDEX] = 0.33;
  params[B11_INDEX] = 1/6.0;
  params[B32_INDEX] = (2 + 7 * params[B11_INDEX]) / 4.0;
  double b21 = params[B11_INDEX];
  double b23 = 0.0;
  params[C33_INDEX] = 1/2.0 - params[B32_INDEX] + (7/4.0) * params[B11_INDEX];
  params[C34_INDEX] = 3/4.0; // Unconstrained

  double b33 = (1.0 + 3 * params[B11_INDEX] ) / 2.0;
  double b41 = 4 * params[B11_INDEX];
  double c21 = (1/2.0) - params[C11_INDEX];

  CEXCEPTION_T e = 0;

  Try {
    Kuhn3pEquilibriumPlayer patient = new_kuhn_3p_equilibrium_player(
        &game_def,
        params,
        12
    );

    TEST_ASSERT_EQUAL_FLOAT(params[C11_INDEX], patient.params[C11_INDEX]);
    TEST_ASSERT_EQUAL_FLOAT(params[B11_INDEX], patient.params[B11_INDEX]);
    TEST_ASSERT_EQUAL_FLOAT(b21, patient.params[B21_INDEX]);
    TEST_ASSERT_EQUAL_FLOAT(b23, patient.params[B23_INDEX]);
    TEST_ASSERT_EQUAL_FLOAT(params[B32_INDEX], patient.params[B32_INDEX]);
    TEST_ASSERT_EQUAL_FLOAT(params[C33_INDEX], patient.params[C33_INDEX]);
    TEST_ASSERT_EQUAL_FLOAT(params[C34_INDEX], patient.params[C34_INDEX]);

    TEST_ASSERT_EQUAL_FLOAT(12, patient.seed);

    TEST_ASSERT_EQUAL_FLOAT(b33, patient.params[B33_INDEX]);
    TEST_ASSERT_EQUAL_FLOAT(b41, patient.params[B41_INDEX]);
    TEST_ASSERT_EQUAL_FLOAT(c21, patient.params[C21_INDEX]);
  } Catch(e) {
    TEST_FAIL_MESSAGE("Should Not Have Thrown An Error");
  }
}

void test_b11_upper_bound()
{
  Game game_def = init_kuhn_poker_game_def();

  double params[NUM_PARAMS] = {0};
  params[C11_INDEX] = 0.33;
  params[B11_INDEX] = 1/4.0 + 0.01;
  params[B32_INDEX] = (2 + 7 * params[B11_INDEX] - 0.01) / 4.0;
  params[C33_INDEX] = 1/2.0 - params[B32_INDEX] + (7/4.0) * (
      params[B11_INDEX] - 0.01
  );
  params[C34_INDEX] = 3/4.0; // Unconstrained

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
  params[C11_INDEX] = 0.33;
  params[B11_INDEX] = 1/6.0;
  params[B32_INDEX] = ((2 + 7 * params[B11_INDEX] - 0.01) / 4.0) + 0.01;
  params[C33_INDEX] = 1/2.0 - params[B32_INDEX] + (7/4.0) * (
      params[B11_INDEX] - 0.01
  );
  params[C34_INDEX] = 3/4.0; // Unconstrained

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

void test_c11()
{
  Game game_def = init_kuhn_poker_game_def();

  double params[NUM_PARAMS] = {0};

  // When b11 <= 1/6.0
  params[C11_INDEX] = 1/2.0 - 0.01;
  params[B11_INDEX] = 1/6.0;
  params[B32_INDEX] = 0.0;
  params[C33_INDEX] = 1/2.0 - params[B32_INDEX];
  params[C34_INDEX] = 3/4.0; // Unconstrained

  CEXCEPTION_T e = 0;

  Try {
    Kuhn3pEquilibriumPlayer patient = new_kuhn_3p_equilibrium_player(
        &game_def,
        params,
        12
    );
  }
  Catch(e) {
    TEST_FAIL_MESSAGE("Should Not Have Thrown An Error");
  }

  // When b11 > 1/6.0
  params[B11_INDEX] = 1/4.0;
  params[C11_INDEX] = (2 - params[B11_INDEX]) / (3 + 4 * params[B11_INDEX]) +
      0.01;
  params[B32_INDEX] = 0.0;
  params[C33_INDEX] = 1/2.0 - params[B32_INDEX];
  params[C34_INDEX] = 3/4.0; // Unconstrained

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
