# Display Terminal Architecture - Implementation Guide

## 1. Header Files Organization

This guide describes two independently built repositories. `DisplayServer` and the master-side display
model belong to HomeControlStation. `DisplayClient`, `DisplayUIThread`, LVGL, and Guition hardware code
belong only to the Guition repository. HomeControlStation must not link to or compile client/hardware
files from Guition.

### 1.1 Repository-owned files

```
HomeControlStation repository:
include/os/app/display/
├── DisplayServer.hpp              ← Master-side multi-display coordinator
├── Display.hpp                    ← Master-side display endpoint state
├── DisplayDatatypes.hpp           ← Wire protocol copied into Guition
└── DeviceDescriptionPacker.hpp    ← Protocol packing utility

Guition repository:
include/os/app/display/
├── DisplayClient.hpp              ← Display-side protocol application
├── DisplayUIThread.hpp            ← LVGL rendering thread
└── DisplayDatatypes.hpp           ← Physical copy of the wire protocol
```

### 1.2 Message Header Template

```c++
// include/os/app/display/DisplayDatatypes.hpp

#pragma once
#include <Arduino.h>
#include <cstdint>
#include <cstring>

// Message IDs for display protocol (range 200-249)
enum class DisplayMessageIds : uint8_t
{
    DISCOVER_DISPLAYS = 200,
    DISPLAY_PRESENT = 201,
    DISPLAY_HANDSHAKE = 202,
    
    DEVICE_LIST_REQUEST = 210,
    DEVICE_LIST_CHUNK = 211,
    DEVICE_LIST_END = 212,
    DEVICE_STATE_UPDATE = 213,
    
    DEVICE_ACTION_REQUEST = 220,
    DEVICE_ACTION_RESPONSE = 221,
    
    DISPLAY_STATE_REFRESH = 230,
    DISPLAY_HASH_SYNC = 231,
    
    KEEP_ALIVE = 240,
    KEEP_ALIVE_ACK = 241
};

// All payloads use this header
#pragma pack(1)
struct DisplayMessageHeader
{
    uint16_t sequenceNumber;
    uint32_t timestamp;
    uint8_t reserved[4];
};

struct DisplayPresencePayload
{
    DisplayMessageHeader header;
    uint8_t firmwareVersion;
    uint8_t screenWidth;              // 480
    uint8_t screenHeight;             // 320
    uint8_t maxSimultaneousDevices;   // 30
    uint8_t reserved[8];
};

struct DisplayHandshakePayload
{
    DisplayMessageHeader header;
    uint8_t displayAssignedId;
    uint8_t numberOfDevices;
    uint16_t deviceListHash;
    uint32_t masterPlatformVersion;
    uint8_t reserved[8];
};

// ... (other payload structures)
#pragma pack()

// Constants
#define DISPLAY_DISCOVERY_FREQ_MS 15000
#define DISPLAY_HANDSHAKE_TIMEOUT_MS 5000
#define DISPLAY_KEEP_ALIVE_INTERVAL_MS 20000
#define DISPLAY_KEEP_ALIVE_TIMEOUT_MS 3000
#define DISPLAY_KEEP_ALIVE_RETRIES 3
#define DISPLAY_DISCONNECT_TIMEOUT_MS 60000
#define MAX_DISPLAYS_SUPPORTED 20
#define MAX_DEVICES_PER_CHUNK 5
#define MAX_DEVICE_LIST_CHUNKS 256
```

---

## 2. DisplayClient State Machine

### 2.1 State Transitions

```c++
// include/os/app/display/DisplayClient.hpp

class DisplayClient
{
    enum class ClientState : uint8_t
    {
        IDLE = 0,                  // Power-on, no network
        DISCOVERY = 1,             // Broadcasting DISCOVER_DISPLAYS
        WAITING_HANDSHAKE = 2,     // Sent DISPLAY_PRESENT
        SYNCING_DEVICES = 3,       // Receiving device list chunks
        CONNECTED = 4,             // Normal operation
        DISCONNECTED = 5,          // Lost connection, needs recovery
        ERROR = 6                  // Fatal error
    };
    
    // State machine
    static void transitionTo(ClientState newState)
    {
        if (newState != currentState) {
            Logger::log("DisplayClient: " + stateToString(currentState) + 
                        " → " + stateToString(newState));
            currentState = newState;
            onStateChanged(newState);
        }
    }
    
    // Cyclic state handlers
    static void handleDiscoveryState();      // Send DISCOVER broadcasts
    static void handleWaitingHandshakeState(); // Timeout if no response
    static void handleSyncingDevicesState();   // Receive chunks
    static void handleConnectedState();        // Keep-alive, action dispatch
    static void handleDisconnectedState();     // Transition back to DISCOVERY
};
```

