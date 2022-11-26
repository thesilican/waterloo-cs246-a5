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
    Player to_move;
    void add_piece(Point point, PieceType piece, Player player);
    void remove_piece(Point point);

  public:
    void run_setup(); // text command loop
    void perform_command(std::string command);
};

#endif
