#ifndef PIECE_H
#define PIECE_H
#include <iostream>
#include <vector>
class Point;
class Move;
class Board;
enum class Player;

enum class PieceType { Pawn, Knight, Bishop, Rook, Queen, King };

class Piece {
  public:
    Player player;
    virtual std::vector<Move> possible_moves(Board &board, Point from) = 0;
    virtual PieceType piece_type() = 0;
    virtual std::unique_ptr<Piece> clone() = 0;
    virtual ~Piece();
};

class Pawn : public Piece {
    std::vector<Move> possible_moves(Board &board, Point from) override;
    PieceType piece_type() override;
    std::unique_ptr<Piece> clone() override;
};

class Knight : public Piece {
    std::vector<Move> possible_moves(Board &board, Point from) override;
    PieceType piece_type() override;
    std::unique_ptr<Piece> clone() override;
};

class Bishop : public Piece {
    std::vector<Move> possible_moves(Board &board, Point from) override;
    PieceType piece_type() override;
    std::unique_ptr<Piece> clone() override;
};

class Rook : public Piece {
    std::vector<Move> possible_moves(Board &board, Point from) override;
    PieceType piece_type() override;
    std::unique_ptr<Piece> clone() override;
};

class Queen : public Piece {
    std::vector<Move> possible_moves(Board &board, Point from) override;
    PieceType piece_type() override;
    std::unique_ptr<Piece> clone() override;
};

class King : public Piece {
    std::vector<Move> possible_moves(Board &board, Point from) override;
    PieceType piece_type() override;
    std::unique_ptr<Piece> clone() override;
};

#endif
