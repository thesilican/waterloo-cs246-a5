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
    game = Game("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    // notify_observers(*this);
    // for (auto move : game.board.legal_moves()) {
    //     std::cout << move.uci() << std::endl;
    // }
    // game.make_move(Move("a2a4"));
    // game.make_move(Move("e7e5"));
    // game.make_move(Move("f1c4"));
    // game.make_move(Move("f8c5"));
    // game.make_move(Move("d1h5"));
    // game.make_move(Move("g8f6"));
    // game.make_move(Move("h5f7"));

    TestBot t{game.board};
    notify_observers(*this);
    std::string l;
    while (true) {
        std::cin >> l;
        Move m = t.uncompress_move((t.alpha_beta(-10000,10000,4)).move);
        game.make_move(m);
        t.move(t.compress_move(m));
        notify_observers(*this);
        std::cin >> l;
        Move first(l);
        game.make_move(l);
        t.move(t.compress_move(l));
        notify_observers(*this);
        
    }
    getchar();
}
