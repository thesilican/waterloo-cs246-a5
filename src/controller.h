#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "game.h"
#include "observer.h"
#include "setup.h"
#include "testbot.h"
#include "testbot2.h"
#include "ai.h"
class Setup;
class Game;
class Subject;

class Controller : public Subject {
    void run_setup();
    // Text command loop for game
    void run_game();
    void do_game_command(std::string command);

  public:
    Controller();

    std::unique_ptr<Bot> white_bot;
    std::unique_ptr<Bot> black_bot;

    Setup setup;
    Game game;
    int white_wins;
    int black_wins;
    int draws;
    // Outermost text command loop, can choose to setup or start game
    void command_loop();
};

#endif
