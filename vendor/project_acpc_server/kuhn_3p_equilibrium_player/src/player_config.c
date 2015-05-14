/*
Copyright (C) 2013 by the Computer Poker Research Group, University of Alberta
*/

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/time.h>
#include "rng.h"
#include "commander.h"
#include "player_config.h"

static void parse(
    const char* to_parse,
    const char* template,
    const char* error_message,
    size_t num_targets,
    ...
)
{
  va_list scan_targets;
  va_start(scan_targets, num_targets);

  if(vsscanf(to_parse, template, scan_targets) < num_targets) {
    fprintf(stderr, "ERROR: %s: %s\n", error_message, to_parse);
    exit(EXIT_FAILURE);
  }
  va_end(scan_targets);
}

static void c11(command_t *self) {
  assert(self->data);

  PlayerConfig* config = self->data;
  parse(
      self->arg,
      "%lf",
      "Invalid C11 parameter",
      1,
      &config->params[C11_INDEX]
  );
}

static void b11(command_t *self) {
  assert(self->data);
  PlayerConfig* config = self->data;
  parse(
      self->arg,
      "%lf",
      "Invalid B11 parameter",
      1,
      &config->params[B11_INDEX]
  );
}

static void b21(command_t *self) {
  assert(self->data);
  PlayerConfig* config = self->data;
  parse(
      self->arg,
      "%lf",
      "Invalid B21 parameter",
      1,
      &config->params[B21_INDEX]
  );
}

static void b23(command_t *self) {
  assert(self->data);
  PlayerConfig* config = self->data;
  parse(
      self->arg,
      "%lf",
      "Invalid B23 parameter",
      1,
      &config->params[B23_INDEX]
  );
}

static void b32(command_t *self) {
  assert(self->data);
  PlayerConfig* config = self->data;
  parse(
      self->arg,
      "%lf",
      "Invalid B32 parameter",
      1,
      &config->params[B32_INDEX]
  );
}

static void c33(command_t *self) {
  assert(self->data);
  PlayerConfig* config = self->data;
  parse(
      self->arg,
      "%lf",
      "Invalid C33 parameter",
      1,
      &config->params[C33_INDEX]
  );
}

static void c34(command_t *self) {
  assert(self->data);
  PlayerConfig* config = self->data;
  parse(
      self->arg,
      "%lf",
      "Invalid C34 parameter",
      1,
      &config->params[C34_INDEX]
  );
}

static void r(command_t *self) {
  assert(self->data);
  PlayerConfig* config = self->data;
  parse(
      self->arg,
      "%"SCNu32,
      "Invalid random seed",
      1,
      &config->seed
  );
}

static void parse_game_def(command_t *self)
{
  assert(self->data);

  PlayerConfig* config = self->data;

  FILE* file = fopen(self->arg, "r");
  if (!file) {
    fprintf( stderr, "ERROR: could not open game %s\n", self->arg );
    exit( EXIT_FAILURE );
  }

  config->game = readGame( file );
  if (!config->game) {
    fprintf( stderr, "ERROR: could not read game %s\n", self->arg );
    exit( EXIT_FAILURE );
  }
  fclose( file );
}

static void get_host(command_t *self)
{
  if (!self->arg) {
    return;
  }
  assert(self->data);

  PlayerConfig* config = self->data;

  memset(config->dealer.host, 0,
         _POSIX_HOST_NAME_MAX * sizeof(*config->dealer.host));
  strncpy(config->dealer.host, self->arg, _POSIX_HOST_NAME_MAX);
}

static void get_port(command_t *self)
{
  assert(self->data);

  PlayerConfig* config = self->data;

  parse(
      self->arg,
      "%"SCNu16,
      "Invalid port",
      1,
      &config->dealer.port
  );
}

PlayerConfig new_player_config()
{
  // If no random seed is given, initialize the player's random number seed
  // using time
  struct timeval tv;
  gettimeofday(&tv, NULL);
  rng_state_t rng;
  init_genrand(&rng, tv.tv_usec);

  PlayerConfig this = {
      .dealer = new_dealer_connection(),
      .game = NULL,
      .params = {0},
      .seed = genrand_int32(&rng)
  };

  return this;
}

PlayerConfig new_player_config_from_argv(int argc, char** argv)
{
  command_t cmd;
  command_init(&cmd, argv[0], "0.0.1");

  command_option(
      &cmd,
      "-n",
      "--host [host name]",
      "Name of the host on which dealer is running (defaults to 'localhost')",
      get_host
  );
  command_option(
      &cmd,
      "-p",
      "--port <port number>",
      "Port number to connect to dealer",
      get_port
  );
  command_option(
      &cmd,
      "-g",
      "--game <file name>",
      "Game definition file name",
      parse_game_def
  );
  command_option(
      &cmd,
      "-r",
      "--seed [non-negative integer]",
      "Random seed, defaults to a random random seed based on system time.",
      r
  );

  command_option(&cmd, "-0", "--c11 <probability>", "C11 value", c11);
  command_option(&cmd, "-1", "--b11 <probability>", "B11 value", b11);
  command_option(&cmd, "-2", "--b21 [probability]", "B21 value", b21);
  command_option(&cmd, "-3", "--b23 [probability]", "B23 value", b23);
  command_option(&cmd, "-4", "--b32 <probability>", "B32 value", b32);
  command_option(&cmd, "-8", "--c33 <probability>", "C33 value", c33);
  command_option(&cmd, "-9", "--c34 <probability>", "C34 value", c34);

  if( argc < 14 ) {
    command_help(&cmd);
    exit(EXIT_SUCCESS);
  }

  PlayerConfig config = new_player_config();
  cmd.data = &config;

  command_parse(&cmd, argc, argv);
  command_free(&cmd);

  return config;
}
