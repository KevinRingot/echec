#include "hpp/board.hpp"
#include "hpp/mask.hpp"

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

/** @brief Indique si des coordonnees appartiennent au plateau. @param i Indice de ligne. @param j Indice de colonne. @return true si la case est valide. */
static bool in_bounds(int i, int j) {
    return i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE;
}

/** @brief Marque une case attaquee si elle contient une piece adverse. @param board Plateau a analyser. @param mask Masque a remplir. @param i Ligne cible. @param j Colonne cible. @param attacker Piece attaquante. */
static void mark_if_attacked(const Board &board, Mask mask, int i, int j, Piece attacker) {
    if (!in_bounds(i, j))
        return;
    if (is_opponent(attacker, board[i][j])) {
        set_mask(mask, i, j, MASK_RED);
    }
}

/** @brief Marque un coup possible simple ou une prise. @param board Plateau a analyser. @param mask Masque a remplir. @param i Ligne cible. @param j Colonne cible. @param piece Piece qui se deplace. */
static void mark_possible_move(const Board &board, Mask mask, int i, int j, Piece piece) {
    if (!in_bounds(i, j))
        return;
    if (board[i][j] == EMPTY) {
        set_mask(mask, i, j, MASK_BLUE);
    } else if (is_opponent(piece, board[i][j])) {
        set_mask(mask, i, j, MASK_RED);
    }
}

/** @brief Parcourt des directions glissantes jusqu'au blocage. @param board Plateau a analyser. @param mask Masque a remplir. @param i Ligne de depart. @param j Colonne de depart. @param dirs Tableau des directions. @param dir_count Nombre de directions. */
static void highlight_sliding_moves(const Board &board, Mask mask, int i, int j, const int dirs[][2], int dir_count) {
    Piece piece = board[i][j];
    if (piece == EMPTY)
        return;

    clear_mask(mask);

    for (int d = 0; d < dir_count; ++d) {
        int di = dirs[d][0];
        int dj = dirs[d][1];

        for (int step = 1; step < BOARD_SIZE; ++step) {
            int ii = i + di * step;
            int jj = j + dj * step;
            if (!in_bounds(ii, jj))
                break;
            if (board[ii][jj] == EMPTY) {
                set_mask(mask, ii, jj, MASK_BLUE);
                continue;
            }
            if (is_opponent(piece, board[ii][jj])) {
                set_mask(mask, ii, jj, MASK_RED);
            }
            break;
        }
    }
}

/** @brief Verifie si un masque contient au moins une case marquee. @param mask Masque a analyser. @return true si une valeur non nulle est presente. */
static bool mask_has_highlight(const Mask mask) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (mask[i][j] != 0)
                return true;
        }
    }
    return false;
}

/** @brief Applique le bon calcul de coups selon la piece donnee. @param board Plateau a analyser. @param mask Masque de sortie. @param i Ligne de la piece. @param j Colonne de la piece. @return true si la piece est prise en charge par les highlights. */
static bool highlight_possible_moves_piece(const Board &board, Mask mask, int i, int j) {
    Piece piece = board[i][j];

    switch (piece) {
        case WTOUR:
        case BTOUR:
            highlight_possible_moves_rook(board, mask, i, j);
            return true;
        case WFOU:
        case BFOU:
            highlight_possible_moves_bishop(board, mask, i, j);
            return true;
        case WREINE:
        case BREINE:
            highlight_possible_moves_queen(board, mask, i, j);
            return true;
        case WROY:
        case BROY:
            highlight_possible_moves_king(board, mask, i, j);
            return true;
        case WCAVALIER:
        case BCAVALIER:
            highlight_possible_moves_knight(board, mask, i, j);
            return true;
        case WPION:
        case BPION:
            highlight_possible_moves_pawn(board, mask, i, j);
            return true;
        default:
            clear_mask(mask);
            return false;
    }
}

