#include "board.h"
#include "debug.h"
#include "piece.h"
#include "point.h"
#include <ctype.h>
#include <regex>
#include <sstream>
#include <stdexcept>

Board::Board()
    : Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {
}

Board::Board(std::unique_ptr<Piece> pieces[8][8], Player to_move,
             bool can_castle[4], Point en_passent_square, int half_move,
             int full_move)
    : to_move(to_move), en_passent_square(en_passent_square),
      half_move(half_move), full_move(full_move) {
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

std::vector<Move> Board::possible_moves() {
    std::vector<Move> moves;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Point from(i, j);
            std::unique_ptr<Piece> &piece = get(from);
            if (piece == nullptr || piece->player != to_move)
                continue;
            std::vector<Move> piece_moves = piece->possible_moves(*this, from);
            for (auto m : piece_moves) {
                moves.push_back(m);
            }
        }
    }
    return moves;
}

std::vector<Move> Board::legal_moves() {
    bool white = to_move == Player::White;
    std::vector<Move> legal_moves;
    for (auto move : possible_moves()) {
        bool legal = true;

        // Check for checks
        Board board = this->clone();
        board.make_move(move);
        std::vector<Move> child_moves = board.possible_moves();
        for (auto cmove : child_moves) {
            std::unique_ptr<Piece> &p = board.get(cmove.to);
            if (p != nullptr && p->piece_type() == PieceType::King &&
                p->player == to_move) {
                legal = false;
                break;
            }
        }

        // Check for castling in/through check
        int br = white ? 0 : 7;
        if (move.from == Point(4, br) && get(move.from) != nullptr &&
            get(move.from)->piece_type() == PieceType::King &&
            move.to.y == br && (move.to.x == 2 || move.to.x == 6)) {

            Board board = this->clone();
            Point middle;
            if (move.to.x == 6) {
                middle = Point(5, br);
            } else if (move.to.x == 2) {
                middle = Point(3, br);
            }
            board.make_move(Move(move.from, middle));
            std::vector<Move> child_moves = board.possible_moves();
            for (auto cmove : child_moves) {
                if (cmove.to == middle || cmove.to == Point(4, br)) {
                    legal = false;
                    break;
                }
            }
        }

        if (legal) {
            legal_moves.push_back(move);
        }
    }
    return legal_moves;
}

bool Board::is_check() {
    Board board = this->clone();
    board.en_passent_square = Point(-1, -1);
    board.to_move = to_move == Player::White ? Player::Black : Player::White;
    std::vector<Move> child_moves = board.possible_moves();
    for (auto cmove : child_moves) {
        std::unique_ptr<Piece> &p = board.get(cmove.to);
        if (p != nullptr && p->piece_type() == PieceType::King &&
            p->player == to_move) {
            return true;
        }
    }
    return false;
}

bool Board::is_checkmate() {
    return legal_moves().size() == 0 && is_check();
}

bool Board::is_stalemate() {
    return legal_moves().size() == 0 && !is_check();
}

void Board::make_move(Move m) {
    if (get(m.from) == nullptr) {
        throw std::runtime_error("expected non-empty square at " +
                                 m.from.algebraic());
    }
    PieceType piece = get(m.from)->piece_type();
    Player player = get(m.from)->player;
    bool white = player == Player::White;
    std::unique_ptr<Piece> captured_piece =
        (get(m.to) == nullptr) ? nullptr : get(m.to)->clone();
    get(m.to) = std::move(get(m.from));
    get(m.from) = nullptr;

    // Handle piece promotion
    if (piece == PieceType::Pawn && m.to.y == (white ? 7 : 0)) {
        get(m.to) = new_piece(m.promotes_to, player);
    }

    // Handle en passent capture
    if (piece == PieceType::Pawn && m.to == en_passent_square) {
        Point behind = white ? Point(0, -1) : Point(0, 1);
        get(m.to + behind) = nullptr;
    }

    // Handle castling
    if (piece == PieceType::King && m.from.x == 4) {
        if (m.to.x == 6) {
            // King side
            get(Point(5, m.from.y)) = new_piece(PieceType::Rook, player);
            get(Point(7, m.from.y)) = nullptr;
        } else if (m.to.x == 2) {
            // Queen side
            get(Point(0, m.from.y)) = nullptr;
            get(Point(3, m.from.y)) = new_piece(PieceType::Rook, player);
        }
    }

    // Update en passent square
    if (piece == PieceType::Pawn &&
        ((white && m.from.y == 1 && m.to.y == 3) ||
         (!white && m.from.y == 6 && m.to.y == 4))) {
        Point ep = Point(m.from.x, white ? 3 : 4);
        Point l = ep + Point(1, 0);
        Point r = ep + Point(-1, 0);
        if ((l.in_bounds() && get(l) != nullptr && get(l)->player != to_move) ||
            (r.in_bounds() && get(r) != nullptr && get(r)->player != to_move)) {
            en_passent_square = ep;
        } else {
            en_passent_square = Point(-1, -1);
        }
    } else {
        en_passent_square = Point(-1, -1);
    }

    // Update can castle
    std::unique_ptr<Piece> &wrq = get(Point(0, 0));
    std::unique_ptr<Piece> &wk = get(Point(4, 0));
    std::unique_ptr<Piece> &wrk = get(Point(7, 0));
    std::unique_ptr<Piece> &brq = get(Point(0, 7));
    std::unique_ptr<Piece> &bk = get(Point(4, 7));
    std::unique_ptr<Piece> &brk = get(Point(7, 7));
    if (wrk == nullptr || wrk->piece_type() != PieceType::Rook ||
        wrk->player != Player::White) {
        can_castle[0] = false;
    }
    if (wrq == nullptr || wrq->piece_type() != PieceType::Rook ||
        wrq->player != Player::White) {
        can_castle[1] = false;
    }
    if (wk == nullptr || wk->piece_type() != PieceType::King ||
        wk->player != Player::White) {
        can_castle[0] = false;
        can_castle[1] = false;
    }
    if (brk == nullptr || brk->piece_type() != PieceType::Rook ||
        brk->player != Player::Black) {
        can_castle[2] = false;
    }
    if (brq == nullptr || brq->piece_type() != PieceType::Rook ||
        brq->player != Player::Black) {
        can_castle[3] = false;
    }
    if (bk == nullptr || bk->piece_type() != PieceType::King ||
        bk->player != Player::Black) {
        can_castle[2] = false;
        can_castle[3] = false;
    }

    // Update half move clock
    if (piece == PieceType::Pawn || captured_piece != nullptr) {
        half_move = 0;
    } else {
        half_move++;
    }

    // Change player to move, update full move clock
    if (to_move == Player::White) {
        to_move = Player::Black;
    } else {
        to_move = Player::White;
        full_move++;
    }
}

