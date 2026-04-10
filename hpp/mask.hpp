#ifndef MASK_HPP_
#define MASK_HPP_

#include "types.hpp"

/** @brief Met toutes les cases du masque a zero. @param mask Masque a reinitialiser. */
void empty_mask(Mask mask);
/** @brief Efface le contenu d'un masque. @param mask Masque a vider. */
void clear_mask(Mask mask);
/** @brief Retourne la valeur stockee dans le masque sur une case. @param mask Masque a consulter. @param i Indice de ligne. @param j Indice de colonne. @return Valeur presente dans le masque. */
int get_mask(const Mask mask, int i, int j);
/** @brief Modifie la valeur stockee dans le masque sur une case. @param mask Masque a modifier. @param i Indice de ligne. @param j Indice de colonne. @param value Nouvelle valeur. */
void set_mask(Mask mask, int i, int j, int value);

#endif
