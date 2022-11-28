#include "testing.h"
#include "game.h"
#include <iostream>
#include <string>

void test_run_suite() {
}

void test_gen_moves(std::string fen) {
    Board board(fen);
    auto moves = board.legal_moves();
    for (auto move : moves) {
        std::cout << move.uci() << std::endl;
    }
}

void test_apply_move(std::string fen, std::string uci) {
    Board board(fen);
    Move move(uci);
    board.make_move(move);
    std::cout << board.fen() << std::endl;
}