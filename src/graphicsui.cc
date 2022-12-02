#include "graphicsui.h"
#include <utility>

GraphicsUi::GraphicsUi(Subject& s) : Observer(s) {
    win.drawImage(0, 0, BOARD);
    win.flush();
}

// assumes "piece" is not nullptr
PixelImg& get_image(std::unique_ptr<Piece>& p) {
    if (p->player == Player::White) {
        if (p->piece_type() == PieceType::Pawn) {
            return WHITE_PAWN;
        } else if (p->piece_type() == PieceType::Knight) {
            return WHITE_KNIGHT;
        } else if (p->piece_type() == PieceType::Bishop) {
            return WHITE_BISHOP;
        } else if (p->piece_type() == PieceType::Rook) {
            return WHITE_ROOK;
        } else if (p->piece_type() == PieceType::Queen) {
            return WHITE_QUEEN;
        } else {
            return WHITE_KING;
        }
    }
    // otherwise, black piece
    if (p->piece_type() == PieceType::Pawn) {
        return BLACK_PAWN;
    } else if (p->piece_type() == PieceType::Knight) {
        return BLACK_KNIGHT;
    } else if (p->piece_type() == PieceType::Bishop) {
        return BLACK_BISHOP;
    } else if (p->piece_type() == PieceType::Rook) {
        return BLACK_ROOK;
    } else if (p->piece_type() == PieceType::Queen) {
        return BLACK_QUEEN;
    } else {
        return BLACK_KING;
    }
}

void GraphicsUi::display_game(Game& game) {
    for (int j = 7; j >= 0; j--) {
        for (int i = 0; i < 8; i++) {
            std::unique_ptr<Piece>& p = game.board.get(Point(i,j));
            if (p == nullptr) continue;
            win.drawImage(50 + i*50, 50 + (7-j)*50, get_image(p));
        }
    }
    win.flush();
}

void GraphicsUi::notify(Controller& controller) {
    display_game(controller.game);
}
