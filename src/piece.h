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

Player player_from_char(char c);

char player_to_char(Player p);

enum class PieceType { Pawn, Knight, Bishop, Rook, Queen, King };

PieceType piece_type_from_char(char c);

char piece_type_to_char(PieceType p);

class Piece {
  public:
    Piece(Player player);
    Player player;
    virtual std::vector<Move> possible_moves(Board &board, Point from) = 0;
    virtual PieceType piece_type() = 0;
    virtual std::unique_ptr<Piece> clone();
    virtual char to_char() = 0;
    virtual ~Piece();
};

std::unique_ptr<Piece> new_piece(PieceType type, Player player);

std::unique_ptr<Piece> piece_from_char(char c);

class Pawn : public Piece {
  public:
    Pawn(Player player);
    std::vector<Move> possible_moves(Board &board, Point from) override;
    PieceType piece_type() override;
    char to_char() override;
};

class Knight : public Piece {
  public:
    Knight(Player player);
    std::vector<Move> possible_moves(Board &board, Point from) override;
    PieceType piece_type() override;
    char to_char() override;
};

class Bishop : public Piece {
  public:
    Bishop(Player player);
    std::vector<Move> possible_moves(Board &board, Point from) override;
    PieceType piece_type() override;
    char to_char() override;
};

class Rook : public Piece {
  public:
    Rook(Player player);
    std::vector<Move> possible_moves(Board &board, Point from) override;
    PieceType piece_type() override;
    char to_char() override;
};

class Queen : public Piece {
  public:
    Queen(Player player);
    std::vector<Move> possible_moves(Board &board, Point from) override;
    PieceType piece_type() override;
    char to_char() override;
};

class King : public Piece {
  public:
    King(Player player);
    std::vector<Move> possible_moves(Board &board, Point from) override;
    PieceType piece_type() override;
    char to_char() override;
};

#endif
