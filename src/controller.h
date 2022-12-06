#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "ai.h"
#include "game.h"
#include "observer.h"
#include "setup.h"
#include "chucknorrisbot.h"
class Setup;
class Game;
class Subject;

// The main controller class for the program
// Handles all commands
class Controller : public Subject {
    // Chess setup object
    Setup setup;
    // The number of games that white has won
    int white_wins;
    // The number of games that black has won
    int black_wins;
    // The number of drawn games
    int draws;
    // The bot currently used by the white player (null for human player)
    std::unique_ptr<Bot> white_bot;
    // The bot currently used by the black player (null for human player)
    std::unique_ptr<Bot> black_bot;
    // Enable bonus features
    bool enable_bonus;

    // Command loop for setup
    void run_setup();
    // Command loop for gameplay
    void run_game();

  public:
    // The current game state
    Game game;

    // Default constructor for the controller
    Controller(bool enable_bonus = false);
    // Outermost text command loop, can choose to setup or start game
    void command_loop();
};

#endif
