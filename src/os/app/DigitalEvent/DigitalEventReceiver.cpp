#include <os/app/DigitalEvent/DigitalEventReceiver.hpp>
#include <os/drivers/networkdriver.hpp>
#include "generated/GeneratedDigitalEventActions.hpp"
#include "os/Logger.hpp"
#include <algorithm>

/**
 * @file src/os/app/DigitalEvent/DigitalEventReceiver.cpp
 * @brief Digital event processing for the Home Control Station.
 */


std::vector<std::pair<uint64_t, DigitalEvent::Event>> DigitalEventReceiver::digitalEventsMapping;
std::queue<DigitalEventOccurrence> DigitalEventReceiver::eventsQueue;
std::vector<DigitalEventOccurrence> DigitalEventReceiver::unmappedEvents;
std::queue<ServiceCallData> DigitalEventReceiver::pendingServiceCalls;

std::vector<std::pair<uint32_t, uint8_t>> DigitalEventReceiver::receivedTransmissionIds;

const uint8_t NVM_VALID = 0xCD;
const uint8_t LEGACY_ROOM_TYPE = 11;
const uint8_t LEGACY_DEVICE_TYPE = 12;

static uint8_t nextMappingId(const std::vector<std::pair<uint64_t, DigitalEvent::Event>> &mappings)
{
    for (uint16_t candidate = 1; candidate <= 255; ++candidate)
    {
        if (candidate == LEGACY_ROOM_TYPE || candidate == LEGACY_DEVICE_TYPE) continue;
        bool used = false;
        for (const auto &mapping : mappings)
        {
            if (mapping.second.mappingId == candidate)
            {
                used = true;
                break;
            }
        }
        if (!used) return static_cast<uint8_t>(candidate);
    }
    return 0;
}

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

                    if (event.mappingId == LEGACY_ROOM_TYPE)
                    {
                        Logger::log("DigitalEventReceiver:// Ignoring legacy room mapping");
                        continue;
                    }
                    if (event.mappingId == LEGACY_DEVICE_TYPE)
                    {
                        event.mappingId = nextMappingId(digitalEventsMapping);
                    }
                    if (event.mappingId != 0)
                    {
                        digitalEventsMapping.push_back(std::pair<uint64_t, DigitalEvent::Event>(EventUniqueId, event));
                    }
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
        std::function<void(uint64_t)>(static_cast<void (*)(uint64_t)>(&DigitalEventReceiver::fireEvent)));
    DataContainer::setSignalValue(
        CBK_FIRE_DIGITAL_EVENT_WITH_SOURCE,
        std::function<void(uint64_t, const String &)>(
            static_cast<void (*)(uint64_t, const String &)>(&DigitalEventReceiver::fireEvent)));

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
            if (!obj.containsKey("eventId") || !obj.containsKey("deviceId") || !obj.containsKey("actionId"))
            {
                Logger::log("DigitalEventReceiver:// Missing keys in JSON object, skipping");
                continue;
            }

            if (digitalEventsMapping.size() < 25)
            {
                uint64_t eventId = obj["eventId"].as<uint64_t>();
                uint32_t deviceId = obj["deviceId"].as<uint32_t>();
                uint8_t actionId = obj["actionId"].as<uint8_t>();
                uint8_t mappingId = obj.containsKey("mappingId") ? obj["mappingId"].as<uint8_t>() : nextMappingId(digitalEventsMapping);
                if (mappingId == 0 || mappingId == LEGACY_ROOM_TYPE || mappingId == LEGACY_DEVICE_TYPE)
                {
                    mappingId = nextMappingId(digitalEventsMapping);
                }

                uint8_t deviceType = 255;
                auto devices = std::any_cast<std::vector<DeviceDescription>>(DataContainer::getSignalValue(SIG_DEVICE_COLLECTION));
                for (const auto &device : devices)
                {
                    if (device.deviceId == deviceId)
                    {
                        deviceType = device.deviceType;
                        break;
                    }
                }
                if (mappingId == 0 || GeneratedDigitalEventActions::find(deviceType, actionId) == nullptr)
                {
                    Logger::log("DigitalEventReceiver:// Invalid device or action, skipping mapping");
                    continue;
                }
                bool duplicateMappingId = false;
                for (const auto &mapping : digitalEventsMapping)
                {
                    if (mapping.second.mappingId == mappingId) duplicateMappingId = true;
                }
                if (duplicateMappingId)
                {
                    Logger::log("DigitalEventReceiver:// Duplicate mapping ID, skipping mapping");
                    continue;
                }

                DigitalEvent::Event event{mappingId, deviceId, actionId};
                Logger::log("DigitalEventReceiver:// Adding mapping ID: " + String((int)mappingId) + " device ID: " + String((int)deviceId) + " action ID: " + String((int)actionId));
                digitalEventsMapping.push_back({eventId, event});
            }else {
                UserInterfaceNotification notif;
                notif.title = "Problem occured";
                notif.body = "Too many digital event mappings, maximum is 25. Some mappings have been skipped.";
                notif.type = UserInterfaceNotification::ERROR;
                std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);
            }
        }

        unmappedEvents.erase(
            std::remove_if(unmappedEvents.begin(), unmappedEvents.end(), [](const DigitalEventOccurrence &occurrence)
                           {
                               return std::any_of(digitalEventsMapping.begin(), digitalEventsMapping.end(),
                                                  [&occurrence](const auto &mapping)
                                                  { return mapping.first == occurrence.eventId; });
                           }),
            unmappedEvents.end());

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

        Logger::log("Triggering service : " + String((int)callData.serviceType) + " on ID " + String((int)callData.deviceId) + " with param value : " + String((int)callData.parameters.a));
        ServiceRequestErrorCode errorCode =
            std::any_cast<DeviceServicesAPI>(DataContainer::getSignalValue(SIG_DEVICE_SERVICES)).serviceCall_set1(callData.deviceId, callData.serviceType, callData.parameters);

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
    fireEvent(eventId, "Legacy");
}

