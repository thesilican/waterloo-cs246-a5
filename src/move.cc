#include "board.h"
#include "move.h"
#include "debug.h"
#include <stdexcept>

Move::Move(Point from, Point to) : from(from), to(to), has_promotes_to(false) {
}

Move::Move(Point from, Point to, PieceType promotes_to)
    : from(from), to(to), has_promotes_to(true), promotes_to(promotes_to) {
}

Move::Move(std::string uci) {
    if (uci.size() < 4 || uci.size() > 5) {
        throw std::runtime_error("invalid uci: " + uci);
    }
    from = Point(uci.substr(0, 2));
    to = Point(uci.substr(2, 4));
    if (uci.size() == 5) {
        has_promotes_to = true;
        promotes_to = piece_type_from_char(uci[4]);
    } else {
        has_promotes_to = false;
    }
}

bool operator==(Move a, Move b) {
    return a.from == b.from && a.to == b.to &&
           a.has_promotes_to == b.has_promotes_to &&
           (!a.has_promotes_to || a.promotes_to == b.promotes_to);
}

bool operator!=(Move a, Move b) {
    return !(a == b);
}

std::string Move::uci() {
    std::string result;
    result += from.algebraic();
    result += to.algebraic();
    if (has_promotes_to) {
        result += piece_type_to_char(promotes_to);
    }
    return result;
}

std::unique_ptr<Piece> &Move::piece(Board &b) {
    return b.get(from);
}

std::unique_ptr<Piece> &Move::captured(Board &b) {
    return b.get(to);
}