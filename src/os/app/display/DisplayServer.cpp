#include "os/app/display/DisplayServer.hpp"

#include "os/drivers/networkdriver.hpp"
#include "os/app/DeviceProvider.hpp"
#include "os/datacontainer/DataContainer.hpp"
#include "devices/device.hpp"
#include <algorithm>
#include <map>
#include <cstring>

/**
 * @file src/os/app/display/DisplayServer.cpp
 * @brief Display and packing logic for the Home Control Station UI.
 */


std::vector<Display> DisplayServer::knownDisplays;
unsigned long DisplayServer::lastDiscoveryTime = 0;
std::queue<MessageUDP> DisplayServer::receptionUdpQueue;
DisplayServer::PendingAction DisplayServer::pendingAction;
std::vector<std::pair<uint8_t, uint8_t>> DisplayServer::roomStates;

void DisplayServer::init()
{
    Logger::log("DisplayServer init ...");

    knownDisplays.clear();
    receptionUdpQueue = {};
    pendingAction = {};
    roomStates.clear();
    DataContainer::subscribe(SIG_DEVICE_COLLECTION, [](std::any) {
        auto devices = std::any_cast<std::vector<DeviceDescription>>(DataContainer::getSignalValue(SIG_DEVICE_COLLECTION));
        for (const auto& device : devices) DisplayServer::broadcastDeviceState(device.deviceId);
        DisplayServer::refreshRoomStates();
    });
    DataContainer::subscribe(SIG_ROOM_NAMES_MAPPING, [](std::any) {
        for (const auto& display : knownDisplays) DisplayServer::sendRoomNames(display);
    });

    Logger::log("... done");
}

void DisplayServer::deinit()
{
    Logger::log("DisplayServer deinit ...");

    Logger::log("... done");
    knownDisplays.clear();
    receptionUdpQueue = {};
    pendingAction = {};
}

void DisplayServer::cyclic()
{
    discoverDisplays();
    processPendingAction();
    processReceptionBuffer();
}

void DisplayServer::discoverDisplays(){
    if(millis() - lastDiscoveryTime < DISPLAY_DISCOVERY_FREQ_MS){
        return;
    }

    MessageUDP discoveryMsg((uint8_t)DisplayMessageIds::DISCOVER_DISPLAYS, NETWORK_BROADCAST, 9001);
    NetworkDriver::sendBroadcast(discoveryMsg);

    lastDiscoveryTime = millis();
}


void DisplayServer::processReceptionBuffer(){
    // Only process single message per cyclic to avoid long processing time in case of many messages received in short time
    if(receptionUdpQueue.size() > 0){
        MessageUDP msg = receptionUdpQueue.front();
        receptionUdpQueue.pop();
        if(msg.getId() == (uint8_t)DisplayMessageIds::DISPLAY_PRESENT){
            handleNewDisplay(msg);
        } else {
            handleMessage(msg);
        }
    }
}

void DisplayServer::receiveUDP(MessageUDP &msg)
{
    // add received message to reception queue for further processing in cyclic
    receptionUdpQueue.push(msg);
}

void DisplayServer::handleNewDisplay(MessageUDP &msg){
    // Check if display is already know by unpacking data from message as DisplayInfo and add to knownDisplays if not present
    DisplayInfo newDisplayInfo;
    std::vector<uint8_t> &payload = msg.getPayload();
    if(payload.size() == sizeof(DisplayInfo)){
        memcpy(&newDisplayInfo, payload.data(), sizeof(DisplayInfo));
        newDisplayInfo.ip = msg.getIPAddress();
        Display newDisplay(newDisplayInfo);
        auto it = std::find(knownDisplays.begin(), knownDisplays.end(), newDisplay);
        if(it == knownDisplays.end()){
            Logger::log("DisplayServer:// New display discovered with MAC: " + String((unsigned long long)newDisplayInfo.macAddress));
            knownDisplays.push_back(newDisplay);
            it = std::prev(knownDisplays.end());
        } else {
            it->updateInfo(newDisplayInfo);
        }
        DisplayHandshake handshake;
        handshake.displayId = static_cast<uint8_t>(std::distance(knownDisplays.begin(), it));
        auto devices = std::any_cast<std::vector<DeviceDescription>>(DataContainer::getSignalValue(SIG_DEVICE_COLLECTION));
        handshake.deviceCount = std::count_if(devices.begin(), devices.end(), [&newDisplayInfo](const DeviceDescription& device) {
            return !(newDisplayInfo.macAddress != 0 && device.macAddress == newDisplayInfo.macAddress);
        });
        MessageUDP response((uint8_t)DisplayMessageIds::DISPLAY_HANDSHAKE, msg.getIPAddress(), DISPLAY_UDP_PORT);
        response.pushData(reinterpret_cast<byte*>(&handshake), sizeof(handshake));
        NetworkDriver::send(response);
        sendDeviceList(*it);
    }else {
        Logger::log("DisplayServer:// Received DISPLAY_PRESENT message with invalid payload size");
    }
}

