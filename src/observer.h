#ifndef OBSERVER_H
#define OBSERVER_H
#include "game.h"
#include "window.h"
#include <memory>
#include <vector>
class Game;
class Controller;
class Subject;
class Xwindow;

// Abstract base class for an observer in the observer pattern
class Observer {
    // The subject that is being observed
    Subject &subject;

  public:
    // Default constructor for the observer
    Observer(Subject &s);
    // Virtual destructor
    virtual ~Observer();
    // Notify the observer of an update
    virtual void notify(Controller &controller) = 0;
};

// Base class for a subject being observed in the observer pattern
class Subject {
    // A list of observers that are observing the subject
    std::vector<Observer *> observers;

  public:
    // Default constructor for the subject
    Subject();
    // Attach an observer to this subject
    void attach(Observer *o);
    // Detach an observer from this subject
    void deattach(Observer *o);
    // Notify all observers of an update
    void notify_observers(Controller &controller);
};

#endif
