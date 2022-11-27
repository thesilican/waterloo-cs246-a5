#ifndef PIECE_H
#define PIECE_H
#include "board.h"
#include "move.h"
#include "point.h"
#include <iostream>
#include <vector>
class Point;
class Move;
class Board;
enum class Player;

enum class Player { White, Black };

std::ostream &operator<<(std::ostream &o, Player p);

enum class PieceType { Pawn, Knight, Bishop, Rook, Queen, King };

std::ostream &operator<<(std::ostream &o, PieceType p);

class Piece {
  public:
    Piece(Player player);
    Player player;
    virtual std::vector<Move> possible_moves(Board &board, Point from) = 0;
    virtual PieceType piece_type() = 0;
    virtual std::unique_ptr<Piece> clone();
    virtual ~Piece();
};

std::unique_ptr<Piece> new_piece(PieceType type, Player player);

class Pawn : public Piece {
  public:
    Pawn(Player player);
    std::vector<Move> possible_moves(Board &board, Point from) override;
    PieceType piece_type() override;
};

class Knight : public Piece {
  public:
    Knight(Player player);
    std::vector<Move> possible_moves(Board &board, Point from) override;
    PieceType piece_type() override;
};

class Bishop : public Piece {
  public:
    Bishop(Player player);
    std::vector<Move> possible_moves(Board &board, Point from) override;
    PieceType piece_type() override;
};

class Rook : public Piece {
  public:
    Rook(Player player);
    std::vector<Move> possible_moves(Board &board, Point from) override;
    PieceType piece_type() override;
};

class Queen : public Piece {
  public:
    Queen(Player player);
    std::vector<Move> possible_moves(Board &board, Point from) override;
    PieceType piece_type() override;
};

class King : public Piece {
  public:
    King(Player player);
    std::vector<Move> possible_moves(Board &board, Point from) override;
    PieceType piece_type() override;
};

#endif
