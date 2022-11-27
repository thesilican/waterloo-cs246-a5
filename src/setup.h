#ifndef SETUP_H
#define SETUP_H
#include "game.h"
#include "board.h"
class Move;
class Board;
class Point;
class Piece;
enum class Player;

class Setup {
    Board board;

  public:
    Setup();
    void set_to_move(Player player);
    void add_piece(Point point, PieceType piece, Player player);
    void remove_piece(Point point);
    void clear_pieces();
    Game finish();

    void run_setup(); // text command loop
};

#endif
