#include "testbot.h"
#include <iostream>
#include <vector>
#include <memory>
#include <cctype>

TestBot::TestBot() {
    int_form['P'] = 0;
    int_form['N'] = 1;
    int_form['B'] = 2;
    int_form['R'] = 3;
    int_form['Q'] = 4;
    int_form['K'] = 5;
    int_form['p'] = 6;
    int_form['n'] = 7;
    int_form['b'] = 8;
    int_form['r'] = 9;
    int_form['q'] = 10;
    int_form['k'] = 11;
    int_form['*'] = 12;

    char_form[0] = 'P';
    char_form[1] = 'N';
    char_form[2] = 'B';
    char_form[3] = 'R';
    char_form[4] = 'Q';
    char_form[5] = 'K';
    char_form[6] = 'p';
    char_form[7] = 'n';
    char_form[8] = 'b';
    char_form[9] = 'r';
    char_form[10] = 'q';
    char_form[11] = 'k';
    char_form[12] = '*';
}

TestBot::TestBot(Board& b) : TestBot() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Point p{i,j};
            std::unique_ptr<Piece>& ptr = b.get(p);
            if (ptr != nullptr) {
                board[i][j] = ptr->to_char();
            } else {
                board[i][j] = '*';
            }
        }
    }
    castling[0] = b.can_castle[1];
    castling[1] = b.can_castle[0];
    castling[2] = b.can_castle[3];
    castling[3] = b.can_castle[2];
    for (int i = 0; i < 8; i++) {
        en_passant_good[i] = false;
    }
    Point ep = b.en_passent_square;
    if (ep.in_bounds()) {
        en_passant_good[ep.x] = true;
    }
    if (b.to_move == Player::Black) {
        black_turn = true;
    } else {
        black_turn = false;
    }
}

Board TestBot::get_board_object() {
    std::unique_ptr<Piece> pieces[8][8];
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] == '*') {
                pieces[i][j] = nullptr;
            } else {
                pieces[i][j] = piece_from_char(board[i][j]);
            }
        }
    }
    Player to_move = (black_turn) ? Player::Black : Player::White;
    Point ep{-1,-1};
    for (int i = 0; i < 8; i++) {
        if (en_passant_good[i]) {
            ep.x = i;
            ep.y = (black_turn) ? 2 : 5;
            break;
        }
    }
    bool new_castle[4];
    new_castle[0] = castling[1];
    new_castle[1] = castling[0];
    new_castle[2] = castling[3];
    new_castle[3] = castling[2];
    return Board{pieces,to_move,new_castle,ep,0,1};
}

Move TestBot::uncompress_move(unsigned short m) {
    Point from = get_from(m);
    Point to = get_to(m);
    char p = get_promote(m);
    if (p != '*') {
        PieceType pr = piece_type_from_char(tolower(p));
        return Move{from,to,pr};
    }
    return Move{from,to};
}

unsigned short TestBot::compress_move(Move m) {
    if (!m.has_promotes_to) {
        return get_move(m.from, m.to, '*');
    }
    char p = piece_type_to_char(m.promotes_to);
    if (piece_color(m.from,'w')) {
        p = toupper(p);
    }
    return get_move(m.from,m.to,p);
}

unsigned short TestBot::get_move(Point from, Point to, char promote) {
    unsigned short pr = int_form[promote];
    unsigned short m = from.x << 13;
    m |= (from.y << 10);
    m |= (to.x << 7);
    m |= (to.y << 4);
    m |= pr;
    return m;
}

char TestBot::get_promote(unsigned short move) {
    return char_form[move & ((1 << 4) - 1)];
}

Point TestBot::get_from(unsigned short move) {
    move = (move >> 10);
    int y = (move & ((1 << 3) - 1));
    move = (move >> 3);
    return Point(move,y);
}

Point TestBot::get_to(unsigned short move) {
    move = (move >> 4);
    int y = (move & ((1 << 3) - 1));
    move = ((move >> 3) & ((1 << 3) - 1));
    return Point(move,y);
}

