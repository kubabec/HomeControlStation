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
    static bool requestToggle(uint8_t deviceId, bool isLocal = false);
    static bool requestBrightness(uint8_t deviceId, uint8_t percent, bool isLocal = false);
    static bool requestLedSlot(uint8_t deviceId, uint8_t slot, bool isLocal = false);
    static bool requestSegmentState(uint8_t deviceId, uint8_t segment, bool enable, bool isLocal = false);
    static bool requestRoomToggle(uint8_t roomId);
    static const DeviceList& devices();

private:
    struct LocalAction {
        uint8_t deviceId = 0;
        uint8_t service = 0;
        uint8_t parameter = 0;
        uint8_t parameterB = 0;
        bool toggle = false;
        unsigned long deadline = 0;
    };

    static DeviceList deviceList;   // merged list shown in UI: local devices first, then master devices
    static DeviceList remoteList;   // devices synchronized from the master
    static DeviceList localList;    // devices hosted by this display terminal
    static std::queue<LocalAction> localActions;
    static bool localChanged;
    static bool localSubscribed;
    static unsigned long lastMasterRxTime;
    static bool uiConnected;
    static std::vector<DisplayDeviceWire> pendingDevices;
    static RoomNames pendingRoomNames;
    static std::vector<std::pair<uint8_t, uint8_t>> pendingRoomStates;
    static std::queue<MessageUDP> receptionQueue;
    static MessageUDP::IPAddr masterIp;
    static bool connected;
    static uint16_t sequence;
    static unsigned long lastPresenceTime;
    static unsigned long lastKeepAliveTime;
    static unsigned long lastUiTickTime;
    static HomeControlApp* application;

    static uint64_t localMac();
    static void subscribeLocalDevices();
    static void refreshLocalDevices();
    static void rebuildAndReload();
    static void processLocalActions();
    static bool requestLocal(uint8_t deviceId, uint8_t service, uint8_t parameter, bool toggle, uint8_t parameterB = 0);
    static void updateConnectionState();
    static void processMessage(MessageUDP& message);
    static void sendPresent(const MessageUDP::IPAddr& target);
    static void requestDeviceList();
    static void handleChunk(const std::vector<uint8_t>& payload);
    static void handleRoomNamesChunk(const std::vector<uint8_t>& payload);
    static void handleRoomStatesChunk(const std::vector<uint8_t>& payload);
    static void finishDeviceList();
    static DisplayDeviceView convertDevice(const DisplayDeviceWire& source);
    static bool sendAction(uint8_t deviceId, DisplayAction action, uint8_t parameter, uint8_t parameterB = 0);
    static bool send(uint8_t id, const void* payload, size_t payloadSize,
                     const MessageUDP::IPAddr& target);
};
