/*
Copyright (C) 2013 by the Computer Poker Research Group, University of Alberta
*/

#ifndef __DEALER_CONNECTION_H__
#define __DEALER_CONNECTION_H__

#include <limits.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
  char host[_POSIX_HOST_NAME_MAX];
  uint16_t port;
  FILE *toServer;
  FILE *fromServer;
} DealerConnection;

DealerConnection new_dealer_connection();
void connect_to_dealer(DealerConnection* thisPtr);

#endif
