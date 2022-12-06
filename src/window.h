#ifndef __WINDOW_H__
#define __WINDOW_H__
#include <X11/Xlib.h>
#include <iostream>
#include <string>
#include <vector>

// Represents a 2D pixel image that can be drawn to the screen
class PixelImg {
  public:
    // Pixel color data for the image
    std::vector<std::vector<int>> data;
    // Width of the image
    int width;
    // Height of the image
    int height;
    // Constructor for PixelImg
    PixelImg(int width, int height, std::vector<std::vector<int>> data);
};

// Represents a X11 window
class Xwindow {
    Display *d;
    Window w;
    int s;
    GC gc;

  public:
    Xwindow(int width = 500, int height = 500);
    ~Xwindow();
    Xwindow(const Xwindow &) = delete;
    Xwindow &operator=(const Xwindow &) = delete;

    // Draws a rectangle to the window
    void fillRectangle(int x, int y, int width, int height, int colour = 0);

    // Draw a pixel image to the window
    void drawImage(int x, int y, PixelImg &img);

    // Draws a string to the window
    void drawString(int x, int y, std::string msg);

    // Flushes all graphical changes to the screen
    void flush();
};

#endif
