#include "piece.h"
#include "debug.h"
#include <string>
#include <stdexcept>

Player player_from_char(char c) {
    if (c == 'w') {
        return Player::White;
    } else if (c == 'b') {
        return Player::Black;
    }
    throw std::runtime_error("invalid player char: " + std::string(1, c));
}

char player_to_char(Player p) {
    if (p == Player::White) {
        return 'w';
    } else {
        return 'b';
    }
}

PieceType piece_type_from_char(char c) {
    if (c == 'p') {
        return PieceType::Pawn;
    } else if (c == 'n') {
        return PieceType::Knight;
    } else if (c == 'b') {
        return PieceType::Bishop;
    } else if (c == 'r') {
        return PieceType::Rook;
    } else if (c == 'q') {
        return PieceType::Queen;
    } else if (c == 'k') {
        return PieceType::King;
    }
    throw std::runtime_error("invalid piece type char: " + std::string(1, c));
}

char piece_type_to_char(PieceType p) {
    if (p == PieceType::Pawn) {
        return 'p';
    } else if (p == PieceType::Knight) {
        return 'n';
    } else if (p == PieceType::Bishop) {
        return 'b';
    } else if (p == PieceType::Rook) {
        return 'r';
    } else if (p == PieceType::Queen) {
        return 'q';
    } else {
        return 'k';
    }
}

Piece::Piece(Player player) : player(player) {
}

std::unique_ptr<Piece> Piece::clone() {
    return new_piece(piece_type(), player);
}

Piece::~Piece() {
}

void gen_jumper_moves(std::vector<Move> &moves, std::vector<Point> &dirs,
                      Board &board, Point from) {
    for (auto dir : dirs) {
        Point to = from + dir;
        if (to.in_bounds() && (board.get(to) == nullptr ||
                               board.get(to)->player != board.to_move)) {
            moves.push_back(Move(from, to));
        }
    }
}

void gen_rider_moves(std::vector<Move> &moves, std::vector<Point> &dirs,
                     Board &board, Point from) {
    for (auto d : dirs) {
        Point to = from + d;
        while (true) {
            if (!to.in_bounds()) {
                break;
            } else if (board.get(to) == nullptr) {
                moves.push_back(Move(from, to));
            } else if (board.get(to)->player != board.get(from)->player) {
                moves.push_back(Move(from, to));
                break;
            } else {
                break;
            }
            to = to + d;
        }
    }
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
    throw std::runtime_error("this shouldn't happen");
}

std::unique_ptr<Piece> piece_from_char(char c) {
    if (c == 'P') {
        return new_piece(PieceType::Pawn, Player::White);
    } else if (c == 'N') {
        return new_piece(PieceType::Knight, Player::White);
    } else if (c == 'B') {
        return new_piece(PieceType::Bishop, Player::White);
    } else if (c == 'R') {
        return new_piece(PieceType::Rook, Player::White);
    } else if (c == 'Q') {
        return new_piece(PieceType::Queen, Player::White);
    } else if (c == 'K') {
        return new_piece(PieceType::King, Player::White);
    } else if (c == 'p') {
        return new_piece(PieceType::Pawn, Player::Black);
    } else if (c == 'n') {
        return new_piece(PieceType::Knight, Player::Black);
    } else if (c == 'b') {
        return new_piece(PieceType::Bishop, Player::Black);
    } else if (c == 'r') {
        return new_piece(PieceType::Rook, Player::Black);
    } else if (c == 'q') {
        return new_piece(PieceType::Queen, Player::Black);
    } else if (c == 'k') {
        return new_piece(PieceType::King, Player::Black);
    }
    throw std::runtime_error("invalid piece char: " + std::string(1, c));
}

Pawn::Pawn(Player player) : Piece(player) {
}

std::vector<Move> Pawn::possible_moves(Board &board, Point from) {
    if (board.get(from) == nullptr ||
        board.get(from)->piece_type() != PieceType::Pawn) {
        throw std::runtime_error("expected pawn at " + from.algebraic());
    }
    std::vector<Move> moves;
    bool white = player == Player::White;
    Point fwd = white ? Point(0, 1) : Point(0, -1);

    // Move one square forward
    Point m1 = from + fwd;
    if (m1.in_bounds() && board.get(m1) == nullptr) {
        // Check for pawn promotion
        if (m1.y == (white ? 7 : 0)) {
            moves.push_back(Move(from, m1, PieceType::Queen));
            moves.push_back(Move(from, m1, PieceType::Rook));
            moves.push_back(Move(from, m1, PieceType::Bishop));
            moves.push_back(Move(from, m1, PieceType::Knight));
        } else {
            moves.push_back(Move(from, m1));
        }
        // Move two squares forward
        Point m2 = m1 + fwd;
        if (from.y == (white ? 1 : 6) && m2.in_bounds() &&
            board.get(m2) == nullptr) {
            moves.push_back(Move(from, m2));
        }
    }

    // Capture moves (including en passent)
    Point cl = from + fwd + Point(-1, 0);
    bool cl_possible =
        cl.in_bounds() && ((board.get(cl) != nullptr &&
                            board.get(cl)->player != player) ||
                           cl == board.en_passent_square);
    if (cl_possible) {
        if (m1.y == (white ? 7 : 0)) {
            moves.push_back(Move(from, cl, PieceType::Queen));
            moves.push_back(Move(from, cl, PieceType::Rook));
            moves.push_back(Move(from, cl, PieceType::Bishop));
            moves.push_back(Move(from, cl, PieceType::Knight));
        } else {
            moves.push_back(Move(from, cl));
        }
    }
    Point cr = from + fwd + Point(1, 0);
    bool cr_possible =
        cr.in_bounds() && ((board.get(cr) != nullptr &&
                            board.get(cr)->player != player) ||
                           cr == board.en_passent_square);
    if (cr_possible) {
        if (m1.y == (white ? 7 : 0)) {
            moves.push_back(Move(from, cr, PieceType::Queen));
            moves.push_back(Move(from, cr, PieceType::Rook));
            moves.push_back(Move(from, cr, PieceType::Bishop));
            moves.push_back(Move(from, cr, PieceType::Knight));
        } else {
            moves.push_back(Move(from, cr));
        }
    }
    return moves;
}

