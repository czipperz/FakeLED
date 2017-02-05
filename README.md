# FakeLED

The purpose of this repository is to emulate the FastLED library for
an Arduino without one present.  The only change you have to do to
your code is add into `setup()` a description of the output:

    const int PIN = 6;
    void setup() {
        FastLED.addLeds<NEOPIXEL, PIN>(leds, NUM_LEDS);

    #if USING_FAKELED
        Display display;
        int INDEX = 0;
        int center_x = 30;
        int center_y = 30;
        int radius = 10;
        disply.addShape(Port(PIN, INDEX),
                        Shape::circle(Point(center_x, center_y),
                                      radius));

        registerDisplay(display);
    #endif
    }

This **ONLY** supports emulation of **NEOPIXEL**.  Use others at your
own risk.

After each call to `loop()`, a call to `FastLED.show()` will be ran.
This makes your program quitable, outside of force quitting, when
`loop()` completes.  If you program your `loop()` to do a single
iteration of your algorithm, as shown in `Projects/Blink.cc` (which
does much more than blink), then you gain the ability to pause
mid-algorithm via the space key.  The space key also unpauses.  Hit q
to quit, or Control+C from the terminal.

## Dependencies

* [SDL2](www.libsdl.org) for rendering.
* A `c++11` compliant compiler.  If `g++` is not preferred, add
  `CXX=MY_COMPILER` to the call to `make` below.

## Building

    ./configure
    make PROJECT=MY_PROJECT
    ./Projects/out/MY_PROJECT

Where `Projects/MY_PROJECT.cc` exists.
