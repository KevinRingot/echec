#include <iostream>

#include "board.hpp"
#include "view.hpp"

using namespace std;

int main() {
    Board board;
    start(board);
    print_board(board);
    cout << endl;

    move_piece(board, 1, 1, 2, 1);
    print_board(board);
    cout << endl;

    move_piece(board, 6, 0, 5, 0);
    print_board(board);
    cout << endl;

    move_piece(board, 6, 1, 5, 1);
    print_board(board);
    cout << endl;

    move_piece(board, 1, 0, 2, 0);
    print_board(board);
    cout << endl;

    cout << board_to_FEN(board) << endl << endl;
    cout << "Test OK" << endl;
}
