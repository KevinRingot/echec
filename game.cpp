#include <ctime>
#include <iostream>
#include <limits>
#include <random>

#include "hpp/board.hpp"
#include "hpp/view.hpp"
#include "hpp/mask.hpp"
#include "hpp/outils.hpp"

using namespace std;

struct Move {
    int i1;
    int j1;
    int i2;
    int j2;
};

static void init_captured_pieces(CapturedPieces *captured) {
    captured->count = 0;
    for (int i = 0; i < MAX_CAPTURED_PIECES; i++) {
        captured->pieces[i] = EMPTY;
    }
}

void init_game(Game *game) {
    start(&game->board);
    init_captured_pieces(&game->captured_by_white);
    init_captured_pieces(&game->captured_by_black);
}

static void add_captured_piece(Game *game, Piece captured, bool whiteTurn) {
    if (captured == EMPTY) {
        return;
    }

    CapturedPieces *list = whiteTurn ? &game->captured_by_white : &game->captured_by_black;
    if (list->count < MAX_CAPTURED_PIECES) {
        list->pieces[list->count] = captured;
        list->count++;
    }
}

static bool king_present(const Board *board, bool whiteKing) {
    Piece king = whiteKing ? WROY : BROY;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if ((*board)[i][j] == king) {
                return true;
            }
        }
    }
    return false;
}

static void mask_choices_menu() {
    cout << "\n--- Menu de visualisation ---" << endl;
    cout << "1. Deplacements disponibles de vos pieces" << endl;
    cout << "2. Pieces adverses que vous pouvez prendre" << endl;
    cout << "3. Vos pieces qui peuvent etre prises" << endl;
    cout << "0. Continuer (jouer un coup)" << endl;
    cout << "Choix > ";
}

static void start_choice_menu() {
    cout << "\n--- Menu Principale ---" << endl;
    cout << "1. Nouvelle Partie" << endl;
    cout << "2. Reprendre une Partie" << endl;
    cout << "3. Quitter le jeux" << endl;
    cout << "Choix > ";
}

static void saves_choices_menu() {
    cout << "\n--- Listes des Sauvegardes ---" << endl;
}

static void players_choice_menu() {
    cout << "\n--- Menu de Mode ---" << endl;
    cout << "1. Joueur contre Joueur" << endl;
    cout << "2. Joueur contre Bot" << endl;
    cout << "3. Bot contre Bot" << endl;
    cout << "Choix > ";
}

static void mask_choices(const Board *board, bool whiteTurn) {
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
        clear_mask(&visualMask);

        switch (choice) {
            case 1:
                highlight_movable_pieces(board, &visualMask, whiteTurn);
                cout << "\n[INFO] Deplacements disponibles de vos pieces :" << endl;
                print_board(board, &visualMask);
                break;
            case 2:
                highlight_attacked_pieces(board, &visualMask, whiteTurn);
                cout << "\n[INFO] Pieces adverses que vous pouvez prendre :" << endl;
                print_board(board, &visualMask);
                break;
            case 3:
                highlight_attacked_pieces(board, &visualMask, !whiteTurn);
                cout << "\n[INFO] Vos pieces qui peuvent etre prises :" << endl;
                print_board(board, &visualMask);
                break;
            default:
                cout << "[ERREUR] Choix invalide." << endl;
                break;
        }
    }
}

static bool read_square(const string *label, int *i, int *j) {
    while (true) {
        cout << "\n--- Entrez " << *label << " ---" << endl;
        cout << "Format: colonne ligne" << endl;
        cout << "Exemple: a 3" << endl;
        cout << "Ou tapez q pour quitter." << endl;
        cout << "\n" << *label << " > ";

        string cmd;
        if (!(cin >> cmd) || cmd == "q" || cmd == "Q") {
            cout << "Fin de partie." << endl;
            return false;
        }

        try {
            *j = abc_to_int(cmd[0]);
        } catch (...) {
            cout << "[ERREUR] Colonne invalide. Utilisez une lettre entre a et h." << endl;
            continue;
        }

        if (!(cin >> *i)) {
            cout << "[ERREUR] Format invalide. Essayez par exemple: a 3" << endl;
            cin.clear();
            continue;
        }

        if (!in_bounds(*i, *j)) {
            cout << "[ERREUR] Coordonnees hors limite (0-7)." << endl;
            continue;
        }

        return true;
    }
}

static bool test_run(const Board *board, const Mask *mask, bool whiteTurn, int i1, int j1, int i2, int j2) {
    Piece p = (*board)[i1][j1];

    if (p == EMPTY) {
        cout << "[ERREUR] Case de depart vide." << endl;
        return false;
    }

    if ((whiteTurn && !is_white(p)) || (!whiteTurn && !is_black(p))) {
        cout << "[ERREUR] Vous devez jouer votre propre piece." << endl;
        return false;
    }

    if (i1 == i2 && j1 == j2) {
        cout << "[ERREUR] Le deplacement doit etre different de la position actuelle." << endl;
        return false;
    }

    if (get_mask(mask, i2, j2) == MASK_NONE) {
        cout << "[ERREUR] Destination non autorisee pour cette piece." << endl;
        return false;
    }

    return true;
}

