#ifndef MOVE_H
#define MOVE_H
#include "board.h"
#include "piece.h"
#include "point.h"
#include <memory>
#include <optional>
enum class PieceType;
class Point;
class Piece;
class Board;

// Represents a piece movement from one point to another
class Move {
  public:
    // The square the piece is moving from
    Point from;
    // The square the piece is moving to
    Point to;
    // Whether the move contains promotion piece type
    bool has_promotes_to;
    // The piece type that a pawn movement promotes to
    // undefined value if has_promotes_to is false
    PieceType promotes_to;

    // Default constructor for a move (should only be used for temporary values)
    Move();
    // Construct a move from a UCI extended algebraic notation string
    Move(std::string uci);
    // Construct a move from standard algebraic notation
    Move(std::string san, Board &b);
    // Construct a move with no promotion piece type
    Move(Point from, Point to);
    // Construct a move with a promotion piece type
    Move(Point from, Point to, PieceType promotes_to);
    // Returns the UCI string representation of the move
    std::string uci();

    // Returns the piece that moved
    // b - The board state before the move was made
    std::unique_ptr<Piece> &piece(Board &b);
    // Returns the piece that was captured from the board
    // b - The board state before the move was made
    std::unique_ptr<Piece> &captured(Board &b);
};

bool operator==(Move a, Move b);
bool operator!=(Move a, Move b);
std::ostream &operator<<(std::ostream &o, Move m);

#endif