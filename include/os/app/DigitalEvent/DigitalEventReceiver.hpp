#ifndef DIGITAL_Event_RECEIVER_H
#define DIGITAL_Event_RECEIVER_H
#include <Arduino.h>
#include <os/datacontainer/DataContainer.hpp>
#include <os/app/DigitalEvent/DigitalEventDefinitions.hpp>
#include <vector>
#include <queue>
/**
 * @struct ServiceCallData
 * @brief Captures a device action that should be triggered when a digital event is received.
 */

struct ServiceCallData
{
    /**
    * Target device identifier that should receive the service call.
     */
    uint32_t deviceId = 0;

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
    static std::queue<DigitalEventOccurrence> eventsQueue;

    /** Last ten unmapped event occurrences from the current power cycle. */
    static std::vector<DigitalEventOccurrence> unmappedEvents;

    /**
     * Queue of executable service calls produced by the processed events.
     */
    static std::queue<ServiceCallData> pendingServiceCalls;

    /** Latest transmission byte observed for each remote IPv4 sender. */
    static std::vector<std::pair<uint32_t, uint8_t>> receivedTransmissionIds;

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

    /**
     * Fires a locally known event with diagnostic source information.
     * @param eventId Event ID to execute.
     * @param source Human-readable application, device, or abstraction name.
     */
    static void fireEvent(uint64_t eventId, const String &source);

    /** @return Read-only snapshot of recent unmapped events in newest-first order. */
    static const std::vector<DigitalEventOccurrence> &getUnmappedEvents();

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
     * Drains the pending event queue and dispatches the resulting actions.
     */
    static void processEvents();
};

#endif