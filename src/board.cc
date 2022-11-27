#include "board.h"
#include "debug.h"
#include "piece.h"
#include "point.h"
#include <stdexcept>

Board::Board() : to_move(Player::White), en_passent_square(Point(-1, -1)) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Point p(i, j);
            if (p == Point(0, 0) || p == Point(7, 0)) {
                pieces[i][j] = new_piece(PieceType::Rook, Player::White);
            } else if (p == Point(1, 0) || p == Point(6, 0)) {
                pieces[i][j] = new_piece(PieceType::Knight, Player::White);
            } else if (p == Point(2, 0) || p == Point(5, 0)) {
                pieces[i][j] = new_piece(PieceType::Bishop, Player::White);
            } else if (p == Point(3, 0)) {
                pieces[i][j] = new_piece(PieceType::Queen, Player::White);
            } else if (p == Point(4, 0)) {
                pieces[i][j] = new_piece(PieceType::King, Player::White);
            } else if (j == 1) {
                pieces[i][j] = new_piece(PieceType::Pawn, Player::White);
            } else if (p == Point(0, 7) || p == Point(7, 7)) {
                pieces[i][j] = new_piece(PieceType::Rook, Player::Black);
            } else if (p == Point(1, 7) || p == Point(6, 7)) {
                pieces[i][j] = new_piece(PieceType::Knight, Player::Black);
            } else if (p == Point(2, 7) || p == Point(5, 7)) {
                pieces[i][j] = new_piece(PieceType::Bishop, Player::Black);
            } else if (p == Point(3, 7)) {
                pieces[i][j] = new_piece(PieceType::Queen, Player::Black);
            } else if (p == Point(4, 7)) {
                pieces[i][j] = new_piece(PieceType::King, Player::Black);
            } else if (j == 6) {
                pieces[i][j] = new_piece(PieceType::Pawn, Player::Black);
            } else {
                pieces[i][j] = nullptr;
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        can_castle[i] = true;
    }
}

Board::Board(std::unique_ptr<Piece> pieces[8][8], Point en_passent_square,
             bool can_castle[4], Player to_move)
    : to_move(to_move), en_passent_square(en_passent_square) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (pieces[i][j] == nullptr) {
                this->pieces[i][j] = nullptr;
            } else {
                this->pieces[i][j] = pieces[i][j]->clone();
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        this->can_castle[i] = can_castle[i];
    }
}

std::unique_ptr<Piece> &Board::get(Point p) {
    return pieces[p.x][p.y];
}

void Board::set(Point p, PieceType type, Player player) {
    pieces[p.x][p.y] = new_piece(type, player);
}

std::vector<Move> Board::legal_moves() {
    std::vector<Move> moves;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Point from(i, j);
            std::unique_ptr<Piece> &piece = get(from);
            if (piece == nullptr || piece->player != to_move)
                continue;
            std::vector<Move> piece_moves = piece->possible_moves(*this, from);
            // TODO: Remove moves that give check
            for (auto m : piece_moves) {
                moves.push_back(m);
            }
        }
    }
    return moves;
}

bool Board::in_check() {
    throw std::runtime_error("not implemented");
}

bool Board::is_checkmate() {
    return legal_moves().size() == 0 && in_check();
}

bool Board::is_stalemate() {
    return legal_moves().size() == 0 && !in_check();
}

void Board::make_move(Move m) {
    std::vector<Move> moves = legal_moves();
    bool found = false;
    for (auto move : moves) {
        if (move == m) {
            found = true;
            break;
        }
    }
    if (!found) {
        throw std::runtime_error("invalid move");
    }

    get(m.to) = std::move(get(m.from));
    get(m.from) = nullptr;

    // Handle piece promotion
    int last_rank = to_move == Player::White ? 7 : 0;
    if (get(m.to)->piece_type() == PieceType::Pawn && m.to.y == last_rank) {
        get(m.to) = new_piece(m.promotes_to, to_move);
    }

    // TODO: handle castling and en passent

    if (to_move == Player::White) {
        to_move = Player::Black;
    } else {
        to_move = Player::White;
    }
}

Board Board::clone() {
    return Board(pieces, en_passent_square, can_castle, to_move);
}