### 2.2 Cyclic Execution Pattern

```c++
void DisplayClient::cyclic()
{
    // 1. Process all queued UDP messages
    processReceptionBuffer();
    
    // 2. Execute state-specific actions
    switch (currentState)
    {
        case ClientState::DISCOVERY:
            handleDiscoveryState();
            break;
            
        case ClientState::WAITING_HANDSHAKE:
            handleWaitingHandshakeState();
            break;
            
        case ClientState::SYNCING_DEVICES:
            handleSyncingDevicesState();
            break;
            
        case ClientState::CONNECTED:
            handleConnectedState();
            break;
            
        case ClientState::DISCONNECTED:
            handleDisconnectedState();
            break;
            
        default:
            break;
    }
    
    // 3. Dispatch any pending user actions (if connected)
    if (currentState == ClientState::CONNECTED) {
        dispatchPendingActions();
    }
}

void DisplayClient::handleDiscoveryState()
{
    unsigned long now = millis();
    if (now - lastDiscoveryTime > DISPLAY_DISCOVERY_FREQ_MS) {
        // Prepare DISCOVER_DISPLAYS message
        MessageUDP discoverMsg((uint8_t)DisplayMessageIds::DISCOVER_DISPLAYS,
                               MessageUDP::IPAddr{255, 255, 255, 255},
                               9001);
        
        // Broadcast (handled by NetworkDriver for WiFi subnet)
        NetworkDriver::sendBroadcast(discoverMsg);
        
        lastDiscoveryTime = now;
        Logger::log("DisplayClient: Broadcasting DISCOVER_DISPLAYS");
    }
}

void DisplayClient::handleWaitingHandshakeState()
{
    unsigned long now = millis();
    if (now - lastHandshakeTime > DISPLAY_HANDSHAKE_TIMEOUT_MS * 3) {
        // No handshake response, back to discovery
        Logger::log("DisplayClient: Handshake timeout, returning to discovery");
        transitionTo(ClientState::DISCOVERY);
    }
}

void DisplayClient::handleSyncingDevicesState()
{
    unsigned long now = millis();
    
    // Check if we've received all chunks
    if (currentChunkIndex >= expectedTotalChunks) {
        // Validate hash
        uint16_t receivedHash = computeDeviceListHash();
        if (receivedHash == expectedDeviceListHash) {
            Logger::log("DisplayClient: Device list synced successfully");
            
            // Signal UI thread that list is ready
            {
                std::lock_guard<std::mutex> lock(deviceListMutex);
                deviceListReady.notify_all();
            }
            
            transitionTo(ClientState::CONNECTED);
            lastKeepAliveTime = now;
        } else {
            Logger::log("DisplayClient: Device list hash mismatch, retrying");
            transitionTo(ClientState::WAITING_HANDSHAKE);
        }
    }
    
    // Timeout if sync takes too long
    if (now - lastHandshakeTime > 30000) {
        Logger::log("DisplayClient: Device sync timeout");
        transitionTo(ClientState::DISCOVERY);
    }
}

void DisplayClient::handleConnectedState()
{
    unsigned long now = millis();
    
    // Send keep-alive if interval elapsed
    if (now - lastKeepAliveTime > DISPLAY_KEEP_ALIVE_INTERVAL_MS) {
        KeepAlivePayload keepAlive;
        keepAlive.header.sequenceNumber = ++lastKeepaliveSequence;
        keepAlive.header.timestamp = now;
        keepAlive.sender = 1;  // Display
        keepAlive.displayId = assignedDisplayId;
        
        MessageUDP msg((uint8_t)DisplayMessageIds::KEEP_ALIVE,
                       masterIPAddress, 9001);
        msg.addPayload((uint8_t*)&keepAlive, sizeof(keepAlive));
        NetworkDriver::sendUDP(msg);
        
        lastKeepAliveTime = now;
        keepAliveRetryCount = 0;
    }
    
    // Check for keep-alive timeout
    if (now - lastKeepAliveTime > DISPLAY_KEEP_ALIVE_INTERVAL_MS + 
                                   (DISPLAY_KEEP_ALIVE_TIMEOUT_MS * 3)) {
        if (keepAliveRetryCount >= DISPLAY_KEEP_ALIVE_RETRIES) {
            Logger::log("DisplayClient: Keep-alive timeout, disconnecting");
            transitionTo(ClientState::DISCONNECTED);
        }
    }
}

void DisplayClient::handleDisconnectedState()
{
    unsigned long now = millis();
    
    // Wait briefly before returning to discovery
    if (now - disconnectedSinceTime > 5000) {
        Logger::log("DisplayClient: Attempting reconnection");
        transitionTo(ClientState::DISCOVERY);
    }
}
```

