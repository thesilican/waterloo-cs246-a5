#include "controller.h"
#include "textui.h"
#include <iostream>

int main() {
    Controller c;
    TextUi t(c);
    c.command_loop();
}
