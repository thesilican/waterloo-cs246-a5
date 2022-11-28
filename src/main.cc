#include "controller.h"
#include "testing.h"
#include "textui.h"
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc == 3 && std::string(argv[1]) == "--gen-moves") {
        test_gen_moves(std::string(argv[2]));
        return 0;
    }
    if (argc == 4 && std::string(argv[1]) == "--apply-move") {
        test_apply_move(std::string(argv[2]), std::string(argv[3]));
        return 0;
    }

    // Main loop
    Controller c;
    TextUi u(c);
    c.command_loop();
    return 0;
}
