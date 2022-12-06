#ifndef POINT_H
#define POINT_H
#include <string>

// Represents a 2-dimension coordinate point
class Point {

  public:
    // Coordinates of the point
    int x, y;

    // Constructs the point (0, 0)
    Point();
    // Construct the point (x, y)
    Point(int x, int y);
    // Construct a point using algebraic notation, e.g. a5
    Point(std::string a);
    // Whether or not the point is a valid coordinate on the chessboard
    // i.e. 0 <= x, y <= 7
    bool in_bounds();
    // Returns the algebraic notation form of the point
    std::string algebraic();
};

Point operator+(Point a, Point b);
Point operator-(Point a, Point b);
bool operator==(Point a, Point b);
bool operator!=(Point a, Point b);

#endif