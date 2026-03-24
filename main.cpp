#include <iostream>
#include "board.hpp"
#include "view.hpp"
using namespace std;

int main() {
    Board board;
    start(board);
    print_board(board);
    cout << endl;

    // test move
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
    string fen = write_tab(board);
    cout << fen << endl;
    cout << "Test OK" << endl;
    for(int i = 0 ; i < 8 ; i++){
        for( int j = 0 ; j < 8 ; j++){
            cout << board[i][j];
        }
    }
    return 0;
}

