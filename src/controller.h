#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "game.h"
#include "observer.h"
#include "setup.h"
class Setup;
class Game;
class Subject;

enum class ControllerState {
    Setup,
    InProgress,
    WhiteWin,
    BlackWin,
    Draw,
    Finished
};

class Controller : public Subject {
  public:
    Setup s;
    Game g;
    ControllerState state;
    int white_wins;
    int black_wins;
    int draws;
    bool command_loop_end = false;
    void command_loop(); // outermost text command loop, can choose to setup or
                         // start game
    void setup();
    void run_game(); // text command loop for game
    void do_game_command(std::string command);
};

#endif
