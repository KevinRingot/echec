#include <iostream>
#include <limits>
#include <windows.h>

#include "hpp/board.hpp"
#include "hpp/view.hpp"
#include "hpp/mask.hpp"

using namespace std;

/** @brief Active l'affichage Unicode et ANSI du terminal. */
void configure_console_for_unicode() {
#ifdef _WIN32
    static bool configured = false;
    if (!configured) {
        SetConsoleOutputCP(CP_UTF8);
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hConsole != INVALID_HANDLE_VALUE) {
            DWORD mode = 0;
            if (GetConsoleMode(hConsole, &mode)) {
                SetConsoleMode(hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
            }
        }
        configured = true;
    }
#endif
}

/** @brief Convertit une piece en caractere FEN.
 * @param p Piece a convertir.
 * @return Caractere FEN associe. */
char piece_to_fen(Piece p) {
    switch (p) {
        case WROY:
            return 'K';
        case WREINE:
            return 'Q';
        case WCAVALIER:
            return 'N';
        case WTOUR:
            return 'R';
        case WFOU:
            return 'B';
        case WPION:
            return 'P';
        case BROY:
            return 'k';
        case BREINE:
            return 'q';
        case BCAVALIER:
            return 'n';
        case BTOUR:
            return 'r';
        case BFOU:
            return 'b';
        case BPION:
            return 'p';
        case EMPTY:
            return ' ';
    }
    throw runtime_error("Piece inconnue");
}

/** @brief Convertit un caractere FEN en piece.
 * @param c Caractere FEN a convertir.
 * @return Piece correspondante. */
Piece fen_to_piece(char c) {
    switch (c) {
        case 'K':
            return WROY;
        case 'Q':
            return WREINE;
        case 'N':
            return WCAVALIER;
        case 'R': 
            return WTOUR;
        case 'B':
            return WFOU;
        case 'P':
            return WPION;
        case 'k':
            return BROY;
        case 'q':
            return BREINE;
        case 'n':
            return BCAVALIER;
        case 'r':
            return BTOUR;
        case 'b':
            return BFOU;
        case 'p':
            return BPION;
        default:
            throw runtime_error("Caractere FEN invalide");
    }
}

/** @brief Indique si des coordonnees appartiennent au plateau.
 * @param i Indice de ligne.
 * @param j Indice de colonne.
 * @return true si la case est valide. */
bool in_bounds(int i, int j) {
    return i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE;
}

/** @brief Convertit une colonne alphabetique en indice numerique.
 * @param c Lettre comprise entre a et h.
 * @return Indice de colonne correspondant. */
int abc_to_int(char c) 
{
    switch (c) {
        case 'a':
            return 0;
        case 'b':
            return 1;
        case 'c':
            return 2;
        case 'd':
            return 3;
        case 'e':
            return 4;
        case 'f':
            return 5;
        case 'g':
            return 6;
        case 'h':
            return 7;
    }
    throw runtime_error("abc inconnu");
}
