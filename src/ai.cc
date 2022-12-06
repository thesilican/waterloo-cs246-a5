#include "ai.h"

const char *NoMovesException::what() const throw() {
    return "board has no legal moves";
}

Move BumblingBuffoonBot::best_move(Game &game) {
    std::vector<Move> moves = game.board.legal_moves();
    if (moves.size() == 0) {
        throw NoMovesException();
    }
    int idx = dist(mt) % moves.size();
    return moves[idx];
}

Move SlightlyCompetentBot::best_move(Game &game) {
    std::vector<Move> moves = game.board.legal_moves();
    if (moves.size() == 0) {
        throw NoMovesException();
    }
    for (auto m : moves) {
        Board b = game.board.clone();
        b.make_move(m);
        if (b.is_checkmate()) {
            return m;
        }
    }
    for (auto m : moves) {
        Board b = game.board.clone();
        b.make_move(m);
        if (b.is_check()) {
            return m;
        }
    }
    for (auto m : moves) {
        if (m.captured(game.board) != nullptr) {
            return m;
        }
    }
    int idx = dist(mt) % moves.size();
    return moves[idx];
}

Move AverageCsStudentBot::best_move(Game &game) {
    std::vector<Move> moves = game.board.legal_moves();
    if (moves.size() == 0) {
        throw NoMovesException();
    }
    for (auto m : moves) {
        Board b = game.board.clone();
        b.make_move(m);
        if (b.is_checkmate()) {
            return m;
        }
    }
    for (auto m : moves) {
        Board b = game.board.clone();
        b.make_move(m);
        if (b.is_check()) {
            return m;
        }
    }
    for (auto m : moves) {
        if (m.captured(game.board) != nullptr) {
            return m;
        }
    }
    // Minimize the number of opponent capture moves
    int min = INT_MAX;
    std::vector<Move> best{moves[0]};
    for (auto move : moves) {
        int count = 0;
        Board board = game.board.clone();
        board.make_move(move);
        for (auto m : board.legal_moves()) {
            if (m.captured(board) != nullptr) {
                count++;
            }
        }
        if (count == min) {
            best.push_back(move);
        } else if (count < min) {
            best.clear();
            best.push_back(move);
            min = count;
        }
    }
    int idx = dist(mt) % best.size();
    return best[idx];
}

Move ChuckNorrisBot::best_move(Game &game) {
    std::vector<Move> moves = game.board.legal_moves();
    if (moves.size() == 0) {
        throw NoMovesException();
    }
    for (auto m : moves) {
        Board b = game.board.clone();
        b.make_move(m);
        if (b.is_checkmate()) {
            return m;
        }
    }
    int best_score = INT_MIN;
    std::vector<Move> best_moves{moves[0]};
    for (auto move : moves) {
        int count = 0;
        Board board = game.board.clone();
        board.make_move(move);
        count -= board.legal_moves().size();
        board.to_move = game.board.to_move;
        for (int i = 0; i < 4; i++) {
            board.can_castle[i] = false;
        }
        board.en_passent_square = Point(-1, -1);
        count += board.legal_moves().size();
        if (count == best_score) {
            best_moves.push_back(move);
        } else if (count > best_score) {
            best_moves.clear();
            best_moves.push_back(move);
            best_score = count;
        }
    }
    int idx = dist(mt) % best_moves.size();
    return best_moves[idx];
}
