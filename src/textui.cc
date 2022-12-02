#include "textui.h"
#include "debug.h"

TextUi::TextUi(Subject &s) : Observer(s) {
}

bool UNICODE_CHARS = true;

std::string TextUi::square_char(std::unique_ptr<Piece> &piece) {
    if (piece == nullptr) {
        return UNICODE_CHARS ? "·" : ".";
    } else if (piece->player == Player::White) {
        if (piece->piece_type() == PieceType::Pawn) {
            return UNICODE_CHARS ? "♟︎" : "P";
        } else if (piece->piece_type() == PieceType::Knight) {
            return UNICODE_CHARS ? "♞" : "N";
        } else if (piece->piece_type() == PieceType::Bishop) {
            return UNICODE_CHARS ? "♝" : "B";
        } else if (piece->piece_type() == PieceType::Rook) {
            return UNICODE_CHARS ? "♜" : "R";
        } else if (piece->piece_type() == PieceType::Queen) {
            return UNICODE_CHARS ? "♛" : "Q";
        } else {
            return UNICODE_CHARS ? "♚" : "K";
        }
    } else {
        if (piece->piece_type() == PieceType::Pawn) {
            return UNICODE_CHARS ? "♙" : "p";
        } else if (piece->piece_type() == PieceType::Knight) {
            return UNICODE_CHARS ? "♘" : "n";
        } else if (piece->piece_type() == PieceType::Bishop) {
            return UNICODE_CHARS ? "♗" : "b";
        } else if (piece->piece_type() == PieceType::Rook) {
            return UNICODE_CHARS ? "♖" : "r";
        } else if (piece->piece_type() == PieceType::Queen) {
            return UNICODE_CHARS ? "♕" : "q";
        } else {
            return UNICODE_CHARS ? "♔" : "k";
        }
    }
}

void TextUi::print_game(Game &game) {
    for (int j = 7; j >= 0; j--) {
        std::cout << (j + 1) << " ";
        for (int i = 0; i < 8; i++) {
            std::unique_ptr<Piece> &p = game.board.get(Point(i, j));
            std::cout << square_char(p);
        }
        std::cout << std::endl;
    }
    std::cout << "\n  abcdefgh" << std::endl;

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

void TextUi::notify(Controller &controller) {
    print_game(controller.game);
}