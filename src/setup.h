#ifndef SETUP_H
#define SETUP_H
#include "board.h"
#include "game.h"
class Move;
class Board;
class Point;
class Piece;
enum class Player;

// Represents a custom board setup state
class Setup {
    // The internal board state
    Board board;

  public:
    // Default constructor for setup
    Setup();
    // Set which player moves first
    void set_to_move(Player player);
    // Add a piece to a square on the board
    void add_piece(Point point, PieceType piece, Player player);
    // Remove a piece from a square on the board, if exists
    void remove_piece(Point point);
    // Remove all pieces from the board
    void clear_pieces();
    // Resets the board to the default board state
    void reset_board();
    // Set the board based on a fen string
    void set_fen(std::string fen);
    // Returns a game with the setup board state
    Game finish();
};

#endif
