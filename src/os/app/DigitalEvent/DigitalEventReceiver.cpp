#include <os/app/DigitalEvent/DigitalEventReceiver.hpp>
#include <os/drivers/networkdriver.hpp>
#include "os/Logger.hpp"

std::vector<std::pair<uint64_t, DigitalEvent::Event>> DigitalEventReceiver::digitalEventsMapping;
std::queue<uint64_t> DigitalEventReceiver::eventsQueue;
std::queue<ServiceCallData> DigitalEventReceiver::pendingServiceCalls;

uint8_t DigitalEventReceiver::lastReceivedTransmissionId = 0;

const uint8_t NVM_VALID = 0xCD;

void DigitalEventReceiver::init()
{
    Logger::log("DigitalEventReceiver init ...");

    /* Read NVM data for DigitalEventReceiver application */
    uint16_t sizeOfNvm = (e_BLOCK_DIGITAL_EVENT_6 - e_BLOCK_DIGITAL_EVENT_1 + 1) * PERSISTENT_DATABLOCK_SIZE;
    /* Allocate memory for NVM data */
    uint8_t *nvmData = (uint8_t *)malloc(sizeOfNvm);
    uint8_t offset = 0;
    if (nvmData != nullptr)
    {
        for (uint8_t blockID = e_BLOCK_DIGITAL_EVENT_1; blockID <= e_BLOCK_DIGITAL_EVENT_6; blockID++)
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

            // Are there any Event events saved in nvm?
            if (numberOfElementsInNvm > 0)
            {
                for (uint8_t i = 0; i < numberOfElementsInNvm; i++)
                {
                    uint64_t EventUniqueId = 0x00;
                    DigitalEvent::Event event;

                    // Copy corresponding EventUniqueId
                    memcpy(&EventUniqueId, &(nvmData[2 + (i * (sizeof(EventUniqueId) + sizeof(event)))]), sizeof(uint64_t));

                    // Copy corresponding Event event
                    memcpy(&event, &(nvmData[2 + (i * (sizeof(EventUniqueId) + sizeof(event))) + sizeof(EventUniqueId)]), sizeof(event));

                    digitalEventsMapping.push_back(std::pair<uint64_t, DigitalEvent::Event>(EventUniqueId, event));
                }

                Logger::log("DigitalEventReceiver:// Restored " + String((int)numberOfElementsInNvm) + " Event events");
            }
        }

        free(nvmData);
    }

    DataContainer::setSignalValue(SIG_DIGITAL_EVNT_MAPPING, digitalEventsMapping);
    DataContainer::setSignalValue(CBK_UPDATE_DIG_EVNT_TABLE, std::function<void(String &)>(updateDigitalEventMappingViaJson));

    DataContainer::setSignalValue(
        CBK_FIRE_DIGITAL_EVENT,
        static_cast<std::function<void(uint64_t)>>(DigitalEventReceiver::fireEvent));

    // for(auto& mapping : digitalEventsMapping){
    //     Logger::log("Mapping ID: " + String((int) mapping.first) + " ");
    //     Logger::log("Mapping affectedType: " + String((int) mapping.second.affectedType) + " ");
    //     Logger::log("Mapping affectedId: " + String((int) mapping.second.affectedId) + " ");
    //     Logger::log("Mapping actionType: " + String((int) mapping.second.actionType) + " ");
    // }

    Logger::log("... done");
}

void DigitalEventReceiver::updateDigitalEventMappingViaJson(String &json)
{

    // Logger::log("  ");
    // Logger::log(json);

    JsonDocument doc;
    DeserializationError success = deserializeJson(doc, json.c_str());
    if (success == DeserializationError::Code::Ok)
    {
        digitalEventsMapping.clear();
        // oczekujemy tablicy na rootzie
        JsonArray arr = doc.as<JsonArray>();

        // iteracja po tablicy obiektów
        for (JsonObject obj : arr)
        {
            uint8_t requiredKeysCnt = 0;
            // domyślne puste Stringi
            uint64_t id = 0;
            uint8_t type = 0;
            uint32_t targetId = 0;
            uint8_t action = 0;

            // sprawdzamy czy klucz istnieje i przypisujemy
            if (obj.containsKey("id"))
            {
                id = obj["id"].as<uint64_t>();
                requiredKeysCnt++;
            }
            if (obj.containsKey("type"))
            {
                type = obj["type"].as<uint8_t>();
                requiredKeysCnt++;
            }
            if (obj.containsKey("targetId"))
            {
                targetId = obj["targetId"].as<uint32_t>();
                requiredKeysCnt++;
            }
            if (obj.containsKey("action"))
            {
                action = obj["action"].as<uint8_t>();
                requiredKeysCnt++;
            }
            if (requiredKeysCnt < 4)
            {
                Logger::log("DigitalEventReceiver:// Missing keys in JSON object, skipping");
                continue;
            }

            if (digitalEventsMapping.size() < 25)
            {
                DigitalEvent::Event ev;
                uint8_t affectedType = type == DigitalEvent::AffectedType::ROOM ? type : (uint8_t)DigitalEvent::AffectedType::DEVICE;
                uint8_t actionType;
                if (action >= DigitalEvent::ActionType::ON && action <= DigitalEvent::ActionType::TOGGLE)
                {
                    actionType = action;
                }
                else
                {
                    actionType = (int8_t)DigitalEvent::ActionType::OFF;
                }

                ev.actionType = actionType;
                ev.affectedType = affectedType;
                ev.affectedId = targetId;

                Logger::log("DigitalEventReceiver://Adding mapping ID: " + String((int)id) + " " + " affectedType: " + String((int)ev.affectedType) + " " + " affectedId: " + String((int)ev.affectedId) + " " + " actionType: " + String((int)ev.actionType) + " ");
                digitalEventsMapping.push_back({id, ev});
            }else {
                UserInterfaceNotification notif;
                notif.title = "Problem occured";
                notif.body = "Too many digital event mappings, maximum is 25. Some mappings have been skipped.";
                notif.type = UserInterfaceNotification::ERROR;
                std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);
            }
        }

        // Update data container
        DataContainer::setSignalValue(SIG_DIGITAL_EVNT_MAPPING, digitalEventsMapping);

        // Start NVM save countdown
        std::any_cast<std::function<void()>>(DataContainer::getSignalValue(CBK_START_NVM_SAVE_TIMER))();
    }
    else
    {
        Logger::log("DigitalEventReceiver:// Invalid JSON received");
    }
    // Logger::log("  ");
}

