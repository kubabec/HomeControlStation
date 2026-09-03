#include <os/app/DigitalEvent/DigitalEventTransmitter.hpp>
#include <os/drivers/networkdriver.hpp>

/**
 * @file src/os/app/DigitalEvent/DigitalEventTransmitter.cpp
 * @brief Digital event processing for the Home Control Station.
 */


std::queue<DigitalEventTransmitter::PendingEvent> DigitalEventTransmitter::pendingEvents;
uint64_t DigitalEventTransmitter::lastTransmittedId = 0;
String DigitalEventTransmitter::lastTransmittedSource = "";
long long DigitalEventTransmitter::lastTransmittedTime = 0;
bool DigitalEventTransmitter::wasRequestRepeated = false;

uint8_t DigitalEventTransmitter::transmissionIdentifier = 1;
unsigned long DigitalEventTransmitter::nextTransmissionTime = 0;

namespace
{
constexpr size_t MAX_PENDING_EVENTS = 32;
constexpr unsigned long TRANSMISSION_HANDOFF_DELAY_MS = 50;
}

void DigitalEventTransmitter::init()
{
    Serial.println("DigitalEventTransmitter init ...");

    DataContainer::setSignalValue(
        CBK_FIRE_DIGITAL_EVENT,
        std::function<void(uint64_t)>(static_cast<void (*)(uint64_t)>(&DigitalEventTransmitter::fireEvent)));
    DataContainer::setSignalValue(
        CBK_FIRE_DIGITAL_EVENT_WITH_SOURCE,
        std::function<void(uint64_t, const String &)>(
            static_cast<void (*)(uint64_t, const String &)>(&DigitalEventTransmitter::fireEvent)));

    Serial.println("... done");
}

void DigitalEventTransmitter::receiveUdp(MessageUDP &msg)
{
    /* Received UDP Message */
    // Check if we received confirmation that our event was handled
    if (msg.getId() == DIGITAL_EVENT_CONFIRMED_MSG_ID)
    {
        std::vector<uint8_t> &payload = msg.getPayload();
        if (payload.size() == sizeof(uint64_t))
        {
            uint64_t triggeredEvent = 0;
            memcpy(&triggeredEvent, &(payload.at(0)), sizeof(triggeredEvent));

            if (lastTransmittedId == triggeredEvent)
            {
                completeActiveEvent();
            }
        }
    }
}

void DigitalEventTransmitter::cyclic()
{
    if (lastTransmittedId != 0)
    {
        if (!wasRequestRepeated && ((millis() - lastTransmittedTime) > 2000))
        {
            sendActiveEvent();
            wasRequestRepeated = true;
        }

        if (wasRequestRepeated && ((millis() - lastTransmittedTime) > 4000))
        {
            // Event abandoned, no response received
            completeActiveEvent();
        }
    }

    startNextEvent();
}

void DigitalEventTransmitter::deinit()
{
}

void DigitalEventTransmitter::fireEvent(uint64_t eventId)
{
    enqueueEvent(eventId, "", false);
}

void DigitalEventTransmitter::fireEvent(uint64_t eventId, const String &source)
{
    enqueueEvent(eventId, source.length() ? source.substring(0, DIGITAL_EVENT_SOURCE_MAX_LENGTH) : "Unknown", true);
}

void DigitalEventTransmitter::enqueueEvent(uint64_t eventId, const String &source, bool sourceAware)
{
    if (pendingEvents.size() >= MAX_PENDING_EVENTS)
    {
        Logger::log("DigitalEventTransmitter:// Pending event queue full, dropping event ID: " +
                    String((unsigned long long)eventId));
        return;
    }

    pendingEvents.push({eventId, source, sourceAware});
    startNextEvent();
}

void DigitalEventTransmitter::startNextEvent()
{
    if (lastTransmittedId != 0 || pendingEvents.empty() ||
        static_cast<long>(millis() - nextTransmissionTime) < 0)
    {
        return;
    }

    PendingEvent event = pendingEvents.front();
    pendingEvents.pop();
    lastTransmittedId = event.eventId;
    lastTransmittedSource = event.sourceAware ? event.source : "";
    wasRequestRepeated = false;
    sendActiveEvent();
}

void DigitalEventTransmitter::completeActiveEvent()
{
    lastTransmittedId = 0;
    lastTransmittedSource = "";
    lastTransmittedTime = 0;
    wasRequestRepeated = false;
    transmissionIdentifier++;
    if (transmissionIdentifier == 0) transmissionIdentifier++;
    nextTransmissionTime = millis() + TRANSMISSION_HANDOFF_DELAY_MS;
}

void DigitalEventTransmitter::sendActiveEvent()
{
    if (lastTransmittedId == 0) return;

    const uint64_t eventId = lastTransmittedId;
    std::any localAny{DataContainer::getSignalValue(SIG_LAST_KNOWN_MASTER_IP_ADDR)};
    MessageUDP::IPAddr destination = NETWORK_BROADCAST;
    bool direct = false;
    if (auto p = std::any_cast<MessageUDP::IPAddr>(&localAny))
    {
        destination = *p;
        direct = true;
    }
    if (lastTransmittedSource.length())
    {
        MessageUDP msg(DIGITAL_EVENT_FIRED_WITH_SOURCE_MSG_ID, destination, 9001);
        msg.pushData((uint8_t *)&transmissionIdentifier, sizeof(uint8_t));
        msg.pushData((uint8_t *)&eventId, sizeof(uint64_t));
        msg.pushData((uint8_t *)lastTransmittedSource.c_str(), lastTransmittedSource.length());
        if (direct) NetworkDriver::send(msg); else NetworkDriver::sendBroadcast(msg);

        MessageUDP legacyMessage(DIGITAL_EVENT_FIRED_MSG_ID, destination, 9001);
        legacyMessage.pushData((uint8_t *)&transmissionIdentifier, sizeof(uint8_t));
        legacyMessage.pushData((uint8_t *)&eventId, sizeof(uint64_t));
        if (direct) NetworkDriver::send(legacyMessage); else NetworkDriver::sendBroadcast(legacyMessage);
    }
    else
    {
        MessageUDP msg(DIGITAL_EVENT_FIRED_MSG_ID, destination, 9001);
        msg.pushData((uint8_t *)&transmissionIdentifier, sizeof(uint8_t));
        msg.pushData((uint8_t *)&eventId, sizeof(uint64_t));
        if (direct) NetworkDriver::send(msg); else NetworkDriver::sendBroadcast(msg);
    }

    Logger::log("DigitalEventTransmitter:// Firing event ID: " + String((unsigned long long)eventId) +
                " with transmission ID: " + String((int)transmissionIdentifier) +
                " to " + destination.toString());
    lastTransmittedTime = millis();
}