static bool choose_mouvement_human(const Board *board, Mask *mask, bool whiteTurn, Move *move) {
    static const string labelDepart = "la case de depart";
    static const string labelArrivee = "la case d'arrivee";

    while (true) {
        if (!read_square(&labelDepart, &move->i1, &move->j1)) {
            return false;
        }

        Piece p = (*board)[move->i1][move->j1];
        if (p == EMPTY) {
            cout << "[ERREUR] Case de depart vide." << endl;
            continue;
        }

        if ((whiteTurn && !is_white(p)) || (!whiteTurn && !is_black(p))) {
            cout << "[ERREUR] Vous devez jouer votre propre piece." << endl;
            continue;
        }

        clear_mask(mask);
        bool highlighted = highlight_possible_moves(board, mask, move->i1, move->j1);
        if (!highlighted) {
            cout << "[ERREUR] Aucun coup possible pour cette piece." << endl;
            continue;
        }

        set_mask(mask, move->i1, move->j1, MASK_BROWN);
        cout << "\nCoups possibles pour la piece selectionnee :" << endl;
        print_board(board, mask);

        if (!read_square(&labelArrivee, &move->i2, &move->j2)) {
            return false;
        }

        if (test_run(board, mask, whiteTurn, move->i1, move->j1, move->i2, move->j2)) {
            return true;
        }
    }
}

static int random_index(int maxValue) {
    static mt19937 generator(static_cast<unsigned int>(time(nullptr)));
    uniform_int_distribution<int> distribution(0, maxValue - 1);
    return distribution(generator);
}

static bool choose_mouvement_computer(const Board *board, Mask *mask, bool whiteTurn, Move *move) {
    Move possibleMoves[256];
    int moveCount = 0;

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            Piece piece = (*board)[i][j];
            if (piece == EMPTY) {
                continue;
            }
            if ((whiteTurn && !is_white(piece)) || (!whiteTurn && !is_black(piece))) {
                continue;
            }

            Mask pieceMask;
            clear_mask(&pieceMask);
            if (!highlight_possible_moves(board, &pieceMask, i, j)) {
                continue;
            }

            for (int ii = 0; ii < BOARD_SIZE; ii++) {
                for (int jj = 0; jj < BOARD_SIZE; jj++) {
                    if (get_mask(&pieceMask, ii, jj) == MASK_NONE) {
                        continue;
                    }
                    possibleMoves[moveCount].i1 = i;
                    possibleMoves[moveCount].j1 = j;
                    possibleMoves[moveCount].i2 = ii;
                    possibleMoves[moveCount].j2 = jj;
                    moveCount++;
                }
            }
        }
    }

    if (moveCount == 0) {
        return false;
    }

    *move = possibleMoves[random_index(moveCount)];

    clear_mask(mask);
    highlight_possible_moves(board, mask, move->i1, move->j1);
    set_mask(mask, move->i1, move->j1, MASK_BROWN);

    cout << "\n[ORDINATEUR] Coup choisi : "
         << char('a' + move->j1) << " " << move->i1
         << " vers "
         << char('a' + move->j2) << " " << move->i2 << endl;

    return true;
}

static bool apply_move(Game *game, bool *whiteTurn, const Move *move) {
    Piece captured = game->board[move->i2][move->j2];
    add_captured_piece(game, captured, *whiteTurn);

    move_piece(&game->board, move->i1, move->j1, move->i2, move->j2);
    cout << "[OK] Coup accepte !" << endl;

    if (!king_present(&game->board, !(*whiteTurn))) {
        cout << "[FIN] Le roi " << (*whiteTurn ? "NOIR" : "BLANC") << " a ete capture. Le jeu s'arrete." << endl;
        return false;
    }

    *whiteTurn = !(*whiteTurn);
    return true;
}

static bool one_run_human(Game *game, Mask *mask, bool *whiteTurn) {
    highlight_movable_pieces(&game->board, mask, *whiteTurn);

    cout << "\n================================================" << endl;
    cout << "TOUR: " << (*whiteTurn ? "BLANC (pieces blanches)" : "NOIR (pieces noires)") << endl;
    cout << "================================================" << endl;
    cout << endl;

    print_board(&game->board, mask);
    mask_choices(&game->board, *whiteTurn);

    highlight_movable_pieces(&game->board, mask, *whiteTurn);
    print_board(&game->board, mask);

    Move move;
    if (!choose_mouvement_human(&game->board, mask, *whiteTurn, &move)) {
        return false;
    }

    return apply_move(game, whiteTurn, &move);
}

static bool one_run_computer(Game *game, Mask *mask, bool *whiteTurn) {
    highlight_movable_pieces(&game->board, mask, *whiteTurn);

    cout << "\n================================================" << endl;
    cout << "TOUR ORDINATEUR: " << (*whiteTurn ? "BLANC" : "NOIR") << endl;
    cout << "================================================" << endl;
    cout << endl;

    print_board(&game->board, mask);

    Move move;
    if (!choose_mouvement_computer(&game->board, mask, *whiteTurn, &move)) {
        cout << "[FIN] Aucun coup valide disponible pour l'ordinateur." << endl;
        return false;
    }

    print_board(&game->board, mask);
    return apply_move(game, whiteTurn, &move);
}

bool one_run(Game *game, Mask *mask, bool *whiteTurn, bool humanPlayer) {
    if (humanPlayer) {
        return one_run_human(game, mask, whiteTurn);
    }
    return one_run_computer(game, mask, whiteTurn);
}
