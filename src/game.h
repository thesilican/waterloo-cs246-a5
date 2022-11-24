#ifndef GAME_H
#define GAME_H
#include <vector>
class Move;
class Board;
enum class Player { White, Black };

class Game {
  public:
    Player to_move;
    Board board;
    std::vector<Move> move_history;

    Game();
    //used when first done setup
    Game(Board &b, Player to_move);
};

#endif
