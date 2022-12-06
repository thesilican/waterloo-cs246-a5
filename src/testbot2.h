#ifndef TESTBOT2_H
#define TESTBOT2_H

#include "point.h"
#include "board.h"
#include <vector>
#include <iostream>
#include <unordered_map>
#include <utility>


class TestBot2 {
    Board& b;

    public:
    TestBot2(Board& b);
    int alpha_beta(int alpha, int beta, int depth);
    int evaluate(int depth);
    Move ab_driver(int alpha, int beta, int depth);
};    

#endif