void DigitalEventReceiver::cyclic()
{
    processEvents();

    if (pendingServiceCalls.size() > 0)
    {
        Logger::log("DigitalEventReceiver:// Processing pending service call, queue size: " + String((int)pendingServiceCalls.size()));
        ServiceCallData &callData = pendingServiceCalls.front();

        Logger::log("Triggering service : " + String((int)callData.serviceType) + " on ID " + String((int)callData.deviceOrRoomId) + " with param value : " + String((int)callData.parameters.a));
        ServiceRequestErrorCode errorCode =
            std::any_cast<DeviceServicesAPI>(DataContainer::getSignalValue(SIG_DEVICE_SERVICES)).serviceCall_set1(callData.deviceOrRoomId, callData.serviceType, callData.parameters);

        // Logger::log("Service error code : " + String((int)errorCode));
        if (errorCode != SERV_BUSY && errorCode != SERV_PENDING)
        {
            String result = (errorCode == SERV_SUCCESS) ? "SUCCESS" : "FAILURE";
            Logger::log("DigitalEventReceiver:// Service call completed with result: " + result);
            // Processing succeeded or failed, but for sure not queued
            pendingServiceCalls.pop();
            // Logger::log("x x x x Event processing completed");
        }
    }
}

void DigitalEventReceiver::fireEvent(uint64_t eventId)
{
    Logger::log("DigitalEventReceiver:// Event with ID: " + String((unsigned long long)eventId) + " fired");
    // Push event to the queue
    eventsQueue.push(eventId);
}

