#ifndef GRAPHICSUI_H
#define GRAPHICSUI_H
#include "assets.h"
#include "controller.h"
#include "observer.h"
#include "window.h"
class Observer;
class Subject;
class Xwindow;
class Controller;

// Represents a graphical ui that implements the observer pattern
class GraphicsUi : public Observer {
    // The X11 window used by the observer
    Xwindow win{500, 560};
    // Internal state of the rendered pieces
    char board[8][8];

    // Draw a game to the window
    // This function is optimized by only drawing pieces that have changed since
    // the last render
    void display_game(Game &game);
    // Returns an image for a given piece
    PixelImg &get_image(std::unique_ptr<Piece> &p);

  public:
    // Constructor for the graphics ui
    GraphicsUi(Subject &s);
    // Notification function for the observer pattern
    void notify(Controller &controller) override;
};

#endif
