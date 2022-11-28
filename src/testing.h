#ifndef TESTING_H
#define TESTING_H
#include <string>

void test_run_suite();

void test_gen_moves(std::string fen);

void test_apply_move(std::string fen, std::string uci);

#endif