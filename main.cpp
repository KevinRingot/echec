#include <iostream>
#include <limits>

#include "board.hpp"
#include "view.hpp"
#include "mask.hpp"

using namespace std;

static bool in_bounds(int i, int j) {
    return i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE;
}

static int abc_to_int(char c) 
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

static bool highlight_possible_moves(const Board &board, Mask mask, int i, int j) {
    Piece p = board[i][j];

    switch (p) {
        case WTOUR:
        case BTOUR:
            highlight_possible_moves_rook(board, mask, i, j);
            return true;
        case WFOU:
        case BFOU:
            highlight_possible_moves_bishop(board, mask, i, j);
            return true;
        case WREINE:
        case BREINE:
            highlight_possible_moves_queen(board, mask, i, j);
            return true;
        case WROY:
        case BROY:
            highlight_possible_moves_king(board, mask, i, j);
            return true;
        case WCAVALIER:
        case BCAVALIER:
            highlight_possible_moves_knight(board, mask, i, j);
            return true;
        case WPION:
        case BPION:
            highlight_possible_moves_pawn(board, mask, i, j);
            return true;
        default:
            clear_mask(mask);
            return false;
    }
}

int main() {
    Board board;
    Mask mask;
    bool whiteTurn = true;

    start(board);
    empty_mask(mask);

    while (true) {
        highlight_movable_pieces(board, mask, whiteTurn);

        cout << "\n================================================" << endl;
        cout << "TOUR: " << (whiteTurn ? "BLANC (pieces blanches)" : "NOIR (pieces noires)") << endl;
        cout << "================================================" << endl;
        cout << endl;
        print_board(board, mask);

        bool validMove = false;
        while (!validMove) {
            cout << "\n--- Choisissez la piece a deplacer ---" << endl;
            cout << "Format: j1 i1" << endl;
            cout << "  j1 = colonne depart (0-7, 0=a, 1=b, ..., 7=h)" << endl;
            cout << "  i1 = ligne depart (0-7, 0=bas noir, 7=haut blanc)" << endl;
            cout << "Ou tapez q pour quitter." << endl;
            cout << "\nPiece > ";

            string cmd;
            if (!(cin >> cmd) || cmd == "q" || cmd == "Q") {
                cout << "Fin de partie." << endl;
                return 0;
            }

            try {
                int j1 = abc_to_int(cmd[0]);
                int i1;
                if (!(cin >> i1)) {
                    cout << "[ERREUR] Format invalide. Essayez: 0 6" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }

                if (!in_bounds(i1, j1)) {
                    cout << "[ERREUR] Coordonnees hors limite (0-7)." << endl;
                    continue;
                }

                Piece p = get_square(board, i1, j1);
                if (p == EMPTY) {
                    cout << "[ERREUR] Case de depart vide." << endl;
                    continue;
                }

                if ((whiteTurn && !is_white(p)) || (!whiteTurn && !is_black(p))) {
                    cout << "[ERREUR] Vous devez jouer votre propre piece." << endl;
                    continue;
                }

                bool highlighted = highlight_possible_moves(board, mask, i1, j1);
                if (highlighted) {
                    set_mask(mask, i1, j1, 3);
                    cout << "\nCoups possibles pour la piece selectionnee :" << endl;
                    print_board(board, mask);
                } else {
                    cout << "\nAucun highlight specifique pour cette piece." << endl;
                }

                cout << "\n--- Entrez la destination ---" << endl;
                cout << "Format: j2 i2" << endl;
                cout << "Ou tapez q pour quitter." << endl;
                cout << "\nDestination > ";

                if (!(cin >> cmd) || cmd == "q" || cmd == "Q") {
                    cout << "Fin de partie." << endl;
                    return 0;
                }

                int j2 = abc_to_int(cmd[0]);;
                int i2;
                if (!(cin >> i2)) {
                    cout << "[ERREUR] Format invalide. Essayez: 0 5" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }

                if (!in_bounds(i2, j2)) {
                    cout << "[ERREUR] Coordonnees hors limite (0-7)." << endl;
                    continue;
                }

                if (i1 == i2 && j1 == j2) {
                    cout << "[ERREUR] Le deplacement doit etre different de la position actuelle." << endl;
                    continue;
                }

                if (highlighted && get_mask(mask, i2, j2) == 0) {
                    cout << "[ERREUR] Destination non autorisee pour cette piece." << endl;
                    continue;
                }

                move_piece(board, i1, j1, i2, j2);
                validMove = true;
                cout << "[OK] Coup accepte !" << endl;
                whiteTurn = !whiteTurn;

            } catch (const exception &e) {
                cout << "[ERREUR] Exception: " << e.what() << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    }

    return 0;
}
