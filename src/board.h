#ifndef BOARD_H
#define BOARD_H
#include "move.h"
#include "piece.h"
#include "point.h"
#include <array>
#include <iostream>
#include <memory>
#include <optional>
class Point;
class Piece;
class Move;
enum class PieceType;
enum class Player;

class Board {

  public:
    Player to_move;
    std::unique_ptr<Piece> pieces[8][8];
    Point en_passent_square;
    // KQkq
    bool can_castle[4];

    Board();
    Board(std::unique_ptr<Piece> pieces[8][8], Point en_passent_square,
          bool can_castle[4], Player to_move);
    std::unique_ptr<Piece> &get(Point p);
    void set(Point p, PieceType piece, Player player);
    std::vector<Move> legal_moves();
    bool in_check();
    bool is_checkmate();
    bool is_stalemate();
    void make_move(Move m);
    Board clone();
};

#endif
