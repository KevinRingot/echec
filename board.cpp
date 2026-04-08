#include "board.hpp"
#include "mask.hpp"

void empty(Board &board) {
    for(int i=0;i<BOARD_SIZE;i++)
        for(int j=0;j<BOARD_SIZE;j++)
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

bool is_white(Piece p) {
    return p >= WROY && p <= WPION;
}

bool is_black(Piece p) {
    return p >= BROY && p <= BPION;
}

bool is_opponent(Piece attacker, Piece target) {
    if (target == EMPTY)
        return false;
    if (is_white(attacker))
        return is_black(target);
    if (is_black(attacker))
        return is_white(target);
    return false;
}

static bool in_bounds(int i, int j) {
    return i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE;
}

static void mark_if_attacked(const Board &board, Mask mask, int i, int j, Piece attacker) {
    if (!in_bounds(i, j))
        return;
    if (is_opponent(attacker, board[i][j])) {
        set_mask(mask, i, j, 2);
    }
}

void highlight_attacked_pieces(const Board &board, Mask mask, bool whiteTurn) {
    clear_mask(mask);

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            Piece p = board[i][j];
            if (p == EMPTY)
                continue;
            if (whiteTurn ? !is_white(p) : !is_black(p))
                continue;

            switch (p) {
                case WPION:
                    mark_if_attacked(board, mask, i - 1, j - 1, p);
                    mark_if_attacked(board, mask, i - 1, j + 1, p);
                    break;
                case BPION:
                    mark_if_attacked(board, mask, i + 1, j - 1, p);
                    mark_if_attacked(board, mask, i + 1, j + 1, p);
                    break;
                case WCAVALIER:
                case BCAVALIER:
                    {
                        const int di[] = {-2,-2,-1,-1,1,1,2,2};
                        const int dj[] = {-1,1,-2,2,-2,2,-1,1};
                        for (int k = 0; k < 8; ++k)
                            mark_if_attacked(board, mask, i + di[k], j + dj[k], p);
                    }
                    break;
                case WTOUR:
                case BTOUR:
                case WFOU:
                case BFOU:
                case WREINE:
                case BREINE:
                case WROY:
                case BROY:
                    {
                        const int dirs[8][2] = {
                            {1,0},{-1,0},{0,1},{0,-1},
                            {1,1},{1,-1},{-1,1},{-1,-1}
                        };
                        int max_steps = 1;
                        if (p == WTOUR || p == BTOUR || p == WFOU || p == BFOU || p == WREINE || p == BREINE)
                            max_steps = BOARD_SIZE;

                        for (int d = 0; d < 8; ++d) {
                            if ((p == WTOUR || p == BTOUR) && d >= 4) continue;
                            if ((p == WFOU || p == BFOU) && d < 4) continue;

                            int di = dirs[d][0];
                            int dj = dirs[d][1];

                            for (int step = 1; step <= max_steps; ++step) {
                                int ii = i + di * step;
                                int jj = j + dj * step;
                                if (!in_bounds(ii, jj))
                                    break;
                                if (board[ii][jj] == EMPTY)
                                    continue;
                                if (is_opponent(p, board[ii][jj]))
                                    set_mask(mask, ii, jj, 2);
                                break;
                            }
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

