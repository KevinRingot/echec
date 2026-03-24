#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#include "view.hpp"
#include "board.hpp"  



void print_square(Board &board, int i, int j) {
    cout << board[i][j] << " ";
}

void print_board(Board &board) {

    cout << "  ";
    for (int j = 0; j < 8; j++)
    {
        cout << char(97 + j) << " ";
    }
    cout << endl;
    for(int i=0;i<8;i++) {
        cout << i << " ";
        for(int j=0;j<8;j++) {
            cout << piece_to_char(board[i][j]) << " ";
        }
        cout << endl;
    }
}




string write_tab(Board Board) {
    string fen = "";

    for (int r = 0; r < 8; ++r) {
        int empty_count = 0;

        for (int c = 0; c < 8; ++c) {
            Piece code = Board[r][c];
            char piece = piece_to_char(code);

            if (piece == '.') {
                empty_count++;
            } else {
                if (empty_count > 0) {
                    fen += to_string(empty_count);
                    empty_count = 0;
                }
                fen += piece;
            }
        }

        if (empty_count > 0)
            fen += std::to_string(empty_count);

        if (r != 7)
            fen += "/";
    }

    return fen;
}




void write_fen(Board Board, const string &filename) {
    string fen = write_tab(Board);
    ofstream file(filename);
    if (!file) {
        cout << "Erreur : impossible d'ouvrir le fichier " << filename << "\n";
        return;
    }
    file << fen << "\n";

}