//maybe need en passant and castling ints (bit manip)??
void TestBot::move(unsigned short move) {
    black_turn = !black_turn;
    Point from = get_from(move);
    Point to = get_to(move);
    char pr = get_promote(move);
    char piece = board[from.x][from.y];

    //update castling array
    if (castling[0] == 1) {
        if (piece == 'K' || (piece == 'R' && from.x == 0)) {
            castling[0] = 0;
        }
    }
    if (castling[1] == 1) {
        if (piece == 'K' || (piece == 'R' && from.x == 7)) {
            castling[1] = 0;
        }
    }
    if (castling[2] == 1) {
        if (piece == 'k' || (piece == 'r' && from.x == 0)) {
            castling[2] = 0;
        }
    }
    if (castling[3] == 1) {
        if (piece == 'k' || (piece == 'r' && from.x == 7)) {
            castling[3] = 0;
        }
    }

    //update en passant array
    for (int i = 0; i < 8; i++) {
        en_passant_good[i] = false;
    }
    if (piece == 'p' && from.y == 6 && to.y == 4) {
        Point left{to.x - 1, to.y};
        Point right{to.x + 1, to.y};
        if ((left.in_bounds() && (board[left.x][left.y] == 'P')) || (right.in_bounds() && (board[right.x][right.y] == 'P'))) {
            en_passant_good[to.x] = true;
        }
    } else if (piece == 'P' && from.y == 1 && to.y == 3) {
        Point left{to.x - 1, to.y};
        Point right{to.x + 1, to.y};
        if ((left.in_bounds() && (board[left.x][left.y] == 'p')) || (right.in_bounds() && (board[right.x][right.y] == 'p'))) {
            en_passant_good[to.x] = true;
        }        
    }

    //update board
    if (piece == 'p' || piece == 'P') {
        if (from.x != to.x && board[to.x][to.y] == '*') {
            //en passant
            board[from.x][from.y] = '*';
            board[to.x][to.y] = piece;
            board[to.x][from.y] = '*';
            return;
        } else if (to.y == 7 || to.y == 0) {
            //pawn promotion
            board[from.x][from.y] = '*';
            board[to.x][to.y] = pr;
            return;
        }
    } else if (piece == 'k' || piece == 'K') {
        if (from.x - to.x == 2 || from.x - to.x == -2) {
            //castling
            if (from.x - to.x == 2) {
                //queen side
                board[from.x][from.y] = '*';
                board[to.x][to.y] = piece;
                board[3][to.y] = board[0][to.y];
                board[0][to.y] = '*';
            } else {
                //king side
                board[from.x][from.y] = '*';
                board[to.x][to.y] = piece;
                board[5][to.y] = board[7][to.y];
                board[7][to.y] = '*';
            }
            return;
        }
    } 
    board[from.x][from.y] = '*';
    board[to.x][to.y] = piece;
}

//doesn't undo castling and enpassant info
void TestBot::undo_move(unsigned short move, bool is_enpassant, char captured) {
    Point from = get_from(move);
    Point to = get_to(move);
    char pr = get_promote(move);
    char piece;
    if (pr == '*') {
        piece = board[to.x][to.y];
    } else {
        if (black_turn) {
            piece = 'P';
        } else {
            piece = 'p';
        }
    }
    black_turn = !black_turn;
    if ((piece == 'K' || piece == 'k') && (from.x - to.x == 2)) {
        //queen side castle
        board[to.x][to.y] = '*';
        board[from.x][from.y] = piece;
        board[0][to.y] = board[3][to.y];
        board[3][to.y] = '*';
        return;
    }
    if ((piece == 'K' || piece == 'k') && (from.x - to.x == -2)) {
        //king side castle
        board[to.x][to.y] = '*';
        board[from.x][from.y] = piece;
        board[7][to.y] = board[5][to.y];
        board[5][to.y] = '*';        
        return;
    }
    if (is_enpassant) {
        //en passant
        board[to.x][to.y] = '*';
        board[from.x][from.y] = piece;
        board[to.x][from.y] = captured;
        return;
    }
    board[to.x][to.y] = captured;
    board[from.x][from.y] = piece;
}

//color is 'b' or 'w', returns whether piece at "at" is of that color
bool TestBot::piece_color(Point at, char color) {
    if (color == 'b') {
        return ((int_form[board[at.x][at.y]] >= 6) && (int_form[board[at.x][at.y]] <= 11));
    } else {
        return (int_form[board[at.x][at.y]] < 6);   
    }
}

