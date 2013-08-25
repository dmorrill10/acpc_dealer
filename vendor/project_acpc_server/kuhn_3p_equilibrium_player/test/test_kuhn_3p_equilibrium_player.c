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

  CEXCEPTION_T e;

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

  CEXCEPTION_T e;

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

  CEXCEPTION_T e;

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

  CEXCEPTION_T e;

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

  CEXCEPTION_T e;

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

void test_action_probs_for_family_2()
{
  Game game_def = init_kuhn_poker_game_def();
  TEST_IGNORE_MESSAGE("IMPLEMENT");
}

void test_action_probs_for_family_3()
{
  Game game_def = init_kuhn_poker_game_def();
  TEST_IGNORE_MESSAGE("IMPLEMENT");
}
