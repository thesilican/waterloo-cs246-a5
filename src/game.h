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
    Game(std::string fen);
    // used when first done setup
    Game(Board b);
    void make_move(Move move);
    void undo_move();
};

#endif
