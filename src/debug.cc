#include "debug.h"
#include <iostream>

void dprint(std::string s) {
    std::cerr << "[DEBUG] " << s << std::endl;
}