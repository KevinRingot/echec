#include <iostream>
using namespace std;
#include "view.hpp"
#include "board.hpp"  

void print_board(Board &board) {

    cout << "  ";
    for (int j = 0; j < 8; j++)
    {
        cout << char(97 + j) << " ";
    }
    cout << endl;
    for(int i=0;i<8;i++) {
        cout << i << " ";
        for(int j=0;j<8;j++) {
            cout << piece_to_char(board[i][j]) << " ";
        }
        cout << endl;
    }
}