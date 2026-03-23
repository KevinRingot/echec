#ifndef TYPE_HPP_
#define TYPE_HPP_


enum Piece {
    EMPTY,
    WPION, WTOUR, WCAVALIER, WFOU, WREINE, WROY,
    BPION, BTOUR, BCAVALIER, BFOU, BREINE, BROY
};

using Board = Piece[8][8];

struct Game {
    Board board;
};

#endif
