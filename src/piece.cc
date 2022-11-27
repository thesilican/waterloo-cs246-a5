#include "piece.h"
#include "debug.h"

std::ostream &operator<<(std::ostream &o, Player p) {
    if (p == Player::White) {
        o << "W";
    } else {
        o << "B";
    }
    return o;
}

std::ostream &operator<<(std::ostream &o, PieceType p) {
    if (p == PieceType::Pawn) {
        o << "P";
    } else if (p == PieceType::Knight) {
        o << "K";
    } else if (p == PieceType::Bishop) {
        o << "B";
    } else if (p == PieceType::Rook) {
        o << "R";
    } else if (p == PieceType::Queen) {
        o << "Q";
    } else if (p == PieceType::King) {
        o << "K";
    }
    return o;
}

Piece::Piece(Player player) : player(player) {
}

std::unique_ptr<Piece> Piece::clone() {
    return new_piece(piece_type(), player);
}

Piece::~Piece() {
}

std::unique_ptr<Piece> new_piece(PieceType type, Player player) {
    switch (type) {
    case PieceType::Pawn:
        return std::unique_ptr<Piece>(new Pawn(player));
    case PieceType::Knight:
        return std::unique_ptr<Piece>(new Knight(player));
    case PieceType::Bishop:
        return std::unique_ptr<Piece>(new Bishop(player));
    case PieceType::Rook:
        return std::unique_ptr<Piece>(new Rook(player));
    case PieceType::Queen:
        return std::unique_ptr<Piece>(new Queen(player));
    case PieceType::King:
        return std::unique_ptr<Piece>(new King(player));
    }
}

Pawn::Pawn(Player player) : Piece(player) {
}

std::vector<Move> Pawn::possible_moves(Board &board, Point from) {
    if (board.get(from) == nullptr ||
        board.get(from)->piece_type() != PieceType::Pawn) {
        throw std::runtime_error("expected pawn at " + from.algebraic());
    }
    std::vector<Move> moves;
    Point fwd;
    if (player == Player::White) {
        fwd = Point(0, 1);
    } else {
        fwd = Point(0, -1);
    }

    // Move one square forward
    Point m1 = from + fwd;
    if (m1.in_bounds() && board.get(m1) == nullptr) {
        // Check for pawn promotion
        int last_row = player == Player::White ? 7 : 0;
        if (m1.y == last_row) {
            moves.push_back(Move(piece_type(), from, m1, PieceType::Queen));
            moves.push_back(Move(piece_type(), from, m1, PieceType::Rook));
            moves.push_back(Move(piece_type(), from, m1, PieceType::Bishop));
            moves.push_back(Move(piece_type(), from, m1, PieceType::Knight));
        } else {
            moves.push_back(Move(piece_type(), from, m1));
        }
        // Move two squares forward
        Point m2 = m1 + fwd;
        if (m2.in_bounds() && board.get(m2) == nullptr) {
            moves.push_back(Move(piece_type(), from, m2));
        }
    }

    // Capture moves
    Point cl = from + fwd + Point(-1, 0);
    if (cl.in_bounds() && board.get(cl) != nullptr &&
        board.get(cl)->player != board.get(cl)->player) {
        moves.push_back(Move(piece_type(), from, cl));
    }
    Point cr = from + fwd + Point(1, 0);
    if (cr.in_bounds() && board.get(cr) != nullptr &&
        board.get(cr)->player != board.get(cr)->player) {
        moves.push_back(Move(piece_type(), from, cr));
    }
    return moves;
}

PieceType Pawn::piece_type() {
    return PieceType::Pawn;
}

Knight::Knight(Player player) : Piece(player) {
}

std::vector<Move> Knight::possible_moves(Board &board, Point from) {
    std::vector<Move> moves;
    // TODO: implement piece movements
    return moves;
}

PieceType Knight::piece_type() {
    return PieceType::Knight;
}

Bishop::Bishop(Player player) : Piece(player) {
}

std::vector<Move> Bishop::possible_moves(Board &board, Point from) {
    std::vector<Move> moves;
    // TODO: implement piece movements
    return moves;
}

PieceType Bishop::piece_type() {
    return PieceType::Bishop;
}

Rook::Rook(Player player) : Piece(player) {
}

std::vector<Move> Rook::possible_moves(Board &board, Point from) {
    std::vector<Move> moves;
    // TODO: implement piece movements
    return moves;
}

PieceType Rook::piece_type() {
    return PieceType::Rook;
}

Queen::Queen(Player player) : Piece(player) {
}

std::vector<Move> Queen::possible_moves(Board &board, Point from) {
    std::vector<Move> moves;
    // TODO: implement piece movements
    return moves;
}

PieceType Queen::piece_type() {
    return PieceType::Queen;
}

King::King(Player player) : Piece(player) {
}

std::vector<Move> King::possible_moves(Board &board, Point from) {
    std::vector<Move> moves;
    // TODO: implement piece movements
    return moves;
}

PieceType King::piece_type() {
    return PieceType::King;
}
