#ifndef DIGITAL_BUTTON_RECEIVER_H
#define DIGITAL_BUTTON_RECEIVER_H
#include <Arduino.h>
#include <os/datacontainer/DataContainer.hpp>
#include <os/app/DigitalButton/DigitalButtonDefinitions.hpp>
#include <vector>

class DigitalButtonReceiver
{
    static std::vector<std::pair<uint64_t, DigitalButton::ButtonEvent>> digitalButtonsMapping;

public:
    static void init();
    static void cyclic();
    static void deinit();

    static void receiveUDP(MessageUDP& msg);
    static void updateDigitalButtonMappingViaJson(String& json);

private :
    static void processButtonEvent(DigitalButton::ButtonEvent& event);
};

#endif