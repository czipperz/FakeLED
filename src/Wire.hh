#ifndef HEADER_GUARD_WIRE_HH
#define HEADER_GUARD_WIRE_HH

#include <string>

class FakeWire {
    std::string _buffer;
    std::string::iterator _it;
    void (*_callback)(int);

    void _addString(const char*);
    friend int main();
public:
    FakeWire() noexcept : _it(_buffer.begin()), _callback(nullptr) {};
    void begin(int) noexcept {}
    void onReceive(void (*callback)(int length)) {
        _callback = callback;
    }
    int available() noexcept { return _buffer.end() - _it; }
    char read();
};
extern FakeWire Wire;

#endif
