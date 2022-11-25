#ifndef GAME_H
#define GAME_H
#include <vector>
#include "board.h"
class Move;
class Board;
enum class Player { White, Black };

class Game {
  public:
    Board board;
    std::vector<Board> move_history;

    Game();
    //used when first done setup
    Game(Board &b, Player to_move);
    void make_move(Move m);
    void undo_move(Move m);
};

#endif
