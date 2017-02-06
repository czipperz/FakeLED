#include "Wire.h"
#include "FastLED.h"
#include <cstdio>

const int PIN = 6;
const int NUM_LEDS = 19;

CRGB leds[NUM_LEDS];

void setRange(int begin, int end, const CRGB& color) {
    for (; begin != end; ++begin) {
        leds[begin] = color;
    }
}

void receiveEvent(int);

void setup() {
    FastLED.addLeds<NEOPIXEL, PIN>(leds, NUM_LEDS);
    setRange(0, 19, CRGB::White);

    Wire.begin(84);
    Wire.onReceive(receiveEvent);

#if USING_FAKELED
    Display display;
    display.addShape(Port(PIN, 0), Shape::circle(Point(100, 60), 5));

    display.addShape(Port(PIN, 1), Shape::circle(Point(100, 50), 5));
    display.addShape(Port(PIN, 2), Shape::circle(Point(110, 55), 5));
    display.addShape(Port(PIN, 3), Shape::circle(Point(110, 65), 5));
    display.addShape(Port(PIN, 4), Shape::circle(Point(100, 70), 5));
    display.addShape(Port(PIN, 5), Shape::circle(Point(90, 65), 5));
    display.addShape(Port(PIN, 6), Shape::circle(Point(90, 55), 5));

    display.addShape(Port(PIN, 7), Shape::circle(Point(100, 35), 5));
    display.addShape(Port(PIN, 8), Shape::circle(Point(110, 40), 5));
    display.addShape(Port(PIN, 9), Shape::circle(Point(120, 50), 5));
    display.addShape(Port(PIN, 10), Shape::circle(Point(125, 60), 5));
    display.addShape(Port(PIN, 11), Shape::circle(Point(120, 70), 5));
    display.addShape(Port(PIN, 12), Shape::circle(Point(110, 80), 5));
    display.addShape(Port(PIN, 13), Shape::circle(Point(100, 85), 5));
    display.addShape(Port(PIN, 14), Shape::circle(Point(90, 80), 5));
    display.addShape(Port(PIN, 15), Shape::circle(Point(80, 70), 5));
    display.addShape(Port(PIN, 16), Shape::circle(Point(75, 60), 5));
    display.addShape(Port(PIN, 17), Shape::circle(Point(80, 50), 5));
    display.addShape(Port(PIN, 18), Shape::circle(Point(90, 40), 5));

    registerDisplay(display);
#endif
}

void receiveEvent(int) {
    while (Wire.available()) {
        char c = Wire.read();
        putchar(c);
        if (c == 'J') {
            setRange(0, 7, CRGB::Blue);
        } else if (c == ';') {
            setRange(0, 7, CRGB::White);
        } else if (c == 'K') {
            setRange(7, 19, CRGB::Blue);
        } else if (c == 'L') {
            setRange(7, 19, CRGB::White);
        }
    }
    putchar('\n');
}

void loop() {
}