---

## 3. DisplayServer Integration Points

### 3.1 Hooking into Existing Systems

The DisplayServer must integrate with these existing HCS components:

#### Integration with DeviceProvider
```c++
// In DisplayServer::handleDeviceActionRequest()

DeviceDescription targetDevice;
bool found = DeviceProvider::getDeviceByIdentifiers(
    request.targetNodeMac,
    request.targetDeviceId,
    targetDevice
);

if (found) {
    // Call same routing as HTTP server
    auto result = DeviceProvider::applyServiceCall(
        targetDevice,
        request.actionType,        // 0=toggle, 1=set_value, 2=service_call
        request.actionParameter
    );
    
    if (result.success) {
        // Collect updated device description
        DeviceProvider::getDeviceByIdentifiers(
            request.targetNodeMac,
            request.targetDeviceId,
            targetDevice  // Refreshed with new state
        );
    }
}
```

#### Integration with DataContainer
```c++
// In DisplayServer::init()

// Hook into device state change signals
DataContainer::onDeviceStateChanged.subscribe(
    [](const DeviceChangeNotification& notif) {
        DisplayServer::onDeviceStateChanged(notif);
    }
);

// DisplayServer::onDeviceStateChanged implementation:
static void onDeviceStateChanged(const DeviceChangeNotification& notif)
{
    // Broadcast to all connected displays
    DeviceStateUpdatePayload update;
    update.header.sequenceNumber = ++stateUpdateSequence;
    update.header.timestamp = millis();
    update.sourceDeviceType = notif.device.deviceType;
    update.sourceNodeMac = notif.device.macAddress;
    update.sourceDeviceId = notif.device.deviceId;
    update.stateChangeReason = notif.changeReason;
    memcpy(update.newStateCustomBytes, notif.device.customBytes, 50);
    
    broadcastToConnectedDisplays(
        (uint8_t)DisplayMessageIds::DEVICE_STATE_UPDATE,
        &update,
        sizeof(update)
    );
}
```

#### Integration with NetworkDriver
```c++
// In DisplayServer::init()

// Register UDP reception callback for display messages
NetworkDriver::registerUDPCallback(
    9001,  // Port
    DisplayMessageIds::DISCOVER_DISPLAYS,
    DisplayServer::receiveUDP
);

// Other message IDs handled similarly
```

### 3.2 DisplayServer Lifecycle

```c++
// In main platform initialization (before HTTP server starts)

void OperatingSystem::init()
{
    // ... existing initialization
    
    // NEW: Initialize DisplayServer after platform core is ready
    if (ConfigProvider::isDisplayServerEnabled()) {
        DisplayServer::init();  // Registers with DataContainer, NetworkDriver
        Logger::log("DisplayServer initialized");
    }
}

void OperatingSystem::cyclic()
{
    // ... existing cyclic calls
    
    if (ConfigProvider::isDisplayServerEnabled()) {
        DisplayServer::cyclic();
    }
}

void OperatingSystem::deinit()
{
    if (ConfigProvider::isDisplayServerEnabled()) {
        DisplayServer::deinit();
    }
    
    // ... existing deinit
}
```

---

## 4. DisplayUIThread Implementation Skeleton

### 4.1 Thread Lifecycle

