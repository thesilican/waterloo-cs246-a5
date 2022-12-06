#include "ai.h"
#include "assets.h"
#include "controller.h"
#include "graphicsui.h"
#include "testbot.h"
#include "testing.h"
#include "textui.h"
#include <iostream>

int main(int argc, char *argv[]) {
    // Parse test flags
    if (argc >= 2 && std::string(argv[1]) == "--test") {
        if (argc == 2) {
            test_run();
        } else if (argc == 4 && std::string(argv[2]) == "gen-moves") {
            test_gen_moves(std::string(argv[3]));
        } else if (argc == 4 && std::string(argv[2]) == "gen-moves-bot") {
            test_gen_moves_bot(std::string(argv[3]));
        } else if (argc == 5 && std::string(argv[2]) == "apply-move") {
            test_apply_move(std::string(argv[3]), std::string(argv[4]));
        } else if (argc == 5 && std::string(argv[2]) == "apply-move-bot") {
            test_apply_move_bot(std::string(argv[3]), std::string(argv[4]));
        } else if (argc == 4 && std::string(argv[2]) == "checks-state") {
            test_checks_state(std::string(argv[3]));
        } else if (argc == 4 && std::string(argv[2]) == "checks-state-bot") {
            test_checks_state_bot(std::string(argv[3]));
        } else {
            throw std::runtime_error("error parsing test flags");
        }
        return 0;
    }
    
    // Parse additional command line flags
    bool debug = false, unicode = false;
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--unicode") {
            unicode = true;
        } else if (std::string(argv[i]) == "--debug") {
            debug = true;
        }
    }

    Controller c;
    GraphicsUi g(c);
    TextUi t(c, unicode, debug);
    c.command_loop();

    return 0;
}
