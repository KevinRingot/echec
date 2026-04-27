#ifndef GAME_HPP_
#define GAME_HPP_

#include <string>
#include "types.hpp"
#include <string>
using namespace std;

bool one_run(Game *game, Mask *mask, bool *whiteTurn, bool humanPlayer);
void init_game(Game *game);

#endif
