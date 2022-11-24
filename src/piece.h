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
    Player player;
    virtual std::vector<Move> possible_moves(Board &board, Point from) = 0;
    virtual PieceType piece_type() = 0;
};

class Pawn : public Piece {
    std::vector<Move> possible_moves(Board &board, Point from) override;
    PieceType piece_type() override;
};

class Knight : public Piece {
    std::vector<Move> possible_moves(Board &board, Point from) override;
    PieceType piece_type() override;
};

class Bishop : public Piece {
    std::vector<Move> possible_moves(Board &board, Point from) override;
    PieceType piece_type() override;
};

class Rook : public Piece {
    std::vector<Move> possible_moves(Board &board, Point from) override;
    PieceType piece_type() override;
};

class Queen : public Piece {
    std::vector<Move> possible_moves(Board &board, Point from) override;
    PieceType piece_type() override;
};

class King : public Piece {
    std::vector<Move> possible_moves(Board &board, Point from) override;
    PieceType piece_type() override;
};

#endif
