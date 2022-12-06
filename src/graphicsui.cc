#include "graphicsui.h"
#include <utility>

GraphicsUi::GraphicsUi(Subject& s) : Observer(s) {
    win.drawImage(0, 0, BOARD);
    win.flush();
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = '.';
        }
    }
}

// assumes "piece" is not nullptr
PixelImg& GraphicsUi::get_image(std::unique_ptr<Piece>& p) {
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
            if (p == nullptr) {
                if (board[i][j] == '.') continue;
                board[i][j] = '.';
                if ((i + j) % 2 == 0) {
                    win.drawImage(50 + i*50, 50 + (7-j)*50, ORANGE_SQUARE);
                    continue;
                }
                win.drawImage(50 + i*50, 50 + (7-j)*50, YELLOW_SQUARE);
                continue;
            }
            if (p->to_char() == board[i][j]) continue;
            if ((i + j) % 2 == 0) {
                win.drawImage(50 + i*50, 50 + (7-j)*50, ORANGE_SQUARE);
            } else {
                win.drawImage(50 + i*50, 50 + (7-j)*50, YELLOW_SQUARE);
            }
            win.drawImage(50 + i*50, 50 + (7-j)*50, get_image(p));
            board[i][j] = p->to_char();
        }
    }
    win.flush();
}

void GraphicsUi::notify(Controller& controller) {
    display_game(controller.game);
    if (controller.game.resigned) {
        if (controller.game.board.to_move == Player::Black) {
            // black resigned
            win.drawImage(0, 500, TEXT_WHITE_WINS);
        } else {
            // white resigned
            win.drawImage(0, 500, TEXT_BLACK_WINS);
        }
    } else if (controller.game.board.is_checkmate()) {
        if (controller.game.board.to_move == Player::Black) {
            // black in checkmate
            win.drawImage(0, 500, TEXT_WHITE_CHECKMATE);           
        } else {
            // white in checkmate
            win.drawImage(0, 500, TEXT_BLACK_CHECKMATE);  
        }
    } else if (controller.game.board.is_stalemate()) {
        // stalemate
        win.drawImage(0, 500, TEXT_STALEMATE);  
    } else if (controller.game.board.in_check()) {
        if (controller.game.board.to_move == Player::Black) {
            // black in check
            win.drawImage(0, 500, TEXT_BLACK_CHECK);  
        } else {
            // white in check
            win.drawImage(0, 500, TEXT_WHITE_CHECK);  
        }
    }
}
