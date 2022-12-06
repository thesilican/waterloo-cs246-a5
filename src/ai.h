#ifndef AI_H
#define AI_H
#include "game.h"
#include "move.h"
#include <climits>
#include <random>
class Move;
class Game;

class Bot {
  public:
    virtual Move best_move(Game &game) = 0;
    virtual ~Bot() = default;
};

class NoMovesException : public std::exception {
  public:
    NoMovesException() = default;
    virtual ~NoMovesException() throw() = default;
    virtual const char *what() const throw();
};

// Bot Level 1
// Makes random moves
class BumblingBuffoonBot : public Bot {
    std::random_device rd;
    std::mt19937 mt{1234};
    std::uniform_int_distribution<unsigned int> dist{0, UINT_MAX};

  public:
    BumblingBuffoonBot() = default;
    Move best_move(Game &game) override;
};

// Bot Level 2
// Prefers capturing and checks over other moves
class SlightlyCompetentBot : public Bot {
    std::random_device rd;
    std::mt19937 mt{1234};
    std::uniform_int_distribution<unsigned int> dist{0, UINT_MAX};

  public:
    SlightlyCompetentBot() = default;
    Move best_move(Game &game) override;
};

// Bot Level 3
// Prefers avoiding capture, capturing moves, and checks
class AverageCsStudentBot : public Bot {
    std::random_device rd;
    std::mt19937 mt{1234};
    std::uniform_int_distribution<unsigned int> dist{0, UINT_MAX};

  public:
    AverageCsStudentBot() = default;
    Move best_move(Game &game) override;
};

// Bot Level 4
class ChuckNorrisBot : public Bot {
  public:
    ChuckNorrisBot() = default;
    Move best_move(Game &game) override;
};

#endif
