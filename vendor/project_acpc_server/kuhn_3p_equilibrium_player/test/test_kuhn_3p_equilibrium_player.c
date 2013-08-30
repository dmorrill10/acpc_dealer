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

void test_init_private_info_throws_error_when_game_def_not_kuhn()
{
  Game game_def;

  CEXCEPTION_T e = 0;

  Try
  {
    double params[NUM_PARAMS] = {0.1};
    new_kuhn_3p_equilibrium_player(&game_def, params, 12);
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

  double params[NUM_PARAMS] = {0};
  params[C11_INDEX] = 0.0;
  params[B11_INDEX] = 0.25;
  params[B21_INDEX] = 0.25;
  params[B32_INDEX] = 0.9375;
  params[C33_INDEX] = 0.0;
  params[C34_INDEX] = 1.0;

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
}

void test_out_of_bounds_c11_throws_error()
{
  Game game_def = init_kuhn_poker_game_def();

  CEXCEPTION_T e = 0;

  Try
  {
    double params[NUM_PARAMS] = {0};
    params[C11_INDEX] = 0.51;
    params[B11_INDEX] = 0.25;
    params[B21_INDEX] = 0.25;
    params[B32_INDEX] = 0.9375;
    params[C33_INDEX] = 0.0;
    params[C34_INDEX] = 1.0;

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

void test_param_invalid_probability_throws_error()
{
  Game game_def = init_kuhn_poker_game_def();

  CEXCEPTION_T e = 0;

  double params[NUM_PARAMS] = {0};
  params[C11_INDEX] = 0.0;
  params[B11_INDEX] = 0.25;
  params[B21_INDEX] = 0.25;
  params[B32_INDEX] = 0.9375;
  params[C33_INDEX] = 0.0;
  params[C34_INDEX] = 1.0;

  for (float invalid_param = -0.1; invalid_param < 2.0; invalid_param += 2.0)
  {
    Try
    {
      params[C34_INDEX] = invalid_param;

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
}

void test_action_probs_in_position_A()
{
  Game game_def = init_kuhn_poker_game_def();

#define NUM_PARAM_LISTS 3

  double* params_list[NUM_PARAM_LISTS] = {0};

  double params1[NUM_PARAMS] = {0};
  params1[C11_INDEX] = 0.0;
  params1[B11_INDEX] = 0.25;
  params1[B21_INDEX] = 0.25;
  params1[B32_INDEX] = 0.9375;
  params1[C33_INDEX] = 0.0;
  params1[C34_INDEX] = 1.0;

  double params2[NUM_PARAMS] = {0};
  params2[C11_INDEX] = 0.5;
  params2[B11_INDEX] = 0.25;
  params2[B21_INDEX] = 1/2.0 - 2 * params2[B11_INDEX];
  params2[B23_INDEX] = (params2[B11_INDEX] - params2[B21_INDEX]) /
                      (2 * (1 - params2[B21_INDEX]));
  params2[B32_INDEX] = (2 + 4*params2[B11_INDEX] + 3 * params2[B21_INDEX]) / 4.0;
  params2[C33_INDEX] = 1/2.0 - params2[B32_INDEX] +
      (4 * params2[B11_INDEX] + 3 * params2[B21_INDEX]) / 4.0;
  params2[C34_INDEX] = 3/4.0; // Unconstrained

  double params3[NUM_PARAMS] = {0};
  params3[C11_INDEX] = 0.33;
  params3[B11_INDEX] = 1/6.0;
  params3[B32_INDEX] = (2 + 7 * params3[B11_INDEX]) / 4.0;
  params3[C33_INDEX] = 1/2.0 - params3[B32_INDEX] + (7/4.0) * params3[B11_INDEX];
  params3[C34_INDEX] = 3/4.0; // Unconstrained

  params_list[0] = params1;
  params_list[1] = params2;
  params_list[2] = params3;

  CEXCEPTION_T e = 0;

  Try
  {
    for (size_t i = 0; i < NUM_PARAM_LISTS; ++i) {
      Kuhn3pEquilibriumPlayer patient = new_kuhn_3p_equilibrium_player(
          &game_def,
          params_list[i],
          12
      );

      size_t num_actions = 0;

      double probs[NUM_ACTION_TYPES];
      memset(probs, -1, NUM_ACTION_TYPES * sizeof(*probs));

      enum ActionType actions1[] = {};
      enum ActionType actions2[] = {a_call, a_call, a_raise};
      enum ActionType actions3[] = {a_call, a_raise, a_fold};
      enum ActionType actions4[] = {a_call, a_raise, a_call};

      for (CardRank card_rank = JACK_RANK; card_rank <= ACE_RANK; ++card_rank)
      {
        // Situation 1
        num_actions = 0;
        memset(probs, -1, NUM_ACTION_TYPES * sizeof(*probs));

        action_probs(
            &patient,
            init_match_state(
                &game_def,
                A_POSITION,
                makeCard(card_rank, KUHN_SUIT),
                actions1,
                num_actions
            ),
            probs
        );

        TEST_ASSERT_EQUAL_FLOAT(0.0, probs[a_fold]);
        TEST_ASSERT_EQUAL_FLOAT(1 - A[card_rank - JACK_RANK][0], probs[a_call]);
        TEST_ASSERT_EQUAL_FLOAT(A[card_rank - JACK_RANK][0], probs[a_raise]);

        // Situation 2
        num_actions = 3;
        memset(probs, -1, NUM_ACTION_TYPES * sizeof(*probs));

        action_probs(
            &patient,
            init_match_state(
                &game_def,
                A_POSITION,
                makeCard(card_rank, KUHN_SUIT),
                actions2,
                num_actions
            ),
            probs
        );
        TEST_ASSERT_EQUAL_FLOAT(
            1 - A[card_rank - JACK_RANK][1], probs[a_fold]
        );
        TEST_ASSERT_EQUAL_FLOAT(A[card_rank - JACK_RANK][1], probs[a_call]);
        TEST_ASSERT_EQUAL_FLOAT(0.0, probs[a_raise]);

        // Situation 3
        num_actions = 3;
        memset(probs, -1, NUM_ACTION_TYPES * sizeof(*probs));

        action_probs(
            &patient,
            init_match_state(
                &game_def,
                A_POSITION,
                makeCard(card_rank, KUHN_SUIT),
                actions3,
                num_actions
            ),
            probs
        );
        TEST_ASSERT_EQUAL_FLOAT(1 - A[card_rank - JACK_RANK][2], probs[a_fold]);
        TEST_ASSERT_EQUAL_FLOAT(A[card_rank - JACK_RANK][2], probs[a_call]);
        TEST_ASSERT_EQUAL_FLOAT(0.0, probs[a_raise]);

        // Situation 4
        num_actions = 3;
        memset(probs, -1, NUM_ACTION_TYPES * sizeof(*probs));

        action_probs(
            &patient,
            init_match_state(
                &game_def,
                A_POSITION,
                makeCard(card_rank, KUHN_SUIT),
                actions4,
                num_actions
            ),
            probs
        );
        TEST_ASSERT_EQUAL_FLOAT(
            1 - A[card_rank - JACK_RANK][3], probs[a_fold]
        );
        TEST_ASSERT_EQUAL_FLOAT(A[card_rank - JACK_RANK][3], probs[a_call]);
        TEST_ASSERT_EQUAL_FLOAT(0.0, probs[a_raise]);
      }
    }
  }
  Catch(e)
  {
    TEST_FAIL_MESSAGE("Should Not Have Thrown An Error");
  }
}

void test_action_probs_in_position_B()
{
  Game game_def = init_kuhn_poker_game_def();

#define NUM_PARAM_LISTS 3

  double* params_list[NUM_PARAM_LISTS] = {0};

  double params1[NUM_PARAMS] = {0};
  params1[C11_INDEX] = 0.0;
  params1[B11_INDEX] = 0.25;
  params1[B21_INDEX] = 0.25;
  params1[B32_INDEX] = 0.9375;
  params1[C33_INDEX] = 0.0;
  params1[C34_INDEX] = 1.0;

  double params2[NUM_PARAMS] = {0};
  params2[C11_INDEX] = 0.5;
  params2[B11_INDEX] = 0.25;
  params2[B21_INDEX] = 1/2.0 - 2 * params2[B11_INDEX];
  params2[B23_INDEX] = (params2[B11_INDEX] - params2[B21_INDEX]) /
                      (2 * (1 - params2[B21_INDEX]));
  params2[B32_INDEX] = (2 + 4*params2[B11_INDEX] + 3 * params2[B21_INDEX]) / 4.0;
  params2[C33_INDEX] = 1/2.0 - params2[B32_INDEX] +
      (4 * params2[B11_INDEX] + 3 * params2[B21_INDEX]) / 4.0;
  params2[C34_INDEX] = 3/4.0; // Unconstrained

  double params3[NUM_PARAMS] = {0};
  params3[C11_INDEX] = 0.33;
  params3[B11_INDEX] = 1/6.0;
  params3[B32_INDEX] = (2 + 7 * params3[B11_INDEX]) / 4.0;
  params3[C33_INDEX] = 1/2.0 - params3[B32_INDEX] + (7/4.0) * params3[B11_INDEX];
  params3[C34_INDEX] = 3/4.0; // Unconstrained

  params_list[0] = params1;
  params_list[1] = params2;
  params_list[2] = params3;

  CEXCEPTION_T e = 0;

  Try
  {
    for (size_t i = 0; i < NUM_PARAM_LISTS; ++i) {
      Kuhn3pEquilibriumPlayer patient = new_kuhn_3p_equilibrium_player(
          &game_def,
          params_list[i],
          12
      );

      double probs[NUM_ACTION_TYPES];
      memset(probs, -1, NUM_ACTION_TYPES * sizeof(*probs));
      size_t num_actions = 0;
      MatchState view;

      enum ActionType actions1[] = {a_call};
      enum ActionType actions2[] = {a_raise};
      enum ActionType actions3[] = {
          a_call, a_call, a_raise, a_fold
      };
      enum ActionType actions4[] = {
          a_call, a_call, a_raise, a_call
      };
      double param;

      for (CardRank card_rank = JACK_RANK; card_rank <= ACE_RANK; ++card_rank)
      {
        // Situation 1
        num_actions = 1;
        memset(probs, -1, NUM_ACTION_TYPES * sizeof(*probs));

        switch (card_rank)
        {
        case JACK_RANK:
          param = patient.params[B11_INDEX];
          break;
        case QUEEN_RANK:
          param = patient.params[B21_INDEX];
          break;
        case KING_RANK:
          param = B31;
          break;
        default:
          param = patient.params[B41_INDEX];
        }
        action_probs(
            &patient,
            init_match_state(
                &game_def,
                B_POSITION,
                makeCard(card_rank, KUHN_SUIT),
                actions1,
                num_actions
            ),
            probs
        );

        TEST_ASSERT_EQUAL_FLOAT(0.0, probs[a_fold]);
        TEST_ASSERT_EQUAL_FLOAT(1 - param, probs[a_call]);
        TEST_ASSERT_EQUAL_FLOAT(param, probs[a_raise]);

        // Situation 2
        num_actions = 1;
        memset(probs, -1, NUM_ACTION_TYPES * sizeof(*probs));

        switch (card_rank)
        {
        case JACK_RANK:
          param = B12;
          break;
        case QUEEN_RANK:
          param = B22;
          break;
        case KING_RANK:
          param = patient.params[B32_INDEX];
          break;
        default:
          param = B42;
        }
        action_probs(
            &patient,
            init_match_state(
                &game_def,
                B_POSITION,
                makeCard(card_rank, KUHN_SUIT),
                actions2,
                num_actions
            ),
            probs
        );

        TEST_ASSERT_EQUAL_FLOAT(1 - param, probs[a_fold]);
        TEST_ASSERT_EQUAL_FLOAT(param, probs[a_call]);
        TEST_ASSERT_EQUAL_FLOAT(0.0, probs[a_raise]);

        // Situation 3
        num_actions = 4;
        memset(probs, -1, NUM_ACTION_TYPES * sizeof(*probs));

        switch (card_rank)
        {
        case JACK_RANK:
          param = B13;
          break;
        case QUEEN_RANK:
          param = patient.params[B23_INDEX];
          break;
        case KING_RANK:
          param = patient.params[B33_INDEX];
          break;
        default:
          param = B43;
        }
        action_probs(
            &patient,
            init_match_state(
                &game_def,
                B_POSITION,
                makeCard(card_rank, KUHN_SUIT),
                actions3,
                num_actions
            ),
            probs
        );

        TEST_ASSERT_EQUAL_FLOAT(1 - param, probs[a_fold]);
        TEST_ASSERT_EQUAL_FLOAT(param, probs[a_call]);
        TEST_ASSERT_EQUAL_FLOAT(0.0, probs[a_raise]);

        // Situation 4
        num_actions = 4;
        memset(probs, -1, NUM_ACTION_TYPES * sizeof(*probs));

        switch (card_rank)
        {
        case JACK_RANK:
          param = B14;
          break;
        case QUEEN_RANK:
          param = B24;
          break;
        case KING_RANK:
          param = B34;
          break;
        default:
          param = B44;
        }
        action_probs(
            &patient,
            init_match_state(
                &game_def,
                B_POSITION,
                makeCard(card_rank, KUHN_SUIT),
                actions4,
                num_actions
            ),
            probs
        );

        TEST_ASSERT_EQUAL_FLOAT(1 - param, probs[a_fold]);
        TEST_ASSERT_EQUAL_FLOAT(param, probs[a_call]);
        TEST_ASSERT_EQUAL_FLOAT(0.0, probs[a_raise]);
      }
    }
  }
  Catch(e)
  {
    TEST_FAIL_MESSAGE("Should Not Have Thrown An Error");
  }
}

void test_action_probs_in_position_C()
{
  Game game_def = init_kuhn_poker_game_def();

#define NUM_PARAM_LISTS 3

  double* params_list[NUM_PARAM_LISTS] = {0};

  double params1[NUM_PARAMS] = {0};
  params1[C11_INDEX] = 0.0;
  params1[B11_INDEX] = 0.25;
  params1[B21_INDEX] = 0.25;
  params1[B32_INDEX] = 0.9375;
  params1[C33_INDEX] = 0.0;
  params1[C34_INDEX] = 1.0;

  double params2[NUM_PARAMS] = {0};
  params2[C11_INDEX] = 0.5;
  params2[B11_INDEX] = 0.25;
  params2[B21_INDEX] = 1/2.0 - 2 * params2[B11_INDEX];
  params2[B23_INDEX] = (params2[B11_INDEX] - params2[B21_INDEX]) /
                      (2 * (1 - params2[B21_INDEX]));
  params2[B32_INDEX] = (2 + 4*params2[B11_INDEX] + 3 * params2[B21_INDEX]) / 4.0;
  params2[C33_INDEX] = 1/2.0 - params2[B32_INDEX] +
      (4 * params2[B11_INDEX] + 3 * params2[B21_INDEX]) / 4.0;
  params2[C34_INDEX] = 3/4.0; // Unconstrained

  double params3[NUM_PARAMS] = {0};
  params3[C11_INDEX] = 0.33;
  params3[B11_INDEX] = 1/6.0;
  params3[B32_INDEX] = (2 + 7 * params3[B11_INDEX]) / 4.0;
  params3[C33_INDEX] = 1/2.0 - params3[B32_INDEX] + (7/4.0) * params3[B11_INDEX];
  params3[C34_INDEX] = 3/4.0; // Unconstrained

  params_list[0] = params1;
  params_list[1] = params2;
  params_list[2] = params3;

  CEXCEPTION_T e = 0;

  Try
  {
    for (size_t i = 0; i < NUM_PARAM_LISTS; ++i) {
      Kuhn3pEquilibriumPlayer patient = new_kuhn_3p_equilibrium_player(
          &game_def,
          params_list[i],
          12
      );
      double probs[NUM_ACTION_TYPES];
      memset(probs, -1, NUM_ACTION_TYPES * sizeof(*probs));
      size_t num_actions = 2;
      MatchState view;

      enum ActionType actions1[] = {a_call, a_call};
      enum ActionType actions2[] = {a_call, a_raise};
      enum ActionType actions3[] = {a_raise, a_fold};
      enum ActionType actions4[] = {a_raise, a_call};
      double param;

      for (CardRank card_rank = JACK_RANK; card_rank <= ACE_RANK; ++card_rank)
      {
        // Situation 1
        memset(probs, -1, NUM_ACTION_TYPES * sizeof(*probs));

        switch (card_rank)
        {
        case JACK_RANK:
          param = patient.params[C11_INDEX];
          break;
        case QUEEN_RANK:
          param = patient.params[C21_INDEX];
          break;
        case KING_RANK:
          param = C31;
          break;
        default:
          param = C4[0];
        }
        action_probs(
            &patient,
            init_match_state(
                &game_def,
                C_POSITION,
                makeCard(card_rank, KUHN_SUIT),
                actions1,
                num_actions
            ),
            probs
        );

        TEST_ASSERT_EQUAL_FLOAT(0.0, probs[a_fold]);
        TEST_ASSERT_EQUAL_FLOAT(1 - param, probs[a_call]);
        TEST_ASSERT_EQUAL_FLOAT(param, probs[a_raise]);

        // Situation 2
        memset(probs, -1, NUM_ACTION_TYPES * sizeof(*probs));

        switch (card_rank)
        {
        case JACK_RANK:
          param = C12;
          break;
        case QUEEN_RANK:
          param = C22;
          break;
        case KING_RANK:
          param = C32;
          break;
        default:
          param = C4[1];
        }
        action_probs(
            &patient,
            init_match_state(
                &game_def,
                C_POSITION,
                makeCard(card_rank, KUHN_SUIT),
                actions2,
                num_actions
            ),
            probs
        );

        TEST_ASSERT_EQUAL_FLOAT(1 - param, probs[a_fold]);
        TEST_ASSERT_EQUAL_FLOAT(param, probs[a_call]);
        TEST_ASSERT_EQUAL_FLOAT(0.0, probs[a_raise]);

        // Situation 3
        memset(probs, -1, NUM_ACTION_TYPES * sizeof(*probs));

        switch (card_rank)
        {
        case JACK_RANK:
          param = C13;
          break;
        case QUEEN_RANK:
          param = C23;
          break;
        case KING_RANK:
          param = patient.params[C33_INDEX];
          break;
        default:
          param = C4[2];
        }
        action_probs(
            &patient,
            init_match_state(
                &game_def,
                C_POSITION,
                makeCard(card_rank, KUHN_SUIT),
                actions3,
                num_actions
            ),
            probs
        );

        TEST_ASSERT_EQUAL_FLOAT(1 - param, probs[a_fold]);
        TEST_ASSERT_EQUAL_FLOAT(param, probs[a_call]);
        TEST_ASSERT_EQUAL_FLOAT(0.0, probs[a_raise]);

        // Situation 4
        memset(probs, -1, NUM_ACTION_TYPES * sizeof(*probs));

        switch (card_rank)
        {
        case JACK_RANK:
          param = C14;
          break;
        case QUEEN_RANK:
          param = C24;
          break;
        case KING_RANK:
          param = patient.params[C34_INDEX];
          break;
        default:
          param = C4[3];
        }
        action_probs(
            &patient,
            init_match_state(
                &game_def,
                C_POSITION,
                makeCard(card_rank, KUHN_SUIT),
                actions4,
                num_actions
            ),
            probs
        );

        TEST_ASSERT_EQUAL_FLOAT(1 - param, probs[a_fold]);
        TEST_ASSERT_EQUAL_FLOAT(param, probs[a_call]);
        TEST_ASSERT_EQUAL_FLOAT(0.0, probs[a_raise]);
      }
    }
  }
  Catch(e)
  {
    TEST_FAIL_MESSAGE("Should Not Have Thrown An Error");
  }
}