```c++
// include/os/app/display/DisplayUIThread.hpp

class DisplayUIThread
{
public:
    /**
     * Called after HCS platform initialization is complete.
     * Spawns a new FreeRTOS task running threadLoop().
     */
    static void start()
    {
        if (isRunning) {
            Logger::log("DisplayUIThread: Already running");
            return;
        }
        
        isRunning = true;
        xTaskCreatePinnedToCore(
            threadEntryPoint,        // Function
            "DisplayUI",             // Name
            8192,                    // Stack size (bytes)
            nullptr,                 // Parameters
            3,                       // Priority (higher = more urgent)
            &threadHandle,           // Output handle
            1                        // Core (ESP32 has 2 cores)
        );
        
        Logger::log("DisplayUIThread: Started");
    }
    
    /**
     * Signal graceful shutdown.
     */
    static void stop()
    {
        if (!isRunning) return;
        
        shouldStop = true;
        
        // Wait for thread to exit
        unsigned long timeout = millis() + 5000;
        while (isRunning && millis() < timeout) {
            delay(10);
        }
        
        if (isRunning) {
            Logger::log("DisplayUIThread: Force-killing (timeout)");
            vTaskDelete(threadHandle);
        }
        
        threadHandle = nullptr;
        Logger::log("DisplayUIThread: Stopped");
    }
    
    /**
     * Query current connection status from UI thread context.
     */
    static ConnectionStatus getStatus()
    {
        std::lock_guard<std::mutex> lock(statusMutex);
        return currentStatus;
    }

private:
    static bool isRunning;
    static bool shouldStop;
    static TaskHandle_t threadHandle;
    static std::mutex statusMutex;
    static ConnectionStatus currentStatus;
    
    static void threadEntryPoint(void* params)
    {
        threadLoop();
        isRunning = false;
        vTaskDelete(nullptr);
    }
    
    static void threadLoop()
    {
        // Initialize LVGL if not already done
        lv_init();
        
        // Create display buffer and register with LVGL
        setupLVGLDisplay();
        
        // Create UI hierarchy (screens, widgets)
        HomeControlApp uiApp;
        uiApp.init();
        
        unsigned long lastDeviceRefresh = 0;
        unsigned long lastScreenRender = 0;
        
        while (!shouldStop) {
            unsigned long now = millis();
            
            // 1. Update device list if needed
            if (now - lastDeviceRefresh > 5000) {
                if (DisplayClient::isConnected()) {
                    try {
                        const auto& devices = DisplayClient::getDeviceList();
                        
                        // Filter for current room
                        std::vector<DeviceDescriptionWire> roomDevices;
                        for (const auto& dev : devices) {
                            if (dev.roomId == currentRoomId) {
                                roomDevices.push_back(dev);
                            }
                        }
                        
                        // Update UI
                        uiApp.reloadDevices(roomDevices);
                        
                        {
                            std::lock_guard<std::mutex> lock(statusMutex);
                            currentStatus.isConnected = true;
                            currentStatus.connectedDisplayCount = devices.size();
                        }
                    } catch (...) {
                        Logger::log("DisplayUIThread: Error updating device list");
                    }
                } else {
                    Logger::log("DisplayUIThread: Waiting for connection...");
                    {
                        std::lock_guard<std::mutex> lock(statusMutex);
                        currentStatus.isConnected = false;
                    }
                }
                
                lastDeviceRefresh = now;
            }
            
            // 2. Process device state updates from DisplayClient
            processDeviceStateUpdates();
            
            // 3. LVGL tick (rendering, animation)
            lv_tick_inc(10);  // Tell LVGL that 10ms passed
            lv_timer_handler();
            
            // 4. Render display
            if (now - lastScreenRender > 16) {  // ~60 FPS
                // LVGL typically handles rendering
                lastScreenRender = now;
            }
            
            // Yield to other tasks
            vTaskDelay(pdMS_TO_TICKS(10));
        }
        
        // Cleanup
        uiApp.deinit();
        lv_deinit();
    }
    
    static void processDeviceStateUpdates()
    {
        // Check if DisplayClient has queued any updates
        // (Implementation depends on thread-safe queue design)
        
        auto updates = DisplayClient::getStateUpdates();  // Non-blocking
        for (const auto& update : updates) {
            // Find device in currentRoomDevices
            for (auto& dev : currentRoomDevices) {
                if (dev.macAddress == update.sourceNodeMac &&
                    dev.deviceId == update.sourceDeviceId) {
                    // Update state
                    memcpy(dev.customBytes, update.newStateCustomBytes, 50);
                    
                    // Refresh widget
                    uiApp.onDeviceStateChanged(dev);
                    break;
                }
            }
        }
    }
    
    static void setupLVGLDisplay()
    {
        // Create LVGL display buffer
        static lv_disp_draw_buf_t draw_buf;
        static lv_color_t buf1[480 * 320];  // Full screen
        static lv_color_t buf2[480 * 320];  // Double buffer
        
        lv_disp_draw_buf_init(&draw_buf, buf1, buf2, 480 * 320);
        
        // Create LVGL display driver
        static lv_disp_drv_t disp_drv;
        lv_disp_drv_init(&disp_drv);
        disp_drv.draw_buf = &draw_buf;
        disp_drv.hor_res = 480;
        disp_drv.ver_res = 320;
        disp_drv.flush_cb = lvglFlushCallback;
        
        lv_disp_t * disp = lv_disp_drv_register(&disp_drv);
        
        // Setup input device (touch screen)
        static lv_indev_drv_t indev_drv;
        lv_indev_drv_init(&indev_drv);
        indev_drv.type = LV_INDEV_TYPE_POINTER;
        indev_drv.read_cb = lvglInputCallback;
        lv_indev_drv_register(&indev_drv);
    }
    
    static void lvglFlushCallback(lv_disp_drv_t * disp, const lv_area_t * area, 
                                  lv_color_t * color_p)
    {
        // Update the physical display with LVGL buffer content
        // Implementation depends on the display driver (Guition SPI interface)
        
        // Example pseudo-code:
        // displayDriver.setArea(area->x1, area->y1, area->x2, area->y2);
        // displayDriver.writePixels(color_p, ...);
        
        lv_disp_flush_ready(disp);
    }
    
    static void lvglInputCallback(lv_indev_drv_t * indev, lv_indev_data_t * data)
    {
        // Read touch input from display hardware
        // Set data->point.x, data->point.y, data->state (LV_INDEV_STATE_PR/REL)
    }
};
```

