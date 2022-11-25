#include "board.h"
#include "piece.h"
#include "point.h"

std::ostream &operator<<(std::ostream &s, const Board &b);

char get_char(PieceType p);

Board Board::clone() {
    std::unique_ptr<Piece> new_pieces[8][8];
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            new_pieces[i][j] = pieces[i][j]->clone();
        }
    }
    Point new_en_passent_square = en_passent_square;
    bool new_can_castle[4];
    for (int i = 0; i < 4; i++) {
        new_can_castle[i] = can_castle[i];
    }
    return Board(new_pieces, new_en_passent_square, new_can_castle);
}