void DigitalEventReceiver::deinit()
{
    // We only have NVM data if we handshaked at least 1 slave node
    /* Write NVM data for DigitalEventReceiver application */
    uint16_t sizeOfNvm = (e_BLOCK_DIGITAL_EVENT_6 - e_BLOCK_DIGITAL_EVENT_1 + 1) * PERSISTENT_DATABLOCK_SIZE;
    /* Allocate memory for NVM data */
    uint8_t *nvmData = (uint8_t *)malloc(sizeOfNvm);

    // Data validity indicator
    nvmData[0] = NVM_VALID;
    // Number of mappings present in the system
    nvmData[1] = digitalEventsMapping.size();

    uint8_t i = 0;
    // Serialize map to nvmData raw memory
    for (auto it = digitalEventsMapping.begin(); it != digitalEventsMapping.end(); ++it, ++i)
    {
        memcpy(&(nvmData[2 + i * (sizeof(uint64_t) + sizeof(DigitalEvent::Event))]), &(it->first), sizeof(uint64_t));
        memcpy(&(nvmData[2 + (i * (sizeof(uint64_t) + sizeof(DigitalEvent::Event)) + sizeof(uint64_t))]), &(it->second), sizeof(DigitalEvent::Event));
    }

    // Copy raw memory to datablocks
    uint8_t offset = 0;
    for (uint8_t blockID = e_BLOCK_DIGITAL_EVENT_1; blockID <= e_BLOCK_DIGITAL_EVENT_6; blockID++)
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

void DigitalEventReceiver::receiveUDP(MessageUDP &msg)
{
    /* Received UDP Message */
    // Process the received digital Event message here
    if (msg.getId() == DIGITAL_EVENT_FIRED_MSG_ID)
    {
        std::vector<uint8_t> &payload = msg.getPayload();
        if (payload.size() == (sizeof(uint8_t) + sizeof(uint64_t)))
        {
            uint8_t transmissionIdentfier = 0x00;
            memcpy(&transmissionIdentfier, &(payload.at(0)), sizeof(transmissionIdentfier));
            uint64_t triggeredEvent = 0;
            memcpy(&triggeredEvent, &(payload.at(1)), sizeof(triggeredEvent));

            // Protection against repeated request fake activation
            if (transmissionIdentfier != lastReceivedTransmissionId)
            {
                Logger::log("DigitalEventReceiver:// Received DIGITAL_EVENT_FIRED_MSG_ID for event ID: " + String((unsigned long long)triggeredEvent) + " with transmission ID: " + String((int)transmissionIdentfier));
                lastReceivedTransmissionId = transmissionIdentfier;
                fireEvent(triggeredEvent);

                // Send back the handling confirmation
                MessageUDP msg(DIGITAL_EVENT_CONFIRMED_MSG_ID, NETWORK_BROADCAST, 9001);
                msg.pushData((uint8_t *)&triggeredEvent, sizeof(uint64_t));
                NetworkDriver::sendBroadcast(msg);
            }
        }
        else
        {
            Logger::log("DigitalEventReceiver:// Invalid length of received DigitalEvent message");
        }
    }
}

void DigitalEventReceiver::processEvents()
{

    if (eventsQueue.size() > 0)
    {
        uint64_t eventHappened = eventsQueue.front();
        bool found = false;
        for (auto &mapping : digitalEventsMapping)
        {
            if (mapping.first == eventHappened)
            {
                executeAction(mapping.second);
                found = true;
            }
        }

        if(!found){
            UserInterfaceNotification notif;
                notif.title = "Unmapped Event";
                notif.body = "Event with ID: " + String((unsigned long long)eventHappened) + " has been fired but there is no action mapped to it.";
                notif.type = UserInterfaceNotification::INFO;
                std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);
        }

        eventsQueue.pop();
    }
}

const uint8_t deviceOrRoomIdIndex = 2;
const uint8_t serviceOverloadingIndex = 1;
const uint8_t serviceTypeIndex = 0;
const uint8_t valueIndex = 3;
void DigitalEventReceiver::executeAction(DigitalEvent::Event &action)
{
    switch (action.affectedType)
    {
    case DigitalEvent::AffectedType::DEVICE:
        deviceAction(action);
        break;

    case DigitalEvent::AffectedType::ROOM:
        roomAction(action);
        break;

    default:
        break;
    }
}

void DigitalEventReceiver::deviceAction(DigitalEvent::Event &action)
{
    ServiceParameters_set1 parameters;

    if (action.actionType == DigitalEvent::ActionType::TOGGLE)
    {
        // We need device current status in order to toggle it
        std::vector<DeviceDescription> devicesVector =
            std::any_cast<std::vector<DeviceDescription>>(
                DataContainer::getSignalValue(SIG_DEVICE_COLLECTION));
        for (auto &device : devicesVector)
        {
            if (device.deviceId == action.affectedId)
            {
                parameters.a = !(device.isEnabled);
                break;
            }
        }
    }
    else
    {
        parameters.a = action.actionType == DigitalEvent::ActionType::ON ? 1 : 0;
    }

    Logger::log("DigitalEventReceiver:// Added DEVICE action");
    pendingServiceCalls.push({action.affectedId, DEVSERVICE_STATE_SWITCH, parameters});
    // std::any_cast<DeviceServicesAPI>(DataContainer::getSignalValue(SIG_DEVICE_SERVICES)).serviceCall_set1(action.affectedId, DEVSERVICE_STATE_SWITCH, parameters);
}

void DigitalEventReceiver::roomAction(DigitalEvent::Event &action)
{
    ServiceParameters_set1 parameters;

    if (action.actionType == DigitalEvent::ActionType::TOGGLE)
    {
        // We need to evaluate current room state to toggle it
        std::vector<DeviceDescription> devicesVector =
            std::any_cast<std::vector<DeviceDescription>>(
                DataContainer::getSignalValue(SIG_DEVICE_COLLECTION));
        uint8_t isRoomOn = 0;

        for (auto &device : devicesVector)
        {
            if (device.roomId == action.affectedId)
            {
                // Found at least 1 device enabled in target ROOM
                if (device.isEnabled)
                {
                    isRoomOn = 1;
                    break;
                }
            }
        }

        parameters.a = !(isRoomOn);
    }
    else
    {
        parameters.a = action.actionType == DigitalEvent::ActionType::ON ? 1 : 0;
    }

    Logger::log("DigitalEventReceiver:// Added ROOM action");
    pendingServiceCalls.push({action.affectedId, DEVSERVICE_ROOM_STATE_CHANGE, parameters});
    // std::any_cast<DeviceServicesAPI>(DataContainer::getSignalValue(SIG_DEVICE_SERVICES)).serviceCall_set1(action.affectedId, DEVSERVICE_ROOM_STATE_CHANGE, parameters);
}
