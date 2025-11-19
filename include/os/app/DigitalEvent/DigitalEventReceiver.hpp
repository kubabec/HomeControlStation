#ifndef DIGITAL_Event_RECEIVER_H
#define DIGITAL_Event_RECEIVER_H
#include <Arduino.h>
#include <os/datacontainer/DataContainer.hpp>
#include <os/app/DigitalEvent/DigitalEventDefinitions.hpp>
#include <vector>
#include <queue>

struct ServiceCallData
{
    uint32_t deviceOrRoomId = 0;
    DeviceServicesType serviceType;
    ServiceParameters_set1 parameters;
};

class DigitalEventReceiver
{

    static std::vector<std::pair<uint64_t, DigitalEvent::Event>> digitalEventsMapping;
    static std::queue<uint64_t> eventsQueue;
    static std::queue<ServiceCallData> pendingServiceCalls;

    static uint8_t lastReceivedTransmissionId;
    static long lastEventOccurrenceTime;

public:
    static void init();
    static void cyclic();
    static void deinit();

    static void receiveUDP(MessageUDP &msg);
    static void updateDigitalEventMappingViaJson(String &json);

    // Used locally on master device
    static void fireEvent(uint64_t eventId);

private:
    static void executeAction(DigitalEvent::Event &action);
    static void deviceAction(DigitalEvent::Event &action);
    static void roomAction(DigitalEvent::Event &action);
    static void processEvents();
};

#endif