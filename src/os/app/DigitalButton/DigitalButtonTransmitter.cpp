#include <os/app/DigitalButton/DigitalButtonTransmitter.hpp>
#include <os/drivers/networkdriver.hpp>

void DigitalButtonTransmitter::init()
{
    Serial.println("DigitalButtonTransmitter init ...");

    DataContainer::setSignalValue(
        CBK_FIRE_DIGITAL_BUTTON,
        static_cast<std::function<void(DigitalButton::ButtonEvent)>>(DigitalButtonTransmitter::fireButton)
    );

    Serial.println("... done");
}

void DigitalButtonTransmitter::cyclic()
{
}

void DigitalButtonTransmitter::deinit()
{
}

void DigitalButtonTransmitter::fireButton(DigitalButton::ButtonEvent event)
{
    MessageUDP msg(DIGITAL_BUTTON_FIRED_MSG_ID, NETWORK_BROADCAST, 9001);
    msg.pushData((uint8_t *)&event, sizeof(DigitalButton::ButtonEvent));

    NetworkDriver::sendBroadcast(msg);
}
