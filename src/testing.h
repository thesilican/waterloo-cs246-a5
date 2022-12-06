#ifndef TESTING_H
#define TESTING_H
#include <string>

// General-purpose test function
void test_run();

// Test function for generating legal moves
void test_gen_moves(std::string fen);

// Test function for making a move on a board
void test_apply_move(std::string fen, std::string uci);

// Test function for generating legal moves (bot version)
void test_gen_moves_bot(std::string fen);

// Test function for making a move on a board (bot version)
void test_apply_move_bot(std::string fen, std::string uci);

// Test function for getting the checks state of a board
// i.e. none, check, checkmate, or stalemate
void test_checks_state(std::string fen);

// Test function for getting the checks state of a board (bot version)
void test_checks_state_bot(std::string fen);

#endif
