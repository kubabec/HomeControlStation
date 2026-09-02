#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <sstream>
#include <string>
#include <type_traits>

using byte = uint8_t;

class String
{
public:
    String() = default;
    String(const char* value) : value_(value ? value : "") {}
    String(const std::string& value) : value_(value) {}
    String(char value) : value_(1, value) {}
    String(const char* format, int value)
    {
        char buffer[128];
        std::snprintf(buffer, sizeof(buffer), format, value);
        value_ = buffer;
    }
    template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    String(T value)
    {
        std::ostringstream output;
        output << value;
        value_ = output.str();
    }

    size_t length() const { return value_.length(); }
    const char* c_str() const { return value_.c_str(); }
    void getBytes(uint8_t* destination, size_t length) const
    {
        if (!destination || length == 0) return;
        const size_t copied = std::min(value_.length(), length - 1);
        std::memcpy(destination, value_.data(), copied);
        destination[copied] = 0;
    }
    String& operator+=(const String& other) { value_ += other.value_; return *this; }
    String& operator+=(const char* other) { value_ += other ? other : ""; return *this; }
    bool operator==(const String& other) const { return value_ == other.value_; }
    bool operator!=(const String& other) const { return !(*this == other); }
    bool operator<(const String& other) const { return value_ < other.value_; }
    operator std::string() const { return value_; }

private:
    std::string value_;
};

inline String operator+(const String& left, const String& right)
{
    String result(left); result += right; return result;
}
inline String operator+(const char* left, const String& right) { return String(left) + right; }
inline String operator+(const String& left, const char* right) { return left + String(right); }

class SerialStub
{
public:
    template <typename T> void print(const T&) {}
    template <typename T> void println(const T&) {}
    void println() {}
    void flush() {}
};

extern SerialStub Serial;

#define HIGH 1
#define LOW 0
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2
#define CHANGE 3
#define ICACHE_RAM_ATTR
#define F(value) value

using InterruptHandler = void (*)(void*);
unsigned long millis();
unsigned long micros();
void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t value);
int digitalRead(uint8_t pin);
void analogWrite(uint8_t pin, int value);
int analogRead(uint8_t pin);
int digitalPinToInterrupt(uint8_t pin);
void attachInterruptArg(int interrupt, InterruptHandler handler, void* argument, int mode);

namespace ArduinoFake
{
void reset();
void setMillis(unsigned long value);
void advanceMillis(unsigned long value);
void setMicros(unsigned long value);
void setDigitalInput(uint8_t pin, int value);
void setAnalogInput(uint8_t pin, int value);
int digitalOutput(uint8_t pin);
int analogOutput(uint8_t pin);
int pinModeOf(uint8_t pin);
void triggerInterrupt(uint8_t pin);
}

template <typename T> constexpr T min(T left, T right) { return std::min(left, right); }
template <typename T> constexpr T max(T left, T right) { return std::max(left, right); }
