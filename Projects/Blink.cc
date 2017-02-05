#include "FastLED.h"
#include <time.h>
#include <stdlib.h>

const int NUM_LEDS = 19;
const int PIN = 6;

CRGB leds[NUM_LEDS];

void setRange(int begin, int end, const CRGB& color) {
    for (; begin != end; ++begin) {
        leds[begin] = color;
    }
}

void setup() {
    FastLED.addLeds<NEOPIXEL, PIN>(leds, NUM_LEDS);
    srand(time(0));
    setRange(0, 19, CRGB::White);

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

int previous(int i, int num = 1, int min = 0, int max = NUM_LEDS) {
    for (; num != 0; --num) {
        if (i == min) {
            i = max;
        }
        --i;
    }
    return i;
}

int i;

void chase(int& i) {
    if (++i == NUM_LEDS) { i = 0; }
    CRGB color(0, 100, 255);
    leds[i] = color;
    leds[previous(i)] = color;
    leds[previous(i, 2)] = color;
    leds[previous(i, 3)] = CRGB::White;
    FastLED.show();
    delay(30);
}

const CRGB teamBlue(0, 100, 255);

CRGB bullseye1 = teamBlue;
CRGB bullseye2 = CRGB::White;

void bullseye(CRGB& color1, CRGB& color2) {
    setRange(0, 12, color1);
    setRange(12, 18, color2);
    setRange(18, 19, color1);

    CRGB temp = color1;
    color1 = color2;
    color2 = temp;

    FastLED.show();
    delay(400);
}

void chaseBullseye(int& i, CRGB& color1, CRGB& color2) {
    if (++i == NUM_LEDS) {
        i = 7;
        CRGB temp = color1;
        color1 = color2;
        color2 = temp;
    }
    int x = (i - 7) % 4;
    setRange(7, NUM_LEDS, color1);
    leds[7 + x] = color2;
    leds[7 + x + 4] = color2;
    leds[7 + (x + 8) % 12] = color2;

    setRange(1, 7, color2);
    setRange(0, 1, color1);

    FastLED.show();
    delay(50);
}

void flipRange(int begin, int end, const CRGB& color1,
               const CRGB& color2) {
    for (; begin != end; ++begin) {
        if (leds[begin] == color1) { leds[begin] = color2; }
        else if (leds[begin] == color2) { leds[begin] = color1; }
    }
}

void chaseOutsideBullseye(int& i, const CRGB& color1,
                          const CRGB& color2, bool first = false) {
    if (first) {
        setRange(8, NUM_LEDS, color1);
        setRange(1, 7, color2);
        setRange(0, 1, color1);
    }
    if (++i == NUM_LEDS) {
        i = 7;
        flipRange(0, 7, color1, color2);
    }
    int x = (i - 7) % 4;
    setRange(7, NUM_LEDS, color1);
    leds[7 + x] = color2;
    leds[7 + x + 4] = color2;
    leds[7 + (x + 8) % (NUM_LEDS - 7)] = color2;

    FastLED.show();
    delay(50);
}

void sparkle() {
    int index = rand() % NUM_LEDS;
    if (leds[index] == CRGB::White) {
        leds[index] = teamBlue;
    } else {
        leds[index] = CRGB::White;
    }

    FastLED.show();
    delay(50);
}

void chaseFunnel(int& i, const CRGB& color1, const CRGB& color2) {
    if (++i == NUM_LEDS) {
        i = 7;
    }

    // outside
    leds[i] = color1;
    leds[previous(i, 1, 7, NUM_LEDS)] = color1;
    leds[previous(i, 2, 7, NUM_LEDS)] = color1;
    leds[previous(i, 3, 7, NUM_LEDS)] = color2;

    // middle
    leds[1] = (i >= 7 && i <= 9) ? color1 : color2;
    leds[2] = (i >= 9 && i <= 11) ? color1 : color2;
    leds[3] = (i >= 11 && i <= 13) ? color1 : color2;
    leds[4] = (i >= 13 && i <= 15) ? color1 : color2;
    leds[5] = (i >= 15 && i <= 17) ? color1 : color2;
    leds[6] = (i >= 17 || i == 7) ? color1 : color2;

    // inside
    leds[0] = color1;

    FastLED.show();
    delay(50);
}

void chaseOutside(int& i) {
    setRange(0, 7, teamBlue);

    if (++i == NUM_LEDS) { i = 7; }
    leds[i] = teamBlue;
    leds[previous(i, 1, 7)] = teamBlue;
    leds[previous(i, 2, 7)] = teamBlue;
    leds[previous(i, 3, 7)] = CRGB::White;

    FastLED.show();
    delay(30);
}

void chaseLine(int& i, const CRGB& color1, const CRGB& color2) {
    if (++i == NUM_LEDS) {
        i = 7;
    }

    // outside
    leds[i] = color1;
    leds[previous(i, 1, 7, NUM_LEDS)] = color2;
    leds[previous(i, 6, 7, NUM_LEDS)] = color1;
    leds[previous(i, 7, 7, NUM_LEDS)] = color2;

    // middle
    const CRGB& a =
        (i >= 7 && i <= 8 || i >= 13 && i <= 14) ? color1 : color2;
    const CRGB& b =
        (i >= 9 && i <= 10 || i >= 15 && i <= 16) ? color1 : color2;
    const CRGB& c =
        (i >= 11 && i <= 12 || i >= 17) ? color1 : color2;
    leds[1] = a;
    leds[2] = b;
    leds[3] = c;
    leds[4] = a;
    leds[5] = b;
    leds[6] = c;

    // inside
    leds[0] = color1;

    FastLED.show();
    delay(50);
}

void loop() {
    // chase(i);
    // bullseye(bullseye1, bullseye2);
    // chaseBullseye(i, bullseye1, bullseye2);
    sparkle();
    // lineSpiral(i);
    // chaseOutside(i);
    // {
    //     static bool first = true;
    //     chaseOutsideBullseye(i, bullseye1, bullseye2, first);
    //     first = false;
    // }
    // chaseLine(i, bullseye1, bullseye2);
    // chaseFunnel(i, bullseye1, bullseye2);
}
