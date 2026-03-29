#include "board.hpp"

void empty(Board &board) {
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            board[i][j] = EMPTY;
}

Piece get_square(Board &board, int i, int j) {
    return board[i][j];
}

void set_square(Board &board, int i, int j, Piece p) {
    board[i][j] = p;
}

void start(Board &board) {
    empty(board);

    for(int j=0;j<8;j++) {
        board[1][j] = BPION;
        board[6][j] = WPION;
    }
// [ligne][colonne]
    board[0][0] = board[0][7] = BTOUR;
    board[7][0] = board[7][7] = WTOUR;

    board[0][1] = board[0][6] = BCAVALIER;
    board[7][1] = board[7][6] = WCAVALIER;

    board[0][2] = board[0][5] = BFOU;
    board[7][2] = board[7][5] = WFOU;

    board[0][3] = BREINE;
    board[7][3] = WREINE;

    board[0][4] = BROY;
    board[7][4] = WROY;
}

void move_piece(Board &board, int i1, int j1, int i2, int j2) {
    board[i2][j2] = board[i1][j1];
    board[i1][j1] = EMPTY;
}


const char *piece_to_char(Piece p) {
    const char *pieces[] = {
        " ",
        u8"\u265A", u8"\u265B", u8"\u265E", u8"\u265C", u8"\u265D", u8"\u265F",
        u8"\u265A", u8"\u265B", u8"\u265E", u8"\u265C", u8"\u265D", u8"\u265F"
    };
    return pieces[p];
}
