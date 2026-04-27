#include <iostream>
#include <limits>

#include "hpp/board.hpp"
#include "hpp/view.hpp"
#include "hpp/mask.hpp"
#include "hpp/outils.hpp"

using namespace std;

/** @brief Affiche les differents masques consultables avant un coup. */
static void mask_choices_menu() {
    cout << "\n--- Menu de visualisation ---" << endl;
    cout << "1. Deplacements disponibles de vos pieces" << endl;
    cout << "2. Pieces adverses que vous pouvez prendre" << endl;
    cout << "3. Vos pieces qui peuvent etre prises" << endl;
    cout << "0. Continuer (jouer un coup)" << endl;
    cout << "Choix > ";
}

/** @brief Permet au joueur d'afficher autant de masques qu'il le souhaite avant de jouer. */
static void mask_choices(const Board &board, bool whiteTurn) {
    while (true) {
        mask_choices_menu();

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "[ERREUR] Choix invalide." << endl;
            continue;
        }

        if (choice == 0) {
            return;
        }

        Mask visualMask;
        clear_mask(visualMask);

        switch (choice) {
            case 1:
                highlight_movable_pieces(board, visualMask, whiteTurn);
                cout << "\n[INFO] Deplacements disponibles de vos pieces :" << endl;
                print_board(board, visualMask);
                break;
            case 2:
                highlight_attacked_pieces(board, visualMask, whiteTurn);
                cout << "\n[INFO] Pieces adverses que vous pouvez prendre :" << endl;
                print_board(board, visualMask);
                break;
            case 3:
                highlight_attacked_pieces(board, visualMask, !whiteTurn);
                cout << "\n[INFO] Vos pieces qui peuvent etre prises :" << endl;
                print_board(board, visualMask);
                break;
            default:
                cout << "[ERREUR] Choix invalide." << endl;
                break;
        }
    }
}

/** @brief Point d'entree du programme interactif.
 *  @return Code de sortie du programme. */
int one_loop() {
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
        mask_choices(board, whiteTurn);
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
                    set_mask(mask, i1, j1, MASK_BROWN);
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
                
                
                bool currentWhiteTurn = whiteTurn;


                if (!king_present(board, !currentWhiteTurn)) {
                cout << "[FIN] Le roi " << (currentWhiteTurn ? "NOIR" : "BLANC") << " a ete capture. Le jeu s'arrete." << endl;
                return;
                }
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


bool king_present(const Board &board, bool whiteKing) {
    Piece king = whiteKing ? WROY : BROY;
    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            if(board[i][j] == king) {
                return true;
            }
        }
    }
    return false;
}