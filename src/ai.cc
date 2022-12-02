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
        if (b.in_check()) {
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
        if (b.in_check()) {
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
    Move best = moves[0];
    for (auto move : moves) {
        int count = 0;
        Board board = game.board.clone();
        for (auto m : board.legal_moves()) {
            if (m.captured(board) != nullptr) {
                count++;
            }
        }
        if (count < min) {
            best = move;
            min = count;
        }
    }
    return best;
}

Move ChuckNorrisBot::best_move(Game &game) {
    throw std::runtime_error("not implemented");
}
