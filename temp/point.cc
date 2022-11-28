#include "point.h"
#include <ostream>

Point::Point() : x{0}, y{0} {
}

Point::Point(int x, int y) : x{x}, y{y} {
}

Point::Point(std::string a) {
    if ('a' <= a[0] && a[0] <= 'h' && '1' <= a[1] && a[1] <= '8') {
        x = a[0] - 'a';
        y = a[1] - '1';
    } else {
        throw std::invalid_argument("invalid algebraic notation: " + a);
    }
}

bool Point::in_bounds() {
    return 0 <= x && x <= 7 && 0 <= y && y <= 7;
}

std::string Point::algebraic() {
    std::string s;
    s.push_back(x + 'a');
    s.push_back(y + '1');
    return s;
}

Point operator+(Point a, Point b) {
    return Point(a.x + b.x, a.y + b.y);
}

Point operator-(Point a, Point b) {
    return Point(a.x - b.x, a.y - b.y);
}

bool operator==(Point a, Point b) {
    return (a.x == b.x) && (a.y == b.y);
}

bool operator!=(Point a, Point b) {
    return !(a == b);
}

std::ostream &operator<<(std::ostream &o, Point p) {
    o << p.algebraic();
    return o;
}
