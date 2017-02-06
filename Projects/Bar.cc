#include "FastLED.h"

const int PIN = 6;
const int NUM_LEDS = 8;

CRGB leds[NUM_LEDS];

void setRange(int begin, int end, const CRGB& color) {
    for (; begin != end; ++begin) {
        leds[begin] = color;
    }
}

void setup() {
    FastLED.addLeds<NEOPIXEL, PIN>(leds, NUM_LEDS);
    setRange(0, NUM_LEDS, CRGB::White);

#if USING_FAKELED
    Display display;
    display.addShape(Port(PIN, 0), Shape::circle(Point(10, 150), 10));
    display.addShape(Port(PIN, 1), Shape::circle(Point(10, 130), 10));
    display.addShape(Port(PIN, 2), Shape::circle(Point(10, 110), 10));
    display.addShape(Port(PIN, 3), Shape::circle(Point(10, 90), 10));
    display.addShape(Port(PIN, 4), Shape::circle(Point(10, 70), 10));
    display.addShape(Port(PIN, 5), Shape::circle(Point(10, 50), 10));
    display.addShape(Port(PIN, 6), Shape::circle(Point(10, 30), 10));
    display.addShape(Port(PIN, 7), Shape::circle(Point(10, 10), 10));
    registerDisplay(display);
#endif
}

int i;

int previous(int i, int num = 1, int min = 0, int max = NUM_LEDS) {
    for (; num != 0; --num) {
        if (i == min) {
            i = max;
        }
        --i;
    }
    return i;
}

void chaseOffEnd(int& i) {
    if (++i >= NUM_LEDS + 2) {
        i = 0;
    }
    if (i < NUM_LEDS) { leds[i] = CRGB::Blue; }
    if (i - 1 >= 0 && i - 1 < NUM_LEDS) { leds[i - 1] = CRGB::Blue; }
    if (i > 1) { leds[i - 2] = CRGB::White; }
    FastLED.show();
    delay(100);
}

void chase(int& i) {
    if (++i == NUM_LEDS) { i = 0; }
    leds[i] = CRGB::Blue;
    leds[previous(i)] = CRGB::Blue;
    leds[previous(i, 2)] = CRGB::White;
    FastLED.show();
    delay(100);
}

void doubleChase(int& i) {
    if (++i == NUM_LEDS) { i = 0; }
    leds[i] = CRGB::White;
    leds[previous(i)] = CRGB::Blue;
    leds[previous(i, 4)] = CRGB::White;
    leds[previous(i, 5)] = CRGB::Blue;
    delay(50);
}

void loop() {
    // chase(i);
    // chaseOffEnd(i);
    doubleChase(i);
}
