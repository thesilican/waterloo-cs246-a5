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

class TestBot {
    // move representation: from.x (3 bits), from.y (3
    // bits), to.x (3 bits), to.y (3 bits), promote piece (4 bits) all in a
    // 16-bit unsigned short, promote piece '*' if n/a
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
    double evaluate(int depth);
    unsigned short get_move(Point from, Point to, char promote);
    Point get_from(unsigned short move);
    Point get_to(unsigned short move);
    char get_promote(unsigned short move);
    void print_board_debug();
    void init_board_debug();
    bool piece_color(Point at, char color);
    bool empty_square(Point at);
    void copy_array(bool from[], bool to[], int size);
    TestBot();
    TestBot(Board& b);
    Board get_board_object();
    Move uncompress_move(unsigned short m);
    unsigned short compress_move(Move m);
    //changes board state according to move, assumes move is legal
    void move(unsigned short move);
    //captured is '*' if no piece was captured
    void undo_move(unsigned short move, bool is_enpassant, char captured);
    //gives list of moves (follows movement rules, doesn't put king in check)
    std::vector<unsigned short> legal_moves();
    
    //helpers for the above. each one assumes moving piece is of correct color
    //they also don't check if the moves put friendly king at check
    std::vector<unsigned short> jumpers(Point at, const std::vector<Point>& targets);
    std::vector<unsigned short> riders(Point at, const std::vector<Point>& directions);
    std::vector<unsigned short> moves_pawn(Point at);
    std::vector<unsigned short> moves_rook(Point at);
    std::vector<unsigned short> moves_bishop(Point at);
    std::vector<unsigned short> moves_knight(Point at);
    std::vector<unsigned short> moves_queen(Point at);
    std::vector<unsigned short> moves_king(Point at);
    std::vector<unsigned short> moves_general(Point at);
    std::vector<unsigned short> moves_castle(Point at);
    bool is_check(bool black);
};
