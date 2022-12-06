#include "controller.h"
#include "ai.h"
#include "debug.h"
#include <iostream>
#include <regex>

std::unique_ptr<Bot> bot_from_string(std::string s) {
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

bool prompt(std::string &line) {
    std::cout << "> ";
    return !std::getline(std::cin, line).fail();
}

Controller::Controller()
    : setup(), game(), white_wins(0), black_wins(0), draws(0) {
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
        static std::regex move_regex =
            std::regex("^move\\s+([a-h][1-8])\\s*([a-h][1-8])\\s*([nbrq]|)");
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
                Point from(from_str);
                Point to(to_str);
                Move move(from, to);
                if (promote_str != "") {
                    PieceType piece = piece_type_from_char(promote_str[0]);
                    move = Move(from, to, piece);
                }
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
            Move move = bot->best_move(game);
            // Sanity check
            bool found = false;
            for (auto m : game.board.legal_moves()) {
                if (m == move) {
                    found = true;
                }
            }
            if (found == false) {
                throw std::runtime_error("bot made illegal move");
            }

            game.make_move(move);
            success = true;
        } else {
            std::cout << "Invalid command" << std::endl;
        }

        if (success) {
            notify_observers(*this);
        }
    }
}

void Controller::do_game_command(std::string command) {
    throw std::runtime_error("not implemented");
}

void Controller::command_loop() {
    std::cout << "Starting Chess Program" << std::endl;

    std::string line;
    std::smatch result;
    while (prompt(line)) {
        static std::regex game_regex = std::regex(
            "^game\\s+(human|computer[1-4])\\s+(human|computer[1-4])");
        if (line == "setup") {
            run_setup();
        } else if (std::regex_match(line, result, game_regex)) {
            auto iter = ++result.begin();
            white_bot = bot_from_string(*(iter++));
            black_bot = bot_from_string(*(iter++));
            std::cout << "Starting game" << std::endl;
            run_game();
        } else {
            std::cout << "Invalid command" << std::endl;
        }
    }
    std::cout << "\n\nFinal Score:\nWhite: " << white_wins + (draws / 2)
              << "\nBlack: " << black_wins + (draws / 2) << std::endl;
}
