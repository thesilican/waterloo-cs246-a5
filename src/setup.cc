#include "setup.h"
#include "debug.h"

Setup::Setup() : board() {
}

void Setup::set_to_move(Player player) {
    board.to_move = player;
}

void Setup::add_piece(Point point, PieceType piece, Player player) {
    board.get(point) = new_piece(piece, player);
}

void Setup::remove_piece(Point point) {
    board.get(point) = nullptr;
}

void Setup::clear_pieces() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board.get(Point(i, j)) = nullptr;
        }
    }
}

Game Setup::finish() {
    return Game(board.clone());
}

void Setup::run_setup() {
    throw std::runtime_error("not implemented");
}
