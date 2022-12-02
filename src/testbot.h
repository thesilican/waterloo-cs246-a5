#ifndef TESTBOT_H
#define TESTBOT_H

#include "point.h"
#include "zobrist.h"
#include "board.h"
#include <vector>
#include <iostream>
#include <unordered_map>

/*
  Goals:
  - given a board state, return a list of legal moves
  - given a board state and move, return the new board state
  - given a board state, move, bool is_enpassant, and char representing captured piece (if any), return old board array (rest of state must be stored)

  - given a board state, evaluate it
  - given a board state, return the best move
*/

/*
  Potential optimizations:
  - currently, all legal moves vector is created from merging individual legal moves vectors, so O(n). O(1) solution?
*/

struct Eval {
  int move;
  int ev;
};

class TestBot {
    // move representation: from.x (3 bits), from.y (3
    // bits), to.x (3 bits), to.y (3 bits), promote piece (4 bits) all in a
    // 16-bit int, promote piece '*' if n/a
    char board[8][8];
    bool black_turn = false;
    //represents whether king/queen castling is possible assuming no obstacles between king and rook
    // 0 1 -> queen and king side for white
    // 2 3 -> queen and king side for black
    bool castling[4];
    //represents whether en passant possible in column x
    bool en_passant_good[8];
    // map from char form of piece to integer form
    std::unordered_map<char, int> int_form;
    std::unordered_map<int, char> char_form;
  public:
    int evaluate(int depth);
    Eval alpha_beta(int alpha, int beta, int depth);
    int get_move(Point from, Point to, char promote);
    Point get_from(int move);
    Point get_to(int move);
    char get_promote(int move);
    void print_board_debug();
    void init_board_debug();
    bool piece_color(Point at, char color);
    bool empty_square(Point at);
    void copy_array(bool from[], bool to[], int size);
    TestBot();
    TestBot(Board& b);
    Board get_board_object();
    Move uncompress_move(int m);
    int compress_move(Move m);
    //changes board state according to move, assumes move is legal
    void move(int move);
    //captured is '*' if no piece was captured
    void undo_move(int move, bool is_enpassant, char captured);
    //gives list of moves (follows movement rules, doesn't put king in check)
    std::vector<int> legal_moves();
    std::vector<int> all_moves();
    void is_proper_move(int move, std::vector<int>& moves);
    
    //helpers for the above. each one assumes moving piece is of correct color
    //they also don't check if the moves put friendly king at check
    std::vector<int> jumpers(Point at, const std::vector<Point>& targets);
    std::vector<int> riders(Point at, const std::vector<Point>& directions);
    std::vector<int> moves_pawn(Point at);
    std::vector<int> moves_rook(Point at);
    std::vector<int> moves_bishop(Point at);
    std::vector<int> moves_knight(Point at);
    std::vector<int> moves_queen(Point at);
    std::vector<int> moves_king(Point at);
    std::vector<int> moves_general(Point at);
    std::vector<int> moves_castle(Point at);
    bool is_check(bool black);
};

#endif
