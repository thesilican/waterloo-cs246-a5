#include "textui.h"
#include "debug.h"

TextUi::TextUi(Subject &s, bool use_unicode, bool show_debug)
    : Observer(s), use_unicode(use_unicode), show_debug(show_debug) {
}

std::string TextUi::square_char(std::unique_ptr<Piece> &piece, Point p) {
    if (piece == nullptr) {
        return use_unicode ? "·" : ((p.x + p.y) % 2) ? "_" : " ";
    } else if (piece->player == Player::White) {
        if (piece->piece_type() == PieceType::Pawn) {
            return use_unicode ? "♟︎" : "P";
        } else if (piece->piece_type() == PieceType::Knight) {
            return use_unicode ? "♞" : "N";
        } else if (piece->piece_type() == PieceType::Bishop) {
            return use_unicode ? "♝" : "B";
        } else if (piece->piece_type() == PieceType::Rook) {
            return use_unicode ? "♜" : "R";
        } else if (piece->piece_type() == PieceType::Queen) {
            return use_unicode ? "♛" : "Q";
        } else {
            return use_unicode ? "♚" : "K";
        }
    } else {
        if (piece->piece_type() == PieceType::Pawn) {
            return use_unicode ? "♙" : "p";
        } else if (piece->piece_type() == PieceType::Knight) {
            return use_unicode ? "♘" : "n";
        } else if (piece->piece_type() == PieceType::Bishop) {
            return use_unicode ? "♗" : "b";
        } else if (piece->piece_type() == PieceType::Rook) {
            return use_unicode ? "♖" : "r";
        } else if (piece->piece_type() == PieceType::Queen) {
            return use_unicode ? "♕" : "q";
        } else {
            return use_unicode ? "♔" : "k";
        }
    }
}

void TextUi::print_game(Game &game) {
    for (int j = 7; j >= 0; j--) {
        std::cout << (j + 1) << " ";
        for (int i = 0; i < 8; i++) {
            std::unique_ptr<Piece> &p = game.board.get(Point(i, j));
            std::cout << square_char(p, Point(i, j));
        }
        std::cout << std::endl;
    }
    std::cout << "\n  abcdefgh" << std::endl;

    if (show_debug) {
        if (game.board.to_move == Player::White) {
            std::cout << "w";
        } else {
            std::cout << "b";
        }

        std::cout << " ";

        char lookup[] = "KQkq";
        for (int i = 0; i < 4; i++) {
            if (game.board.can_castle[i]) {
                std::cout << lookup[i];
            } else {
                std::cout << "-";
            }
        }

        std::cout << " ";

        if (game.board.en_passent_square.in_bounds()) {
            std::cout << game.board.en_passent_square.algebraic();
        } else {
            std::cout << "-";
        }
        std::cout << std::endl;
    }
}

void TextUi::notify(Controller &controller) {
    std::cout << std::endl;
    print_game(controller.game);
    if (controller.game.resigned) {
        if (controller.game.board.to_move == Player::Black) {
            // black resigned
            std::cout << "White wins!\n";
        } else {
            // white resigned
            std::cout << "Black wins!\n";
        }
    } else if (controller.game.board.is_checkmate()) {
        if (controller.game.board.to_move == Player::Black) {
            // black in checkmate
            std::cout << "Checkmate! White wins!\n";
        } else {
            // white in checkmate
            std::cout << "Checkmate! Black wins!\n";
        }
    } else if (controller.game.board.is_stalemate()) {
        // stalemate
        std::cout << "Stalemate!\n";
    } else if (controller.game.board.in_check()) {
        if (controller.game.board.to_move == Player::Black) {
            // black in check
            std::cout << "Black is in check.\n";
        } else {
            // white in check
            std::cout << "White is in check.\n";
        }
    }
    std::cout << std::endl;
}