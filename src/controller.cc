#include "controller.h"
#include "ai.h"
#include "debug.h"
#include <iostream>
#include <regex>
#include <sstream>

static std::unique_ptr<Bot> bot_from_string(std::string s) {
    if (s == "human") {
        return nullptr;
    } else if (s == "computer1") {
        return std::unique_ptr<Bot>(new BumblingBuffoonBot());
    } else if (s == "computer2") {
        return std::unique_ptr<Bot>(new SlightlyCompetentBot());
    } else if (s == "computer3") {
        return std::unique_ptr<Bot>(new AverageCsStudentBot());
    } else if (s == "computer4") {
        return std::unique_ptr<Bot>(new ChuckNorrisBot());
    } else {
        throw std::runtime_error("invalid player type string: " + s);
    }
}

static bool prompt(std::string &line) {
    std::cout << "> ";
    return !std::getline(std::cin, line).fail();
}

Controller::Controller(bool enable_bonus)
    : setup(), white_wins(0), black_wins(0), draws(0),
      enable_bonus(enable_bonus), game() {
}

void Controller::run_setup() {
    setup.remove_rights();

    game = setup.finish();

    notify_observers(*this);

    std::string line;
    while (prompt(line)) {
        std::stringstream ss{line};
        std::string command;
        ss >> command;
        if (command == "done") {
            // Checks to ensure setup is valid
            if (game.board.illegal_kings()) {
                std::cout << "Illegal king count!\n";
                continue;
            }
            if (game.board.illegal_pawns()) {
                std::cout << "Illegal pawn placement!\n";
                continue;
            }
            if (!game.board.is_check()) {
                Board c = game.board.clone();
                c.to_move = (c.to_move == Player::Black) ? Player::White
                                                         : Player::Black;
                if (c.is_check()) {
                    std::cout << "No kings can be in check!\n";
                    continue;
                }
            } else {
                std::cout << "No kings can be in check!\n";
                continue;
            }
            break;
        }
        if (command == "=") {
            std::string col;
            ss >> col;
            if (col == "black" || col == "Black") {
                setup.set_to_move(Player::Black);
                game = setup.finish();
                notify_observers(*this);
            } else if (col == "white" || col == "White") {
                setup.set_to_move(Player::White);
                game = setup.finish();
                notify_observers(*this);
            } else {
                std::cout << "Invalid color!\n";
            }
        } else if (command == "+") {
            char p;
            std::string pos;
            if (!(ss >> p) || !(ss >> pos)) {
                std::cout << "Invalid format!\n";
                continue;
            }
            try {
                if (p <= 90) {
                    setup.add_piece(Point(pos),
                                    piece_type_from_char(tolower(p)),
                                    Player::White);
                } else {
                    setup.add_piece(Point(pos), piece_type_from_char(p),
                                    Player::Black);
                }
                game = setup.finish();
                notify_observers(*this);
            } catch (...) {
                std::cout << "Invalid piece or position!\n";
            }
        } else if (command == "-") {
            std::string pos;
            ss >> pos;
            try {
                setup.remove_piece(Point(pos));
                game = setup.finish();
                notify_observers(*this);
            } catch (...) {
                std::cout << "Invalid position!\n";
            }
        } else {
            std::cout << "Invalid command!\n";
        }
    }
}

