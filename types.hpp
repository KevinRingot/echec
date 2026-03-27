#ifndef TYPE_HPP_
#define TYPE_HPP_


enum Piece {
    EMPTY = 0,
    WROY = 1  , WREINE = 2 , WCAVALIER = 3 , WTOUR = 4 , WFOU = 5 , WPION = 6 ,
    BROY = 7 , BREINE = 8 , BCAVALIER = 9 , BTOUR = 10  , BFOU = 11 , BPION = 12
};

enum Color {
    NOIR = 0,
    ROUGE = 1,
    VERT = 2,
    JAUNE = 3,
    BLEU = 4,
    VIOLET = 5,
    CYAN = 6,
    GRIS = 7,
};

using Board = Piece[8][8];

struct Game {
    Board board;
};

#endif
