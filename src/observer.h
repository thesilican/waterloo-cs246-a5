#ifndef OBSERVER_H
#define OBSERVER_H
#include "game.h"
#include "window.h"
#include <vector>
class Game;
class Controller;
class Subject;
class Xwindow;

class Observer {
    Subject &subject;

  public:
    Observer(Subject &s);
    virtual void notify(Controller &controller) = 0;
    virtual ~Observer();
};

class Subject {
    std::vector<Observer *> games;

  public:
    Subject();
    void attach(Observer *o);
    void deattach(Observer *o);
    void notify_observers(Controller &controller);
};

#endif
