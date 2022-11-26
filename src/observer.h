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

class TextUi : public Observer {
  public:
    TextUi(Subject &s);
    // outputs board or who wins or final score to text output
    void notify(Controller &controller) override;
};

class GraphicsUi : public Observer {
    Xwindow window;

  public:
    GraphicsUi(Subject &s);
    // outputs board or who wins or final score to graphics output
    void notify(Controller &controller) override;
};

class Subject {
    std::vector<Observer *> games;

  public:
    void attach(Observer *o);
    void deattach(Observer *o);
    void notify_observers(Controller &controller);
};

#endif
