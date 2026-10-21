#if defined(HCS_TARGET_DISPLAY_S3_4INCH)

#include "os/app/display/DisplayClient.hpp"

#include <algorithm>
#include <cstring>

#include "os/datacontainer/DataContainer.hpp"
#include "os/datacontainer/datacontainertypes.hpp"
#include "os/drivers/NetworkDriver.hpp"

DisplayClient::DeviceList DisplayClient::deviceList;
std::vector<DisplayDeviceWire> DisplayClient::pendingDevices;
DisplayClient::RoomNames DisplayClient::pendingRoomNames;
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
    pendingDevices.clear();
    pendingRoomNames.clear();
    receptionQueue = {};
    connected = false;
    application = nullptr;
}

void DisplayClient::cyclic()
{
    if (!receptionQueue.empty()) {
        MessageUDP message = receptionQueue.front();
        receptionQueue.pop();
        processMessage(message);
    }

    if (millis() - lastPresenceTime >= 5000) {
        sendPresent(NETWORK_BROADCAST);
        lastPresenceTime = millis();
    }

    if (connected && millis() - lastKeepAliveTime >= 20000) {
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
    } else if (id == DisplayMessageIds::DEVICE_STATE_UPDATE && payload.size() == sizeof(DisplayStateUpdate)) {
        DisplayStateUpdate update;
        memcpy(&update, payload.data(), sizeof(update));
        for (auto& device : deviceList) {
            if (device.deviceId == update.device.deviceId) {
                device.isEnabled = update.device.isEnabled;
                memcpy(device.customBytes, update.device.customBytes, sizeof(device.customBytes));
            }
        }
        if (application != nullptr) application->reloadDevices(deviceList);
    } else if (id == DisplayMessageIds::DEVICE_ACTION_RESPONSE && payload.size() == sizeof(DisplayActionResponse)) {
        DisplayActionResponse response;
        memcpy(&response, payload.data(), sizeof(response));
        if (response.status == 0) {
            for (auto& device : deviceList) {
                if (device.deviceId == response.device.deviceId) {
                    device.isEnabled = response.device.isEnabled;
                    memcpy(device.customBytes, response.device.customBytes, sizeof(device.customBytes));
                }
            }
            if (application != nullptr) application->reloadDevices(deviceList);
        }
    } else if (id == DisplayMessageIds::KEEP_ALIVE_ACK) {
        connected = true;
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

void DisplayClient::finishDeviceList()
{
    deviceList.clear();
    for (const auto& device : pendingDevices) deviceList.push_back(convertDevice(device));
    pendingDevices.clear();
    connected = true;
    if (application != nullptr) application->reloadDevices(deviceList);
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

bool DisplayClient::requestToggle(uint8_t deviceId)
{
    return sendAction(deviceId, DisplayAction::TOGGLE, 0);
}

bool DisplayClient::requestBrightness(uint8_t deviceId, uint8_t percent)
{
    if (percent > 100) percent = 100;
    return sendAction(deviceId, DisplayAction::SET_BRIGHTNESS, percent);
}

bool DisplayClient::requestLedSlot(uint8_t deviceId, uint8_t slot)
{
    return sendAction(deviceId, DisplayAction::LOAD_LED_SLOT, slot);
}

bool DisplayClient::isConnected() { return connected; }
const DisplayClient::DeviceList& DisplayClient::devices() { return deviceList; }

#endif