---

## 5. Thread-Safe Communication Primitives

### 5.1 Device State Update Queue

```c++
// include/os/app/display/DisplayThreadSync.hpp

class ThreadSafeDeviceStateQueue
{
    struct QueueItem {
        DeviceStateUpdatePayload update;
        unsigned long timestamp;
    };
    
    static const size_t MAX_QUEUE_SIZE = 64;
    static QueueItem queue[MAX_QUEUE_SIZE];
    static size_t head = 0;
    static size_t tail = 0;
    static std::mutex queueMutex;
    static std::atomic<size_t> size;
    
public:
    /**
     * Called by DisplayClient thread to enqueue a state update.
     * Non-blocking, drops oldest if queue is full.
     */
    static void push(const DeviceStateUpdatePayload& update)
    {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            
            if (size >= MAX_QUEUE_SIZE) {
                // Drop oldest
                head = (head + 1) % MAX_QUEUE_SIZE;
                size--;
            }
            
            queue[tail].update = update;
            queue[tail].timestamp = millis();
            tail = (tail + 1) % MAX_QUEUE_SIZE;
            size++;
        }
    }
    
    /**
     * Called by DisplayUIThread to dequeue all pending updates.
     * Non-blocking.
     */
    static std::vector<DeviceStateUpdatePayload> popAll()
    {
        std::vector<DeviceStateUpdatePayload> result;
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            
            while (size > 0) {
                result.push_back(queue[head].update);
                head = (head + 1) % MAX_QUEUE_SIZE;
                size--;
            }
        }
        return result;
    }
    
    static size_t getSize()
    {
        return size.load();
    }
};
```

### 5.2 Action Request/Response Synchronization

