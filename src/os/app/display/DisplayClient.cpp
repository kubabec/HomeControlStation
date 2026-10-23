#if defined(HCS_TARGET_DISPLAY_S3_4INCH)

#include "os/app/display/DisplayClient.hpp"

#include <algorithm>
#include <cstring>

#include "os/datacontainer/DataContainer.hpp"
#include "os/datacontainer/datacontainertypes.hpp"
#include "os/drivers/NetworkDriver.hpp"
#include "devices/device.hpp"

DisplayClient::DeviceList DisplayClient::deviceList;
DisplayClient::DeviceList DisplayClient::remoteList;
DisplayClient::DeviceList DisplayClient::localList;
std::queue<DisplayClient::LocalAction> DisplayClient::localActions;
bool DisplayClient::localChanged = true;
bool DisplayClient::localSubscribed = false;
unsigned long DisplayClient::lastMasterRxTime = 0;
bool DisplayClient::uiConnected = true;

namespace {
constexpr unsigned long MASTER_TIMEOUT_MS = 15000;
constexpr unsigned long KEEP_ALIVE_PERIOD_MS = 5000;
constexpr unsigned long LOCAL_ACTION_TIMEOUT_MS = 3000;
}
std::vector<DisplayDeviceWire> DisplayClient::pendingDevices;
DisplayClient::RoomNames DisplayClient::pendingRoomNames;
std::vector<std::pair<uint8_t, uint8_t>> DisplayClient::pendingRoomStates;
std::queue<MessageUDP> DisplayClient::receptionQueue;
MessageUDP::IPAddr DisplayClient::masterIp{0, 0, 0, 0};
bool DisplayClient::connected = false;
uint16_t DisplayClient::sequence = 1;
unsigned long DisplayClient::lastPresenceTime = 0;
unsigned long DisplayClient::lastKeepAliveTime = 0;
unsigned long DisplayClient::lastUiTickTime = 0;
HomeControlApp* DisplayClient::application = nullptr;

void DisplayClient::init(HomeControlApp* uiApplication)
{
    application = uiApplication;
    deviceList.clear();
    remoteList.clear();
    localList.clear();
    localActions = {};
    localChanged = true;
    lastMasterRxTime = 0;
    uiConnected = true;
    pendingDevices.clear();
    pendingRoomNames.clear();
    receptionQueue = {};
    masterIp = {0, 0, 0, 0};
    connected = false;
    sequence = 1;
    lastPresenceTime = 0;
    lastKeepAliveTime = 0;
    lastUiTickTime = millis();
}

void DisplayClient::deinit()
{
    deviceList.clear();
    remoteList.clear();
    localList.clear();
    localActions = {};
    pendingDevices.clear();
    pendingRoomNames.clear();
    receptionQueue = {};
    connected = false;
    application = nullptr;
}

void DisplayClient::cyclic()
{
    subscribeLocalDevices();
    processLocalActions();
    refreshLocalDevices();
    updateConnectionState();

    if (!receptionQueue.empty()) {
        MessageUDP message = receptionQueue.front();
        receptionQueue.pop();
        processMessage(message);
    }

    if (millis() - lastPresenceTime >= 5000) {
        sendPresent(NETWORK_BROADCAST);
        lastPresenceTime = millis();
    }

    if (connected && millis() - lastKeepAliveTime >= KEEP_ALIVE_PERIOD_MS) {
        DisplayMessageHeader header;
        header.sequence = sequence++;
        send(static_cast<uint8_t>(DisplayMessageIds::KEEP_ALIVE), &header, sizeof(header), masterIp);
        lastKeepAliveTime = millis();
    }

    const unsigned long now = millis();
    lv_tick_inc(now - lastUiTickTime);
    lastUiTickTime = now;
    lv_timer_handler();
    if (application != nullptr) application->updateStandbyTime();
}

void DisplayClient::receiveUDP(MessageUDP& message)
{
    receptionQueue.push(message);
}

