#!/bin/bash
THIS_DIR=$( cd "$( dirname "$0" )" && pwd )
cd $THIS_DIR && ./kuhn_3p_equilibrium_player/kuhn_3p_equilibrium_player -n $1 -p $2 --c11 0.0 --b11 0.25 --b21 0.25 --b32 0.9375 --c33 0.0 --c34 1.0 -g kuhn.limit.3p.game -r 237363252