const std::vector<Display>& DisplayServer::getKnownDisplays()
{
    return knownDisplays;
}

void DisplayServer::sendDeviceList(const Display& display)
{
    auto devices = std::any_cast<std::vector<DeviceDescription>>(DataContainer::getSignalValue(SIG_DEVICE_COLLECTION));
    // Devices hosted by the display itself are controlled and kept locally - no need to synchronize them
    const uint64_t displayMac = display.getInfo().macAddress;
    if (displayMac != 0) {
        devices.erase(std::remove_if(devices.begin(), devices.end(), [displayMac](const DeviceDescription& device) {
            return device.macAddress == displayMac;
        }), devices.end());
    }
    const uint8_t totalChunks = static_cast<uint8_t>((devices.size() + DISPLAY_MAX_DEVICES_PER_CHUNK - 1) / DISPLAY_MAX_DEVICES_PER_CHUNK);
    for (uint8_t chunkIndex = 0; chunkIndex < totalChunks; ++chunkIndex) {
        DisplayDeviceListChunk chunk;
        chunk.chunkIndex = chunkIndex;
        chunk.totalChunks = totalChunks;
        const size_t start = chunkIndex * DISPLAY_MAX_DEVICES_PER_CHUNK;
        chunk.deviceCount = static_cast<uint8_t>(std::min<size_t>(DISPLAY_MAX_DEVICES_PER_CHUNK, devices.size() - start));
        for (uint8_t i = 0; i < chunk.deviceCount; ++i) {
            const auto& source = devices[start + i];
            auto& target = chunk.devices[i];
            target.deviceType = source.deviceType;
            target.macAddress = source.macAddress;
            target.deviceId = source.deviceId;
            target.roomId = source.roomId;
            target.isEnabled = source.isEnabled;
            target.nameLength = static_cast<uint8_t>(std::min<size_t>(DEVICE_NAME_MAX_LENGHT, source.deviceName.length()));
            memcpy(target.deviceName, source.deviceName.c_str(), target.nameLength);
            memcpy(target.customBytes, source.customBytes, NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION);
        }
        MessageUDP message((uint8_t)DisplayMessageIds::DEVICE_LIST_CHUNK, display.getInfo().ip, DISPLAY_UDP_PORT);
        message.pushData(reinterpret_cast<byte*>(&chunk), sizeof(chunk));
        NetworkDriver::send(message);
    }
    MessageUDP end((uint8_t)DisplayMessageIds::DEVICE_LIST_END, display.getInfo().ip, DISPLAY_UDP_PORT);
    NetworkDriver::send(end);
    sendRoomNames(display);
    sendRoomStates(display);
}

