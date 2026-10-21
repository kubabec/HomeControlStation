#pragma once

#include <queue>
#include <vector>

#include "display/HomeControlScreen.h"
#include "os/app/display/DisplayDatatypes.hpp"
#include "os/tools/messageUDP.hpp"

class DisplayClient {
public:
    using DeviceList = HomeControlApp::DeviceList;
    using RoomNames = std::vector<std::pair<uint8_t, std::string>>;

    static void init(HomeControlApp* application);
    static void deinit();
    static void cyclic();
    static void receiveUDP(MessageUDP& message);
    static bool isConnected();
    static bool requestToggle(uint8_t deviceId);
    static bool requestBrightness(uint8_t deviceId, uint8_t percent);
    static bool requestLedSlot(uint8_t deviceId, uint8_t slot);
    static const DeviceList& devices();

private:
    static DeviceList deviceList;
    static std::vector<DisplayDeviceWire> pendingDevices;
    static RoomNames pendingRoomNames;
    static std::queue<MessageUDP> receptionQueue;
    static MessageUDP::IPAddr masterIp;
    static bool connected;
    static uint16_t sequence;
    static unsigned long lastPresenceTime;
    static unsigned long lastKeepAliveTime;
    static unsigned long lastUiTickTime;
    static HomeControlApp* application;

    static uint64_t localMac();
    static void processMessage(MessageUDP& message);
    static void sendPresent(const MessageUDP::IPAddr& target);
    static void requestDeviceList();
    static void handleChunk(const std::vector<uint8_t>& payload);
    static void handleRoomNamesChunk(const std::vector<uint8_t>& payload);
    static void finishDeviceList();
    static DisplayDeviceView convertDevice(const DisplayDeviceWire& source);
    static bool sendAction(uint8_t deviceId, DisplayAction action, uint8_t parameter);
    static bool send(uint8_t id, const void* payload, size_t payloadSize,
                     const MessageUDP::IPAddr& target);
};
