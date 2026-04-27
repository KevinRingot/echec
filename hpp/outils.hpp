#ifndef OUTILS_HPP_
#define OUTILS_HPP_

#include <string>
#include "types.hpp"
#include <string>
using namespace std;

/** @brief Indique si des coordonnees appartiennent au plateau.
 * @param i Indice de ligne.
 * @param j Indice de colonne.
 * @return true si la case est valide. */
bool in_bounds(int i, int j);
/** @brief Convertit une colonne alphabetique en indice numerique.
 * @param c Lettre comprise entre a et h.
 * @return Indice de colonne correspondant. */
int abc_to_int(char c);
/** @brief Convertit une piece en caractere FEN.
 * @param p Piece a convertir.
 * @return Caractere FEN associe. */
char piece_to_fen(Piece p);
/** @brief Convertit un caractere FEN en piece.
 * @param c Caractere FEN a convertir.
 * @return Piece correspondante. */
Piece fen_to_piece(char c);
/** @brief Active l'affichage Unicode et ANSI du terminal. */
void configure_console_for_unicode();

#endif
