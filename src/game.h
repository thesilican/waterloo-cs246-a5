#ifndef GAME_H
#define GAME_H
#include "board.h"
#include <vector>
class Move;
class Board;

// Represents a chess game with move undo capabilities
class Game {
    // The board history stack
    std::vector<Board> history;

  public:
    // Used to denote whether the current player has resigned the game
    bool resigned = false;

    // The current board state of the game
    Board board;

    // Construct a game with a default board
    Game();
    // Construct a game with a given initial board
    Game(Board b);
    // Construct a game with a board fen
    Game(std::string fen);

    // Make a move to the current board,
    // pushing the previous board to the history stack
    void make_move(Move move);

    // Pop a board off the history stack
    void undo_move();
};

#endif
