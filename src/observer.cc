#include "observer.h"

Observer::Observer(Subject &subject) : subject(subject) {
    subject.attach(this);
}

Observer::~Observer() {
    subject.deattach(this);
}

Subject::Subject() : games() {
}

void Subject::attach(Observer *o) {
    games.push_back(o);
}

void Subject::deattach(Observer *o) {
    for (auto i = games.begin(); i != games.end(); i++) {
        if (*i == o) {
            games.erase(i);
            break;
        }
    }
}

void Subject::notify_observers(Controller &controller) {
    for (auto i : games) {
        i->notify(controller);
    }
}