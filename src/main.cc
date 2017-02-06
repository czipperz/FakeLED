#include "FastLED.h"
#include "Wire.hh"
#include <SDL2/SDL.h>
#include <cstdio>
#include <assert.h>

void delay(int milliseconds) {
    SDL_Delay(milliseconds);
}

FakeLED FastLED;
FakeWire Wire;

namespace {
SDL_Window* window;
SDL_Surface* screen_surface;

std::vector<Port> ports;
std::vector<Shape> shapes;
}

static void renderCircle(Point center, double radius, CRGB crgb) {
    Uint32 color = SDL_MapRGB(screen_surface->format,
                              crgb.r, crgb.g, crgb.b);
    for (int y = center.y - radius; y != center.y + radius; ++y) {
        for (int x = center.x - radius; x != center.x + radius; ++x) {
            if (std::sqrt((center.x-x)*(center.x-x) +
                          (center.y-y)*(center.y-y)) <=
                radius) {
                SDL_Rect rect;
                rect.x = x;
                rect.y = y;
                rect.w = 1;
                rect.h = 1;
                SDL_FillRect(screen_surface, &rect, color);
            }
        }
    }
}
static void
renderRectangle(Point top_left, int width, int height, CRGB crgb) {
    Uint32 color = SDL_MapRGB(screen_surface->format,
                              crgb.r, crgb.g, crgb.b);
    SDL_Rect rect;
    rect.x = top_left.x;
    rect.y = top_left.y;
    rect.w = width;
    rect.h = height;
    SDL_FillRect(screen_surface, &rect, color);
}

void FakeLED::show() {
    for (std::vector<PinnedValue>::const_iterator color_i =
             pinned.begin();
         color_i != pinned.end(); ++color_i) {
        for (std::size_t shape_i = 0; shape_i != ports.size();
             ++shape_i) {
            Port& port = ports[shape_i];
            if (port.pin == color_i->pin) {
                assert(color_i->leds_len > port.index &&
                       "LED index is outside of registered CRGB bounds.  Try checking calls to FastLED.addLeds()?");
                CRGB& color = color_i->leds[ports[shape_i].index];
                Shape& shape = shapes[shape_i];
                if (shape.type == Shape::CircleType) {
                    renderCircle(shape.pt, shape.radius, color);
                } else if (shape.type == Shape::RectangleType) {
                    renderRectangle(shape.pt, shape.width,
                                    shape.height, color);
                }
            }
        }
    }
    SDL_UpdateWindowSurface(window);
}

Shape Shape::circle(Point center, int radius) {
    Shape s;
    s.type = CircleType;
    s.pt = center;
    s.radius = radius;
    return s;
}

Shape Shape::rectangle(Point top_left, int width, int height) {
    Shape s;
    s.type = RectangleType;
    s.pt = top_left;
    s.width = width;
    s.height = height;
    return s;
}

void Display::addShape(const Port& port, const Shape& shape) {
    this->ports.reserve(ports.size() + 1);
    this->shapes.reserve(shapes.size() + 1);
    this->ports.push_back(port);
    this->shapes.push_back(shape);
}

void registerDisplay(const Display& display) {
    ports.reserve(ports.size() + display.ports.size());
    shapes.reserve(shapes.size() + display.shapes.size());
    ports.insert(ports.end(), display.ports.begin(),
                 display.ports.end());
    shapes.insert(shapes.end(), display.shapes.begin(),
                  display.shapes.end());
}

char FakeWire::read() {
    if (_it == _buffer.end()) {
        return 0;
    }
    return *(_it++);
}

void FakeWire::_addString(const char* str) {
    _buffer.erase(_buffer.begin(), _it);
    _buffer += str;
    _it = _buffer.begin();
    if (_callback) {
        _callback(_buffer.size());
    }
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::fprintf(stderr, "Error initializing SDL: %s\n",
                     SDL_GetError());
        return EXIT_FAILURE;
    }
    if (!(window =
              SDL_CreateWindow("FakeLED", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, 640, 480,
                               SDL_WINDOW_SHOWN))) {
        std::fprintf(stderr, "Error creating a window: %s\n",
                     SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }
    screen_surface = SDL_GetWindowSurface(window);
    SDL_FillRect(screen_surface, NULL,
                 SDL_MapRGB(screen_surface->format, 30, 30, 30));

    setup();
    while (true) {
        loop();
        FastLED.show();
        SDL_Event event;
        bool paused = false;
        while (SDL_PollEvent(&event) || paused) {
            if (event.type == SDL_QUIT) {
                goto exit;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_q) {
                    goto exit;
                } else if (event.key.keysym.sym == SDLK_SPACE) {
                    paused = !paused;
                } else {
                    Wire._addString(SDL_GetKeyName(event.key.keysym.sym));
                }
            }
        }
    }
exit:

    SDL_DestroyWindow(window);
    SDL_Quit();
}