void DisplayClient::processMessage(MessageUDP& message)
{
    const auto id = static_cast<DisplayMessageIds>(message.getId());
    auto& payload = message.getPayload();
    lastMasterRxTime = millis();
    connected = true;

    if (id == DisplayMessageIds::DISCOVER_DISPLAYS) {
        masterIp = message.getIPAddress();
        sendPresent(masterIp);
    } else if (id == DisplayMessageIds::DISPLAY_HANDSHAKE && payload.size() == sizeof(DisplayHandshake)) {
        masterIp = message.getIPAddress();
        connected = true;
        requestDeviceList();
    } else if (id == DisplayMessageIds::DEVICE_LIST_CHUNK) {
        handleChunk(payload);
    } else if (id == DisplayMessageIds::DEVICE_LIST_END) {
        finishDeviceList();
    } else if (id == DisplayMessageIds::ROOM_NAMES_CHUNK) {
        handleRoomNamesChunk(payload);
    } else if (id == DisplayMessageIds::ROOM_STATE_CHUNK) {
        handleRoomStatesChunk(payload);
    } else if (id == DisplayMessageIds::DEVICE_STATE_UPDATE && payload.size() == sizeof(DisplayStateUpdate)) {
        DisplayStateUpdate update;
        memcpy(&update, payload.data(), sizeof(update));
        // Updates about devices hosted on this display are ignored - local state is authoritative
        const uint64_t mac = localMac();
        if (mac != 0 && update.device.macAddress == mac) return;
        for (auto& device : remoteList) {
            if (device.deviceId == update.device.deviceId) {
                device.isEnabled = update.device.isEnabled;
                memcpy(device.customBytes, update.device.customBytes, sizeof(device.customBytes));
            }
        }
        rebuildAndReload();
    } else if (id == DisplayMessageIds::DEVICE_ACTION_RESPONSE && payload.size() == sizeof(DisplayActionResponse)) {
        DisplayActionResponse response;
        memcpy(&response, payload.data(), sizeof(response));
        if (response.status == 0) {
            for (auto& device : remoteList) {
                if (device.deviceId == response.device.deviceId) {
                    device.isEnabled = response.device.isEnabled;
                    memcpy(device.customBytes, response.device.customBytes, sizeof(device.customBytes));
                }
            }
            rebuildAndReload();
        }
    }
}

uint64_t DisplayClient::localMac()
{
    std::any value = DataContainer::getSignalValue(SIG_MAC_ADDRESS);
    if (auto mac = std::any_cast<uint64_t>(&value)) return *mac;
    return 0;
}

void DisplayClient::sendPresent(const MessageUDP::IPAddr& target)
{
    DisplayInfo info;
    info.macAddress = localMac();
    std::any value = DataContainer::getSignalValue(SIG_IP_ADDRESS);
    if (auto ip = std::any_cast<uint32_t>(&value)) memcpy(&info.ip, ip, sizeof(info.ip));
    send(static_cast<uint8_t>(DisplayMessageIds::DISPLAY_PRESENT), &info, sizeof(info), target);
}

void DisplayClient::requestDeviceList()
{
    DisplayMessageHeader header;
    header.sequence = sequence++;
    send(static_cast<uint8_t>(DisplayMessageIds::DEVICE_LIST_REQUEST), &header, sizeof(header), masterIp);
}

void DisplayClient::handleChunk(const std::vector<uint8_t>& payload)
{
    if (payload.size() != sizeof(DisplayDeviceListChunk)) return;
    DisplayDeviceListChunk chunk;
    memcpy(&chunk, payload.data(), sizeof(chunk));
    if (chunk.chunkIndex == 0) pendingDevices.clear();
    for (uint8_t index = 0; index < chunk.deviceCount && index < DISPLAY_MAX_DEVICES_PER_CHUNK; ++index) {
        pendingDevices.push_back(chunk.devices[index]);
    }
}

void DisplayClient::handleRoomNamesChunk(const std::vector<uint8_t>& payload)
{
    if (payload.size() != sizeof(DisplayRoomNamesChunk)) return;

    DisplayRoomNamesChunk chunk;
    memcpy(&chunk, payload.data(), sizeof(chunk));
    if (chunk.totalChunks == 0 || chunk.chunkIndex >= chunk.totalChunks ||
        chunk.roomCount > DISPLAY_MAX_ROOMS_PER_CHUNK) return;

    if (chunk.chunkIndex == 0) pendingRoomNames.clear();
    for (uint8_t index = 0; index < chunk.roomCount; ++index) {
        const auto& source = chunk.rooms[index];
        const uint8_t length = std::min<uint8_t>(source.nameLength, DISPLAY_ROOM_NAME_MAX_LENGTH);
        pendingRoomNames.emplace_back(source.roomId, std::string(source.roomName, length));
    }

    if (chunk.chunkIndex + 1 == chunk.totalChunks) {
        if (application != nullptr) application->setRoomNames(pendingRoomNames);
        pendingRoomNames.clear();
    }
}

