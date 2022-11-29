#include "controller.h"
#include "testing.h"
#include "textui.h"
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc >= 3 && std::string(argv[1]) == "--test") {
        if (argc == 4 && std::string(argv[2]) == "gen-moves") {
            test_gen_moves(std::string(argv[3]));
        } else if (argc == 5 && std::string(argv[2]) == "apply-move") {
            test_apply_move(std::string(argv[3]), std::string(argv[4]));
        } else if (argc == 4 && std::string(argv[2]) == "gen-moves-bot") {
            test_gen_moves_bot(std::string(argv[3]));
        } else if (argc == 5 && std::string(argv[2]) == "apply-move-bot") {
            test_apply_move_bot(std::string(argv[3]), std::string(argv[4]));
        } else {
            throw std::runtime_error("error parsing test flags");
        }
        return 0;
    }
    // Main loop
    Controller c;
    TextUi u(c);
    c.command_loop();
    return 0;
}