```c++
// Display UI thread requests action, waits for response

class ThreadSafeActionDispatch
{
    static std::mutex actionMutex;
    static std::condition_variable actionComplete;
    static uint16_t lastRequestSequence = 0;
    static DeviceActionResponsePayload lastResponse;
    static bool responseReady = false;
    
public:
    /**
     * Called by DisplayUIThread to request device action.
     * Blocks briefly (up to timeoutMs) waiting for response.
     * Returns false if timed out.
     */
    static bool requestAndWait(
        const DeviceActionRequestPayload& request,
        DeviceActionResponsePayload& outResponse,
        unsigned long timeoutMs = 5000)
    {
        std::unique_lock<std::mutex> lock(actionMutex);
        
        // Prepare request
        lastRequestSequence = ++requestSequenceCounter;
        auto mutableRequest = request;
        mutableRequest.sequenceNumber = lastRequestSequence;
        
        // Send via DisplayClient
        DisplayClient::enqueueAction(mutableRequest);
        
        responseReady = false;
        
        // Wait for response (with timeout)
        bool gotResponse = actionComplete.wait_for(
            lock,
            std::chrono::milliseconds(timeoutMs),
            []() { return responseReady; }
        );
        
        if (gotResponse && lastResponse.sequenceNumber == lastRequestSequence) {
            outResponse = lastResponse;
            return true;
        }
        
        return false;  // Timeout
    }
    
    /**
     * Called by DisplayClient thread when response received.
     */
    static void onActionResponseReceived(const DeviceActionResponsePayload& response)
    {
        {
            std::lock_guard<std::mutex> lock(actionMutex);
            lastResponse = response;
            responseReady = true;
        }
        actionComplete.notify_one();
    }
    
private:
    static uint16_t requestSequenceCounter;
};
```

---

## 6. NVM Configuration for Display Device

### 6.1 ConfigProvider Extensions

```c++
// include/os/app/config/DisplayConfigProvider.hpp

class DisplayConfigProvider
{
public:
    /**
     * Initialize display-specific NVM blocks.
     * Called during platform init if device is in Display mode.
     */
    static void initializeDisplayConfig()
    {
        // Allocate blocks specifically for display (instead of unused device blocks)
        ConfigProvider::allocateSlot(
            NVMSlot::DISPLAY_WIFI_CONFIG,      // Replaces unused DEVICE_MANAGER_CONFIG
            sizeof(DisplayWiFiConfig),
            0x1000  // Offset
        );
        
        ConfigProvider::allocateSlot(
            NVMSlot::DISPLAY_SETTINGS,
            sizeof(DisplaySettingsConfig),
            0x3000
        );
        
        ConfigProvider::allocateSlot(
            NVMSlot::DISPLAY_NETWORK_CONFIG,
            sizeof(DisplayNetworkConfig),
            0x4800
        );
    }
    
    static bool getWiFiConfig(DisplayWiFiConfig& config)
    {
        return ConfigProvider::readNVM(NVMSlot::DISPLAY_WIFI_CONFIG, 
                                       (uint8_t*)&config, 
                                       sizeof(config));
    }
    
    static void setWiFiConfig(const DisplayWiFiConfig& config)
    {
        ConfigProvider::writeNVM(NVMSlot::DISPLAY_WIFI_CONFIG,
                                 (uint8_t*)&config,
                                 sizeof(config));
    }
};

#pragma pack(1)
struct DisplayWiFiConfig
{
    char ssid[32];
    char password[64];
    uint8_t ipMode;            // 0 = DHCP, 1 = Static
    uint8_t reserved[32];
};

struct DisplaySettingsConfig
{
    char displayName[32];
    uint8_t screenOrientation;  // 0 = normal, 1 = rotated
    uint8_t brightness;         // 0-255
    uint8_t reserved[32];
};

struct DisplayNetworkConfig
{
    uint8_t masterIP[4];        // 0.0.0.0 means use broadcast discovery
    uint16_t masterUDPPort;     // Default 9001
    uint16_t keepAliveInterval; // ms
    uint8_t reserved[32];
};
#pragma pack()
```

---

## 7. Integration Checklist

### 7.1 Display Device Build Configuration

```cmake
# CMakeLists.txt or platformio.ini additions for Display device

# Define display device mode
set(DEVICE_MODE DISPLAY)  # vs. MASTER or SLAVE

# Disable unused components
set(BUILD_DEVICE_MANAGER OFF)
set(BUILD_REMOTE_DEVICES_MANAGER OFF)

# Enable new display components
set(BUILD_DISPLAY_CLIENT ON)
set(BUILD_DISPLAY_UI_THREAD ON)
set(BUILD_DISPLAY_SERVER OFF)  # Only on Master

# LVGL configuration
set(USE_LVGL ON)
set(LVGL_FONT_MONTSERRAT_20 ON)
set(LVGL_FONT_MONTSERRAT_32 ON)
```

