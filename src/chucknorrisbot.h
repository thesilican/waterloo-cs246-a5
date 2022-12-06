#ifndef TESTBOT_H
#define TESTBOT_H

#include "board.h"
#include "point.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include "ai.h"

class Bot;

/*
  Goals:
  - given a board state, return a list of legal moves
  - given a board state and move, return the new board state
  - given a board state, move, bool is_enpassant, and char representing captured
  piece (if any), return old board array (rest of state must be stored)

  - given a board state, evaluate it
  - given a board state, return the best move
*/

/*
  Potential optimizations:
  - currently, all legal moves vector is created from merging individual legal
  moves vectors, so O(n). O(1) solution?
*/

struct Eval {
    int move;
    int ev;
};

class ChuckNorrisBot : public Bot {
    // move representation: from.x (3 bits), from.y (3
    // bits), to.x (3 bits), to.y (3 bits), promote piece (4 bits) all in a
    // 16-bit int, promote piece '*' if n/a
    char board[8][8];
    // represents whether king/queen castling is possible assuming no obstacles
    // between king and rook
    //  0 1 -> queen and king side for white
    //  2 3 -> queen and king side for black
    bool castling[4];
    // represents whether en passant possible in column x
    bool en_passant_good[8];
    // map from char form of piece to integer form
    std::unordered_map<char, int> int_form;
    std::unordered_map<int, char> char_form;

    void jumpers(Point at, std::vector<Point> &targets,
                 std::vector<int> &moves);
    void riders(Point at, std::vector<Point> &directions,
                std::vector<int> &moves);
    void moves_pawn(Point at, std::vector<int> &moves);
    void moves_rook(Point at, std::vector<int> &moves);
    void moves_bishop(Point at, std::vector<int> &moves);
    void moves_knight(Point at, std::vector<int> &moves);
    void moves_queen(Point at, std::vector<int> &moves);
    void moves_king(Point at, std::vector<int> &moves);
    void moves_general(Point at, std::vector<int> &moves);
    void moves_castle(Point at, std::vector<int> &moves);
    int evaluate(int depth);
    int get_move(Point from, Point to, char promote);
    Point get_from(int move);
    Point get_to(int move);
    char get_promote(int move);
    void print_board_debug();
    void init_board_debug();
    bool piece_color(Point at, char color);
    bool empty_square(Point at);
    void all_moves(std::vector<int> &moves);
    void is_proper_move(int move, std::vector<int> &moves);

  public:
    bool black_turn = false;

    ChuckNorrisBot();
    ChuckNorrisBot(Board &b);

    Eval alpha_beta(int alpha, int beta, int depth);
    Board to_board();
    // changes board state according to move, assumes move is legal
    void move(int move);
    // captured is '*' if no piece was captured
    void undo_move(int move, bool is_enpassant, char captured);
    Move uncompress_move(int m);
    int compress_move(Move m);
    void legal_moves(std::vector<int> &final_moves);
    bool is_check(bool black);
    bool is_checkmate();
    bool is_stalemate();

    Move best_move(Game &game) override;
};

#endif
