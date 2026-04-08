#ifndef VIEW_HPP_
#define VIEW_HPP_

#include <string>

#include "types.hpp"
#include <string>
using namespace std;


void print_board(const Board &board);
void print_board(const Board &board, const Mask &mask);
void print_square(const Board &board, int i, int j);
void print_square(const Board &board, const Mask &mask, int i, int j);
void print_square_color(const Board &board, int i, int j);
void print_square_color(const Board &board, const Mask &mask, int i, int j);
void write_FEN(const char *filename, const Board &board);
void read_FEN(const char *filename, Board &board);
void set_foreground(Color Couleur);
void set_background(Color Couleur);
void reset_color();
std::string board_to_FEN(const Board &board);

#endif
