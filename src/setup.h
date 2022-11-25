#ifndef SETUP_H
#define SETUP_H
#include "board.h"
class Move;
class Board;
class Point;
class Piece;
enum class Player;

class Setup {
    Board board;
    void add_piece(Point point, Piece* piece);
    void remove_piece(Point point);
    public:
    Player to_move;
    void run_setup(); //text command loop
    void perform_command(std::string command);
};

#endif