/** @brief Indique si une piece glissante attaque une case cible. @param board Plateau a analyser. @param i Ligne de la piece. @param j Colonne de la piece. @param targetI Ligne cible. @param targetJ Colonne cible. @param dirs Directions autorisees. @param dir_count Nombre de directions. @param max_steps Portee maximale. @return true si la case cible est attaquee. */
static bool sliding_piece_attacks_square(const Board &board, int i, int j, int targetI, int targetJ, const int dirs[][2], int dir_count, int max_steps) {
    for (int d = 0; d < dir_count; ++d) {
        int di = dirs[d][0];
        int dj = dirs[d][1];

        for (int step = 1; step <= max_steps; ++step) {
            int ii = i + di * step;
            int jj = j + dj * step;
            if (!in_bounds(ii, jj))
                break;
            if (ii == targetI && jj == targetJ)
                return true;
            if (board[ii][jj] != EMPTY)
                break;
        }
    }

    return false;
}

/** @brief Indique si une piece peut attaquer une case donnee. @param board Plateau a analyser. @param i Ligne de la piece. @param j Colonne de la piece. @param targetI Ligne cible. @param targetJ Colonne cible. @return true si la piece attaque la case cible. */
static bool piece_attacks_square(const Board &board, int i, int j, int targetI, int targetJ) {
    Piece piece = board[i][j];

    switch (piece) {
        case WPION:
            return (targetI == i - 1) && (targetJ == j - 1 || targetJ == j + 1);
        case BPION:
            return (targetI == i + 1) && (targetJ == j - 1 || targetJ == j + 1);
        case WCAVALIER:
        case BCAVALIER:
            {
                const int di[] = {-2, -2, -1, -1, 1, 1, 2, 2};
                const int dj[] = {-1, 1, -2, 2, -2, 2, -1, 1};
                for (int k = 0; k < 8; ++k) {
                    if (i + di[k] == targetI && j + dj[k] == targetJ)
                        return true;
                }
                return false;
            }
        case WTOUR:
        case BTOUR:
            {
                const int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
                return sliding_piece_attacks_square(board, i, j, targetI, targetJ, dirs, 4, BOARD_SIZE);
            }
        case WFOU:
        case BFOU:
            {
                const int dirs[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
                return sliding_piece_attacks_square(board, i, j, targetI, targetJ, dirs, 4, BOARD_SIZE);
            }
        case WREINE:
        case BREINE:
            {
                const int dirs[8][2] = {
                    {1, 0}, {-1, 0}, {0, 1}, {0, -1},
                    {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
                };
                return sliding_piece_attacks_square(board, i, j, targetI, targetJ, dirs, 8, BOARD_SIZE);
            }
        case WROY:
        case BROY:
            {
                const int dirs[8][2] = {
                    {1, 0}, {-1, 0}, {0, 1}, {0, -1},
                    {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
                };
                return sliding_piece_attacks_square(board, i, j, targetI, targetJ, dirs, 8, 1);
            }
        default:
            return false;
    }
}

void highlight_possible_moves_rook(const Board &board, Mask mask, int i, int j) {
    const int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    highlight_sliding_moves(board, mask, i, j, dirs, 4);
}

void highlight_possible_moves_bishop(const Board &board, Mask mask, int i, int j) {
    const int dirs[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    highlight_sliding_moves(board, mask, i, j, dirs, 4);
}

void highlight_possible_moves_queen(const Board &board, Mask mask, int i, int j) {
    const int dirs[8][2] = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };
    highlight_sliding_moves(board, mask, i, j, dirs, 8);
}

void highlight_possible_moves_king(const Board &board, Mask mask, int i, int j) {
    Piece piece = board[i][j];
    if (piece == EMPTY)
        return;

    clear_mask(mask);

    const int dirs[8][2] = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };
    for (int d = 0; d < 8; ++d) {
        mark_possible_move(board, mask, i + dirs[d][0], j + dirs[d][1], piece);
    }
}

void highlight_possible_moves_knight(const Board &board, Mask mask, int i, int j) {
    Piece piece = board[i][j];
    if (piece == EMPTY)
        return;

    clear_mask(mask);

    const int di[] = {-2, -2, -1, -1, 1, 1, 2, 2};
    const int dj[] = {-1, 1, -2, 2, -2, 2, -1, 1};
    for (int k = 0; k < 8; ++k) {
        mark_possible_move(board, mask, i + di[k], j + dj[k], piece);
    }
}

void highlight_possible_moves_pawn(const Board &board, Mask mask, int i, int j) {
    Piece piece = board[i][j];
    if (piece != WPION && piece != BPION)
        return;

    clear_mask(mask);

    int direction = (piece == WPION) ? -1 : 1;
    int start_row = (piece == WPION) ? 6 : 1;
    int next_i = i + direction;

    if (in_bounds(next_i, j) && board[next_i][j] == EMPTY) {
        set_mask(mask, next_i, j, MASK_BLUE);

        int double_i = i + 2 * direction;
        if (i == start_row && in_bounds(double_i, j) && board[double_i][j] == EMPTY) {
            set_mask(mask, double_i, j, MASK_BLUE);
        }
    }

    if (in_bounds(next_i, j - 1) && is_opponent(piece, board[next_i][j - 1])) {
        set_mask(mask, next_i, j - 1, MASK_RED);
    }
    if (in_bounds(next_i, j + 1) && is_opponent(piece, board[next_i][j + 1])) {
        set_mask(mask, next_i, j + 1, MASK_RED);
    }
}

void highlight_movable_pieces(const Board &board, Mask mask, bool whiteTurn) {
    Mask pieceMask;
    clear_mask(mask);

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            Piece piece = board[i][j];
            if (piece == EMPTY)
                continue;
            if (whiteTurn ? !is_white(piece) : !is_black(piece))
                continue;

            if (!highlight_possible_moves_piece(board, pieceMask, i, j))
                continue;
            if (mask_has_highlight(pieceMask)) {
                set_mask(mask, i, j, MASK_BLUE);
            }
        }
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
                                    set_mask(mask, ii, jj, MASK_RED);
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

void highlight_take_pieces(const Board &board, Mask mask, int i, int j) {
    Piece target = board[i][j];
    clear_mask(mask);

    if (target == EMPTY)
        return;

    for (int ii = 0; ii < BOARD_SIZE; ++ii) {
        for (int jj = 0; jj < BOARD_SIZE; ++jj) {
            Piece attacker = board[ii][jj];
            if (attacker == EMPTY)
                continue;
            if (!is_opponent(attacker, target))
                continue;
            if (piece_attacks_square(board, ii, jj, i, j)) {
                set_mask(mask, ii, jj, MASK_BLUE);
            }
        }
    }
}

/** @brief Applique le highlight de deplacements de la piece selectionnee. @param board Plateau a analyser. @param mask Masque de sortie. @param i Ligne de la piece. @param j Colonne de la piece. @return true si la piece est prise en charge. */
bool highlight_possible_moves(const Board &board, Mask mask, int i, int j) {
    Piece p = board[i][j];

    switch (p) {
        case WTOUR:
        case BTOUR:
            highlight_possible_moves_rook(board, mask, i, j);
            return true;
        case WFOU:
        case BFOU:
            highlight_possible_moves_bishop(board, mask, i, j);
            return true;
        case WREINE:
        case BREINE:
            highlight_possible_moves_queen(board, mask, i, j);
            return true;
        case WROY:
        case BROY:
            highlight_possible_moves_king(board, mask, i, j);
            return true;
        case WCAVALIER:
        case BCAVALIER:
            highlight_possible_moves_knight(board, mask, i, j);
            return true;
        case WPION:
        case BPION:
            highlight_possible_moves_pawn(board, mask, i, j);
            return true;
        default:
            clear_mask(mask);
            return false;
    }
}
