#include "testbot.h"
#include <iostream>

int main() {
    while (true) {
        std::cout << "Enter move info: ";
        char piece, promote;
        int fromx, fromy, tox, toy;
        std::cin >> piece >> fromx >> fromy >> tox >> toy >> promote;
        std::cout << '\n';
        TestBot b;
        int m = b.get_move(piece, Point(fromx, fromy), Point(tox, toy), promote);
        std::cout << m << '\n';
        std::cout << b.get_piece(m) << " " << b.get_from(m).x << " " << b.get_from(m).y << " " << b.get_to(m).x << " " << b.get_to(m).y << " " << b.get_promote(m) << '\n';
    }
}