void DisplayClient::handleRoomStatesChunk(const std::vector<uint8_t>& payload)
{
    if (payload.size() != sizeof(DisplayRoomStatesChunk)) return;
    DisplayRoomStatesChunk chunk;
    memcpy(&chunk, payload.data(), sizeof(chunk));
    if (chunk.totalChunks == 0 || chunk.chunkIndex >= chunk.totalChunks ||
        chunk.roomCount > DISPLAY_MAX_ROOM_STATES_PER_CHUNK) return;

    if (chunk.chunkIndex == 0) pendingRoomStates.clear();
    for (uint8_t index = 0; index < chunk.roomCount; ++index) {
        pendingRoomStates.emplace_back(chunk.rooms[index].roomId, chunk.rooms[index].state);
    }
    if (chunk.chunkIndex + 1 == chunk.totalChunks) {
        if (application != nullptr) application->setRoomToggleStates(pendingRoomStates);
        pendingRoomStates.clear();
    }
}

void DisplayClient::finishDeviceList()
{
    remoteList.clear();
    const uint64_t mac = localMac();
    for (const auto& device : pendingDevices) {
        if (mac != 0 && device.macAddress == mac) continue;
        remoteList.push_back(convertDevice(device));
    }
    pendingDevices.clear();
    connected = true;
    rebuildAndReload();
}

DisplayDeviceView DisplayClient::convertDevice(const DisplayDeviceWire& source)
{
    char name[DEVICE_NAME_MAX_LENGHT + 1] = {};
    const uint8_t length = std::min<uint8_t>(source.nameLength, DEVICE_NAME_MAX_LENGHT);
    memcpy(name, source.deviceName, length);
    DisplayDeviceView device;
    device.deviceType = source.deviceType;
    device.macAddress = source.macAddress;
    device.deviceId = source.deviceId;
    device.roomId = source.roomId;
    device.isEnabled = source.isEnabled;
    memcpy(device.customBytes, source.customBytes, sizeof(device.customBytes));
    device.deviceName = name;
    return device;
}

bool DisplayClient::send(uint8_t id, const void* payload, size_t payloadSize,
                         const MessageUDP::IPAddr& target)
{
    MessageUDP message(id, target, DISPLAY_UDP_PORT);
    if (payload != nullptr && payloadSize > 0) {
        message.pushData(reinterpret_cast<byte*>(const_cast<void*>(payload)), payloadSize);
    }
    return target == NETWORK_BROADCAST
        ? NetworkDriver::sendBroadcast(message)
        : NetworkDriver::send(message);
}

bool DisplayClient::sendAction(uint8_t deviceId, DisplayAction action, uint8_t parameter)
{
    if (!connected) return false;
    DisplayActionRequest request;
    request.header.sequence = sequence++;
    request.deviceId = deviceId;
    request.service = static_cast<uint8_t>(action);
    request.parameter[0] = parameter;
    return send(static_cast<uint8_t>(DisplayMessageIds::DEVICE_ACTION_REQUEST),
                &request, sizeof(request), masterIp);
}

bool DisplayClient::requestToggle(uint8_t deviceId, bool isLocal)
{
    if (isLocal) return requestLocal(deviceId, DEVSERVICE_STATE_SWITCH, 0, true);
    return sendAction(deviceId, DisplayAction::TOGGLE, 0);
}

bool DisplayClient::requestBrightness(uint8_t deviceId, uint8_t percent, bool isLocal)
{
    if (isLocal) return requestLocal(deviceId, DEVSERVICE_BRIGHTNESS_CHANGE, percent, false);
    if (percent > 100) percent = 100;
    return sendAction(deviceId, DisplayAction::SET_BRIGHTNESS, percent);
}

