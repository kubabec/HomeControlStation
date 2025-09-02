#include <os/app/DigitalEvent/DigitalEventTransmitter.hpp>
#include <os/drivers/networkdriver.hpp>

void DigitalEventTransmitter::init()
{
    Serial.println("DigitalEventTransmitter init ...");

    DataContainer::setSignalValue(
        CBK_FIRE_DIGITAL_EVENT,
        static_cast<std::function<void(uint64_t)>>(DigitalEventTransmitter::fireEvent)
    );

    Serial.println("... done");
}

void DigitalEventTransmitter::cyclic()
{
}

void DigitalEventTransmitter::deinit()
{
}

void DigitalEventTransmitter::fireEvent(uint64_t eventId)
{
    MessageUDP msg(DIGITAL_EVENT_FIRED_MSG_ID, NETWORK_BROADCAST, 9001);
    msg.pushData((uint8_t *)&eventId, sizeof(uint64_t));

    NetworkDriver::sendBroadcast(msg);
}
