#ifndef BOARD_H
#define BOARD_H
#include <array>
#include <iostream>
#include <memory>
#include <optional>
#include "point.h"
#include "piece.h"
#include "move.h"
class Point;
class Piece;
class Move;

class Board {

  public:
    Player to_move;
    std::unique_ptr<Piece> pieces[8][8];
    Point en_passent_square;
    bool can_castle[4];

    void set(Point p, PieceType piece, Player player);
    std::vector<Move> legal_moves();
    bool in_check();
    bool is_checkmate();
    void make_move(Move m);
    Board clone();
    Board(std::unique_ptr<Piece> pieces[8][8], Point en_passent_square, bool can_castle[4]);
};

#endif
