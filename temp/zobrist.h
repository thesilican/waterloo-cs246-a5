#ifndef ZOBRIST_H
#define ZOBRIST_H
#include <random>
#include <unordered_map>

class Zobrist {
    unsigned long long int zobrist_table[8][8][12];
    unsigned long long int is_black;
    unsigned long long int can_castle[4];
    unsigned long long int
        en_passant[8]; // represents column in which it's legal

    std::random_device rd;
    std::mt19937 mt{rd()};
    std::uniform_int_distribution<unsigned long long int> dist{0, UINT64_MAX};
    // map from char form of piece to integer form
    std::unordered_map<char, int> int_form;

    // for the bot, pawn promotion can either be queen or knight, rook and
    // bishop are redundant

    /*
        Assumed board structure:
         01234567
        7**K*****
        6*****q**
         ...

        char board[8][8];
        bool is_black; --> if true, next turn is black
        bool can_castle[4];
        bool en_passant[8] --> whether en passant is possible on column x
       (a,b..) -> (0,1..)
    */

  public:
    Zobrist();
    void init_table();
    unsigned long long int get_hash(char board[8][8], bool black,
                                    bool castle[4], bool can_en_passant[8]);
    unsigned long long int update_piece(unsigned long long int h, int i, int j,
                                        char piece);
};

#endif
