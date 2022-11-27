#include "move.h"
#include "debug.h"

Move::Move(PieceType piece, Point from, Point to)
    : piece(piece), from(from), to(to), promotes_to(PieceType::Queen) {
}

Move::Move(PieceType piece, Point from, Point to, PieceType promotes_to)
    : piece(piece), from(from), to(to), promotes_to(promotes_to) {
}

bool operator==(Move a, Move b) {
    return a.from == b.from && a.to == b.to && a.piece == b.piece &&
           a.promotes_to == b.promotes_to;
}

bool operator!=(Move a, Move b) {
    return !(a == b);
}

std::ostream &operator<<(std::ostream &o, Move m) {
    o << m.piece << m.from << m.to;
    if ((m.to.y == 0 || m.to.y == 7) && m.piece == PieceType::Pawn) {
        o << "(" << m.promotes_to << ")";
    }
    return o;
}