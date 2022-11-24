#ifndef BOARD_H
#define BOARD_H
#include <array>
#include <iostream>
#include <memory>
class Point;
class Piece;
class Move;

class Board {
    // nullptr indicates an empty square
    std::unique_ptr<Piece> pieces[8][8];

  public:
    std::unique_ptr<Piece> get(Point p);
    void set(Point p, std::unique_ptr<Piece> piece);
    std::vector<Move> legal_moves();
    void make_move(Move m);
    void undo_move(Move m);
};

#endif
