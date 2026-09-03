#include <os/app/DigitalEvent/DigitalEventReceiver.hpp>
#include <os/drivers/networkdriver.hpp>
#include "generated/GeneratedDigitalEventActions.hpp"
#include "generated/GeneratedEnablingConditions.hpp"
#include "os/Logger.hpp"
#include <algorithm>

/**
 * @file src/os/app/DigitalEvent/DigitalEventReceiver.cpp
 * @brief Digital event processing for the Home Control Station.
 */


std::vector<std::pair<uint64_t, DigitalEvent::Event>> DigitalEventReceiver::digitalEventsMapping;
std::vector<DigitalEvent::EnablingCondition> DigitalEventReceiver::enablingConditions;
std::vector<DigitalEvent::MappingConditions> DigitalEventReceiver::mappingConditions;
std::queue<DigitalEventOccurrence> DigitalEventReceiver::eventsQueue;
std::vector<DigitalEventOccurrence> DigitalEventReceiver::unmappedEvents;
std::queue<ServiceCallData> DigitalEventReceiver::pendingServiceCalls;
std::queue<PendingConditionalAction> DigitalEventReceiver::pendingConditionalActions;

std::vector<std::pair<uint32_t, uint8_t>> DigitalEventReceiver::receivedTransmissionIds;

const uint8_t NVM_VALID = 0xCD;
const uint8_t LEGACY_ROOM_TYPE = 11;
const uint8_t LEGACY_DEVICE_TYPE = 12;

template <typename Record>
static void restoreFixedTable(PersistentDatablockID firstBlock, PersistentDatablockID lastBlock,
                              uint8_t maximumRecords, std::vector<Record> &records)
{
    const uint16_t size = (lastBlock - firstBlock + 1) * PERSISTENT_DATABLOCK_SIZE;
    const uint8_t physicalCapacity = (size - 2) / sizeof(Record);
    std::vector<uint8_t> data(size, 0);
    uint16_t offset = 0;
    for (uint8_t block = firstBlock; block <= lastBlock; ++block, offset += PERSISTENT_DATABLOCK_SIZE)
    {
        std::any_cast<std::function<bool(PersistentDatablockID, uint8_t *)>>(
            DataContainer::getSignalValue(CBK_GET_NVM_DATABLOCK))(
            static_cast<PersistentDatablockID>(block), &data[offset]);
    }
    if (data[0] != NVM_VALID) return;
    const uint8_t count = std::min(data[1], std::min(maximumRecords, physicalCapacity));
    for (uint8_t index = 0; index < count; ++index)
    {
        Record record{};
        memcpy(&record, &data[2 + index * sizeof(Record)], sizeof(Record));
        records.push_back(record);
    }
    Logger::log("DigitalEventReceiver:// Restored " + String((int)count) +
                " records from NVM blocks " + String((int)firstBlock) + "-" + String((int)lastBlock));
}

