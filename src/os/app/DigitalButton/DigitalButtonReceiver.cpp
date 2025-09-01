#include <os/app/DigitalButton/DigitalButtonReceiver.hpp>

std::vector<std::pair<uint64_t, DigitalButton::ButtonEvent>> DigitalButtonReceiver::digitalButtonsMapping;

const uint8_t NVM_VALID = 0xCD;

void DigitalButtonReceiver::init()
{
    Serial.println("DigitalButtonReceiver init ...");

    /* Read NVM data for DigitalButtonReceiver application */
    uint16_t sizeOfNvm = (e_BLOCK_DIGITAL_BUTTON_6 - e_BLOCK_DIGITAL_BUTTON_1 + 1) * PERSISTENT_DATABLOCK_SIZE;
    /* Allocate memory for NVM data */
    uint8_t *nvmData = (uint8_t *)malloc(sizeOfNvm);
    uint8_t offset = 0;
    for (uint8_t blockID = e_BLOCK_DIGITAL_BUTTON_1; blockID <= e_BLOCK_DIGITAL_BUTTON_6; blockID++)
    {
        /* call GET_NVM_DATABLOCK for current datablock to read NVM data */
        std::any_cast<std::function<bool(PersistentDatablockID, uint8_t *)>>(
            DataContainer::getSignalValue(CBK_GET_NVM_DATABLOCK))(
            (PersistentDatablockID)blockID, // Datablock ID
            (uint8_t *)&nvmData[offset]     // local memory buffer for datablock data
        );

        /* Shift the offset, that next datablock will be written next to previous in 'nvmData' */
        offset += PERSISTENT_DATABLOCK_SIZE;
    }
    // check if FIRST byte of NVM contains validity flag
    if (nvmData[0] == NVM_VALID)
    {
        uint8_t numberOfElementsInNvm = nvmData[1];

        // Are there any button events saved in nvm?
        if (numberOfElementsInNvm > 0)
        {
            for (uint8_t i = 0; i < numberOfElementsInNvm; i++)
            {
                uint64_t buttonUniqueId = 0x00;
                DigitalButton::ButtonEvent event;

                // Copy corresponding buttonUniqueId
                memcpy(&buttonUniqueId, &(nvmData[2 + (i * (sizeof(buttonUniqueId) + sizeof(event)))]), sizeof(uint64_t));

                // Copy corresponding button event
                memcpy(&event, &(nvmData[2 + (i * (sizeof(buttonUniqueId) + sizeof(event))) + sizeof(buttonUniqueId)]), sizeof(event));

                digitalButtonsMapping.push_back(std::pair<uint64_t, DigitalButton::ButtonEvent>(buttonUniqueId, event));
            }

            Serial.println("DigitalButtonReceiver:// Restored " + String((int)numberOfElementsInNvm) + " button events");
        }
    }

    free(nvmData);


    DataContainer::setSignalValue(SIG_DIGITAL_BTN_MAPPING, digitalButtonsMapping);
    DataContainer::setSignalValue(CBK_UPDATE_DIG_BTN_TABLE, std::function<void(String&)>(updateDigitalButtonMappingViaJson));

    // for(auto& mapping : digitalButtonsMapping){
    //     Serial.println("Mapping ID: " + String((int) mapping.first) + " ");
    //     Serial.println("Mapping affectedType: " + String((int) mapping.second.affectedType) + " ");
    //     Serial.println("Mapping affectedId: " + String((int) mapping.second.affectedId) + " ");
    //     Serial.println("Mapping actionType: " + String((int) mapping.second.actionType) + " ");
    // }

    Serial.println("... done");
}

void DigitalButtonReceiver::updateDigitalButtonMappingViaJson(String& json)
{

    Serial.println("  ");
    Serial.println(json);
    Serial.println("  ");

}

void DigitalButtonReceiver::cyclic()
{
}

