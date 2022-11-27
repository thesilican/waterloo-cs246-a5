#include "zobrist.h"

Zobrist::Zobrist() {
    int_form['P'] = 0;
    int_form['N'] = 1;
    int_form['B'] = 2;
    int_form['R'] = 3;
    int_form['Q'] = 4;
    int_form['K'] = 5;
    int_form['p'] = 6;
    int_form['n'] = 7;
    int_form['b'] = 8;
    int_form['r'] = 9;
    int_form['q'] = 10;
    int_form['k'] = 11;
}

void Zobrist::init_table() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 12; k++) {
                zobrist_table[i][j][k] = dist(mt);
            }
        }
    }
    is_black = dist(mt);
    for (int i = 0; i < 4; i++) {
        can_castle[i] = dist(mt);
    }
    for (int i = 0; i < 8; i++) {
        en_passant[i] = dist(mt);
    }
}

unsigned long long int Zobrist::get_hash(char board[8][8], bool black, bool castle[4], bool can_en_passant[8]) {
    unsigned long long int h = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] == '*') continue;
            h ^= zobrist_table[i][j][int_form[board[i][j]]];
        }
    }
    if (black) {
        h ^= is_black;
    }
    for (int i = 0; i < 4; i++) {
        if (castle[i]) {
            h ^= can_castle[i];
        }
    }
    for (int i = 0; i < 8; i++) {
        if (can_en_passant[i]) {
            h ^= en_passant[i];
        }
    }
    return h;
}

//gets new hash when a piece needs to be added or removed, assumes position (i,j) in board is empty and piece != '*'
unsigned long long int Zobrist::update_piece(unsigned long long int h, int i, int j, char piece) {
    h ^= zobrist_table[i][j][int_form[piece]];
    return h;
}
