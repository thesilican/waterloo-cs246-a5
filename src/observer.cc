#include "observer.h"

Observer::Observer(Subject &subject) : subject(subject) {
    subject.attach(this);
}

Observer::~Observer() {
    subject.deattach(this);
}

Subject::Subject() : observers() {
}

void Subject::attach(Observer *o) {
    observers.push_back(o);
}

void Subject::deattach(Observer *o) {
    for (auto i = observers.begin(); i != observers.end(); i++) {
        if (*i == o) {
            observers.erase(i);
            break;
        }
    }
}

void Subject::notify_observers(Controller &controller) {
    for (auto i : observers) {
        i->notify(controller);
    }
}