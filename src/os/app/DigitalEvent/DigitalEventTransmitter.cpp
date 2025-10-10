#include <os/app/DigitalEvent/DigitalEventTransmitter.hpp>
#include <os/drivers/networkdriver.hpp>
#include "os/Logger.hpp"

void DigitalEventTransmitter::init()
{
    Logger::log("DigitalEventTransmitter init ...");

    DataContainer::setSignalValue(
        CBK_FIRE_DIGITAL_EVENT,
        static_cast<std::function<void(uint64_t)>>(DigitalEventTransmitter::fireEvent)
    );

    Logger::log("... done");
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
