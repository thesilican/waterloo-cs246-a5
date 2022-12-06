#include "window.h"
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>

PixelImg::PixelImg(int width, int height, std::vector<std::vector<int>> data)
    : data(data), width(width), height(height) {
    if (data.size() != height) {
        throw std::runtime_error("pixel image has mismatching height");
    }
    for (auto x : data) {
        if (x.size() != width) {
            throw std::runtime_error("pixel image has mismatching width");
        }
    }
}

Xwindow::Xwindow(int width, int height) {
    d = XOpenDisplay(NULL);
    if (d == NULL) {
        throw std::runtime_error("cannot open display");
    }
    s = DefaultScreen(d);
    w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, width, height, 1,
                            BlackPixel(d, s), WhitePixel(d, s));
    XSelectInput(d, w, ExposureMask | KeyPressMask);
    XMapWindow(d, w);

    gc = XCreateGC(d, w, 0, NULL);

    XFlush(d);
    usleep(10000);

    // Make window non-resizeable.
    XSizeHints hints;
    hints.flags = (USPosition | PSize | PMinSize | PMaxSize);
    hints.height = hints.base_height = hints.min_height = hints.max_height =
        height;
    hints.width = hints.base_width = hints.min_width = hints.max_width = width;
    XSetNormalHints(d, w, &hints);

    // Make sure we don't race against the Window being shown
    XEvent ev;
    while (1) {
        XNextEvent(d, &ev);
        if (ev.type == Expose)
            break;
    }
}

Xwindow::~Xwindow() {
    XFreeGC(d, gc);
    XUnmapWindow(d, w);
    XDestroyWindow(d, w);
    XCloseDisplay(d);
}

void Xwindow::fillRectangle(int x, int y, int width, int height, int color) {
    XSetForeground(d, gc, color);
    XFillRectangle(d, w, gc, x, y, width, height);
}

void Xwindow::drawString(int x, int y, std::string msg) {
    XDrawString(d, w, gc, x, y, msg.c_str(), msg.length());
}

void Xwindow::flush() {
    XFlush(d);
}

void Xwindow::drawImage(int x, int y, PixelImg &img) {
    for (int j = 0; j < img.height; j++) {
        for (int i = 0; i < img.width; i++) {
            if (img.data[j][i] != -1) {
                fillRectangle(i + x, j + y, 1, 1, img.data[j][i]);
            }
        }
    }
}