bool DisplayClient::requestLedSlot(uint8_t deviceId, uint8_t slot, bool isLocal)
{
    if (isLocal) return requestLocal(deviceId, DEVSERVICE_LED_STRIP_SWITCH_CONTENT, slot, false);
    return sendAction(deviceId, DisplayAction::LOAD_LED_SLOT, slot);
}

bool DisplayClient::requestRoomToggle(uint8_t roomId)
{
    if (!connected) return false;
    DisplayRoomToggleRequest request;
    request.header.sequence = sequence++;
    request.roomId = roomId;
    return send(static_cast<uint8_t>(DisplayMessageIds::ROOM_TOGGLE_REQUEST),
                &request, sizeof(request), masterIp);
}

bool DisplayClient::isConnected() { return connected; }
const DisplayClient::DeviceList& DisplayClient::devices() { return deviceList; }

void DisplayClient::subscribeLocalDevices()
{
    if (localSubscribed) return;
    localSubscribed = true;
    DataContainer::subscribe(SIG_LOCAL_COLLECTION, [](std::any) { localChanged = true; });
}

void DisplayClient::refreshLocalDevices()
{
    if (!localChanged) return;
    localChanged = false;
    localList.clear();
    const std::any value = DataContainer::getSignalValue(SIG_LOCAL_COLLECTION);
    if (const auto devices = std::any_cast<std::vector<DeviceDescription>>(&value)) {
        const uint64_t mac = localMac();
        for (const auto& source : *devices) {
            DisplayDeviceView device;
            device.deviceType = source.deviceType;
            device.macAddress = mac;
            device.deviceId = source.deviceId;
            device.roomId = source.roomId;
            device.isEnabled = source.isEnabled;
            device.isLocal = true;
            memcpy(device.customBytes, source.customBytes, sizeof(device.customBytes));
            device.deviceName = source.deviceName.c_str();
            localList.push_back(device);
        }
    }
    rebuildAndReload();
}

void DisplayClient::rebuildAndReload()
{
    deviceList = localList;
    deviceList.insert(deviceList.end(), remoteList.begin(), remoteList.end());
    if (application != nullptr) application->reloadDevices(deviceList);
}

bool DisplayClient::requestLocal(uint8_t deviceId, uint8_t service, uint8_t parameter, bool toggle)
{
    LocalAction action;
    action.deviceId = deviceId;
    action.service = service;
    action.parameter = parameter;
    action.toggle = toggle;
    action.deadline = millis() + LOCAL_ACTION_TIMEOUT_MS;
    localActions.push(action);
    return true;
}

void DisplayClient::processLocalActions()
{
    if (localActions.empty()) return;
    const LocalAction action = localActions.front();

    const std::any servicesValue = DataContainer::getSignalValue(SIG_LOCAL_DEVICE_SERVICES);
    const auto services = std::any_cast<DeviceServicesAPI>(&servicesValue);
    if (services == nullptr) {
        localActions.pop();
        return;
    }

    ServiceParameters_set1 params;
    params.a = action.parameter;
    if (action.toggle) {
        const auto it = std::find_if(localList.begin(), localList.end(), [&action](const DisplayDeviceView& view) {
            return view.deviceId == action.deviceId;
        });
        const DisplayDeviceView* device = it == localList.end() ? nullptr : &*it;
        if (device == nullptr) {
            localActions.pop();
            return;
        }
        params.a = device->isEnabled ? 0 : 1;
    }

    const ServiceRequestErrorCode result = services->serviceCall_set1(
        action.deviceId, static_cast<DeviceServicesType>(action.service), params);
    const bool retry = (result == SERV_PENDING || result == SERV_BUSY) && millis() < action.deadline;
    if (!retry) localActions.pop();
    if (result == SERV_SUCCESS) localChanged = true;
}

void DisplayClient::updateConnectionState()
{
    if (connected && lastMasterRxTime != 0 && millis() - lastMasterRxTime > MASTER_TIMEOUT_MS) {
        connected = false;
    }
    if (uiConnected != connected) {
        uiConnected = connected;
        if (application != nullptr) application->setMasterConnected(connected);
    }
}

#endif
