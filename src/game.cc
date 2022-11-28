#include "game.h"

Game::Game() : move_history(), board() {
}

Game::Game(std::string fen) : move_history(), board(fen) {
}

Game::Game(Board b) : move_history(), board(std::move(b)) {
}

void Game::make_move(Move move) {
    move_history.push_back(board.clone());
    board.make_move(move);
}

void Game::undo_move() {
    if (move_history.size() == 0)
        throw std::runtime_error("cannot undo move");
    board = move_history[move_history.size() - 1].clone();
    move_history.pop_back();
}
