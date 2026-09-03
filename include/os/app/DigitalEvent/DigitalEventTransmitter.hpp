#ifndef DIGITAL_Event_TRANSMITTER_H
#define DIGITAL_Event_TRANSMITTER_H
#include <Arduino.h>
#include <os/datacontainer/DataContainer.hpp>
#include <os/app/DigitalEvent/DigitalEventDefinitions.hpp>
#include <queue>
/**
 * @class DigitalEventTransmitter
 * @brief Sends digital-event notifications over the network to interested receivers.
 *
 * The transmitter tracks the last transmitted event and ensures repeated messages are not sent without
 * a meaningful reason so the event stream remains compact and deterministic.
 */

class DigitalEventTransmitter
{
    /**
     * Identifier of the last event that was transmitted.
     */
    static uint64_t lastTransmittedId;

    /** Source associated with the event while awaiting confirmation. */
    static String lastTransmittedSource;

    /**
     * Timestamp of the last outbound transmission.
     */
    static long long lastTransmittedTime;

    /**
     * true when a request was already repeated and the system should avoid duplicate sends.
     */
    static bool wasRequestRepeated;

    /**
     * Current transmission identifier assigned to the outgoing event frame.
     */
    static uint8_t transmissionIdentifier;

public:
    /**
     * Initializes the digital-event transmitter and prepares its runtime state.
     */
    static void init();

    /**
     * Runs periodic maintenance for the digital-event transmitter.
     */
    static void cyclic();

    /**
     * Releases runtime state attached to the sender.
     */
    static void deinit();

    /**
     * Sends a digital event with the supplied identifier.
     * @param eventId Event identifier to transmit.
     */
    static void fireEvent(uint64_t eventId);

    /**
     * Sends a source-aware event using the additive network protocol.
     * @param eventId Event identifier to transmit.
     * @param source Human-readable application, device, or abstraction name.
     */
    static void fireEvent(uint64_t eventId, const String &source);

    /**
     * Receives a UDP packet and routes it through the transmitter logic if needed.
     * @param msg Incoming UDP message.
     */
    static void receiveUdp(MessageUDP &msg);
};

#endif