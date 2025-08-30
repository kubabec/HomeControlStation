#include <os/app/DigitalButton/DigitalButtonReceiver.hpp>


void DigitalButtonReceiver::init(){
    Serial.println("DigitalButtonReceiver init ...");


    Serial.println("... done");
}

void DigitalButtonReceiver::cyclic(){
    
}

void DigitalButtonReceiver::deinit(){

}


void DigitalButtonReceiver::receiveUDP(MessageUDP& msg){
    /* Received UDP Message */
    // Process the received digital button message here
    if(msg.getId() == DIGITAL_BUTTON_FIRED_MSG_ID){
        DigitalButton::ButtonEvent event;
        std::vector<uint8_t>& payload = msg.getPayload();
        if(payload.size() ==  sizeof(event))  {
            memcpy(&event, &(payload.at(0)),sizeof(event));

        }else {
            Serial.println("DigitalButtonReceiver:// Invalid length of received DigitalButton message");
        }
    }

}

void DigitalButtonReceiver::processButtonEvent(DigitalButton::ButtonEvent& event)
{
    const uint8_t deviceOrRoomIdIndex = 2;
    const uint8_t serviceOverloadingIndex = 1;
    const uint8_t serviceTypeIndex = 0;
    const uint8_t valueIndex = 3;

    ServiceParameters_set1 parameters;
    parameters.a = event.actionType == DigitalButton::ActionType::ON ? 1 : 0;

    switch(event.affectedType){
        case DigitalButton::AffectedType::DEVICE : {
            // CALL Room state change in requested Room ID
            std::any_cast<DeviceServicesAPI>(DataContainer::getSignalValue(SIG_DEVICE_SERVICES)).serviceCall_set1(
                event.affectedId,
                DEVSERVICE_STATE_SWITCH,
                parameters
            );
            break;
        }
        
        case DigitalButton::AffectedType::ROOM : {
            // CALL Room state change in requested Room ID
            std::any_cast<DeviceServicesAPI>(DataContainer::getSignalValue(SIG_DEVICE_SERVICES)).serviceCall_set1(
                event.affectedId,
                DEVSERVICE_ROOM_STATE_CHANGE,
                parameters
            );
            break;
        }
    }
}

