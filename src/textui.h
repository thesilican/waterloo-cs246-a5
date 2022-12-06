#ifndef TEXTUI_H
#define TEXTUI_H
#include "controller.h"
#include "observer.h"
class Observer;
class Subject;
class Controller;

// Represents a text-based ui observer
class TextUi : public Observer {
    // Whether to render text using unicode chess pieces
    bool use_unicode;
    // Whether to print extra debug info like player to move,
    // castling rights and en passant square
    bool show_debug;

    // Returns a character for a given piece or tile
    std::string square_char(std::unique_ptr<Piece> &piece, Point p);
    // Print a game object to the screen
    void print_game(Game &game);

  public:
    // Constructor for the TextUi
    TextUi(Subject &s, bool use_unicode = false, bool show_debug = false);
    // Notification function for the observer pattern
    void notify(Controller &controller) override;
};

#endif
