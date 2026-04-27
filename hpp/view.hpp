#ifndef VIEW_HPP_
#define VIEW_HPP_

#include <string>
#include "types.hpp"
#include <string>
using namespace std;

/** @brief Affiche le plateau sans masque.
 * @param board Plateau a afficher. */
void print_board(const Board &board);
/** @brief Affiche le plateau avec un masque colore.
 * @param board Plateau a afficher.
 * @param mask Masque de couleurs a superposer. */
void print_board(const Board &board, const Mask &mask);
/** @brief Affiche une case sans masque.
 * @param board Plateau a afficher.
 * @param i Indice de ligne.
 * @param j Indice de colonne. */
void print_square(const Board &board, int i, int j);
/** @brief Affiche une case avec masque.
 * @param board Plateau a afficher.
 * @param mask Masque de couleurs a appliquer.
 * @param i Indice de ligne.
 * @param j Indice de colonne. */
void print_square(const Board &board, const Mask &mask, int i, int j);
/** @brief Choisit les couleurs d'affichage d'une case sans masque.
 * @param board Plateau a afficher.
 * @param i Indice de ligne.
 * @param j Indice de colonne. */
void print_square_color(const Board &board, int i, int j);
/** @brief Choisit les couleurs d'affichage d'une case avec masque.
 * @param board Plateau a afficher.
 * @param mask Masque de couleurs a appliquer.
 * @param i Indice de ligne.
 * @param j Indice de colonne. */
void print_square_color(const Board &board, const Mask &mask, int i, int j);
/** @brief Ecrit le plateau dans un fichier au format FEN simplifie.
 * @param filename Nom du fichier de sortie.
 * @param board Plateau a enregistrer. */
void write_FEN(const char *filename, const Board &board);
/** @brief Lit un plateau depuis un fichier FEN simplifie.
 * @param filename Nom du fichier a lire.
 * @param board Plateau charge en sortie. */
void read_FEN(const char *filename, Board &board);
/** @brief Applique une couleur de premier plan ANSI.
 * @param Couleur Couleur a utiliser. */
void set_foreground(Color Couleur);
/** @brief Applique une couleur de fond ANSI.
 * @param Couleur Couleur a utiliser. */
void set_background(Color Couleur);
/** @brief Reinitialise les couleurs ANSI du terminal. */
void reset_color();
/** @brief Construit la chaine FEN simplifiee du plateau.
 * @param board Plateau a convertir.
 * @return Chaine FEN correspondante. */
string board_to_FEN(const Board &board);

#endif
