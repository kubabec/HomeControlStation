#include <os/app/DigitalEvent/DigitalEventTransmitter.hpp>
#include <os/drivers/networkdriver.hpp>

/**
 * @file src/os/app/DigitalEvent/DigitalEventTransmitter.cpp
 * @brief Digital event processing for the Home Control Station.
 */


uint64_t DigitalEventTransmitter::lastTransmittedId = 0;
String DigitalEventTransmitter::lastTransmittedSource = "";
long long DigitalEventTransmitter::lastTransmittedTime = 0;
bool DigitalEventTransmitter::wasRequestRepeated = false;

uint8_t DigitalEventTransmitter::transmissionIdentifier = 1;

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
                // event handled, reset statistics
                lastTransmittedId = 0;
                lastTransmittedTime = 0;
                wasRequestRepeated = false;
                transmissionIdentifier++;
                if (transmissionIdentifier == 0)
                {
                    transmissionIdentifier++;
                }
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
            // repeat event trigger
            if (lastTransmittedSource.length()) fireEvent(lastTransmittedId, lastTransmittedSource);
            else fireEvent(lastTransmittedId);
            wasRequestRepeated = true;
        }

        if (wasRequestRepeated && ((millis() - lastTransmittedTime) > 4000))
        {
            // Event abandoned, no response received
            lastTransmittedId = 0;
            lastTransmittedTime = 0;
            wasRequestRepeated = false;
            transmissionIdentifier++;
            if (transmissionIdentifier == 0)
            {
                transmissionIdentifier++;
            }
        }
    }
}

void DigitalEventTransmitter::deinit()
{
}

void DigitalEventTransmitter::fireEvent(uint64_t eventId)
{
    lastTransmittedSource = "";
    std::any localAny{DataContainer::getSignalValue(SIG_LAST_KNOWN_MASTER_IP_ADDR)};
    MessageUDP::IPAddr destination = NETWORK_BROADCAST;
    bool direct = false;
    if (auto p = std::any_cast<MessageUDP::IPAddr>(&localAny))
    {
        destination = *p;
        direct = true;
    }
    MessageUDP msg(DIGITAL_EVENT_FIRED_MSG_ID, destination, 9001);
    msg.pushData((uint8_t *)&transmissionIdentifier, sizeof(uint8_t));
    msg.pushData((uint8_t *)&eventId, sizeof(uint64_t));
    if (direct) NetworkDriver::send(msg); else NetworkDriver::sendBroadcast(msg);
    Logger::log("DigitalEventTransmitter:// Firing legacy digital event ID: " + String((unsigned long long)eventId));
    lastTransmittedTime = millis();
    lastTransmittedId = eventId;
}

void DigitalEventTransmitter::fireEvent(uint64_t eventId, const String &source)
{
    lastTransmittedSource = source.length() ? source.substring(0, DIGITAL_EVENT_SOURCE_MAX_LENGTH) : "Unknown";
    std::any localAny{DataContainer::getSignalValue(SIG_LAST_KNOWN_MASTER_IP_ADDR)};
    if (auto p = std::any_cast<MessageUDP::IPAddr>(&localAny))
    {
        MessageUDP::IPAddr masterDirectIp = *p;
        MessageUDP msg(DIGITAL_EVENT_FIRED_WITH_SOURCE_MSG_ID, masterDirectIp, 9001);
        msg.pushData((uint8_t *)&transmissionIdentifier, sizeof(uint8_t));
        msg.pushData((uint8_t *)&eventId, sizeof(uint64_t));
        msg.pushData((uint8_t *)lastTransmittedSource.c_str(), lastTransmittedSource.length());

        NetworkDriver::send(msg);
        MessageUDP legacyMessage(DIGITAL_EVENT_FIRED_MSG_ID, masterDirectIp, 9001);
        legacyMessage.pushData((uint8_t *)&transmissionIdentifier, sizeof(uint8_t));
        legacyMessage.pushData((uint8_t *)&eventId, sizeof(uint64_t));
        NetworkDriver::send(legacyMessage);
        Logger::log("DigitalEventTransmitter:// Firing digital event ID: " + String((unsigned long long)eventId) +
                            " to master IP: " + String((unsigned int)masterDirectIp.octet1) + "." +
                            String((unsigned int)masterDirectIp.octet2) + "." +
                            String((unsigned int)masterDirectIp.octet3) + "." +
                            String((unsigned int)masterDirectIp.octet4));
    }
    else
    {
        Serial.println("DigitalEventTransmitter:// No known master IP address, cannot fire event.");
        MessageUDP msg(DIGITAL_EVENT_FIRED_WITH_SOURCE_MSG_ID, NETWORK_BROADCAST, 9001);
        msg.pushData((uint8_t *)&transmissionIdentifier, sizeof(uint8_t));
        msg.pushData((uint8_t *)&eventId, sizeof(uint64_t));
        msg.pushData((uint8_t *)lastTransmittedSource.c_str(), lastTransmittedSource.length());

        NetworkDriver::sendBroadcast(msg);
        MessageUDP legacyMessage(DIGITAL_EVENT_FIRED_MSG_ID, NETWORK_BROADCAST, 9001);
        legacyMessage.pushData((uint8_t *)&transmissionIdentifier, sizeof(uint8_t));
        legacyMessage.pushData((uint8_t *)&eventId, sizeof(uint64_t));
        NetworkDriver::sendBroadcast(legacyMessage);
        Logger::log("DigitalEventTransmitter:// Firing digital event ID: " + String((unsigned long long)eventId) +
                            " to broadcast address.");
    }

    lastTransmittedTime = millis();
    lastTransmittedId = eventId;
}
