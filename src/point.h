#ifndef POINT_H
#define POINT_H
#include <string>

class Point {

  public:
    int x, y;

    // Constructs the point (0, 0)
    Point();
    // Construct the point (x, y)
    Point(int x, int y);
    // Constructor using algebraic notation
    // e.g. a5
    Point(std::string a);
    // True if 0 <= x, y <= 7
    bool in_bounds();
    // Returns the algebraic notation form of the string
    std::string algebraic();
};

Point operator+(Point a, Point b);
Point operator-(Point a, Point b);
bool operator==(Point a, Point b);
bool operator!=(Point a, Point b);
std::ostream &operator<<(std::ostream &o, Point p);

#endif