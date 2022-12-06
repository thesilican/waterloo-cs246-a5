#include "testbot2.h"

TestBot2::TestBot2(Board& b) : b{b} {}

// assumes depth > 0, and that the current position has > 0 legal moves
Move TestBot2::ab_driver(int alpha, int beta, int depth) {
    std::vector<Move> moves = b.legal_moves();
    Move best = moves[0];
    for (auto m : moves) {
        Board c = b.clone();
        c.make_move(m);
        TestBot2 t(c);
        int eval = -t.alpha_beta(-beta, -alpha, depth-1);
        if (eval > alpha) {
            alpha = eval;
            best = m;
        }
        if (eval >= beta) break;
    }
    return best;
}

int TestBot2::alpha_beta(int alpha, int beta, int depth) {
    if (depth == 0) return evaluate(depth);
    std::vector<Move> moves = b.legal_moves();
    if (moves.size() == 0) {
        if (b.in_check()) return -10000-depth;
        return 0;
    }
    for (auto m : moves) {
        Board c = b.clone();
        c.make_move(m);
        TestBot2 t(c);
        int eval = -t.alpha_beta(-beta, -alpha, depth-1);
        if (eval >= beta) return beta;
        if (eval > alpha) alpha = eval;
    }
}

int TestBot2::evaluate(int depth) {
    int ev = 0;
    int turn = (b.to_move == Player::White) ? 1 : -1;
    if (b.is_checkmate()) return -10000-depth;
    if (b.is_stalemate()) return 0;
    if (b.in_check()) ev -= 20;
    
    ev += turn*b.possible_moves().size();
    Board op = b.clone();
    op.en_passent_square = Point(-1,-1);
    op.can_castle[0] = false;
    op.can_castle[1] = false;
    op.can_castle[2] = false;
    op.can_castle[3] = false;
    op.to_move = (b.to_move == Player::White) ? Player::Black : Player::White; 
    ev -= turn*op.possible_moves().size();

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Point p{i,j};
            std::unique_ptr<Piece>& piece = b.get(p);
            if (piece == nullptr) continue;
            int col = (piece->player == Player::White) ? 1 : -1;
            PieceType t = piece->piece_type();
            if (t == PieceType::Bishop || t == PieceType::Knight) {
                ev += turn*col*30;
            } else if (t == PieceType::Pawn) {
                ev += turn*col*10;
            } else if (t == PieceType::Queen) {
                ev += turn*col*90;
            } else if (t == PieceType::Rook) {
                ev += turn*col*50;
            }
        }
    }

    return ev;
}
