#include "testing.h"
#include "game.h"
#include "testbot.h"
#include <iostream>
#include <string>

void test_run() {
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
    TestBot tb(board);
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
    // print board fen
    TestBot tb(board);
    int m = tb.compress_move(move);
    // std::cout << "INFO " << tb.get_from(m).x << " " << tb.get_from(m).y << " " << tb.get_to(m).x << " " << tb.get_to(m).y << " " << tb.get_promote(m) << '\n';
    // char fuckyou;
    // if (move.promotes_to == PieceType::Queen) {
    //     fuckyou = 'q';
    // } else if (move.promotes_to == PieceType::Knight) {
    //     fuckyou = 'n';
    // } else {
    //     fuckyou = 'B';
    // }
    
    // std::cout << "MORE INFO " << move.has_promotes_to << " " << fuckyou << '\n';
    tb.move(m);
    Board new_board = tb.get_board_object();
    std::cout << new_board.fen() << '\n';
}
