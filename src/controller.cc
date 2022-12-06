#include "controller.h"
#include "ai.h"
#include "debug.h"
#include <iostream>
#include <regex>

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
        throw std::runtime_error("invalid playe type string: " + s);
    }
}

static bool prompt(std::string &line) {
    std::cout << "> ";
    return !std::getline(std::cin, line).fail();
}

Controller::Controller()
    : setup(), white_wins(0), black_wins(0), draws(0), game() {
}

void Controller::run_setup() {
    throw std::runtime_error("not implemented");
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
        static std::regex move_san_regex =
            std::regex("^(?:move\\s+)?([0O]-[0O](?:-[0O])|[nbrqkNBRQK]?[a-h]?["
                       "1-8]?x?[a-h][1-8](?:=?[nbrqNBRQ])?))");
        if (std::regex_match(line, result, move_regex)) {
            std::unique_ptr<Bot> &bot =
                game.board.to_move == Player::White ? white_bot : black_bot;
            if (bot != nullptr) {
                std::cout << "Expected bot move" << std::endl;
            }
            auto iter = ++result.begin();
            std::string from_str = *(iter++);
            std::string to_str = *(iter++);
            std::string promote_str = *(iter++);
            try {
                Move move(from_str + to_str + promote_str);
                game.make_move(move);
                success = true;
            } catch (...) {
                std::cout << "Invalid move" << std::endl;
            }
        } else if (std::regex_match(line, result, move_san_regex)) {
            std::unique_ptr<Bot> &bot =
                game.board.to_move == Player::White ? white_bot : black_bot;
            if (bot != nullptr) {
                std::cout << "Expected bot move" << std::endl;
            }
            auto iter = ++result.begin();
            std::string san_str = *(iter++);
            try {
                Move move(san_str, game.board);
                game.make_move(move);
                success = true;
            } catch (...) {
                std::cout << "Invalid move" << std::endl;
            }
        } else if (line == "move" || line == "") {
            std::unique_ptr<Bot> &bot =
                game.board.to_move == Player::White ? white_bot : black_bot;
            if (bot == nullptr) {
                std::cout << "Expected human move" << std::endl;
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
                std::cout << "Bot made an illegal move: " << move.uci()
                          << std::endl;
            } else {
                game.make_move(move);
                success = true;
            }
        } else if (line == "undo") {
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
