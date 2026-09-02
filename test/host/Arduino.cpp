#include "Arduino.h"

namespace
{
unsigned long milliseconds = 0;
unsigned long microseconds = 0;
std::map<uint8_t, int> digitalInputs;
std::map<uint8_t, int> digitalOutputs;
std::map<uint8_t, int> analogInputs;
std::map<uint8_t, int> analogOutputs;
std::map<uint8_t, int> pinModes;
struct Interrupt { InterruptHandler handler = nullptr; void* argument = nullptr; };
std::map<uint8_t, Interrupt> interrupts;
}

SerialStub Serial;

unsigned long millis() { return milliseconds; }
unsigned long micros() { return microseconds; }
void pinMode(uint8_t pin, uint8_t mode) { pinModes[pin] = mode; }
void digitalWrite(uint8_t pin, uint8_t value) { digitalOutputs[pin] = value; }
int digitalRead(uint8_t pin) { return digitalInputs.contains(pin) ? digitalInputs[pin] : HIGH; }
void analogWrite(uint8_t pin, int value) { analogOutputs[pin] = value; }
int analogRead(uint8_t pin) { return analogInputs.contains(pin) ? analogInputs[pin] : 2048; }
int digitalPinToInterrupt(uint8_t pin) { return pin; }
void attachInterruptArg(int interrupt, InterruptHandler handler, void* argument, int)
{
    interrupts[static_cast<uint8_t>(interrupt)] = {handler, argument};
}

namespace ArduinoFake
{
void reset()
{
    milliseconds = microseconds = 0;
    digitalInputs.clear(); digitalOutputs.clear(); analogInputs.clear(); analogOutputs.clear();
    pinModes.clear(); interrupts.clear();
}
void setMillis(unsigned long value) { milliseconds = value; }
void advanceMillis(unsigned long value) { milliseconds += value; }
void setMicros(unsigned long value) { microseconds = value; }
void setDigitalInput(uint8_t pin, int value) { digitalInputs[pin] = value; }
void setAnalogInput(uint8_t pin, int value) { analogInputs[pin] = value; }
int digitalOutput(uint8_t pin) { return digitalOutputs.contains(pin) ? digitalOutputs[pin] : -1; }
int analogOutput(uint8_t pin) { return analogOutputs.contains(pin) ? analogOutputs[pin] : -1; }
int pinModeOf(uint8_t pin) { return pinModes.contains(pin) ? pinModes[pin] : -1; }
void triggerInterrupt(uint8_t pin)
{
    if (interrupts.contains(pin) && interrupts[pin].handler)
        interrupts[pin].handler(interrupts[pin].argument);
}
}
