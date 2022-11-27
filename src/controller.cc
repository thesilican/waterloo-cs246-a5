#include "controller.h"
#include "debug.h"
#include <iostream>

Controller::Controller()
    : setup(), game(), state(ControllerState::Setup), white_wins(0),
      black_wins(0), draws(0) {
}

void Controller::run_setup() {
    throw std::runtime_error("not implemented");
}

void Controller::run_game() {
    throw std::runtime_error("not implemented");
}

void Controller::do_game_command(std::string command) {
    throw std::runtime_error("not implemented");
}

void Controller::command_loop() {
    setup.clear_pieces();
    setup.add_piece(Point("e7"), PieceType::Pawn, Player::White);
    game = setup.finish();

    notify_observers(*this);
    auto moves = game.board.legal_moves();
    std::cout << "Legal moves:" << std::endl;
    for (auto m : moves) {
        std::cout << m << std::endl;
    }
    game.make_move(Point("e7"), Point("e8"), PieceType::Rook);
    notify_observers(*this);
}
