#include <exception>
#include <iostream>

#include "board.hpp"
#include "view.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Utilisation: " << argv[0] << " <fichier_fen>\n";
        return 1;
    }

    Board board;
    start(board);

    try {
        write_FEN(argv[1], board);
    } catch (const std::exception &e) {
        return 1;
    }

    return 0;
}
