#ifndef MOVE_H
#define MOVE_H
#include <memory>
enum class PieceType;
class Point;
class Piece;

class Move {
  public:
    PieceType piece;
    Point from;
    Point to;
    // nullptr to indicate no piece was captured
    std::unique_ptr<Piece> captured;
    bool is_castling;
    bool is_enpassent;
    Move(PieceType piece, Point from, Point to,
         std::unique_ptr<Piece> captured = nullptr, bool is_castling = false,
         bool is_enpassent = false);
};

#endif