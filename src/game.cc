#include "game.h"

Game::Game() : history(), board() {
}

Game::Game(std::string fen) : history(), board(fen) {
}

Game::Game(Board b) : history(), board(std::move(b)) {
}

void Game::make_move(Move move) {
    history.push_back(board.clone());
    board.make_move(move);
}

void Game::undo_move() {
    if (history.size() == 0)
        throw std::runtime_error("cannot undo move");
    board = history[history.size() - 1].clone();
    history.pop_back();
}
