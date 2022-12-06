#ifndef GRAPHICSUI_H
#define GRAPHICSUI_H
#include "controller.h"
#include "observer.h"
#include "window.h"
#include "assets.h"
class Observer;
class Subject;
class Xwindow;
class Controller;

class GraphicsUi : public Observer {
    Xwindow win;
    void display_game(Game& game);
    PixelImg& get_image(std::unique_ptr<Piece>& p);
    char board[8][8];
  public:
    GraphicsUi(Subject &s);
    // outputs board or who wins or final score to graphics output
    void notify(Controller &controller) override;
};

#endif
