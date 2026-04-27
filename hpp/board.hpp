#ifndef BOARD_HPP_
#define BOARD_HPP_

#include "types.hpp"

/** @brief Vide completement le plateau. @param board Plateau a reinitialiser. */
void empty(Board &board);
/** @brief Retourne la piece presente sur une case. @param board Plateau a consulter. @param i Indice de ligne. @param j Indice de colonne. @return Piece contenue sur la case demandee. */
Piece get_square(Board &board, int i, int j);
/** @brief Place une piece sur une case du plateau. @param board Plateau a modifier. @param i Indice de ligne. @param j Indice de colonne. @param p Piece a placer. */
void set_square(Board &board, int i, int j, Piece p);
/** @brief Place les pieces sur la position initiale. @param board Plateau a initialiser. */
void start(Board &board);
/** @brief Deplace une piece d'une case vers une autre. @param board Plateau a modifier. @param i1 Ligne de depart. @param j1 Colonne de depart. @param i2 Ligne d'arrivee. @param j2 Colonne d'arrivee. */
void move_piece(Board &board, int i1, int j1, int i2, int j2);
/** @brief Convertit une piece en caractere Unicode d'affichage. @param p Piece a convertir. @return Chaine representant la piece. */
const char *piece_to_char(Piece p);

/** @brief Indique si une piece est blanche. @param p Piece a tester. @return true si la piece est blanche. */
bool is_white(Piece p);
/** @brief Indique si une piece est noire. @param p Piece a tester. @return true si la piece est noire. */
bool is_black(Piece p);
/** @brief Indique si deux pieces sont adverses. @param attacker Piece qui attaque. @param target Piece cible. @return true si la cible est une piece adverse. */
bool is_opponent(Piece attacker, Piece target);

/** @brief Met en evidence les coups possibles d'une tour. @param board Plateau a analyser. @param mask Masque de sortie. @param i Ligne de la tour. @param j Colonne de la tour. */
void highlight_possible_moves_rook(const Board &board, Mask mask, int i, int j);
/** @brief Met en evidence les coups possibles d'un fou. @param board Plateau a analyser. @param mask Masque de sortie. @param i Ligne du fou. @param j Colonne du fou. */
void highlight_possible_moves_bishop(const Board &board, Mask mask, int i, int j);
/** @brief Met en evidence les coups possibles d'une reine. @param board Plateau a analyser. @param mask Masque de sortie. @param i Ligne de la reine. @param j Colonne de la reine. */
void highlight_possible_moves_queen(const Board &board, Mask mask, int i, int j);
/** @brief Met en evidence les coups possibles d'un roi. @param board Plateau a analyser. @param mask Masque de sortie. @param i Ligne du roi. @param j Colonne du roi. */
void highlight_possible_moves_king(const Board &board, Mask mask, int i, int j);
/** @brief Met en evidence les coups possibles d'un cavalier. @param board Plateau a analyser. @param mask Masque de sortie. @param i Ligne du cavalier. @param j Colonne du cavalier. */
void highlight_possible_moves_knight(const Board &board, Mask mask, int i, int j);
/** @brief Met en evidence les coups possibles d'un pion. @param board Plateau a analyser. @param mask Masque de sortie. @param i Ligne du pion. @param j Colonne du pion. */
void highlight_possible_moves_pawn(const Board &board, Mask mask, int i, int j);
/** @brief Met en evidence les pieces du joueur courant qui peuvent bouger. @param board Plateau a analyser. @param mask Masque de sortie. @param whiteTurn true si c'est au tour des blancs. */
void highlight_movable_pieces(const Board &board, Mask mask, bool whiteTurn);
/** @brief Met en evidence les pieces attaquables par le joueur courant. @param board Plateau a analyser. @param mask Masque de sortie. @param whiteTurn true si c'est au tour des blancs. */
void highlight_attacked_pieces(const Board &board, Mask mask, bool whiteTurn);
/** @brief Met en evidence les pieces adverses qui peuvent prendre une piece donnee. @param board Plateau a analyser. @param mask Masque de sortie. @param i Ligne de la piece cible. @param j Colonne de la piece cible. */
void highlight_take_pieces(const Board &board, Mask mask, int i, int j);

bool highlight_possible_moves(const Board &board, Mask mask, int i, int j);

#endif
