#ifndef DIGITAL_Event_RECEIVER_H
#define DIGITAL_Event_RECEIVER_H
#include <Arduino.h>
#include <os/datacontainer/DataContainer.hpp>
#include <os/app/DigitalEvent/DigitalEventDefinitions.hpp>
#include <vector>
#include <queue>
/**
 * @struct ServiceCallData
 * @brief Captures a device or room action that should be triggered when a digital event is received.
 */

struct ServiceCallData
{
    /**
     * Target device or room identifier that should receive the service call.
     */
    uint32_t deviceOrRoomId = 0;

    /**
     * Service to invoke on the target entity.
     */
    DeviceServicesType serviceType;

    /**
     * Parameter payload passed to the service call.
     */
    ServiceParameters_set1 parameters;
};
/**
 * @class DigitalEventReceiver
 * @brief Receives digital-event packets, maps them to actions, and dispatches the matching service calls.
 */

class DigitalEventReceiver
{
    /**
     * Mapping between event IDs and their associated digital-action payloads.
     */
    static std::vector<std::pair<uint64_t, DigitalEvent::Event>> digitalEventsMapping;

    /**
     * Queue of event IDs waiting to be processed.
     */
    static std::queue<uint64_t> eventsQueue;

    /**
     * Queue of executable service calls produced by the processed events.
     */
    static std::queue<ServiceCallData> pendingServiceCalls;

    /**
     * Identifier of the latest received transmission.
     */
    static uint8_t lastReceivedTransmissionId;

    /**
     * Timestamp of the last digital event occurrence.
     */
    static long lastEventOccurrenceTime;

public:
    /**
     * Initializes the digital-event receiver and registers its input handling.
     */
    static void init();

    /**
     * Runs the periodic event-processing loop.
     */
    static void cyclic();

    /**
     * Releases runtime state used by the digital-event receiver.
     */
    static void deinit();

    /**
     * Receives and queues a UDP message carrying an incoming digital event.
     * @param msg UDP packet describing the digital event.
     */
    static void receiveUDP(MessageUDP &msg);

    /**
     * Updates the event mapping from a JSON definition payload.
     * @param json Serialized event mapping data.
     */
    static void updateDigitalEventMappingViaJson(String &json);

    /**
     * Fires a locally known event by its identifier.
     * @param eventId Event ID to execute.
     */
    static void fireEvent(uint64_t eventId);

private:
    /**
     * Executes the action payload described by a digital event.
     * @param action Event action to execute.
     */
    static void executeAction(DigitalEvent::Event &action);

    /**
     * Executes an action targeting a single device.
     * @param action Event action to execute.
     */
    static void deviceAction(DigitalEvent::Event &action);

    /**
     * Executes an action targeting a room-level event or group.
     * @param action Event action to execute.
     */
    static void roomAction(DigitalEvent::Event &action);

    /**
     * Drains the pending event queue and dispatches the resulting actions.
     */
    static void processEvents();
};

#endif