bool TestBot::empty_square(Point at) {
    return (board[at.x][at.y] == '*');
}

std::vector<unsigned short> TestBot::moves_pawn(Point at) {
    std::vector<unsigned short> moves;
    int promo, t, one_up, start, ep;
    char color, queen, knight;
    if (board[at.x][at.y] == 'P') {
        //white pawn
        promo = 6;
        t = 7;
        color = 'b';
        one_up = 1;
        start = 1;
        ep = 4;
        queen = 'Q';
        knight = 'N';
    } else {
        //black pawn
        promo = 1;
        t = 0;
        color = 'w';
        one_up = -1;
        start = 6;
        ep = 3;
        queen = 'q';
        knight = 'n';
    }
    if (at.y == promo) {
        //pawn promotion
        Point top{at.x, t};
        if (empty_square(top)) {
            moves.push_back(get_move(at,top,queen));
            moves.push_back(get_move(at,top,knight));
        }
        Point left{at.x - 1, t};
        Point right{at.x + 1, t};
        if (left.in_bounds() && piece_color(left,color)) {
            moves.push_back(get_move(at,left,queen));
            moves.push_back(get_move(at,left,knight));
        }
        if (right.in_bounds() && piece_color(right,color)) {
            moves.push_back(get_move(at,right,queen));
            moves.push_back(get_move(at,right,knight));
        }
        return moves;
    }
    Point oneup_square{at.x, at.y + one_up};
    Point twoup_square{at.x, at.y + 2*one_up};
    if (empty_square(oneup_square)) {
        moves.push_back(get_move(at,oneup_square,'*'));
    }
    if ((at.y == start) && empty_square(oneup_square) && empty_square(twoup_square)) {
        moves.push_back(get_move(at,twoup_square,'*'));
    }
    Point left{at.x - 1, at.y + one_up};
    Point right{at.x + 1, at.y + one_up};
    if (left.in_bounds()) {
        if (piece_color(left,color) || ((at.y == ep) && en_passant_good[left.x])) {
            moves.push_back(get_move(at,left,'*'));
        }
    }
    if (right.in_bounds()) {
        if (piece_color(right,color) || ((at.y == ep) && en_passant_good[right.x])) {
            moves.push_back(get_move(at,right,'*'));
        }
    }
    return std::move(moves);
}

std::vector<unsigned short> TestBot::jumpers(Point at, const std::vector<Point>& targets) {
    char color;
    if (piece_color(at,'w')) {
        //white piece
        color = 'b';
    } else {
        //black piece
        color = 'w';
    }
    std::vector<unsigned short> moves;
    for (const auto& p : targets) {
        Point dest = at + p;
        if (!dest.in_bounds()) continue;
        if (empty_square(dest) || piece_color(dest,color)) {
            moves.push_back(get_move(at,dest,'*'));
        }
    }
    return moves;
}

std::vector<unsigned short> TestBot::riders(Point at, const std::vector<Point>& directions) {
    char color;
    if (piece_color(at,'w')) {
        //white piece
        color = 'b';
    } else {
        //black piece
        color = 'w';
    }
    std::vector<unsigned short> moves;
    for (const auto& p : directions) {
        Point dest = at;
        while (true) {
            dest = dest + p;
            if (!dest.in_bounds()) break;
            if (empty_square(dest)) {
                moves.push_back(get_move(at,dest,'*'));
            } else if (piece_color(dest,color)) {
                moves.push_back(get_move(at,dest,'*'));
                break;
            } else {
                break;
            }
        }
    }
    return moves;
}

void TestBot::copy_array(bool from[], bool to[], int size) {
    for (int i = 0; i < size; i++) {
        to[i] = from[i];
    }
}
// need new function for castle_moves that comes prechecked to ensure king isn't under check due to move
std::vector<unsigned short> TestBot::moves_king(Point at) {
    std::vector<Point> targets = {Point(1,1),
                            Point(0,1),
                            Point(-1,1),
                            Point(-1,0),
                            Point(-1,-1),
                            Point(0,-1),
                            Point(1,-1),
                            Point(1,0)};
    return std::move(jumpers(at,targets));
}

