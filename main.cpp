#include <iostream>
#include <limits>

#include "board.hpp"
#include "view.hpp"
#include "mask.hpp"

using namespace std;

static bool in_bounds(int i, int j) {
    return i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE;
}

int main() {
    Board board;
    Mask mask;
    bool whiteTurn = true;

    start(board);
    empty_mask(mask);

    while (true) {
        highlight_attacked_pieces(board, mask, whiteTurn);

        cout << "\n================================================" << endl;
        cout << "TOUR: " << (whiteTurn ? "BLANC (pieces blanches)" : "NOIR (pieces noires)") << endl;
        cout << "================================================" << endl;
        cout << endl;
        print_board(board, mask);

        bool validMove = false;
        while (!validMove) {
            cout << "\n--- Entrez votre coup ---" << endl;
            cout << "Format: j1 i1 j2 i2" << endl;
            cout << "  j1 = colonne depart (0-7, 0=a, 1=b, ..., 7=h)" << endl;
            cout << "  i1 = ligne depart (0-7, 0=bas noir, 7=haut blanc)" << endl;
            cout << "  j2 i2 = destination (colonne ligne)" << endl;
            cout << "Exemple: 0 6 0 5 = deplacer de colonne a, ligne 6 vers colonne a, ligne 5" << endl;
            cout << "Ou tapez q pour quitter." << endl;
            cout << "\nVotre coup > ";

            string cmd;
            if (!(cin >> cmd) || cmd == "q" || cmd == "Q") {
                cout << "Fin de partie." << endl;
                return 0;
            }

            try {
                int j1 = stoi(cmd);
                int i1, j2, i2;
                if (!(cin >> i1 >> j2 >> i2)) {
                    cout << "[ERREUR] Format invalide. Essayez: 0 6 0 5" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }

                if (!in_bounds(i1, j1) || !in_bounds(i2, j2)) {
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

                if (i1 == i2 && j1 == j2) {
                    cout << "[ERREUR] Le deplacement doit etre different de la position actuelle." << endl;
                    continue;
                }

                // Coup valide !
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
