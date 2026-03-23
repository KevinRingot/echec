#include <iostream>
#include "board.hpp"
#include "view.hpp"
using namespace std;

int main() {
    Board board;
    start(board);
    print_board(board);
    // test move

    cout << "Test OK" << endl;
    return 0;
}

