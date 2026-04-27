#include <iostream>
#include <limits>

#include "hpp/game.hpp"
#include "hpp/mask.hpp"

using namespace std;

/** @brief Point d'entree du programme interactif.
 * @return Code de sortie du programme. */
int main() {
    Game game;
    Mask mask;
    bool whiteTurn = true;
    bool whiteHuman = true;
    bool blackHuman = false;

    init_game(&game);
    empty_mask(&mask);

    while (true) {
        bool humanPlayer = whiteTurn ? whiteHuman : blackHuman;
        if (!one_run(&game, &mask, &whiteTurn, humanPlayer)) {
            return 0;
        }
    }

    return 0;
}
