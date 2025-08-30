#ifndef DIGITAL_BUTTON_RECEIVER_H
#define DIGITAL_BUTTON_RECEIVER_H
#include <Arduino.h>
#include <os/datacontainer/DataContainer.hpp>
#include <os/app/DigitalButton/DigitalButtonDefinitions.hpp>
#include <queue>

class DigitalButtonReceiver
{

public:
    static void init();
    static void cyclic();
    static void deinit();

    static void receiveUDP(MessageUDP& msg);

private :
    static void processButtonEvent(DigitalButton::ButtonEvent& event);
};

#endif