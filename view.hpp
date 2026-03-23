#ifndef VIEW_HPP_
#define VIEW_HPP_
#include "types.hpp"

void print_board(Board &board);
void print_square(Board &board, int i, int j);
void write_Fen(char fich[256], Board b);
string write_TAB(Board b);


#endif