PieceType Pawn::piece_type() {
    return PieceType::Pawn;
}

char Pawn::to_char() {
    if (player == Player::White) {
        return 'P';
    } else {
        return 'p';
    }
}

Knight::Knight(Player player) : Piece(player) {
}

std::vector<Move> Knight::possible_moves(Board &board, Point from) {
    if (board.get(from) == nullptr ||
        board.get(from)->piece_type() != PieceType::Knight) {
        throw std::runtime_error("expected knight at " + from.algebraic());
    }
    static std::vector<Point> dirs = {
        Point(1, 2), Point(1, -2), Point(-1, 2), Point(-1, -2),
        Point(2, 1), Point(2, -1), Point(-2, 1), Point(-2, -1),
    };
    std::vector<Move> moves;
    gen_jumper_moves(moves, dirs, board, from);
    return moves;
}

PieceType Knight::piece_type() {
    return PieceType::Knight;
}

char Knight::to_char() {
    if (player == Player::White) {
        return 'N';
    } else {
        return 'n';
    }
}

Bishop::Bishop(Player player) : Piece(player) {
}

std::vector<Move> Bishop::possible_moves(Board &board, Point from) {
    if (board.get(from) == nullptr ||
        board.get(from)->piece_type() != PieceType::Bishop) {
        throw std::runtime_error("expected bishop at " + from.algebraic());
    }
    std::vector<Move> moves;
    static std::vector<Point> dirs = {
        Point(-1, -1),
        Point(-1, 1),
        Point(1, -1),
        Point(1, 1),
    };
    gen_rider_moves(moves, dirs, board, from);
    return moves;
}

PieceType Bishop::piece_type() {
    return PieceType::Bishop;
}

char Bishop::to_char() {
    if (player == Player::White) {
        return 'B';
    } else {
        return 'b';
    }
}

Rook::Rook(Player player) : Piece(player) {
}

std::vector<Move> Rook::possible_moves(Board &board, Point from) {
    if (board.get(from) == nullptr ||
        board.get(from)->piece_type() != PieceType::Rook) {
        throw std::runtime_error("expected rook at " + from.algebraic());
    }
    std::vector<Move> moves;
    static std::vector<Point> dirs = {
        Point(-1, 0),
        Point(0, -1),
        Point(0, 1),
        Point(1, 0),
    };
    gen_rider_moves(moves, dirs, board, from);
    return moves;
}

PieceType Rook::piece_type() {
    return PieceType::Rook;
}

char Rook::to_char() {
    if (player == Player::White) {
        return 'R';
    } else {
        return 'r';
    }
}

Queen::Queen(Player player) : Piece(player) {
}

std::vector<Move> Queen::possible_moves(Board &board, Point from) {
    if (board.get(from) == nullptr ||
        board.get(from)->piece_type() != PieceType::Queen) {
        throw std::runtime_error("expected queen at " + from.algebraic());
    }
    std::vector<Move> moves;
    static std::vector<Point> dirs = {
        Point(-1, -1), Point(0, -1), Point(1, -1), Point(-1, 0),
        Point(1, 0),   Point(-1, 1), Point(0, 1),  Point(1, 1),
    };
    gen_rider_moves(moves, dirs, board, from);
    return moves;
}

PieceType Queen::piece_type() {
    return PieceType::Queen;
}

char Queen::to_char() {
    if (player == Player::White) {
        return 'Q';
    } else {
        return 'q';
    }
}

King::King(Player player) : Piece(player) {
}

std::vector<Move> King::possible_moves(Board &board, Point from) {
    if (board.get(from) == nullptr ||
        board.get(from)->piece_type() != PieceType::King) {
        throw std::runtime_error("expected king at " + from.algebraic());
    }
    static std::vector<Point> dirs = {Point(-1, -1), Point(0, -1), Point(1, -1),
                                      Point(-1, 0),  Point(1, 0),  Point(-1, 1),
                                      Point(0, 1),   Point(1, 1)};

    std::vector<Move> moves;
    gen_jumper_moves(moves, dirs, board, from);

    // Castling moves
    bool white = board.to_move == Player::White;
    int br = white ? 0 : 7;
    bool can_castle_ks = board.can_castle[white ? 0 : 2];
    for (int i = 5; i <= 6; i++) {
        if (board.get(Point(i, br)) != nullptr) {
            can_castle_ks = false;
            break;
        }
    }
    if (can_castle_ks) {
        moves.push_back(Move(Point(4, br), Point(6, br)));
    }
    bool can_castle_qs = board.can_castle[white ? 1 : 3];
    for (int i = 1; i <= 3; i++) {
        if (board.get(Point(i, br)) != nullptr) {
            can_castle_qs = false;
            break;
        }
    }
    if (can_castle_qs) {
        moves.push_back(Move(Point(4, br), Point(2, br)));
    }

    return moves;
}

PieceType King::piece_type() {
    return PieceType::King;
}

char King::to_char() {
    if (player == Player::White) {
        return 'K';
    } else {
        return 'k';
    }
}
