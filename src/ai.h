#ifndef AI_H
#define AI_H
class Move;
class Game;

class Bot {
    Game& game;
public:
    virtual Move best_move(Game &game);
};

// Makes random moves
class ColossalIdiotBot : public Bot {};

class SlightlyCompetentBot : public Bot {};

class AverageCsStudentBot : public Bot {};

class ChuckNorrisBot : public Bot {};

#endif