### 7.2 Master Device Build Configuration

```cmake
# Master device still builds normally with additions:

set(BUILD_DISPLAY_SERVER ON)
set(BUILD_DEVICE_MANAGER ON)
set(BUILD_REMOTE_CONTROL_SERVER ON)

# No DisplayClient/DisplayUIThread on Master
set(BUILD_DISPLAY_CLIENT OFF)
set(BUILD_DISPLAY_UI_THREAD OFF)
```

### 7.3 Initialization Sequence

```
Platform Boot (Display Device):
├─ 1. HAL & drivers (GPIO, SPI, I2C)
├─ 2. LVGL initialization (no display yet)
├─ 3. ConfigProvider init (read NVM)
├─ 4. NetworkDriver init (WiFi, UDP)
├─ 5. OperatingSystem init (scheduler tick)
├─ 6. DisplayClient init (registration with NetworkDriver)
├─ 7. Platform applications cyclic ready
├─ 8. DisplayUIThread::start() ← UI thread spawned here
└─ 9. Normal cyclic loop

Platform Boot (Master Device):
├─ 1-7. Same as always
├─ 8. DisplayServer init (after HTTP server)
└─ 9. Normal cyclic loop
```

---

## 8. Error Recovery Strategies

### 8.1 Display Disconnection Recovery

```c++
// In DisplayClient::handleDisconnectedState()

if (disconnectionCount < MAX_RECONNECTION_ATTEMPTS) {
    // Exponential backoff: 5s, 10s, 20s, 40s, 60s
    unsigned long backoffMs = 5000 * (1 << std::min(disconnectionCount, 3));
    
    if (millis() - disconnectedSinceTime > backoffMs) {
        transitionTo(ClientState::DISCOVERY);
        disconnectionCount++;
    }
} else {
    // Give up, show UI error, wait for manual intervention
    Logger::log("DisplayClient: Max reconnection attempts reached");
    transitionTo(ClientState::ERROR);
}
```

### 8.2 Device List Corruption Recovery

```c++
// In DisplayClient::handleSyncingDevicesState()

if (receivedHash != expectedDeviceListHash) {
    retryCount++;
    
    if (retryCount >= 3) {
        // Clear cache and restart
        knownDevices.clear();
        Logger::log("DisplayClient: Device list corruption, clearing cache");
        transitionTo(ClientState::DISCOVERY);
    } else {
        // Single retry
        Logger::log("DisplayClient: Hash mismatch, retrying chunk sync");
        currentChunkIndex = 0;
        transitionTo(ClientState::WAITING_HANDSHAKE);
    }
}
```

---

## 9. Debugging & Logging

### 9.1 Protocol Message Logging

```c++
// Only enable if DEBUG_DISPLAY_PROTOCOL is defined

#ifdef DEBUG_DISPLAY_PROTOCOL

void logDisplayMessage(const MessageUDP& msg, bool incoming)
{
    const char* direction = incoming ? "←" : "→";
    
    switch ((DisplayMessageIds)msg.getId()) {
        case DisplayMessageIds::DISCOVER_DISPLAYS:
            Logger::log(direction + " DISCOVER_DISPLAYS");
            break;
            
        case DisplayMessageIds::DEVICE_ACTION_REQUEST: {
            auto payload = (DeviceActionRequestPayload*)msg.getPayload().data();
            Logger::log(direction + " DEVICE_ACTION_REQUEST seq=" + 
                       String(payload->header.sequenceNumber) +
                       " device=" + String(payload->targetDeviceId) +
                       " action=" + String(payload->actionType));
            break;
        }
        
        // ... other message types
    }
}

#endif
```

### 9.2 State Machine Tracing

```c++
#ifdef DEBUG_DISPLAY_STATE

void DisplayClient::transitionTo(ClientState newState)
{
    if (newState != currentState) {
        Serial.printf("[%lu] DisplayClient: %s → %s\n",
                     millis(),
                     stateToString(currentState).c_str(),
                     stateToString(newState).c_str());
        
        currentState = newState;
        onStateChanged(newState);
    }
}

#endif
```

---

End of Implementation Guide
