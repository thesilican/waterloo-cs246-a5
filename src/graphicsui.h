#ifndef GRAPHICSUI_H
#define GRAPHICSUI_h
#include "controller.h"
#include "observer.h"
#include "window.h"
class Observer;
class Subject;
class Xwindow;
class Controller;

class GraphicsUi : public Observer {
    Xwindow window;

  public:
    GraphicsUi(Subject &s);
    // outputs board or who wins or final score to graphics output
    void notify(Controller &controller) override;
};

#endif
