#ifndef __WINDOW_H__
#define __WINDOW_H__
#include <X11/Xlib.h>
#include <iostream>
#include <string>
#include <vector>

class PixelImg {
  public:
    std::vector<std::vector<int>> data;
    int width;
    int height;
    PixelImg(int width, int height, std::vector<std::vector<int>> data);
};

class Xwindow {
    Display *d;
    Window w;
    int s;
    GC gc;
    unsigned long colours[10];

  public:
    Xwindow(int width = 500,
            int height = 500); // Constructor; displays the window.
    ~Xwindow();                // Destructor; destroys the window.
    Xwindow(const Xwindow &) = delete;
    Xwindow &operator=(const Xwindow &) = delete;

    // Available colours.
    enum {
        White = 0,
        Black = 1,
        Red = 2,
        Green = 3,
        Blue = 4,
        Cyan = 5,
        Yellow = 6,
        Magenta = 7,
        Orange = 8,
        Brown
    };

    // Draws a rectangle
    void fillRectangle(int x, int y, int width, int height, int colour = Black);

    // Draw a pixel image
    void drawImage(int x, int y, PixelImg &img);

    // Draws a string
    void drawString(int x, int y, std::string msg);

    void flush();
};

#endif
