#include "testing.h"
#include "game.h"
#include "chucknorrisbot.h"
#include <iostream>
#include <string>

void test_run() {
    Board b("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1");
    ChuckNorrisBot tb(b);
    std::vector<int> moves;
    tb.legal_moves(moves);
    for (auto m : moves) {
        std::cout << tb.uncompress_move(m).uci() << std::endl;
    }
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

void test_gen_moves_bot(std::string fen) {
    Board board(fen);
    // print legal moves (one on each line)
    ChuckNorrisBot tb(board);
    std::vector<int> moves;
    tb.legal_moves(moves);
    for (auto m : moves) {
        Move mo = tb.uncompress_move(m);
        std::cout << mo.uci() << '\n';
    }
}

void test_apply_move_bot(std::string fen, std::string uci) {
    Board board(fen);
    Move move(uci);
    ChuckNorrisBot tb(board);
    int m = tb.compress_move(move);
    tb.move(m);
    Board new_board = tb.to_board();
    std::cout << new_board.fen() << '\n';
}

void test_checks_state(std::string fen) {
    Board board(fen);
    if (board.is_checkmate()) {
        std::cout << "checkmate" << std::endl;
    } else if (board.is_stalemate()) {
        std::cout << "stalemate" << std::endl;
    } else if (board.is_check()) {
        std::cout << "check" << std::endl;
    } else {
        std::cout << "none" << std::endl;
    }
}

void test_checks_state_bot(std::string fen) {
    Board board(fen);
    ChuckNorrisBot tb(board);
    if (tb.is_checkmate()) {
        std::cout << "checkmate" << std::endl;
    } else if (tb.is_stalemate()) {
        std::cout << "stalemate" << std::endl;
    } else if (tb.is_check(tb.black_turn)) {
        std::cout << "check" << std::endl;
    } else {
        std::cout << "none" << std::endl;
    }
}