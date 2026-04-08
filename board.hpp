#ifndef BOARD_HPP_
#define BOARD_HPP_

#include "types.hpp"

void empty(Board &board);
Piece get_square(Board &board, int i, int j);
void set_square(Board &board, int i, int j, Piece p);
void start(Board &board);
void move_piece(Board &board, int i1, int j1, int i2, int j2);
const char *piece_to_char(Piece p);

bool is_white(Piece p);
bool is_black(Piece p);
bool is_opponent(Piece attacker, Piece target);

void highlight_attacked_pieces(const Board &board, Mask mask, bool whiteTurn);

#endif
