/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2017 Chris Gregory czipperz@gmail.com
 * Copyright (c) 2016 FastLED
 */

#ifndef HEADER_GUARD_FAKELED_H
#define HEADER_GUARD_FAKELED_H

#define USING_FAKELED 1
#include <cstdint>
#include <type_traits>
#include <vector>

#include "CRGB.hh"

class NEOPIXEL {};
class FakeLED {
    struct PinnedValue {
        int pin;
        CRGB* leds;
        std::size_t leds_len;
    };
    std::vector<PinnedValue> pinned;
public:
    void show();

    template <class Type, int Pin>
    void addLeds(CRGB* leds, std::size_t leds_len) {
        static_assert(std::is_same<Type, NEOPIXEL>::value, "");
        PinnedValue value = {Pin, leds, leds_len};
        pinned.push_back(value);
    }

};
extern FakeLED FastLED;

#include "Display.hh"

void registerDisplay(const Display&);

void delay(int milliseconds);

// user defined
extern void setup();
extern void loop();

#endif
