#include <os/app/DigitalEvent/DigitalEventTransmitter.hpp>
#include <os/drivers/networkdriver.hpp>

uint64_t DigitalEventTransmitter::lastTransmittedId = 0;
long long DigitalEventTransmitter::lastTransmittedTime = 0;
bool DigitalEventTransmitter::wasRequestRepeated = false;

uint8_t DigitalEventTransmitter::transmissionIdentifier = 1;

void DigitalEventTransmitter::init()
{
    Serial.println("DigitalEventTransmitter init ...");

    DataContainer::setSignalValue(
        CBK_FIRE_DIGITAL_EVENT,
        static_cast<std::function<void(uint64_t)>>(DigitalEventTransmitter::fireEvent));

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
            fireEvent(lastTransmittedId);
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
    try
    {
        MessageUDP::IPAddr masterDirectIp = std::any_cast<MessageUDP::IPAddr>(DataContainer::getSignalValue(SIG_LAST_KNOWN_MASTER_IP_ADDR));
        MessageUDP msg(DIGITAL_EVENT_FIRED_MSG_ID, masterDirectIp, 9001);
        msg.pushData((uint8_t *)&transmissionIdentifier, sizeof(uint8_t));
        msg.pushData((uint8_t *)&eventId, sizeof(uint64_t));

        NetworkDriver::send(msg);
        Logger::log("DigitalEventTransmitter:// Firing digital event ID: " + String((unsigned long long)eventId) +
                            " to master IP: " + String((unsigned int)masterDirectIp.octet1) + "." +
                            String((unsigned int)masterDirectIp.octet2) + "." +
                            String((unsigned int)masterDirectIp.octet3) + "." +
                            String((unsigned int)masterDirectIp.octet4));
    }
    catch (const std::bad_any_cast &e)
    {
        Serial.println("DigitalEventTransmitter:// No known master IP address, cannot fire event.");
        MessageUDP msg(DIGITAL_EVENT_FIRED_MSG_ID, NETWORK_BROADCAST, 9001);
        msg.pushData((uint8_t *)&transmissionIdentifier, sizeof(uint8_t));
        msg.pushData((uint8_t *)&eventId, sizeof(uint64_t));

        NetworkDriver::sendBroadcast(msg);
        Logger::log("DigitalEventTransmitter:// Firing digital event ID: " + String((unsigned long long)eventId) +
                            " to broadcast address.");
    }

    lastTransmittedTime = millis();
    lastTransmittedId = eventId;
}
