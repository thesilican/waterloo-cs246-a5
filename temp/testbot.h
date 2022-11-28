#include "point.h"
#include "zobrist.h"
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

class TestBot {
    // move representation: piece (0-11, 4 bits), from.x (3 bits), from.y (3
    // bits), to.x (3 bits), to.y (3 bits), promote piece (4 bits) all in a
    // 32-bit unsigned integer
    char board[8][8];
    bool black_turn;
    //represents whether king/queen castling is possible assuming no obstacles between king and rook
    bool castling[4];
    //represents whether en passant possible in column x
    bool en_passant_good[8];
    // map from char form of piece to integer form
    std::unordered_map<char, int> int_form;
    std::unordered_map<int, char> char_form;
  public:
    int get_move(char piece, Point from, Point to, char promote);
    char get_piece(int move);
    Point get_from(int move);
    Point get_to(int move);
    char get_promote(int move);

    TestBot();
    // //changes board state according to move
    // void move(int move);
    // //captured is '.' if no piece was captured
    // void undo_move(int move, bool is_enpassant, char captured);
    // //gives list of moves (follows movement rules, doesn't put king in check)
    // std::vector<int> legal_moves();
    
    // //helpers for the above. each one assumes moving piece is of correct color
    // std::vector<int> moves_pawn(Point at);
    // std::vector<int> moves_rook(Point at);
    // std::vector<int> moves_bishop(Point at);
    // std::vector<int> moves_knight(Point at);
    // std::vector<int> moves_queen(Point at);
    // std::vector<int> moves_king(Point at);
    // bool is_check();
};