//returns legal castling moves for king at "at"
std::vector<unsigned short> TestBot::moves_castle(Point at) {
    int arr_index, row;
    bool black;
    if (board[at.x][at.y] == 'K') {
        arr_index = 0;
        row = 0;
        black = false;
    } else {
        arr_index = 2;
        row = 7;
        black = true;
    }
    std::vector<unsigned short> moves;
    for (int k = 0; k < 2; k++) {
        int bw_start, bw_end, start, end;
        if (k == 0) {
            bw_start = 1;
            bw_end = 3;
            start = 4;
            end = 2;
        } else {
            bw_start = 5;
            bw_end = 6;
            start = 6;
            end = 4;
        }
        if (castling[k+arr_index]) {
            bool legal = true;
            for (int i = bw_start; i <= bw_end; i++) {
                if (!empty_square(Point(i,row))) {
                    legal = false;
                    break;
                }
            }
            if (legal) {
                bool old_castling[4];
                bool old_enpassant[8];
                copy_array(castling,old_castling,4);
                copy_array(en_passant_good,old_enpassant,8);
                for (int i = start; i >= end; i--) {
                    unsigned short m = get_move(at,Point(i,row),'*');
                    move(m);
                    if (is_check(black)) {
                        legal = false;
                        undo_move(m,false,'*');
                        break;
                    }
                    undo_move(m,false,'*');
                }
                copy_array(old_castling,castling,4);
                copy_array(old_enpassant,en_passant_good,8);
            }
            if (legal) {
                moves.push_back(get_move(at,Point(bw_start+1,row),'*'));
            }
        }
    }
    return std::move(moves);
}

std::vector<unsigned short> TestBot::moves_knight(Point at) {
    std::vector<Point> targets = {Point(2,1),
                            Point(1,2),
                            Point(-1,2),
                            Point(-2,1),
                            Point(-2,-1),
                            Point(-1,-2),
                            Point(1,-2),
                            Point(2,-1)};
    return std::move(jumpers(at, targets));
}

std::vector<unsigned short> TestBot::moves_rook(Point at) {
    std::vector<Point> directions = {Point(1,0),
                                Point(0,1),
                                Point(0,-1),
                                Point(-1,0)};
    return std::move(riders(at, directions));
}

std::vector<unsigned short> TestBot::moves_bishop(Point at) {
    std::vector<Point> directions = {Point(1,1),
                                Point(-1,1),
                                Point(-1,-1),
                                Point(1,-1)};
    return std::move(riders(at, directions));
}
//get rid of std::move for copy elision

std::vector<unsigned short> TestBot::moves_queen(Point at) {
    std::vector<Point> directions = {Point(1,1),
                                Point(-1,1),
                                Point(-1,-1),
                                Point(1,-1),
                                Point(1,0),
                                Point(0,1),
                                Point(0,-1),
                                Point(-1,0)};
    return std::move(riders(at, directions));
}

std::vector<unsigned short> TestBot::moves_general(Point at) {
    char piece = board[at.x][at.y];
    if (piece == 'P' || piece == 'p') return std::move(moves_pawn(at));
    if (piece == 'R' || piece == 'r') return std::move(moves_rook(at));
    if (piece == 'N' || piece == 'n') return std::move(moves_knight(at));
    if (piece == 'B' || piece == 'b') return std::move(moves_bishop(at));
    if (piece == 'Q' || piece == 'q') return std::move(moves_queen(at));
    return std::move(moves_king(at));
}

//check if given color's king is under check
bool TestBot::is_check(bool black) {
    char color;
    if (black) {
        color = 'w';
    } else {
        color = 'b';
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Point pos{i,j};
            if (!empty_square(pos) && piece_color(pos,color)) {
                std::vector<unsigned short> moves = moves_general(pos);
                for (auto m : moves) {
                    Point to = get_to(m);
                    if (board[to.x][to.y] == 'K' || board[to.x][to.y] == 'k') return true;
                }
            }
        }
    }
    return false;
}

