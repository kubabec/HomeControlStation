#ifndef DIGITAL_Event_TRANSMITTER_H
#define DIGITAL_Event_TRANSMITTER_H
#include <Arduino.h>
#include <os/datacontainer/DataContainer.hpp>
#include <os/app/DigitalEvent/DigitalEventDefinitions.hpp>
#include <queue>

class DigitalEventTransmitter
{

public:
    static void init();
    static void cyclic();
    static void deinit();

    static void fireEvent(uint64_t eventId);
};

#endif