void Controller::run_game() {
    game = setup.finish();

    notify_observers(*this);

    std::string line;
    std::smatch result;
    while (prompt(line)) {
        bool success = false;
        static std::regex move_regex = std::regex(
            "^(?:move\\s+)?([a-h][1-8])\\s*([a-h][1-8])\\s*([nbrq]|)");
        static std::regex move_san_regex = std::regex(
            "^(?:move\\s+)?([0oO]-[0oO](?:-[0oO])?|[nbrqkNBRQK]?[a-h]?["
            "1-8]?x?[a-h][1-8](?:=?[nbrqNBRQ])?)");
        if (std::regex_match(line, result, move_regex)) {
            std::unique_ptr<Bot> &bot =
                game.board.to_move == Player::White ? white_bot : black_bot;
            if (bot != nullptr) {
                std::cout << "Expected bot move" << std::endl;
                continue;
            }
            auto iter = ++result.begin();
            std::string from_str = *(iter++);
            std::string to_str = *(iter++);
            std::string promote_str = *(iter++);
            Move move;
            try {
                move = Move(from_str + to_str + promote_str);
            } catch (...) {
                std::cout << "Invalid move" << std::endl;
            }
            bool found = false;
            for (auto m : game.board.legal_moves()) {
                if (m == move) {
                    found = true;
                    break;
                }
            }
            if (found) {
                game.make_move(move);
                success = true;
            } else {
                std::cout << "Illegal move" << std::endl;
            }
        } else if (enable_bonus &&
                   std::regex_match(line, result, move_san_regex)) {
            std::unique_ptr<Bot> &bot =
                game.board.to_move == Player::White ? white_bot : black_bot;
            if (bot != nullptr) {
                std::cout << "Expected bot move" << std::endl;
                continue;
            }
            auto iter = ++result.begin();
            std::string san_str = *(iter++);
            Move move;
            try {
                move = Move(san_str, game.board);
            } catch (std::exception &e) {
                std::cout << "Invalid move: " << e.what() << std::endl;
            }
            bool found = false;
            for (auto m : game.board.legal_moves()) {
                if (m == move) {
                    found = true;
                    break;
                }
            }
            if (found) {
                game.make_move(move);
                success = true;
            }else {
                std::cout << "Illegal move" << std::endl;
            }
        } else if (line == "move" || (enable_bonus && line == "")) {
            std::unique_ptr<Bot> &bot =
                game.board.to_move == Player::White ? white_bot : black_bot;
            if (bot == nullptr) {
                std::cout << "Expected human move" << std::endl;
                continue;
            }
            std::cout << "Bot is thinking..." << std::endl;
            Move move = bot->best_move(game);

            bool found = false;
            for (auto m : game.board.legal_moves()) {
                if (m == move) {
                    found = true;
                }
            }
            if (found == false) {
                std::cout << "Warning! Bot made an illegal move: " << move.uci()
                          << std::endl;
            } else {
                game.make_move(move);
                success = true;
            }
        } else if (enable_bonus && line == "undo") {
            try {
                game.undo_move();
                success = true;
            } catch (...) {
                std::cout << "No moves to undo" << std::endl;
            }
        } else if (line == "resign") {
            game.resigned = true;
            if (game.board.to_move == Player::White) {
                std::cout << "White resigns" << std::endl;
                black_wins++;
            } else {
                std::cout << "Black resigns" << std::endl;
                white_wins++;
            }
            notify_observers(*this);
            break;
        } else if (line == "quit" || line == "q") {
            // Abort the current game without setting a winner/loser
            std::cout << "Quitting current game" << std::endl;
            break;
        } else {
            std::cout << "Invalid command" << std::endl;
        }

        if (success) {
            notify_observers(*this);

            if (game.board.is_checkmate()) {
                if (game.board.to_move == Player::White) {
                    black_wins++;
                } else {
                    white_wins++;
                }
                break;
            }
            if (game.board.is_stalemate()) {
                draws++;
                break;
            }
        }
    }
}

void Controller::command_loop() {
    std::cout << "Starting Chess Program" << std::endl;

    std::string line;
    std::smatch result;
    while (prompt(line)) {
        static std::regex game_regex = std::regex(
            "^game\\s+(human|computer[1-4])\\s+(human|computer[1-4])");
        if (line == "setup") {
            std::cout << "Entering Setup Mode" << std::endl;
            run_setup();
        } else if (std::regex_match(line, result, game_regex)) {
            auto iter = ++result.begin();
            white_bot = bot_from_string(*(iter++));
            black_bot = bot_from_string(*(iter++));
            std::cout << "Starting Game" << std::endl;
            run_game();
            std::cout << "Score:\nWhite: " << white_wins + (draws / 2)
                      << "\nBlack: " << black_wins + (draws / 2) << std::endl;
        } else if (line == "q" || line == "quit") {
            break;
        } else {
            std::cout << "Invalid command" << std::endl;
        }
    }
    std::cout << "\n\nFinal Score:\nWhite: " << white_wins + (draws / 2)
              << "\nBlack: " << black_wins + (draws / 2) << std::endl;
}