void DigitalButtonReceiver::deinit()
{
    // We only have NVM data if we handshaked at least 1 slave node
    /* Write NVM data for DigitalButtonReceiver application */
    uint16_t sizeOfNvm = (e_BLOCK_DIGITAL_BUTTON_6 - e_BLOCK_DIGITAL_BUTTON_1 + 1) * PERSISTENT_DATABLOCK_SIZE;
    /* Allocate memory for NVM data */
    uint8_t *nvmData = (uint8_t *)malloc(sizeOfNvm);

    // Data validity indicator
    nvmData[0] = NVM_VALID;
    // Number of mappings present in the system
    nvmData[1] = digitalButtonsMapping.size();

    uint8_t i = 0;
    // Serialize map to nvmData raw memory
    for (auto it = digitalButtonsMapping.begin(); it != digitalButtonsMapping.end(); ++it, ++i)
    {
        memcpy(&(nvmData[2 + i * (sizeof(uint64_t) + sizeof(DigitalButton::ButtonEvent))]), &(it->first), sizeof(uint64_t));
        memcpy(&(nvmData[2 + (i * (sizeof(uint64_t) + sizeof(DigitalButton::ButtonEvent)) + sizeof(uint64_t))]), &(it->second), sizeof(DigitalButton::ButtonEvent));
    }

    // Copy raw memory to datablocks
    uint8_t offset = 0;
    for (uint8_t blockID = e_BLOCK_DIGITAL_BUTTON_1; blockID <= e_BLOCK_DIGITAL_BUTTON_6; blockID++)
    {
        /* call GET_NVM_DATABLOCK for current datablock to read NVM data */
        std::any_cast<std::function<bool(PersistentDatablockID, uint8_t *)>>(
            DataContainer::getSignalValue(CBK_SET_NVM_DATABLOCK))(
            (PersistentDatablockID)blockID, // Datablock ID
            (uint8_t *)&nvmData[offset]     // local memory buffer for datablock data
        );

        /* Shift the offset, that next datablock will be written next to previous in 'nvmData' */
        offset += PERSISTENT_DATABLOCK_SIZE;
    }

    /* release heap buffer */
    free(nvmData);
}

void DigitalButtonReceiver::receiveUDP(MessageUDP &msg)
{
    /* Received UDP Message */
    // Process the received digital button message here
    if (msg.getId() == DIGITAL_BUTTON_FIRED_MSG_ID)
    {
        DigitalButton::ButtonEvent event;
        std::vector<uint8_t> &payload = msg.getPayload();
        if (payload.size() == sizeof(event))
        {
            memcpy(&event, &(payload.at(0)), sizeof(event));
        }
        else
        {
            Serial.println("DigitalButtonReceiver:// Invalid length of received DigitalButton message");
        }
    }
}

void DigitalButtonReceiver::processButtonEvent(DigitalButton::ButtonEvent &event)
{
    const uint8_t deviceOrRoomIdIndex = 2;
    const uint8_t serviceOverloadingIndex = 1;
    const uint8_t serviceTypeIndex = 0;
    const uint8_t valueIndex = 3;

    ServiceParameters_set1 parameters;
    parameters.a = event.actionType == DigitalButton::ActionType::ON ? 1 : 0;

    switch (event.affectedType)
    {
    case DigitalButton::AffectedType::DEVICE:
    {
        // CALL Room state change in requested Room ID
        std::any_cast<DeviceServicesAPI>(DataContainer::getSignalValue(SIG_DEVICE_SERVICES)).serviceCall_set1(event.affectedId, DEVSERVICE_STATE_SWITCH, parameters);
        break;
    }

    case DigitalButton::AffectedType::ROOM:
    {
        // CALL Room state change in requested Room ID
        std::any_cast<DeviceServicesAPI>(DataContainer::getSignalValue(SIG_DEVICE_SERVICES)).serviceCall_set1(event.affectedId, DEVSERVICE_ROOM_STATE_CHANGE, parameters);
        break;
    }
    }
}
