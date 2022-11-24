#ifndef SETUP_H
#define SETUP_H
class Move;
class Board;
enum class Player;

class SetUp {
public:
    Board board;
    Player to_move;
    void add_piece(Point point, Piece* piece);
    void remove_piece(Point point);
    void run_setup();
    void perform_command(string command);
}

#endif