void DigitalEventReceiver::fireEvent(uint64_t eventId, const String &source)
{
    String normalizedSource = source.length() ? source.substring(0, DIGITAL_EVENT_SOURCE_MAX_LENGTH) : "Unknown";
    Logger::log("DigitalEventReceiver:// Event with ID: " + String((unsigned long long)eventId) + " fired by " + normalizedSource);
    // Push event to the queue
    eventsQueue.push({eventId, normalizedSource});

}

const std::vector<DigitalEventOccurrence> &DigitalEventReceiver::getUnmappedEvents()
{
    return unmappedEvents;
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
    if (msg.getId() == DIGITAL_EVENT_FIRED_MSG_ID || msg.getId() == DIGITAL_EVENT_FIRED_WITH_SOURCE_MSG_ID)
    {
        std::vector<uint8_t> &payload = msg.getPayload();
        const size_t legacyPayloadSize = sizeof(uint8_t) + sizeof(uint64_t);
        const bool legacyMessage = msg.getId() == DIGITAL_EVENT_FIRED_MSG_ID;
        if ((legacyMessage && payload.size() == legacyPayloadSize) ||
            (!legacyMessage && payload.size() > legacyPayloadSize &&
             payload.size() <= legacyPayloadSize + DIGITAL_EVENT_SOURCE_MAX_LENGTH))
        {
            uint8_t transmissionIdentfier = 0x00;
            memcpy(&transmissionIdentfier, &(payload.at(0)), sizeof(transmissionIdentfier));
            uint64_t triggeredEvent = 0;
            memcpy(&triggeredEvent, &(payload.at(1)), sizeof(triggeredEvent));

            const auto &sender = msg.getIPAddress();
            const uint32_t senderKey = (static_cast<uint32_t>(sender.octet1) << 24) |
                                       (static_cast<uint32_t>(sender.octet2) << 16) |
                                       (static_cast<uint32_t>(sender.octet3) << 8) |
                                       sender.octet4;
            auto senderState = std::find_if(receivedTransmissionIds.begin(), receivedTransmissionIds.end(),
                                            [senderKey](const auto &entry) { return entry.first == senderKey; });
            if (senderState == receivedTransmissionIds.end() || senderState->second != transmissionIdentfier)
            {
                String source = "Legacy remote";
                if (!legacyMessage)
                {
                    source = "";
                    for (size_t i = legacyPayloadSize; i < payload.size(); ++i) source += static_cast<char>(payload[i]);
                }
                Logger::log("DigitalEventReceiver:// Received digital event ID: " + String((unsigned long long)triggeredEvent) + " from " + source + " with transmission ID: " + String((int)transmissionIdentfier));
                if (senderState == receivedTransmissionIds.end())
                {
                    if (receivedTransmissionIds.size() >= 32) receivedTransmissionIds.erase(receivedTransmissionIds.begin());
                    receivedTransmissionIds.push_back({senderKey, transmissionIdentfier});
                }
                else
                {
                    senderState->second = transmissionIdentfier;
                }
                fireEvent(triggeredEvent, source);
            }

            // Re-acknowledge duplicate retries as well. Processing remains idempotent.
            MessageUDP confirmationMessage(DIGITAL_EVENT_CONFIRMED_MSG_ID, msg.getIPAddress(), 9001);
            confirmationMessage.pushData((uint8_t *)&triggeredEvent, sizeof(uint64_t));
            NetworkDriver::send(confirmationMessage);
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
        DigitalEventOccurrence occurrence = eventsQueue.front();
        bool found = false;
        for (auto &mapping : digitalEventsMapping)
        {
            if (mapping.first == occurrence.eventId)
            {
                executeAction(mapping.second);
                found = true;
            }
        }

        if(!found){
            unmappedEvents.insert(unmappedEvents.begin(), occurrence);
            if (unmappedEvents.size() > 10) unmappedEvents.pop_back();
        }

        eventsQueue.pop();
    }
}

void DigitalEventReceiver::executeAction(DigitalEvent::Event &action)
{
    deviceAction(action);
}

void DigitalEventReceiver::deviceAction(DigitalEvent::Event &action)
{
    uint8_t deviceType = 255;
    bool currentState = false;
    std::vector<DeviceDescription> devicesVector =
        std::any_cast<std::vector<DeviceDescription>>(DataContainer::getSignalValue(SIG_DEVICE_COLLECTION));
    for (const auto &device : devicesVector)
    {
        if (device.deviceId == action.deviceId)
        {
            deviceType = device.deviceType;
            currentState = device.isEnabled;
            break;
        }
    }

    const auto *eventAction = GeneratedDigitalEventActions::find(deviceType, action.actionId);
    if (eventAction == nullptr)
    {
        Logger::log("DigitalEventReceiver:// Action is not supported by target device");
        return;
    }

    ServiceParameters_set1 parameters = eventAction->parameters;
    if (eventAction->toggleState) parameters.a = !currentState;
    Logger::log("DigitalEventReceiver:// Added DEVICE action");
    pendingServiceCalls.push({action.deviceId, eventAction->service, parameters});
}
