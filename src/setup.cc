#include "setup.h"
#include "debug.h"

Setup::Setup() : board() {}

void Setup::remove_rights() {
    board.en_passant_square = Point(-1,-1);
    board.can_castle[0] = false;
    board.can_castle[1] = false;
    board.can_castle[2] = false;
    board.can_castle[3] = false;
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

void Setup::reset_board() {
    board = Board();
}

void Setup::set_fen(std::string fen) {
    board = Board(fen);
}

Game Setup::finish() {
    return Game(board.clone());
}