template <typename Record>
static void saveFixedTable(PersistentDatablockID firstBlock, PersistentDatablockID lastBlock,
                           const std::vector<Record> &records)
{
    const uint16_t size = (lastBlock - firstBlock + 1) * PERSISTENT_DATABLOCK_SIZE;
    const size_t physicalCapacity = (size - 2) / sizeof(Record);
    const size_t count = std::min(records.size(), physicalCapacity);
    std::vector<uint8_t> data(size, 0);
    data[0] = NVM_VALID;
    data[1] = static_cast<uint8_t>(count);
    for (size_t index = 0; index < count; ++index)
    {
        memcpy(&data[2 + index * sizeof(Record)], &records[index], sizeof(Record));
    }
    uint16_t offset = 0;
    for (uint8_t block = firstBlock; block <= lastBlock; ++block, offset += PERSISTENT_DATABLOCK_SIZE)
    {
        std::any_cast<std::function<bool(PersistentDatablockID, uint8_t *)>>(
            DataContainer::getSignalValue(CBK_SET_NVM_DATABLOCK))(
            static_cast<PersistentDatablockID>(block), &data[offset]);
    }
    Logger::log("DigitalEventReceiver:// Saved " + String((int)count) +
                " records to NVM blocks " + String((int)firstBlock) + "-" + String((int)lastBlock));
}

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
    digitalEventsMapping.clear();
    enablingConditions.clear();
    mappingConditions.clear();
    eventsQueue = {};
    pendingServiceCalls = {};
    pendingConditionalActions = {};

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
            const uint8_t numberOfElementsInNvm =
                std::min(nvmData[1], DigitalEvent::MAX_EVENT_MAPPINGS);

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

    restoreFixedTable(e_BLOCK_ENABLING_CONDITIONS_1, e_BLOCK_ENABLING_CONDITIONS_3,
                      DigitalEvent::MAX_ENABLING_CONDITIONS, enablingConditions);
    restoreFixedTable(e_BLOCK_DIGITAL_EVENT_CONDITIONS_1, e_BLOCK_DIGITAL_EVENT_CONDITIONS_3,
                      DigitalEvent::MAX_EVENT_MAPPINGS, mappingConditions);

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
        mappingConditions.clear();
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

            if (digitalEventsMapping.size() < DigitalEvent::MAX_EVENT_MAPPINGS)
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

                DigitalEvent::MappingConditions assignment{};
                assignment.mappingId = mappingId;
                JsonArray conditionIds = obj["conditionIds"].as<JsonArray>();
                uint8_t conditionIndex = 0;
                for (JsonVariant conditionValue : conditionIds)
                {
                    if (conditionIndex >= DigitalEvent::MAX_CONDITIONS_PER_MAPPING) break;
                    const uint8_t conditionId = conditionValue.as<uint8_t>();
                    const bool exists = std::any_of(enablingConditions.begin(), enablingConditions.end(),
                                                    [conditionId](const auto &condition) { return condition.conditionId == conditionId; });
                    const bool duplicate = std::find(std::begin(assignment.conditionIds), std::end(assignment.conditionIds), conditionId) != std::end(assignment.conditionIds);
                    if (conditionId != 0 && exists && !duplicate) assignment.conditionIds[conditionIndex++] = conditionId;
                }
                mappingConditions.push_back(assignment);
            }else {
                UserInterfaceNotification notif;
                notif.title = "Problem occured";
                notif.body = "Too many digital event mappings. Some mappings have been skipped.";
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
    processPendingConditions();

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

const std::vector<DigitalEvent::EnablingCondition> &DigitalEventReceiver::getEnablingConditions()
{
    return enablingConditions;
}

const std::vector<DigitalEvent::MappingConditions> &DigitalEventReceiver::getMappingConditions()
{
    return mappingConditions;
}

void DigitalEventReceiver::updateEnablingConditionsViaJson(String &json)
{
    JsonDocument doc;
    if (deserializeJson(doc, json) != DeserializationError::Ok || !doc.is<JsonArray>())
    {
        Logger::log("DigitalEventReceiver:// Invalid enabling conditions JSON");
        return;
    }

    std::vector<DigitalEvent::EnablingCondition> updated;
    const auto devices = std::any_cast<std::vector<DeviceDescription>>(DataContainer::getSignalValue(SIG_DEVICE_COLLECTION));
    for (JsonObject object : doc.as<JsonArray>())
    {
        if (updated.size() >= DigitalEvent::MAX_ENABLING_CONDITIONS) break;
        const uint8_t conditionId = object["conditionId"].as<uint8_t>();
        const uint32_t deviceId = object["deviceId"].as<uint32_t>();
        const uint8_t predicateId = object["predicateId"].as<uint8_t>();
        if (conditionId == 0 || deviceId == 0 || predicateId == 0) continue;
        const bool duplicate = std::any_of(updated.begin(), updated.end(), [conditionId](const auto &condition)
                                           { return condition.conditionId == conditionId; });
        auto device = std::find_if(devices.begin(), devices.end(), [deviceId](const auto &item)
                                   { return item.deviceId == deviceId; });
        if (duplicate || device == devices.end() || GeneratedEnablingConditions::find(device->deviceType, predicateId) == nullptr)
        {
            Logger::log("DigitalEventReceiver:// Invalid or duplicate enabling condition, skipping");
            continue;
        }
        updated.push_back({conditionId, deviceId, predicateId});
    }
    enablingConditions = std::move(updated);
    for (auto &assignment : mappingConditions)
    {
        for (uint8_t &conditionId : assignment.conditionIds)
        {
            const bool exists = std::any_of(enablingConditions.begin(), enablingConditions.end(),
                                            [conditionId](const auto &condition) { return condition.conditionId == conditionId; });
            if (!exists) conditionId = 0;
        }
    }
    std::any_cast<std::function<void()>>(DataContainer::getSignalValue(CBK_START_NVM_SAVE_TIMER))();
}

void DigitalEventReceiver::deinit()
{
    // We only have NVM data if we handshaked at least 1 slave node
    /* Write NVM data for DigitalEventReceiver application */
    uint16_t sizeOfNvm = (e_BLOCK_DIGITAL_EVENT_6 - e_BLOCK_DIGITAL_EVENT_1 + 1) * PERSISTENT_DATABLOCK_SIZE;
    /* Allocate memory for NVM data */
    uint8_t *nvmData = static_cast<uint8_t *>(calloc(sizeOfNvm, sizeof(uint8_t)));
    if (nvmData == nullptr) return;

    // Data validity indicator
    nvmData[0] = NVM_VALID;
    // Number of mappings present in the system
    const size_t mappingCount = std::min(digitalEventsMapping.size(), static_cast<size_t>(DigitalEvent::MAX_EVENT_MAPPINGS));
    nvmData[1] = mappingCount;

    uint8_t i = 0;
    // Serialize map to nvmData raw memory
    for (auto it = digitalEventsMapping.begin(); it != digitalEventsMapping.begin() + mappingCount; ++it, ++i)
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

    saveFixedTable(e_BLOCK_ENABLING_CONDITIONS_1, e_BLOCK_ENABLING_CONDITIONS_3, enablingConditions);
    saveFixedTable(e_BLOCK_DIGITAL_EVENT_CONDITIONS_1, e_BLOCK_DIGITAL_EVENT_CONDITIONS_3, mappingConditions);
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
                auto assignment = std::find_if(mappingConditions.begin(), mappingConditions.end(), [&mapping](const auto &item)
                                               { return item.mappingId == mapping.second.mappingId; });
                if (assignment == mappingConditions.end() ||
                    std::all_of(std::begin(assignment->conditionIds), std::end(assignment->conditionIds), [](uint8_t id) { return id == 0; }))
                {
                    executeAction(mapping.second);
                }
                else
                {
                    PendingConditionalAction pending{};
                    pending.action = mapping.second;
                    std::copy(std::begin(assignment->conditionIds), std::end(assignment->conditionIds), pending.conditionIds);
                    pendingConditionalActions.push(pending);
                }
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

void DigitalEventReceiver::processPendingConditions()
{
    if (pendingConditionalActions.empty()) return;

    PendingConditionalAction &pending = pendingConditionalActions.front();
    while (pending.conditionIndex < DigitalEvent::MAX_CONDITIONS_PER_MAPPING &&
           pending.conditionIds[pending.conditionIndex] == 0)
    {
        ++pending.conditionIndex;
    }
    if (pending.conditionIndex >= DigitalEvent::MAX_CONDITIONS_PER_MAPPING)
    {
        executeAction(pending.action);
        pendingConditionalActions.pop();
        return;
    }

    const uint8_t conditionId = pending.conditionIds[pending.conditionIndex];
    if (pending.activeDeviceId == 0)
    {
        auto condition = std::find_if(enablingConditions.begin(), enablingConditions.end(), [conditionId](const auto &item)
                                      { return item.conditionId == conditionId; });
        if (condition == enablingConditions.end())
        {
            Logger::log("DigitalEventReceiver:// Mapping blocked by missing enabling condition");
            pendingConditionalActions.pop();
            return;
        }
        pending.activeDeviceId = condition->deviceId;
        pending.activePredicateId = condition->predicateId;
    }

    if (!pending.requestPending) pending.result = 0;
    ServiceParameters_set3 parameters;
    parameters.buff = &pending.result;
    parameters.size = 1;
    parameters.additionalParam = pending.activePredicateId;
    parameters.direction = e_OUT_from_DEVICE;
    const ServiceRequestErrorCode result =
        std::any_cast<DeviceServicesAPI>(DataContainer::getSignalValue(SIG_DEVICE_SERVICES))
            .serviceCall_set3(pending.activeDeviceId, DEVSERVICE_CHECK_ENABLING_CONDITION, parameters);

    if (result == SERV_BUSY) return;
    if (result == SERV_PENDING)
    {
        pending.requestPending = true;
        return;
    }
    if (result == SERV_NOT_SUPPORTED)
    {
        const auto devices = std::any_cast<std::vector<DeviceDescription>>(DataContainer::getSignalValue(SIG_DEVICE_COLLECTION));
        auto device = std::find_if(devices.begin(), devices.end(), [&pending](const auto &item)
                                   { return item.deviceId == pending.activeDeviceId; });
        pending.result = device != devices.end() &&
                         GeneratedEnablingConditions::evaluate(*device, pending.activePredicateId);
        Logger::log("DigitalEventReceiver:// Using advertised-state fallback for a legacy condition device");
    }
    if ((result != SERV_SUCCESS && result != SERV_NOT_SUPPORTED) || pending.result == 0)
    {
        Logger::log("DigitalEventReceiver:// Mapping " + String((int)pending.action.mappingId) + " blocked by enabling condition " + String((int)conditionId));
        pendingConditionalActions.pop();
        return;
    }

    pending.requestPending = false;
    pending.activeDeviceId = 0;
    pending.activePredicateId = 0;
    ++pending.conditionIndex;
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
