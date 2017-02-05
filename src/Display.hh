#ifndef HEADER_GUARD_DISPLAY_H
#define HEADER_GUARD_DISPLAY_H

#include <vector>

class Display;
#include "FakeLED.hh"

struct Point {
    int x, y;

    // uninitialized
    Point() noexcept {}
    Point(int x, int y) noexcept : x(x), y(y) {}
};

class Shape {
    enum {
        CircleType, RectangleType
    } type;
    Point pt;
    union {
        int radius;
        struct {
            int width, height;
        };
    };

    Shape() noexcept {};
    friend void FakeLED::show();

public:
    static Shape circle(Point center, int radius);
    static Shape rectangle(Point top_left, int width, int height);
};

struct Port {
    int pin;
    std::size_t index;
    Port(int pin, std::size_t index) noexcept
        : pin(pin)
        , index(index) {}
};

class Display {
    std::vector<Port> ports;
    std::vector<Shape> shapes;
    friend void registerDisplay(const Display&);
public:
    Display() noexcept {}
    void addShape(const Port& port, const Shape& shape);
};

#endif
