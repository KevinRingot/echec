#include <iostream>
#include <fstream>
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

string write_TAB(Board b){
    string* Tab = new string[100];
    int index = 0;
    int number = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (b[i][j] == '.') {
                number++;
            } else {
                if (number != 0) {
                    Tab[index] = to_string(number);
                    index++;
                    number = 0;
                }
                Tab[index] = b[i][j];
                index++;
            }
        }
        if (number != 0) {
            Tab[index] = to_string(number);
            index++;
            number = 0;
        }
        Tab[index] = '/';
        index++;
    }
    return Tab;
}

void write_Fen(char fich[256], Board b){
    string* tab = write_TAB(b);
    cout << tab << endl;
    ofstream fic(fich);
    if (fic) {
        int i = 0;
        while (i < 100) { // enregistrement des valeurs d’un tableau tab de 100  ́el ́ements
            // dans le fichier
            fic << tab[i] << " "; //  ́ecriture de l’ ́el ́ement i syntaxe similaire `a l’affichage
            i++;
        }
    } else {
    cout << "ERREUR : impossible d’ouvrir le fichier en sortie" << endl;
    }
    fic.close();
}