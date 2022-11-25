#ifndef MOVE_H
#define MOVE_H
#include "piece.h"
#include "point.h"
#include <memory>
enum class PieceType;
class Point;
class Piece;

class Move {
  public:
    PieceType piece;
    Point from;
    Point to;
    PieceType promotes_to;
    Move(PieceType piece, Point from, Point to,
         PieceType promotes_to = PieceType::Queen);
};

#endif