#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif

#include "board.hpp"
#include "view.hpp"

using namespace std;

namespace {

void configure_console_for_unicode() {
#ifdef _WIN32
    static bool configured = false;
    if (!configured) {
        SetConsoleOutputCP(CP_UTF8);
        configured = true;
    }
#endif
}

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

}

void set_background(Color Couleur)
{
    cout << "\033[" << (40 + Couleur) << "m";
}

void set_foreground(Color Couleur)
{
    cout << "\033[" << (30 + Couleur) << "m";
}

void set_bright_white_foreground()
{
    cout << "\033[97m";
}

void reset_color()
{
    cout << "\033[0m";
}

void print_square_color(const Board &board, int i, int j)
{
    const bool case_claire = ((i + j) % 2 == 0);
    set_background(case_claire ? JAUNE : VERT);

    char fen_char = piece_to_fen(board[i][j]);
    if ('a' <= fen_char && fen_char <= 'z')
        set_foreground(NOIR);
    else if ('A' <= fen_char && fen_char <= 'Z')
        set_bright_white_foreground();
    else
        set_foreground(NOIR);
}

void print_square_color(const Board &board, const Mask &mask, int i, int j)
{
    int code = mask[i][j];
    switch (code) {
    case 0:
        print_square_color(board, i, j);
        return;
    case 1:
        set_background(BLEU); // déplacements possibles
        break;
    case 2:
        set_background(ROUGE); // prises possibles
        break;
    case 3:
        set_background(VERT); // piéces mobiles
        break;
    default:
        set_background(CYAN); // autre information
        break;
    }

    char fen_char = piece_to_fen(board[i][j]);
    if ('a' <= fen_char && fen_char <= 'z')
        set_foreground(NOIR);
    else if ('A' <= fen_char && fen_char <= 'Z')
        set_bright_white_foreground();
    else
        set_foreground(NOIR);
}

void print_square(const Board &board, int i, int j) {
    print_square_color(board, i, j);
    cout << piece_to_char(board[i][j]) << " ";
    reset_color();
}

void print_square(const Board &board, const Mask &mask, int i, int j) {
    print_square_color(board, mask, i, j);
    cout << piece_to_char(board[i][j]) << " ";
    reset_color();
}

void print_board(const Board &board) {
    configure_console_for_unicode();
    cout << "  ";
    for (int j = 0; j < BOARD_SIZE; j++) {
        cout << char(97 + j) << " ";
    }
    cout << endl;

    for (int i = 0; i < BOARD_SIZE; i++) {
        cout << i << " ";
        for (int j = 0; j < BOARD_SIZE; j++) {
            print_square(board, i, j);
        }
        cout << endl;
    }
    
    cout << "  ";
    for (int j = 0; j < BOARD_SIZE; j++) {
        cout << char(97 + j) << " ";
    }
    cout << endl;
}

void print_board(const Board &board, const Mask &mask) {
    configure_console_for_unicode();
    cout << "  ";
    for (int j = 0; j < BOARD_SIZE; j++) {
        cout << char(97 + j) << " ";
    }
    cout << endl;

    for (int i = 0; i < BOARD_SIZE; i++) {
        cout << i << " ";
        for (int j = 0; j < BOARD_SIZE; j++) {
            print_square(board, mask, i, j);
        }
        cout << endl;
    }
    
    cout << "  ";
    for (int j = 0; j < BOARD_SIZE; j++) {
        cout << char(97 + j) << " ";
    }
    cout << endl;
}

string board_to_FEN(const Board &board) {
    string fen;

    for (int i = 0; i < 8; ++i) {
        int vide = 0;

        for (int j = 0; j < 8; ++j) {
            if (board[i][j] == EMPTY) {
                ++vide;
                continue;
            }

            if (vide > 0) {
                fen += to_string(vide);
                vide = 0;
            }

            fen += piece_to_fen(board[i][j]);
        }

        if (vide > 0) {
            fen += to_string(vide);
        }

        if (i < 7) {
            fen += '/';
        }
    }

    return fen;

    return fen;
}




void write_FEN(const char *filename, const Board &board) {
    ofstream file(filename);
    if (!file) {
        throw runtime_error("Impossible d'ouvrir le fichier en sortie");
    }

    file << board_to_FEN(board) << '\n';
}

void read_FEN(const char *filename, Board &board) {
    ifstream file(filename);
    if (!file) {
        throw runtime_error("Impossible d'ouvrir le fichier en lecture");
    }

    string fen;
    file >> fen;
    if (!file) {
        throw runtime_error("Impossible de lire la notation FEN");
    }

    empty(board);
    int lig = 0;
    int col = 0;

    for (char c : fen) {
        if (c == '/') {
            if (col != 8) {
                throw runtime_error("Ligne FEN incomplete");
            }
            ++lig;
            col = 0;
            if (lig >= 8) {
                throw runtime_error("Trop de lignes dans la FEN");
            }
            continue;
        }

        if (c >= '1' && c <= '8') {
            col += c - '0';
        } else {
            board[lig][col] = fen_to_piece(c);
            ++col;
        }

        if (col > 8) {
            throw runtime_error("Trop de colonnes dans la FEN");
        }
    }

    if (lig != 7 || col != 8) {
        throw runtime_error("Notation FEN incomplete");
    }
}
