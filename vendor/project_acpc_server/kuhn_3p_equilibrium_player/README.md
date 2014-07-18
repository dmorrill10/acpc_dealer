Kuhn 3-Player Equilibrium Player
=================================
`kuhn_3p_equilibrium_player` is an ACPC compatible player that connects to the
ACPC dealer and plays 3-player Kuhn poker with an equilibrium component
strategy specified by its arguments from one of those specified by
[*A Parameterized Family of Equilibrium Profiles for Three-Player Kuhn
Poker*](http://poker.cs.ualberta.ca/publications/2013-techreport-nl-size.pdf),
written by Duane Szafron, Richard Gibson, and Nathan Sturtevant, found in
*Proceedings of the Twelfth International Conference on Autonomous Agents and
Multiagent Systems (AAMAS-13)* in May 2013.

Equilibrium sub-families
--------------------------
There are three sub-families of equilibria identified by Szafron, Gibson, and
Sturtevant, each defined by a different value of parameter `c11`. Depending on
the sub-family, different parameters must necessarily be provided, except for
`c11`, `b11`, `b32`, `c33`, and `c34`, which are always necessary. Please
see
[the paper](http://poker.cs.ualberta.ca/publications/2013-techreport-nl-size.pdf)
for the domain of each paramter.

- `c11 = 0.0` sub-family: `b21` must also be provided.
- `c11 = 1/2.0` sub-family: `b21` and `b23` must also be provided.
- `0.0 < c11 < 1/2.0` sub-family: No futher parameters are necessary.

License
-------
MIT license (see `LICENCE`).

Acknowledgments
----------------
This project includes software developed for
the CException Project, by Mark VanderVoord and other
contributors, as well as the Unity Project, by Mike Karlesky,
Mark VanderVoord, and Greg Williams and other contributors.
