/*
Copyright (C) 2013 by the Computer Poker Research Group, University of Alberta
*/

#include <stdlib.h>
#include <assert.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include "game.h"
#include "rng.h"
#include "net.h"
#include "kuhn_3p_equilibrium_player.h"
#include "player_config.h"

int main(int argc, char **argv)
{
  PlayerConfig config = new_player_config_from_argv(argc, argv);

  connect_to_dealer(&config.dealer);

  int len, r;
  MatchState state;
  char line[ MAX_LINE_LEN ];

  Kuhn3pEquilibriumPlayer player = new_kuhn_3p_equilibrium_player(
      config.game,
      config.params,
      config.seed
  );

  /* play the game! */
  while( fgets( line, MAX_LINE_LEN, config.dealer.fromServer ) ) {
    /* ignore comments */
    if( line[ 0 ] == '#' || line[ 0 ] == ';' ) {
      continue;
    }

    len = readMatchState( line, config.game, &state );
    if( len < 0 ) {
      fprintf( stderr, "ERROR: could not read state %s", line );
      exit( EXIT_FAILURE );
    }

    if(stateFinished( &state.state)) {
      /* ignore the game over message */
      continue;
    }

    if(currentPlayer(config.game, &state.state) != state.viewingPlayer) {
      /* we're not acting */
      continue;
    }

    /* add a colon (guaranteed to fit because we read a new-line in fgets) */
    line[ len ] = ':';
    ++len;

    Action action_to_send = action(&player, state);

        /* do the action! */
    assert(isValidAction(config.game, &state.state, 0, &action_to_send));
    r = printAction(config.game, &action_to_send, MAX_LINE_LEN - len - 2, &line[len]);

    if( r < 0 ) {
      fprintf( stderr, "ERROR: line too long after printing action\n" );
      exit( EXIT_FAILURE );
    }
    len += r;
    line[ len ] = '\r';
    ++len;
    line[ len ] = '\n';
    ++len;

    if( fwrite(line, 1, len, config.dealer.toServer) != len ) {
      fprintf( stderr, "ERROR: could not get send response to server\n" );
      exit(EXIT_FAILURE);
    }
    fflush(config.dealer.toServer);
  }

  return EXIT_SUCCESS;
}