void DisplayServer::sendRoomNames(const Display& display)
{
    RoomNamesMapping roomNames;
    const std::any roomNamesSignal = DataContainer::getSignalValue(SIG_ROOM_NAMES_MAPPING);
    if (const auto configuredRoomNames = std::any_cast<RoomNamesMapping>(&roomNamesSignal)) {
        roomNames = *configuredRoomNames;
    }
    const size_t chunkCount = std::max<size_t>(1,
        (roomNames.size() + DISPLAY_MAX_ROOMS_PER_CHUNK - 1) / DISPLAY_MAX_ROOMS_PER_CHUNK);
    auto room = roomNames.begin();

    for (size_t chunkIndex = 0; chunkIndex < chunkCount; ++chunkIndex) {
        DisplayRoomNamesChunk chunk;
        chunk.chunkIndex = static_cast<uint8_t>(chunkIndex);
        chunk.totalChunks = static_cast<uint8_t>(chunkCount);

        while (room != roomNames.end() && chunk.roomCount < DISPLAY_MAX_ROOMS_PER_CHUNK) {
            auto& target = chunk.rooms[chunk.roomCount++];
            target.roomId = room->first;
            target.nameLength = static_cast<uint8_t>(std::min<size_t>(
                DISPLAY_ROOM_NAME_MAX_LENGTH, room->second.length()));
            memcpy(target.roomName, room->second.c_str(), target.nameLength);
            ++room;
        }

        MessageUDP message((uint8_t)DisplayMessageIds::ROOM_NAMES_CHUNK,
                           display.getInfo().ip, DISPLAY_UDP_PORT);
        message.pushData(reinterpret_cast<byte*>(&chunk), sizeof(chunk));
        NetworkDriver::send(message);
    }
}

void DisplayServer::handleMessage(MessageUDP& msg)
{
    auto& payload = msg.getPayload();
    if (msg.getId() == (uint8_t)DisplayMessageIds::DEVICE_LIST_REQUEST) {
        auto it = std::find_if(knownDisplays.begin(), knownDisplays.end(), [&msg](const Display& display) {
            return display.getInfo().ip == msg.getIPAddress();
        });
        if (it != knownDisplays.end()) sendDeviceList(*it);
        return;
    }
    if (msg.getId() == (uint8_t)DisplayMessageIds::DEVICE_ACTION_REQUEST && payload.size() == sizeof(DisplayActionRequest)) {
        DisplayActionRequest request;
        memcpy(&request, payload.data(), sizeof(request));
        const auto action = static_cast<DisplayAction>(request.service);
        Logger::log("DisplayServer// action " + String((int)request.service) + " for device " + String((int)request.deviceId) + " params " + String((int)request.parameter[0]) + "," + String((int)request.parameter[1]));
        const bool isStateChange = action == DisplayAction::TOGGLE || action == DisplayAction::ENABLE ||
                                   action == DisplayAction::DISABLE;
        const bool isParameterChange = action == DisplayAction::SET_BRIGHTNESS ||
                                       action == DisplayAction::LOAD_LED_SLOT ||
                                       action == DisplayAction::SET_SEGMENT_STATE;
        if (!pendingAction.active && (isStateChange || isParameterChange)) {
            auto devices = std::any_cast<std::vector<DeviceDescription>>(DataContainer::getSignalValue(SIG_DEVICE_COLLECTION));
            auto device = std::find_if(devices.begin(), devices.end(), [&request](const DeviceDescription& candidate) {
                return candidate.deviceId == request.deviceId;
            });
            if (device != devices.end()) {
                pendingAction.active = true;
                pendingAction.deadline = millis() + 2000;
                pendingAction.request = msg;
                pendingAction.action = request;
                if (isStateChange) {
                    pendingAction.service = DEVSERVICE_STATE_SWITCH;
                    pendingAction.parameters.a = action == DisplayAction::TOGGLE ? (device->isEnabled ? 0 : 1) :
                                                 (action == DisplayAction::ENABLE ? 1 : 0);
                } else if (action == DisplayAction::SET_SEGMENT_STATE) {
                    pendingAction.service = DEVSERVICE_SEGMENT_STATE_SWITCH;
                    pendingAction.parameters.a = request.parameter[0];
                    pendingAction.parameters.b = request.parameter[1];
                } else {
                    pendingAction.service = action == DisplayAction::SET_BRIGHTNESS
                        ? DEVSERVICE_BRIGHTNESS_CHANGE
                        : DEVSERVICE_LED_STRIP_SWITCH_CONTENT;
                    pendingAction.parameters.a = request.parameter[0];
                }
                processPendingAction();
                return;
            }
        }
        sendActionResponse(msg, request.header.sequence, 1, request.deviceId);
        return;
    }
    if (msg.getId() == (uint8_t)DisplayMessageIds::ROOM_TOGGLE_REQUEST && payload.size() == sizeof(DisplayRoomToggleRequest)) {
        DisplayRoomToggleRequest request;
        memcpy(&request, payload.data(), sizeof(request));
        handleRoomToggle(request.roomId);
        return;
    }
    if (msg.getId() == (uint8_t)DisplayMessageIds::KEEP_ALIVE) {
        // Periodic resync makes room states resilient to lost UDP datagrams
        auto display = std::find_if(knownDisplays.begin(), knownDisplays.end(), [&msg](const Display& candidate) {
            return candidate.getInfo().ip == msg.getIPAddress();
        });
        if (display != knownDisplays.end()) sendRoomStates(*display);
        MessageUDP ack((uint8_t)DisplayMessageIds::KEEP_ALIVE_ACK, msg.getIPAddress(), DISPLAY_UDP_PORT);
        if (!payload.empty()) ack.pushData(payload.data(), payload.size());
        NetworkDriver::send(ack);
    }
}

