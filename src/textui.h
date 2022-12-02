#ifndef TEXTUI_H
#define TEXTUI_H
#include "controller.h"
#include "observer.h"
class Observer;
class Subject;
class Controller;

class TextUi : public Observer {
  std::string square_char(std::unique_ptr<Piece> &piece);
  void print_game(Game &game);
  public:
    TextUi(Subject &s);
    // outputs board or who wins or final score to text output
    void notify(Controller &controller) override;
};

#endif
