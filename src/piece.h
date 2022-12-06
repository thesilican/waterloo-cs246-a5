#ifndef PIECE_H
#define PIECE_H
#include "board.h"
#include "move.h"
#include "point.h"
#include <iostream>
#include <memory>
#include <vector>
class Point;
class Move;
class Board;
enum class Player;

// Represents a player in a chess game
enum class Player { White, Black };

// Parse a player from a char
Player player_from_char(char c);

// Convert from player to char
char player_to_char(Player p);

// Represents one of the 6 basic chess piece types
enum class PieceType { Pawn, Knight, Bishop, Rook, Queen, King };

// Parse a piece type from a char
PieceType piece_type_from_char(char c);

// Convert from piece type to char
char piece_type_to_char(PieceType p);

// Abstract base class for a chess piece
class Piece {
  public:
    // The player who owns this piece
    Player player;

    // Construct a piece
    Piece(Player player);
    // Virtual destructor
    virtual ~Piece() = default;

    // Abstract function, returns a list of possible moves for a piece
    // on a given board
    virtual std::vector<Move> possible_moves(Board &board, Point from) = 0;
    // Abstract function, returns the piece type of the piece
    virtual PieceType piece_type() = 0;
    // Create a clone of a piece using a smart pointer
    virtual std::unique_ptr<Piece> clone();
    // Abstract function, returns the char representation of a piece
    virtual char to_char() = 0;
};

// Construct a piece given a piece type and a player
std::unique_ptr<Piece> new_piece(PieceType type, Player player);

// Construct a piece from a char
std::unique_ptr<Piece> piece_from_char(char c);

// Represents a pawn on the chess board
class Pawn : public Piece {
  public:
    // Constructor for the pawn
    Pawn(Player player);
    // Returns a list of possible moves for a pawn
    std::vector<Move> possible_moves(Board &board, Point from) override;
    // Returns the piece type of this pawn
    PieceType piece_type() override;
    // Returns the char representation of this pawn
    char to_char() override;
};

// Represents a knight on the chessboard
class Knight : public Piece {
  public:
    // Constructor for the knight
    Knight(Player player);
    // Returns a list of possible moves for the knight
    std::vector<Move> possible_moves(Board &board, Point from) override;
    // Returns the piece type of this knight
    PieceType piece_type() override;
    // Returns the char representation of this knight
    char to_char() override;
};

// Represents a bishop on the chessboard
class Bishop : public Piece {
  public:
    // Constructor for the bishop
    Bishop(Player player);
    // Returns a list of possible moves for the bishop
    std::vector<Move> possible_moves(Board &board, Point from) override;
    // Returns the piece type of this bishop
    PieceType piece_type() override;
    // Returns the char representation of this bishop
    char to_char() override;
};

// Represents a rook on the chessboard
class Rook : public Piece {
  public:
    // Constructor for the rook
    Rook(Player player);
    // Returns a list of possible moves for the rook
    std::vector<Move> possible_moves(Board &board, Point from) override;
    // Returns the piece type of this rook
    PieceType piece_type() override;
    // Returns the char representation of this rook
    char to_char() override;
};

// Represents a queen on the chessboard
class Queen : public Piece {
  public:
    // Constructor for the queen
    Queen(Player player);
    // Returns a list of possible moves for the queen
    std::vector<Move> possible_moves(Board &board, Point from) override;
    // Returns the piece type of this queen
    PieceType piece_type() override;
    // Returns the char representation of this queen
    char to_char() override;
};

// Represents a king on the chessboard
class King : public Piece {
  public:
    // Constructor for the king
    King(Player player);
    // Returns a list of possible moves for the king
    std::vector<Move> possible_moves(Board &board, Point from) override;
    // Returns the piece type of this king
    PieceType piece_type() override;
    // Returns the char representation of this king
    char to_char() override;
};

#endif
