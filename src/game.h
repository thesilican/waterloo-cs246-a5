#ifndef GAME_H
#define GAME_H
#include "board.h"
#include <vector>
class Move;
class Board;
enum class Player { White, Black };

class Game {
    std::vector<Board> move_history;

  public:
    Board board;

    Game();
    // used when first done setup
    Game(Board &b, Player to_move);
    void make_move(Move m);
    void undo_move(Move m);
};

#endif
