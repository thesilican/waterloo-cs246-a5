#ifndef AI_H
#define AI_H
class Move;
class Game;

class Bot {
public:
    virtual Move best_move(Game &game) = 0;
};

// Bot Level 1
// Makes random moves
class BumblingBuffoonBot : public Bot {
    Move best_move(Game &game) override;
};

// Bot Level 2
// Prefers capturing and checks over other moves
class SlightlyCompetentBot : public Bot {
    Move best_move(Game &game) override;
};

// Bot Level 3
// Prefers avoiding capture, capturing moves, and checks
class AverageCsStudentBot : public Bot {
    Move best_move(Game &game) override;
};

// Bot Level 4
class ChuckNorrisBot : public Bot {
    Move best_move(Game &game) override;
};

#endif