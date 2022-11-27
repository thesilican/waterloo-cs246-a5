#ifndef GAME_H
#define GAME_H
#include "board.h"
#include <vector>
class Move;
class Board;

class Game {
    std::vector<Board> move_history;

  public:
    Board board;

    Game();
    // used when first done setup
    Game(Board b);
    void make_move(Point from, Point to);
    void make_move(Point from, Point to, PieceType promotes_to);
    void undo_move();
};

#endif
