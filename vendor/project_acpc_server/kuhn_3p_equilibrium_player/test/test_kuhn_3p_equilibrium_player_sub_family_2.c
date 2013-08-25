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

void test_params_set_properly()
{
  Game game_def = init_kuhn_poker_game_def();

  CEXCEPTION_T e = 0;

  Try
  {
    kuhn_3p_equilibrium_player_t patient = init_private_info(
        &game_def,
        "0 0.25 0.25 0.9375 0.0 1 12"
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

  CEXCEPTION_T e = 0;

  Try
  {
    kuhn_3p_equilibrium_player_t patient = init_private_info(
        &game_def,
        "0 0.25 0.26 0.9375 0.0 12"
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

  CEXCEPTION_T e = 0;

  Try
  {
    kuhn_3p_equilibrium_player_t patient = init_private_info(
        &game_def,
        "0 0.25 0.24 0.9375 0.0 12"
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

  CEXCEPTION_T e = 0;

  Try
  {
    kuhn_3p_equilibrium_player_t patient = init_private_info(
        &game_def,
        "0 0.25 0.25 0.9376 0.0 12"
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

  CEXCEPTION_T e = 0;

  Try
  {
    kuhn_3p_equilibrium_player_t patient = init_private_info(
        &game_def,
        "0 0.25 0.25 0.4 0.098 12"
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

  CEXCEPTION_T e = 0;

  Try
  {
    kuhn_3p_equilibrium_player_t patient = init_private_info(
        &game_def,
        "0 0.25 0.25 0.4 0.54 12"
    );
    TEST_FAIL_MESSAGE("Should Have Thrown An Error");
  }
  Catch(e)
  {
    TEST_ASSERT_EQUAL(1, e);
  }
}
