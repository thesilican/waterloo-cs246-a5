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
    return moves;
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
    return jumpers(at,targets);
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
    return moves;
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
    return jumpers(at, targets);
}

std::vector<unsigned short> TestBot::moves_rook(Point at) {
    std::vector<Point> directions = {Point(1,0),
                                Point(0,1),
                                Point(0,-1),
                                Point(-1,0)};
    return riders(at, directions);
}

std::vector<unsigned short> TestBot::moves_bishop(Point at) {
    std::vector<Point> directions = {Point(1,1),
                                Point(-1,1),
                                Point(-1,-1),
                                Point(1,-1)};
    return riders(at, directions);
}

std::vector<unsigned short> TestBot::moves_queen(Point at) {
    std::vector<Point> directions = {Point(1,1),
                                Point(-1,1),
                                Point(-1,-1),
                                Point(1,-1),
                                Point(1,0),
                                Point(0,1),
                                Point(0,-1),
                                Point(-1,0)};
    return riders(at, directions);
}

std::vector<unsigned short> TestBot::moves_general(Point at) {
    char piece = board[at.x][at.y];
    if (piece == 'P' || piece == 'p') return moves_pawn(at);
    if (piece == 'R' || piece == 'r') return moves_rook(at);
    if (piece == 'N' || piece == 'n') return moves_knight(at);
    if (piece == 'B' || piece == 'b') return moves_bishop(at);
    if (piece == 'Q' || piece == 'q') return moves_queen(at);
    return moves_king(at);
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

std::vector<unsigned short> TestBot::legal_moves() {
    char color;
    if (black_turn) {
        color = 'b';
    } else {
        color = 'w';
    }
    std::vector<unsigned short> moves;
    std::vector<unsigned short> castle_moves;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Point pos{i,j};
            if (!empty_square(pos) && piece_color(pos,color)) {
                std::vector<unsigned short> more_moves = moves_general(pos);
                moves.reserve(moves.size() + more_moves.size());
                moves.insert(moves.end(),more_moves.begin(),more_moves.end());
                if (board[pos.x][pos.y] == 'K' || board[pos.x][pos.y] == 'k') {
                    castle_moves = moves_castle(pos);
                }
            }
        }
    }
    std::vector<unsigned short> final_moves;
    final_moves.reserve(moves.size() + castle_moves.size());
    final_moves.insert(final_moves.end(),castle_moves.begin(),castle_moves.end());
    bool old_castling[4];
    bool old_enpassant[8];
    copy_array(castling,old_castling,4);
    copy_array(en_passant_good,old_enpassant,8);
    char captured;
    bool ep;
    Point to, from;
    for (auto m : moves) {
        to = get_to(m);
        from = get_from(m);
        captured = board[to.x][to.y];
        ep = false;
        if (board[from.x][from.y] == 'P' || board[from.x][from.y] == 'p') {
            if (board[to.x][to.y] == '*' && (from.x != to.x)) {
                ep = true;
                captured = board[to.x][from.y];
            }
        }
        move(m);
        if (!is_check(!black_turn)) {
            final_moves.push_back(m);
        }
        undo_move(m,ep,captured);
    }
    copy_array(old_castling,castling,4);
    copy_array(old_enpassant,en_passant_good,8);
    return final_moves;
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
    board[0][0] = board[7][0] = 'R';
    board[1][0] = board[6][0] = 'N';
    board[2][0] = board[5][0] = 'B';
    board[3][0] = 'Q';
    board[4][0] = 'K';
    board[0][7] = board[7][7] = 'r';
    board[1][7] = board[6][7] = 'n';
    board[2][7] = board[5][7] = 'b';
    board[3][7] = 'q';
    board[4][7] = 'k';
    for (int i = 0; i < 8; i++) {
        board[i][1] = 'P';
        board[i][6] = 'p';
    }
    for (int i = 2; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            board[j][i] = '*';
        }
    }
}