#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "game.h"
#include "observer.h"
#include "setup.h"
#include "testbot.h"
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
    void run_setup();
    // Text command loop for game
    void run_game();
    void do_game_command(std::string command);

  public:
    Controller();

    Setup setup;
    Game game;
    ControllerState state;
    int white_wins;
    int black_wins;
    int draws;
    // Outermost text command loop, can choose to setup or start game
    void command_loop();
};

#endif
