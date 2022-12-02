#include "assets.h"
#include "controller.h"
#include "testing.h"
#include "textui.h"
#include <iostream>
#include "ai.h"

int main(int argc, char *argv[]) {
    if (argc >= 2 && std::string(argv[1]) == "--test") {
        if (argc == 2) {
            test_run();
        } else if (argc == 4 && std::string(argv[2]) == "gen-moves") {
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
    // Controller c;
    // TextUi u(c);
    // c.command_loop();
    BumblingBuffoonBot bot;

    // // Test window
    // Xwindow win(500, 500);

    // win.drawImage(0, 0, BOARD);
    // for (int i = 0; i < 8; i++) {
    //     for (int j = 0; j < 8; j++) {
    //         win.drawImage(50 + i * 50, 50 + j * 50, BLACK_KING);
    //     }
    // }
    // win.flush();
    // getchar();
    // return 0;
}
