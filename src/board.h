#ifndef BOARD_H
#define BOARD_H
#include "move.h"
#include "piece.h"
#include "point.h"
#include <array>
#include <iostream>
#include <memory>
#include <vector>
class Point;
class Piece;
class Move;
enum class PieceType;
enum class Player;

// Represents a chess board
class Board {
    // Returns all possible moves for a given piece
    std::vector<Move> possible_moves();
    // The internal array of pieces
    std::unique_ptr<Piece> pieces[8][8];

  public:
    // Which player's turn it is to move
    Player to_move;
    // Which castlings are allowed (KQkq)
    bool can_castle[4];
    // The square which an en-passant capture can take place
    // (-1,-1) if no en-passant is possible
    Point en_passant_square;
    // Number of half moves since the last pawn move or capture
    int half_move;
    // Number of full moves since the beginning of the game
    int full_move;

// Default constructor for the board, returns the default chess setup
    Board();
    // Constructs a game from a fen representation
    Board(std::string fen);
    // Constructs a game from its various parts
    Board(std::unique_ptr<Piece> pieces[8][8], Player to_move,
          bool can_castle[4], Point en_passant_square, int half_move,
          int full_move);
          // Returns the piece at a given square
    std::unique_ptr<Piece> &get(Point p);
    // Generates a list of all possible legal moves for a board
    std::vector<Move> legal_moves();
    // Whether or not the current player is in check
    bool is_check();
    // Whether or not the current player is in checkmate
    bool is_checkmate();
    // Whether or not the current player is in stalemate
    bool is_stalemate();
    // Makes a move on the board
    // Note: this function does not check whether the move is legal
    void make_move(Move m);
    // Returns a clone of the current board
    Board clone();
    // Returns the fen representation of the current board
    std::string fen();
    // Returns whether pawns exist on first or last rows
    bool illegal_pawns();
    // Returns whether wrong number of Kings exists
    bool illegal_kings();
};

#endif
