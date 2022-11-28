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
    std::vector<Move> possible_moves();
    std::unique_ptr<Piece> pieces[8][8];

  public:
    Player to_move;
    // KQkq
    bool can_castle[4];
    Point en_passent_square;
    int half_move;
    int full_move;

    Board();
    Board(std::string fen);
    Board(std::unique_ptr<Piece> pieces[8][8], Player to_move,
          bool can_castle[4], Point en_passent_square, int half_move,
          int full_move);
    std::unique_ptr<Piece> &get(Point p);
    std::vector<Move> legal_moves();
    bool in_check();
    bool is_checkmate();
    bool is_stalemate();
    // Note: It is assumed that the move is legal
    void make_move(Move m);
    Board clone();
    std::string fen();
};

#endif