Board Board::clone() {
    return Board(pieces, to_move, can_castle, en_passent_square, half_move,
                 full_move);
}

std::string Board::fen() {
    std::stringstream s;

    // Write board
    for (int j = 7; j >= 0; j--) {
        int empty = 0;
        for (int i = 0; i < 8; i++) {
            std::unique_ptr<Piece> &p = get(Point(i, j));
            if (p == nullptr) {
                empty++;
            } else {
                if (empty > 0) {
                    s << empty;
                    empty = 0;
                }
                s << p->to_char();
            }
        }
        if (empty > 0) {
            s << empty;
        }
        if (j != 0) {
            s << '/';
        }
    }
    s << ' ';

    s << player_to_char(to_move);

    s << ' ';

    if (!can_castle[0] && !can_castle[1] && !can_castle[2] && !can_castle[3]) {
        s << '-';
    } else {
        if (can_castle[0]) {
            s << 'K';
        }
        if (can_castle[1]) {
            s << 'Q';
        }
        if (can_castle[2]) {
            s << 'k';
        }
        if (can_castle[3]) {
            s << 'q';
        }
    }

    s << ' ';

    if (en_passent_square.in_bounds()) {
        s << en_passent_square.algebraic();
    } else {
        s << '-';
    }

    s << ' ' << half_move << ' ' << full_move;

    return s.str();
}

Board::Board(std::string fen) {
    static std::regex fen_regex =
        std::regex("^([PNBRQKpnbrqk1-8/]+) (w|b) (-|K?Q?k?q?) (-|[a-h][1-8]) "
                   "(\\d+) (\\d+)$");
    std::smatch fen_result;
    if (!std::regex_match(fen, fen_result, fen_regex)) {
        throw std::runtime_error("invalid fen: " + fen);
    }
    std::smatch::iterator fen_iter = fen_result.begin();
    fen_iter++;
    std::string board_str = *(fen_iter++);
    std::string to_move_str = *(fen_iter++);
    std::string can_castle_str = *(fen_iter++);
    std::string en_passent_str = *(fen_iter++);
    std::string half_move_str = *(fen_iter++);
    std::string full_move_str = *(fen_iter);

    static std::regex board_regex = std::regex(
        "^([PNBRQKpnbrqk1-8]+)\\/([PNBRQKpnbrqk1-8]+)\\/([PNBRQKpnbrqk1-8]+)\\/"
        "([PNBRQKpnbrqk1-8]+)\\/([PNBRQKpnbrqk1-8]+)\\/([PNBRQKpnbrqk1-8]+)\\/"
        "([PNBRQKpnbrqk1-8]+)\\/([PNBRQKpnbrqk1-8]+)$");
    std::smatch board_result;
    if (!std::regex_match(board_str, board_result, board_regex)) {
        throw std::runtime_error("invalid fen board: " + board_str);
    }
    std::smatch::iterator board_iter = board_result.begin();
    board_iter++;
    for (int y = 7; y >= 0; y--) {
        int x = 0;
        std::string row = *(board_iter++);
        for (char c : row) {
            if (isdigit(c)) {
                for (int i = 0; i < c - '0'; i++) {
                    pieces[x][y] = nullptr;
                    x++;
                }
            } else {
                pieces[x][y] = piece_from_char(c);
                x++;
            }
        }
    }

    to_move = to_move_str == "w" ? Player::White : Player::Black;

    if (can_castle_str == "-") {
        for (int i = 0; i < 4; i++) {
            can_castle[i] = false;
        }
    } else {
        can_castle[0] = can_castle_str.find("K") != std::string::npos;
        can_castle[1] = can_castle_str.find("Q") != std::string::npos;
        can_castle[2] = can_castle_str.find("k") != std::string::npos;
        can_castle[3] = can_castle_str.find("q") != std::string::npos;
    }

    if (en_passent_str == "-") {
        en_passent_square = Point(-1, -1);
    } else {
        en_passent_square = Point(en_passent_str);
    }

    half_move = std::stoi(half_move_str);
    full_move = std::stoi(full_move_str);
}