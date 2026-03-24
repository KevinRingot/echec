#ifndef VIEW_HPP_
#define VIEW_HPP_
#include "types.hpp"
#include <string>
using namespace std;


void print_board(Board &board);
void print_square(Board &board, int i, int j);
void write_FEN(Board board, const string& filename);
string write_tab(Board board);


#endif