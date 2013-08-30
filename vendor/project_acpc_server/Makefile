
THIS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

CC = gcc
CFLAGS = -O3 -Wall

KUHN_3P_E_BASE = kuhn_3p_equilibrium_player
KUHN_3P_E_PLAYER := $(KUHN_3P_E_BASE)
KUHN_3P_E_DIR := $(KUHN_3P_E_BASE)

PROGRAMS = bm_server bm_widget dealer example_player

all: $(PROGRAMS) $(KUHN_3P_E_DIR)/$(KUHN_3P_E_PLAYER)

clean:
	rm -f $(PROGRAMS) && cd $(KUHN_3P_E_DIR) && make clean


bm_server: bm_server.c game.c game.h rng.c rng.h net.c net.h
	$(CC) $(CFLAGS) -o $@ bm_server.c game.c rng.c net.c

bm_widget: bm_widget.c net.c net.h
	$(CC) $(CFLAGS) -o $@ bm_widget.c net.c

bm_run_matches: bm_run_matches.c net.c net.h
	$(CC) $(CFLAGS) -o $@ bm_run_matches.c net.c

dealer: game.c game.h evalHandTables rng.c rng.h dealer.c net.c net.h
	$(CC) $(CFLAGS) -o $@ game.c rng.c dealer.c net.c

example_player: game.c game.h evalHandTables rng.c rng.h example_player.c net.c net.h
	$(CC) $(CFLAGS) -o $@ game.c rng.c example_player.c net.c

$(KUHN_3P_E_DIR)/$(KUHN_3P_E_PLAYER):
	cd $(KUHN_3P_E_DIR) && make
