#ifndef DIGITAL_BUTTON_TRANSMITTER_H
#define DIGITAL_BUTTON_TRANSMITTER_H
#include <Arduino.h>
#include <os/datacontainer/DataContainer.hpp>
#include <os/app/DigitalButton/DigitalButtonDefinitions.hpp>
#include <queue>

class DigitalButtonTransmitter
{

public:
    static void init();
    static void cyclic();
    static void deinit();

    static void fireButton(DigitalButton::ButtonEvent event);
};

#endif