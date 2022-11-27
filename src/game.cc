#include "game.h"

Game::Game() {
    board = Board();
    move_history = std::vector<Board>();
}

Game::Game(Board b) {
    board = std::move(b);
    move_history = std::vector<Board>();
}

void Game::make_move(Point from, Point to) {
    make_move(from, to, PieceType::Queen);
}

void Game::make_move(Point from, Point to, PieceType promotes_to) {
    PieceType piece = board.get(from)->piece_type();
    board.make_move(Move(piece, from, to, promotes_to));
}

void Game::undo_move() {
    throw std::runtime_error("not implemented");
}