void DisplayServer::processPendingAction()
{
    if (!pendingAction.active) {
        return;
    }

    const auto services = std::any_cast<DeviceServicesAPI>(DataContainer::getSignalValue(SIG_DEVICE_SERVICES));
    const ServiceRequestErrorCode result = services.serviceCall_set1(
        pendingAction.action.deviceId,
        pendingAction.service,
        pendingAction.parameters);

    if (result != SERV_SUCCESS && result != SERV_PENDING) {
        Logger::log("DisplayServer// service call failed, code " + String((int)result));
    }
    if (result == SERV_PENDING) {
        return;
    }
    // Device is still animating, keep the request for a short while instead of dropping it
    if (result == SERV_BUSY && millis() < pendingAction.deadline) {
        return;
    }

    sendActionResponse(
        pendingAction.request,
        pendingAction.action.header.sequence,
        result == SERV_SUCCESS ? 0 : 1,
        pendingAction.action.deviceId);
    if (result == SERV_SUCCESS) {
        broadcastDeviceState(pendingAction.action.deviceId);
    }
    pendingAction = {};
}

void DisplayServer::sendActionResponse(const MessageUDP& request, uint16_t sequence, uint8_t status, uint8_t deviceId)
{
    DisplayActionResponse response;
    response.header.sequence = sequence;
    response.status = status;
    response.deviceId = deviceId;
    auto devices = std::any_cast<std::vector<DeviceDescription>>(DataContainer::getSignalValue(SIG_DEVICE_COLLECTION));
    auto it = std::find_if(devices.begin(), devices.end(), [deviceId](const DeviceDescription& device) { return device.deviceId == deviceId; });
    if (it != devices.end()) {
        response.device.deviceType = it->deviceType;
        response.device.macAddress = it->macAddress;
        response.device.deviceId = it->deviceId;
        response.device.roomId = it->roomId;
        response.device.isEnabled = it->isEnabled;
        response.device.nameLength = static_cast<uint8_t>(std::min<size_t>(DEVICE_NAME_MAX_LENGHT, it->deviceName.length()));
        memcpy(response.device.deviceName, it->deviceName.c_str(), response.device.nameLength);
        memcpy(response.device.customBytes, it->customBytes, NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION);
    }
    MessageUDP message((uint8_t)DisplayMessageIds::DEVICE_ACTION_RESPONSE, request.getIPAddress(), DISPLAY_UDP_PORT);
    message.pushData(reinterpret_cast<byte*>(&response), sizeof(response));
    NetworkDriver::send(message);
}

