/*
Copyright (C) 2013 by the Computer Poker Research Group, University of Alberta
*/

#include <stdlib.h>

#include <assert.h>

//#include <stdio.h>

//#include <string.h>
//#include <unistd.h>
//#include <netdb.h>
//#include <sys/socket.h>
//#include <sys/time.h>
//#include <netinet/in.h>
//#include <netinet/tcp.h>
//#include <getopt.h>
//#include "game.h"
//#include "rng.h"
//#include "net.h"

#include "kuhn_3p_equilibrium_player.h"
#include "commander.h"

static void c11(command_t *self) {
  assert(self->data);

  double* params = self->data;
  sscanf(self->arg, "%lf", &params[C11_INDEX]);
}

static void b11(command_t *self) {
  assert(self->data);

  double* params = self->data;
  sscanf(self->arg, "%lf", &params[B11_INDEX]);
}

static void b21(command_t *self) {
  assert(self->data);

  double* params = self->data;
  sscanf(self->arg, "%lf", &params[B21_INDEX]);
}

static void b23(command_t *self) {
  assert(self->data);

  double* params = self->data;
  sscanf(self->arg, "%lf", &params[B23_INDEX]);
}

static void b32(command_t *self) {
  assert(self->data);

  double* params = self->data;
  sscanf(self->arg, "%lf", &params[B32_INDEX]);
}

static void c33(command_t *self) {
  assert(self->data);

  double* params = self->data;
  sscanf(self->arg, "%lf", &params[C33_INDEX]);
}

static void c34(command_t *self) {
  assert(self->data);

  double* params = self->data;
  sscanf(self->arg, "%lf", &params[C34_INDEX]);
}

static void r(command_t *self) {
  assert(self->data);

  uint32_t* seed = self->data;
  sscanf(self->arg, "%"SCNu32, seed);
}

static void game_def(command_t *self)
{
  assert(self->data);
  assert(self->arg);

  FILE* file = fopen(self->arg, "r");
  if( file == NULL ) {
    fprintf( stderr, "ERROR: could not open game %s\n", self->arg );
    exit( EXIT_FAILURE );
  }

  Game** game = self->data;

  *game = readGame( file );
  if(*game == NULL) {
    fprintf( stderr, "ERROR: could not read game %s\n", self->arg );
    exit( EXIT_FAILURE );
  }
  fclose( file );
}

int main( int argc, char **argv )
{
  /* we make some assumptions about the actions - check them here */
  assert( NUM_ACTION_TYPES == 3 );

  command_t cmd;
  command_init(&cmd, argv[0], "0.0.1");

  Game *game;
  cmd.data = &game;

  command_option(
      &cmd,
      "-g",
      "--game <game def file name>",
      "Game definition file name",
      game_def
  );

  double params[NUM_PARAMS] = {0};
  cmd.data = params;

  command_option(&cmd, "-0", "--c11 <probability>", "C11 value", c11);
  command_option(&cmd, "-1", "--b11 <probability>", "B11 value", b11);
  command_option(&cmd, "-2", "--b21 [probability]", "B21 value", b21);
  command_option(&cmd, "-3", "--b23 [probability]", "B23 value", b23);
  command_option(&cmd, "-4", "--b32 <probability>", "B32 value", b32);
  command_option(&cmd, "-8", "--c33 <probability>", "C33 value", c33);
  command_option(&cmd, "-9", "--c34 <probability>", "C34 value", c34);

  uint32_t seed = 0;
  cmd.data = &seed;
  command_option(
      &cmd,
      "-r",
      "--seed [non-negative integer]",
      "Random seed",
      r
  );

  if( argc < 12 )
  {
    command_help(&cmd);
  }

  command_parse(&cmd, argc, argv);
  command_free(&cmd);

  int sock, len, r, a;
  int32_t min, max;
  uint16_t port;
  double p;
  MatchState state;
  FILE *file, *toServer, *fromServer;
  struct timeval tv;
  double probs[ NUM_ACTION_TYPES ];
  double actionProbs[ NUM_ACTION_TYPES ];
  rng_state_t rng;
  char line[ MAX_LINE_LEN ];

  /* Initialize the player's random number state using time */
//  gettimeofday( &tv, NULL );
//  init_genrand( &rng, tv.tv_usec );
//
  kuhn_3p_equilibrium_player_t player = init_private_info(game, params, seed);

  /* connect to the dealer */
  if( sscanf( argv[ 3 ], "%"SCNu16, &port ) < 1 ) {
    fprintf( stderr, "ERROR: invalid port %s\n", argv[ 3 ] );
    exit( EXIT_FAILURE );
  }
  sock = connectTo( argv[ 2 ], port );
  if( sock < 0 ) {
    exit( EXIT_FAILURE );
  }
  toServer = fdopen( sock, "w" );
  fromServer = fdopen( sock, "r" );
  if( toServer == NULL || fromServer == NULL ) {
    fprintf( stderr, "ERROR: could not get socket streams\n" );
    exit( EXIT_FAILURE );
  }

  /* send version string to dealer */
  if( fprintf( toServer, "VERSION:%"PRIu32".%"PRIu32".%"PRIu32"\n",
	       VERSION_MAJOR, VERSION_MINOR, VERSION_REVISION ) != 14 ) {
    fprintf( stderr, "ERROR: could not get send version to server\n" );
    exit( EXIT_FAILURE );
  }
  fflush( toServer );

  /* play the game! */
  while( fgets( line, MAX_LINE_LEN, fromServer ) ) {
    /* ignore comments */
    if( line[ 0 ] == '#' || line[ 0 ] == ';' ) {
      continue;
    }

    len = readMatchState( line, game, &state );
    if( len < 0 ) {
      fprintf( stderr, "ERROR: could not read state %s", line );
      exit( EXIT_FAILURE );
    }

    if(stateFinished( &state.state)) {
      /* ignore the game over message */
      continue;
    }

    if(currentPlayer(game, &state.state) != state.viewingPlayer) {
      /* we're not acting */
      continue;
    }

    /* add a colon (guaranteed to fit because we read a new-line in fgets) */
    line[ len ] = ':';
    ++len;

    /* build the set of valid actions */
    p = 0;
    for( a = 0; a < NUM_ACTION_TYPES; ++a ) {
      actionProbs[ a ] = 0.0;
    }

    Action action_to_send = action(&player, state);

        /* do the action! */
    assert(isValidAction(game, &state.state, 0, &action_to_send));
    r = printAction(game, &action_to_send, MAX_LINE_LEN - len - 2, &line[len]);

    if( r < 0 ) {
      fprintf( stderr, "ERROR: line too long after printing action\n" );
      exit( EXIT_FAILURE );
    }
    len += r;
    line[ len ] = '\r';
    ++len;
    line[ len ] = '\n';
    ++len;

    if( fwrite(line, 1, len, toServer) != len ) {
      fprintf( stderr, "ERROR: could not get send response to server\n" );
      exit(EXIT_FAILURE);
    }
    fflush(toServer);
  }

  return EXIT_SUCCESS;
}
