#include <iostream>
#include "board.hpp"

using namespace std;

int main() {
    Board board;
    start(board);

    // test move
    move_piece(board, 6, 0, 4, 0);

    cout << "Test OK" << endl;
    return 0;
}