void DisplayServer::broadcastDeviceState(uint8_t deviceId)
{
    auto devices = std::any_cast<std::vector<DeviceDescription>>(DataContainer::getSignalValue(SIG_DEVICE_COLLECTION));
    auto it = std::find_if(devices.begin(), devices.end(), [deviceId](const DeviceDescription& device) { return device.deviceId == deviceId; });
    if (it == devices.end()) return;
    DisplayStateUpdate update;
    update.device.deviceType = it->deviceType;
    update.device.macAddress = it->macAddress;
    update.device.deviceId = it->deviceId;
    update.device.roomId = it->roomId;
    update.device.isEnabled = it->isEnabled;
    memcpy(update.device.customBytes, it->customBytes, NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION);
    for (const auto& display : knownDisplays) {
        if (it->macAddress != 0 && display.getInfo().macAddress == it->macAddress) continue;
        MessageUDP message((uint8_t)DisplayMessageIds::DEVICE_STATE_UPDATE, display.getInfo().ip, DISPLAY_UDP_PORT);
        message.pushData(reinterpret_cast<byte*>(&update), sizeof(update));
        NetworkDriver::send(message);
    }
}

std::vector<std::pair<uint8_t, uint8_t>> DisplayServer::computeRoomStates()
{
    // Rule shared with the web UI: isEnabled == 1 means ON, 0 means OFF, 2/3 mean the device cannot be switched
    std::map<uint8_t, uint8_t> states;
    const auto devices = std::any_cast<std::vector<DeviceDescription>>(DataContainer::getSignalValue(SIG_DEVICE_COLLECTION));
    for (const auto& device : devices) {
        auto entry = states.emplace(device.roomId, (uint8_t)DisplayRoomState::NOT_TOGGLABLE).first;
        if (device.isEnabled == 1) {
            entry->second = (uint8_t)DisplayRoomState::ON;
        } else if (device.isEnabled == 0 && entry->second != (uint8_t)DisplayRoomState::ON) {
            entry->second = (uint8_t)DisplayRoomState::OFF;
        }
    }
    return std::vector<std::pair<uint8_t, uint8_t>>(states.begin(), states.end());
}

void DisplayServer::sendRoomStates(const Display& display)
{
    const auto states = computeRoomStates();
    const size_t chunkCount = std::max<size_t>(1,
        (states.size() + DISPLAY_MAX_ROOM_STATES_PER_CHUNK - 1) / DISPLAY_MAX_ROOM_STATES_PER_CHUNK);
    auto room = states.begin();
    for (size_t chunkIndex = 0; chunkIndex < chunkCount; ++chunkIndex) {
        DisplayRoomStatesChunk chunk;
        chunk.chunkIndex = static_cast<uint8_t>(chunkIndex);
        chunk.totalChunks = static_cast<uint8_t>(chunkCount);
        while (room != states.end() && chunk.roomCount < DISPLAY_MAX_ROOM_STATES_PER_CHUNK) {
            chunk.rooms[chunk.roomCount].roomId = room->first;
            chunk.rooms[chunk.roomCount].state = room->second;
            ++chunk.roomCount;
            ++room;
        }
        MessageUDP message((uint8_t)DisplayMessageIds::ROOM_STATE_CHUNK, display.getInfo().ip, DISPLAY_UDP_PORT);
        message.pushData(reinterpret_cast<byte*>(&chunk), sizeof(chunk));
        NetworkDriver::send(message);
    }
}

void DisplayServer::refreshRoomStates()
{
    auto states = computeRoomStates();
    if (states == roomStates) return;
    roomStates = std::move(states);
    for (const auto& display : knownDisplays) sendRoomStates(display);
}

void DisplayServer::handleRoomToggle(uint8_t roomId)
{
    // Same evaluation as the web UI: any switchable device ON -> turn the room OFF, otherwise turn it ON
    const auto devices = std::any_cast<std::vector<DeviceDescription>>(DataContainer::getSignalValue(SIG_DEVICE_COLLECTION));
    bool roomExists = false;
    bool anyOn = false;
    for (const auto& device : devices) {
        if (device.roomId != roomId) continue;
        roomExists = true;
        if (device.isEnabled == 1) anyOn = true;
    }
    if (!roomExists) return;

    ServiceParameters_set1 parameters;
    parameters.a = anyOn ? 0 : 1;
    const auto services = std::any_cast<DeviceServicesAPI>(DataContainer::getSignalValue(SIG_DEVICE_SERVICES));
    services.serviceCall_set1(roomId, DEVSERVICE_ROOM_STATE_CHANGE, parameters);
}
