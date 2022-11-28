#ifndef MOVE_H
#define MOVE_H
#include "piece.h"
#include "point.h"
#include <memory>
#include <optional>
#include "board.h"
enum class PieceType;
class Point;
class Piece;

class Move {
  public:
    Point from;
    Point to;
    bool has_promotes_to;
    PieceType promotes_to;
    Move(std::string uci);
    Move(Point from, Point to);
    Move(Point from, Point to, PieceType promotes_to);
    std::string uci();
};

bool operator==(Move a, Move b);
bool operator!=(Move a, Move b);
std::ostream &operator<<(std::ostream &o, Move m);

#endif