//maybe need to construct all moves vector in one go instead of combining multiple vectors??
std::vector<unsigned short> TestBot::all_moves() {
    char color;
    if (black_turn) {
        color = 'b';
    } else {
        color = 'w';
    }
    std::vector<unsigned short> moves;
    moves.reserve(80);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Point pos{i,j};
            if (!empty_square(pos) && piece_color(pos,color)) {
                std::vector<unsigned short> more_moves = moves_general(pos);
                for (auto e : more_moves) moves.push_back(e);
            }
        }
    }
    return std::move(moves);
}

void TestBot::is_proper_move(unsigned short m, std::vector<unsigned short>& moves) {
    Point to = get_to(m);
    Point from = get_from(m);
    char captured = board[to.x][to.y];
    bool ep = false;
    if (board[from.x][from.y] == 'P' || board[from.x][from.y] == 'p') {
        if (board[to.x][to.y] == '*' && (from.x != to.x)) {
            ep = true;
            captured = board[to.x][from.y];
        }
    }
    move(m);
    if (!is_check(!black_turn)) {
        moves.push_back(m);
    }
    undo_move(m,ep,captured);
}

std::vector<unsigned short> TestBot::legal_moves() {
    char color;
    if (black_turn) {
        color = 'b';
    } else {
        color = 'w';
    }
    std::vector<unsigned short> moves;
    moves.reserve(80);
    bool old_castling[4];
    bool old_enpassant[8];
    copy_array(castling,old_castling,4);
    copy_array(en_passant_good,old_enpassant,8);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Point pos{i,j};
            if (!empty_square(pos) && piece_color(pos,color)) {
                std::vector<unsigned short> more_moves = moves_general(pos);
                for (auto m : more_moves) is_proper_move(m,moves);
                if (board[pos.x][pos.y] == 'K' || board[pos.x][pos.y] == 'k') {
                    std::vector<unsigned short> castle_moves = moves_castle(pos);
                    for (auto m : castle_moves) moves.push_back(m);
                }
            }
        }
    }
    copy_array(old_castling,castling,4);
    copy_array(old_enpassant,en_passant_good,8);
    return std::move(moves);
}

//maybe lower penalties for doubled and isolated pawns
float TestBot::evaluate(int depth) {
    int turn;
    if (black_turn) {
        turn = -1;
    } else {
        turn = 1;
    }
    float ev = 0;
    int cur_player_legalmoves_size = all_moves().size();
    //if current player's king is in check
    if (is_check(black_turn)) {
        if (cur_player_legalmoves_size == 0) {
            //current player is checkmated.
            //return very low value MINUS depth so bot prefers earlier checkmates.
            return -1000 - depth;
        } else {
            ev -= 5;
        }
    }
    //stalemate
    if (cur_player_legalmoves_size == 0) {
        return 0; //stalemate should be neutral evaluation
    }

    //difference in mobilization
    int black_mob, white_mob;
    if (black_turn) {
        black_mob = cur_player_legalmoves_size;
    } else {
        white_mob = cur_player_legalmoves_size;
    }
    black_turn = !black_turn;
    bool old_ep[8];
    copy_array(en_passant_good,old_ep,8);
    for (int i = 0; i < 8; i++) en_passant_good[i] = false;
    if (black_turn) {
        black_mob = all_moves().size();
    } else {
        white_mob = all_moves().size();
    }
    copy_array(old_ep,en_passant_good,8);
    black_turn = !black_turn;
    ev += 0.1*(white_mob - black_mob);

    //difference in piece counts
    std::unordered_map<char,int> counts;
    unsigned short white_pawns[8];
    unsigned short black_pawns[8];
    int white_blocked = 0, black_blocked = 0, white_doubled = 0, black_doubled = 0, white_isolated = 0, black_isolated = 0;
    for (int i = 0; i < 8; i++) {
        white_pawns[i] = 0;
        black_pawns[i] = 0;
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] == 'p') {
                black_pawns[i]++;
                counts['p']++;
                if (board[i][j-1] != '*') black_blocked++;
            } else if (board[i][j] == 'P') {
                white_pawns[i]++;
                counts['P']++;
                if (board[i][j+1] != '*') white_blocked++;
            } else {
                counts[board[i][j]]++;
            }
        }
    }
    for (int i = 0; i < 8; i++) {
        if (white_pawns[i] > 1) {white_doubled += white_pawns[i];}
        if (black_pawns[i] > 1) {black_doubled += black_pawns[i];}
        if (white_pawns[i] > 0 && (i > 0 && white_pawns[i-1] == 0) && (i < 7 && white_pawns[i+1] == 0)) {white_isolated += white_pawns[i];}
        if (black_pawns[i] > 0 && (i > 0 && black_pawns[i-1] == 0) && (i < 7 && black_pawns[i+1] == 0)) {black_isolated += black_pawns[i];}
    }
    ev -= 0.5*(white_doubled - black_doubled + white_blocked - black_blocked + white_isolated - black_isolated);
    ev += 9*(counts['Q'] - counts['q']);
    ev += 5*(counts['R'] - counts['r']);
    ev += 3*(counts['B'] - counts['b'] + counts['N'] - counts['n']);
    ev += 1*(counts['P'] - counts['p']);
    return ev;
}

