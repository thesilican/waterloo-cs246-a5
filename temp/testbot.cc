#include "testbot.h"

TestBot::TestBot() {
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

    char_form[0] = 'P';
    char_form[1] = 'N';
    char_form[2] = 'B';
    char_form[3] = 'R';
    char_form[4] = 'Q';
    char_form[5] = 'K';
    char_form[6] = 'p';
    char_form[7] = 'n';
    char_form[8] = 'b';
    char_form[9] = 'r';
    char_form[10] = 'q';
    char_form[11] = 'k';
}

int TestBot::get_move(char piece, Point from, Point to, char promote) {
    int pi = int_form[piece];
    int pr = int_form[promote];
    int m = pi << 16;
    m |= (from.x << 13);
    m |= (from.y << 10);
    m |= (to.x << 7);
    m |= (to.y << 4);
    m |= pr;
    return m;
}

char TestBot::get_promote(int move) {
    return char_form[move & ((1 << 4) - 1)];
}

char TestBot::get_piece(int move) {
    return char_form[move >> 16];
}

Point TestBot::get_from(int move) {
    int temp = move >> 13;
    int x = (temp & ((1 << 3) - 1));
    int temp2 = move >> 10;
    int y = (temp2 & ((1 << 3) - 1));
    return Point(x,y);
}

Point TestBot::get_to(int move) {
    int temp = move >> 7;
    int x = (temp & ((1 << 3) - 1));
    int temp2 = move >> 4;
    int y = (temp2 & ((1 << 3) - 1));
    return Point(x,y);
}
