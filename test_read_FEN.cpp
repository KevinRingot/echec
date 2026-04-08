#include <exception>
#include <iostream>

#include "board.hpp"
#include "view.hpp"
#include "mask.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Utilisation: " << argv[0] << " <fichier_fen>\n";
        return 1;
    }

    Board board;
    Mask mask;
    empty_mask(mask);

    try {
        read_FEN(argv[1], board);
        print_board(board, mask);
    } catch (const std::exception &e) {
        return 1;
    }

    return 0;
}
