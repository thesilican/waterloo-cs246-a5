#include "point.h"
#include "zobrist.h"
#include <vector>

/*
  Goals:
  - given a board state, return a list of legal moves
  - given a board state and move, return the new board state
  - 
*/

/*
  Potential optimization:
  - currently, all legal moves vector is created from merging individual legal moves vectors, so O(n). O(1) solution?
*/

class TestBot {
    // move representation: piece (0-11, 4 bits), from.x (3 bits), from.y (3
    // bits), to.x (3 bits), to.y (3 bits), promote piece (4 bits) all in a
    // 32-bit unsigned integer
    char board[8][8];
    bool black_turn;
    bool castling[4];
    bool en_passant_good[8];
    // map from char form of piece to integer form
    std::unordered_map<char, int> int_form;

  public:
    TestBot();
    int get_move(char piece, Point from, Point to, char promote);
    Point get_from(int move);
    Point get_to(int move);
    char get_piece(int move);
    char get_promote(int move);

    void move(char board[8][8], int move);
    // moves piece according to move, assumes "from" has a piece and "to" is empty
    void shift_piece(char board[8][8], bool black, bool castle[4], bool en_passant[8], Point from,
                     Point to);

    //gives list of moves (follows movement rules, doesn't put king in check) for piece at point "p"
    std::vector<int> legal_moves(char board[8][8], bool black, bool castle[4], bool en_passant[8]);
    
    //helpers for the above. each one assumes moving piece is of correct color
    std::vector<int> moves_pawn(char board[8][8], bool castle[4], bool en_passant[8])

    bool is_check(char board[8][8]);
};
