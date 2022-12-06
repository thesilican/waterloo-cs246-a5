#include "move.h"
#include "board.h"
#include "debug.h"
#include <regex>
#include <stdexcept>

Move::Move() : from(), to(), has_promotes_to(false) {
}

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

Move::Move(std::string san, Board &board) {
    static std::regex san_regex =
        std::regex("^([0O]-[0O](?:-[0O])?)|([nbrqkNBRQK]?)([a-h]?)([1-8]?)(x?)("
                   "[a-h][1-8])(?:=?([nbrqNBRQ]))?$");
    std::smatch result;
    if (!std::regex_match(san, result, san_regex)) {
        throw std::runtime_error("invalid san: " + san);
    }
    auto iter = ++result.begin();
    std::string castle_str = *(iter++);
    std::string piece_str = *(iter++);
    std::string dis_file_str = *(iter++);
    std::string dis_rank_str = *(iter++);
    std::string capture_str = *(iter++);
    std::string to_str = *(iter++);
    std::string promote_str = *(iter++);

    // Handle castling moves
    if (castle_str != "") {
        if (castle_str.size() == 3) {
            if (board.to_move == Player::White) {
                *this = Move(Point("e1"), Point("g1"));
            } else {
                *this = Move(Point("e8"), Point("g8"));
            }
        } else if (castle_str.size() == 5) {
            if (board.to_move == Player::White) {
                *this = Move(Point("e1"), Point("c1"));
            } else {
                *this = Move(Point("e8"), Point("c8"));
            }
        }
        bool found = false;
        for (auto m : board.legal_moves()) {
            if (m == *this) {
                found = true;
                return;
            }
        }
        if (!found) {
            throw std::runtime_error("invalid castling san move: " + san);
        }
    }

    // Handle regular moves
    Point to(to_str);
    PieceType piece = piece_type_from_char(std::tolower(piece_str[0]));
    int dis_file = -1, dis_rank = -1;
    if (dis_file_str != "") {
        dis_file = dis_file_str[0] - 'a';
    }
    if (dis_rank_str != "") {
        dis_rank = dis_rank_str[0] - '1';
    }
    bool capture = capture_str == "x";
    bool has_promote = promote_str != "";
    PieceType promote;
    if (has_promote) {
        promote = piece_type_from_char(std::tolower(promote_str[0]));
    }
    std::vector<Move> matched_moves;
    for (auto m : board.legal_moves()) {
        bool matches = m.to == to && (dis_file == -1 || dis_file == m.from.x) &&
                       (dis_rank == -1 || dis_rank == m.from.y) &&
                       (!capture || m.captured(board) != nullptr) &&
                       has_promote == m.has_promotes_to &&
                       (!m.has_promotes_to || m.promotes_to == promote);
        if (matches) {
            matched_moves.push_back(m);
        }
    }
    if (matched_moves.size() == 0) {
        throw std::runtime_error("no moves matched san: " + san);
    } else if (matched_moves.size() >= 2) {
        throw std::runtime_error("too many moves matched san: " + san);
    } else {
        *this = matched_moves[0];
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