Eval TestBot::alpha_beta(float alpha, float beta, int depth) {
    if (depth == 0) {
        return Eval{0, evaluate(0)};
    }
    std::vector<unsigned short> moves = legal_moves();
    //have already checked for checkmate in legal_moves though??
    if (moves.size() == 0 && is_check(black_turn)) return Eval{0,1.0*(-1000-depth)};
    if (moves.size() == 0) return Eval{0,0};

    unsigned short best_move;
    Point to, from;
    char captured;
    bool ep;
    bool old_ep[8];
    bool old_castle[4];
    for (auto i : moves) {
        copy_array(en_passant_good,old_ep,8);
        copy_array(castling,old_castle,4);
        to = get_to(i);
        from = get_from(i);
        captured = board[to.x][to.y];
        ep = false;
        if (board[from.x][from.y] == 'P' || board[from.x][from.y] == 'p') {
            if (board[to.x][to.y] == '*' && (from.x != to.x)) {
                ep = true;
                captured = board[to.x][from.y];
            }
        }
        move(i);
        Eval p = alpha_beta(-beta, -alpha, depth-1);
        p.ev = -(p.ev);
        undo_move(i, ep, captured);
        copy_array(old_ep,en_passant_good,8);
        copy_array(old_castle,castling,4);
        if (p.ev >= beta) {
            return Eval{0, beta};
        }
        if (p.ev > alpha) {
            alpha = p.ev;
            best_move = i;
        }
    }
    return Eval{best_move,alpha};
}

void TestBot::print_board_debug() {
    for (int i = 7; i >= 0; i--) {
        std::cout << i << " ";
        for (int j = 0; j < 8; j++) {
            std::cout << board[j][i];
        }
        std::cout << '\n';
    }
    std::cout << "  ";
    for (int i = 0; i < 8; i++) std::cout << i;
    std::cout << '\n';
}

void TestBot::init_board_debug() {
    // board[0][0] = board[7][0] = 'R';
    // board[1][0] = board[6][0] = 'N';
    // board[2][0] = board[5][0] = 'B';
    // board[3][0] = 'Q';
    // board[4][0] = 'K';
    // board[0][7] = board[7][7] = 'r';
    // board[1][7] = board[6][7] = 'n';
    // board[2][7] = board[5][7] = 'b';
    // board[3][7] = 'q';
    // board[4][7] = 'k';
    // for (int i = 0; i < 8; i++) {
    //     board[i][1] = 'P';
    //     board[i][6] = 'p';
    // }
    // for (int i = 2; i < 6; i++) {
    //     for (int j = 0; j < 8; j++) {
    //         board[j][i] = '*';
    //     }
    // }
    char new_board[8][8] = {{'*','p','*','*','*','*','P','*'},
                            {'*','p','*','*','*','*','P','*'},
                            {'*','p','*','*','*','*','P','*'},
                            {'*','p','*','*','*','*','P','*'},
                            {'*','p','k','*','*','K','P','*'},
                            {'*','p','*','*','*','*','P','*'},
                            {'*','p','*','*','*','*','P','*'},
                            {'*','p','*','*','*','*','P','*'}};
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = new_board[i][j];
        }
    }
}
