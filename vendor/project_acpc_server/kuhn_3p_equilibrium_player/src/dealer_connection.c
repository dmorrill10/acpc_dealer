/*
Copyright (C) 2013 by the Computer Poker Research Group, University of Alberta
*/

#include <stdlib.h>
#include <assert.h>
#include "dealer_connection.h"
#include "game.h"

DealerConnection new_dealer_connection()
{
  DealerConnection this = {
      .host = "localhost",
      .port = 0,
      .toServer = NULL,
      .fromServer = NULL
  };

  return this;
}

void connect_to_dealer(DealerConnection* thisPtr)
{
  assert(thisPtr);

  int sock = connectTo(thisPtr->host, thisPtr->port);
  if( sock < 0 ) {
    exit( EXIT_FAILURE );
  }
  thisPtr->toServer = fdopen( sock, "w" );
  thisPtr->fromServer = fdopen( sock, "r" );
  if(!(thisPtr->toServer && thisPtr->fromServer)) {
    fprintf( stderr, "ERROR: could not get socket streams\n" );
    exit( EXIT_FAILURE );
  }

  /* send version string to dealer */
  if(
      fprintf(
          thisPtr->toServer,
          "VERSION:%"PRIu32".%"PRIu32".%"PRIu32"\n",
          VERSION_MAJOR, VERSION_MINOR, VERSION_REVISION
      ) != 14
  ) {
    fprintf( stderr, "ERROR: could not get send version to server\n" );
    exit(EXIT_FAILURE);
  }
  fflush(thisPtr->toServer);
}
