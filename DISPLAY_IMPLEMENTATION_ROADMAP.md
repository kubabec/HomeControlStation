# Display Terminal System - Implementation Roadmap & Milestones

**Document Version**: 1.0  
**Date**: 2026-09-07  
**Target Timeline**: ~12-16 weeks (phased development)  
**Author**: Architecture Planning  

---

## Overview

This document provides a detailed step-by-step implementation roadmap for the Display Terminal System. It is structured to support automated implementation (e.g., via scripting, CI/CD pipelines, or AI-assisted code generation). Each milestone includes specific deliverables, test criteria, and dependencies.

**Repository ownership**:
- HomeControlStation contains only `DisplayServer`, the master-side display endpoint model, shared wire-protocol definitions, and integration with existing HCS services.
- Guition is independently built and contains a physical copy of the HCS platform plus `DisplayClient`, `DisplayUIThread`, LVGL, and Guition hardware support.
- No symlink, absolute source path, shared build directory, or client/hardware build dependency is allowed from HomeControlStation to Guition.
- All `DisplayClient` and `DisplayUIThread` paths below refer to the Guition repository.

**Key Implementation Phases**:
1. **Phase 1**: Protocol Infrastructure (Weeks 1-2)
2. **Phase 2**: DisplayClient State Machine (Weeks 3-4)
3. **Phase 3**: DisplayUIThread & LVGL Integration (Weeks 5-6)
4. **Phase 4**: DisplayServer Implementation (Weeks 7-8)
5. **Phase 5**: Thread Communication & Synchronization (Weeks 9-10)
6. **Phase 6**: NVM & Configuration (Weeks 11-12)
7. **Phase 7**: Integration & Testing (Weeks 13-14)
8. **Phase 8**: Access Point Mode & Final Polish (Weeks 15-16)

---

## Phase 1: Protocol Infrastructure (Weeks 1-2)

### Goal

Define and implement all binary message structures, serialization/deserialization functions, and protocol constants. Establish foundation for all higher-level components.

### Milestone 1.1: Message Type Definitions

**Deliverables**:
- Create `include/os/app/display/DisplayProtocol.hpp` with all constants:
  - Message IDs (DISCOVER_DISPLAYS=200, DISPLAY_PRESENT=201, etc.)
  - Magic numbers, protocol versions
  - Payload size limits
  - Timeout constants (keep-alive 20s, ACK 3s, handshake 5s)
  - State machine state enums
  
**Implementation Checklist**:
- [ ] Define `enum class DisplayMessageType { DISCOVER_DISPLAYS=200, ... }`
- [ ] Define `enum class DisplayClientState { IDLE, DISCOVERY, SYNCING, CONNECTED, DISCONNECTED }`
- [ ] Define `enum class ActionType { TOGGLE, SET_VALUE, SERVICE_CALL }`
- [ ] Define `enum class ActionStatus { SUCCESS, FAILURE, TIMEOUT, INVALID_DEVICE }`
- [ ] Define all timeout and interval constants (20000ms keep-alive, 3000ms ACK timeout, etc.)
- [ ] Define max sizes (MAX_DEVICES_PER_CHUNK=5, MAX_DEVICES_PER_DISPLAY=200, MAX_DISPLAYS=20)

**Test Criteria**:
- [ ] All constants compile without errors
- [ ] No naming conflicts with existing HCS constants
- [ ] Values match those in DISPLAY_TERMINAL_ARCHITECTURE.md

---

### Milestone 1.2: Common Message Structures

**Deliverables**:
- Create `include/os/app/display/DisplayDatatypes.hpp` with common structures:

**Implementation Checklist**:
```cpp
// All structures use #pragma pack(1)

struct DisplayMessageHeader {
  uint16_t sequenceNumber;      // Increments per message
  uint32_t timestamp;           // Milliseconds
  uint8_t  reserved[4];         // Future use
};

struct DeviceDescriptionWire {
  // Device metadata: name, type, MAC, ID, room, state size
  // Total: ~98 bytes
};

struct DeviceStateWire {
  // Device ID, MAC, room, state bytes
  // Total: ~60 bytes
};

struct ActionRequestWire {
  // Device target, action type, parameter
};

struct ActionResponseWire {
  // Status, device state update
};
```

- [ ] Define all payload structures with exact byte alignment (`#pragma pack(1)`)
- [ ] Document each field with comments (endianness, units, etc.)
- [ ] Create static assertions for structure sizes
- [ ] Verify total payload sizes ≤ 1400 bytes

**Test Criteria**:
- [ ] `sizeof(DeviceDescriptionWire)` ≤ 98 bytes (5 devices = 490 bytes max)
- [ ] All structures compile without padding
- [ ] No compiler warnings about alignment/padding

---

### Milestone 1.3: Serialization & Deserialization Functions

**Deliverables**:
- Create `include/os/app/display/DisplaySerialization.hpp`
- Create `src/os/app/display/DisplaySerialization.cpp`

**Implementation Checklist**:
```cpp
// Encoder functions (object → binary)
bool encodeDeviceDescription(const Device& dev, DeviceDescriptionWire& wire);
bool encodeDeviceState(uint8_t deviceId, const Device& dev, DeviceStateWire& wire);
bool encodeActionRequest(uint8_t deviceId, ActionType action, 
                         const void* param, ActionRequestWire& wire);

// Decoder functions (binary → object)
bool decodeDeviceDescription(const DeviceDescriptionWire& wire, 
                             ParsedDeviceDescription& out);
bool decodeDeviceState(const DeviceStateWire& wire, 
                      uint8_t& outDeviceId, DeviceState& outState);
bool decodeActionRequest(const ActionRequestWire& wire, 
                        ParsedAction& outAction);

// CRC functions
uint16_t crc16_devices(const std::vector<DeviceDescriptionWire>& devices);
bool validate_device_list_hash(const std::vector<...>& devices, uint16_t hash);
```

- [ ] Implement all encode/decode functions with bounds checking
- [ ] Implement CRC16 hash function (matches existing HCS CRC if available)
- [ ] Add comprehensive error handling (invalid size, out-of-range values)
- [ ] Add unit tests for round-trip encoding/decoding

**Test Criteria**:
- [ ] All encode/decode functions pass unit tests
- [ ] CRC16 matches expected values for test data
- [ ] Invalid input gracefully rejected with error code
- [ ] No buffer overflows on malformed data

---

### Milestone 1.4: UDP Wrapper & Message Handlers

**Deliverables**:
- Create `include/os/app/display/DisplayMessageHandler.hpp`
- Create `src/os/app/display/DisplayMessageHandler.cpp`

**Implementation Checklist**:
```cpp
class DisplayMessageHandler {
public:
    static MessageUDP createDiscoverMessage();
    static MessageUDP createDisplayPresentMessage(MAC, version, screenW, screenH);
    static MessageUDP createDeviceListChunkMessage(index, total, devices[]);
    static MessageUDP createDeviceActionRequestMessage(sequence, deviceId, action);
    static MessageUDP createKeepAliveMessage(sequence);
    
    static bool parseMessage(const MessageUDP& msg, DisplayMessageType& type, 
                            DisplayMessageHeader& header, void* payload);
};
```

- [ ] Create factory functions for all 13+ message types
- [ ] Implement parse function that validates and extracts payload
- [ ] Store message sequences in static state for matching request/response
- [ ] Add logging (DEBUG level) for all message creation/parsing

**Test Criteria**:
- [ ] All message types can be created and parsed
- [ ] Round-trip: create → serialize → parse → verify
- [ ] Malformed messages rejected with DEBUG log
- [ ] Sequence numbers auto-increment correctly

---

## Phase 2: DisplayClient State Machine (Weeks 3-4)

### Goal

Implement DisplayClient application that runs on Display device. Manages discovery, handshake, device list sync, and connection lifecycle.

### Milestone 2.1: DisplayClient Core State Machine

**Deliverables**:
- In the Guition repository, create `include/os/app/display/DisplayClient.hpp`
- In the Guition repository, create `src/os/app/display/DisplayClient.cpp`

**Implementation Checklist**:
```cpp
class DisplayClient {
private:
    enum class State { IDLE, DISCOVERY, SYNCING_DEVICES, CONNECTED, DISCONNECTED };
    State currentState;
    uint8_t displayId;
    MAC masterMAC;
    IPAddr masterIP;
    uint16_t deviceListHash;
    uint32_t lastKeepAliveTime;
    uint8_t keepAliveRetries;
    
    std::vector<DeviceDescriptionWire> knownDevices;
    std::queue<DeviceStateUpdatePayload> stateUpdateQueue;
    std::queue<ActionRequestWithSequence> pendingActions;
    
public:
    static void init();
    static void deinit();
    static void cyclic();  // Call periodically (e.g., 10ms)
    
    static bool isConnected();
    static bool requestDeviceAction(uint8_t deviceId, ActionType action, 
                                   const void* param = nullptr);
    static const std::vector<DeviceDescriptionWire>& getDeviceList();
    static std::vector<DeviceStateUpdatePayload> pollStateUpdates();
    static uint8_t getDisplayId();
};
```

- [ ] Implement state machine with transitions (see DISPLAY_TERMINAL_ARCHITECTURE.md Section 4.2)
- [ ] IDLE state: Wait for manual activation
- [ ] DISCOVERY state: Broadcast DISCOVER_DISPLAYS every 15s; wait for DISPLAY_HANDSHAKE
- [ ] SYNCING_DEVICES state: Send DEVICE_LIST_REQUEST; accumulate chunks; validate hash
- [ ] CONNECTED state: Listen for updates; send keep-alive every 20s; process UI actions
- [ ] DISCONNECTED state: Implement backoff retry (5s → 60s) before returning to DISCOVERY
- [ ] Each state transition logged at INFO level

**Test Criteria**:
- [ ] State machine cycles without crashing
- [ ] Transitions occur at correct times (e.g., 15s discovery timeout)
- [ ] INFO logs show all transitions
- [ ] Thread-safe (uses mutexes for shared state)

---

### Milestone 2.2: Discovery & Handshake

**Deliverables**:
- Extend `src/os/app/display/DisplayClient.cpp` with discovery logic

**Implementation Checklist**:
```cpp
// In DISCOVERY state:
// 1. Timer 15s: Send DISCOVER_DISPLAYS broadcast
// 2. On DISPLAY_HANDSHAKE received:
//    - Validate sender is expected master (or accept any master)
//    - Extract displayId, deviceCount, deviceListHash
//    - Transition to SYNCING_DEVICES
//    - Start device list download

// In SYNCING_DEVICES state:
// 1. Send DEVICE_LIST_REQUEST
// 2. Accumulate DEVICE_LIST_CHUNK packets (track index/total)
// 3. On DEVICE_LIST_END:
//    - Compute hash of received devices
//    - If hash matches, transition to CONNECTED
//    - Else, retry (max 3 times)
// 4. Timeout after 20s without progress → return to DISCOVERY
```

- [ ] Implement discovery broadcast sending (every 15s in DISCOVERY)
- [ ] Implement DISPLAY_HANDSHAKE reception handler
- [ ] Implement device list chunking logic (accumulate, re-order if needed)
- [ ] Implement hash validation (CRC16 match)
- [ ] Add timeout handlers for each state
- [ ] Log all transitions and key events

**Test Criteria**:
- [ ] Display sends DISCOVER_DISPLAYS on startup
- [ ] Display accepts DISPLAY_HANDSHAKE and extracts ID
- [ ] Device list chunks accumulated in correct order
- [ ] Hash validation passes for known-good list
- [ ] Timeout after 20s if no list completion
- [ ] Retry mechanism works (backoff)

---

### Milestone 2.3: Device State Updates Reception

**Deliverables**:
- Extend `src/os/app/display/DisplayClient.cpp` with state update handling

**Implementation Checklist**:
```cpp
// Main cyclic() function:
// 1. Check UDP message queue from NetworkDriver
// 2. For each message:
//    a. Validate sender (master)
//    b. Parse message type
//    c. If DEVICE_STATE_UPDATE and currentState == CONNECTED:
//       - Extract device ID, new state
//       - Update knownDevices[deviceId]
//       - Queue update for DisplayUIThread
//    d. If KEEP_ALIVE:
//       - Update lastKeepAliveTime
//       - Queue KEEP_ALIVE_ACK response
//    e. If DEVICE_ACTION_RESPONSE:
//       - Match sequence number with pendingActions
//       - Update device state from response
//       - Signal waiting caller (event variable)
```

- [ ] Register UDP reception callback with NetworkDriver (port 9001)
- [ ] Parse all incoming messages (DEVICE_STATE_UPDATE, KEEP_ALIVE, ACTION_RESPONSE)
- [ ] Queue state updates to DisplayUIThread (thread-safe queue)
- [ ] Update local knownDevices on state change
- [ ] Send KEEP_ALIVE_ACK on receipt of KEEP_ALIVE

**Test Criteria**:
- [ ] State updates received and queued
- [ ] Local device state reflects updates
- [ ] DisplayUIThread can poll updates via getStateUpdates()
- [ ] Keep-alive ACK sent correctly
- [ ] Sequence numbers matched for action responses

---

### Milestone 2.4: User Action Submission

**Deliverables**:
- Extend `src/os/app/display/DisplayClient.cpp` with action submission

**Implementation Checklist**:
```cpp
// requestDeviceAction() function:
// 1. Find device in knownDevices by ID
// 2. Create ActionRequestWire with action type and parameter
// 3. Allocate sequence number and store in pendingActions
// 4. Send DEVICE_ACTION_REQUEST to master
// 5. Wait (blocking with timeout) for DEVICE_ACTION_RESPONSE
// 6. On response: update device state, return status
// 7. On timeout (5s): return TIMEOUT status
// 8. If master not connected: return NOT_CONNECTED status

// Support action types:
// - TOGGLE: No parameter needed
// - SET_VALUE: uint32_t value parameter (e.g., brightness)
// - SERVICE_CALL: String service name + optional parameter
```

- [ ] Implement requestDeviceAction() with sequence number allocation
- [ ] Support multiple concurrent actions (queue up to 2-3)
- [ ] Implement 5-second timeout with error handling
- [ ] Return ActionStatus (SUCCESS, FAILURE, TIMEOUT, INVALID_DEVICE)
- [ ] Update local state immediately on success
- [ ] Log action requests and responses

**Test Criteria**:
- [ ] Action request sent with unique sequence number
- [ ] Response matched to request by sequence number
- [ ] Timeout after 5s if no response
- [ ] Local state updated on success
- [ ] Multiple concurrent actions handled

---

### Milestone 2.5: Keep-Alive Management

**Deliverables**:
- Extend `src/os/app/display/DisplayClient.cpp` with keep-alive

**Implementation Checklist**:
```cpp
// In CONNECTED state, every 20 seconds:
// 1. Send KEEP_ALIVE message with incrementing sequence
// 2. Wait for KEEP_ALIVE_ACK (timeout 3s)
// 3. If ACK received: reset retry counter, continue
// 4. If timeout: increment retry counter
// 5. If retry counter reaches 3: transition to DISCONNECTED
// 6. In DISCONNECTED: start backoff retry (5s, 10s, 20s, 40s, 60s)
// 7. On reconnection: return to DISCOVERY

// Keep-alive data: { sequenceNumber, timestamp }
```

- [ ] Implement keep-alive sending (every 20s in CONNECTED)
- [ ] Implement ACK reception with timeout (3s)
- [ ] Implement retry logic (up to 3 retries)
- [ ] Implement exponential backoff (5s → 60s) in DISCONNECTED
- [ ] Log all keep-alive events (INFO: sent, received; ERROR: timeout)
- [ ] Notify DisplayUIThread of connection status changes

**Test Criteria**:
- [ ] Keep-alive sent on 20s interval
- [ ] ACK received and matched by sequence
- [ ] Disconnection after 3 failed keep-alives
- [ ] Backoff retry times: 5s, 10s, 20s, 40s, 60s
- [ ] Connection status queryable via isConnected()

---

## Phase 3: DisplayUIThread & LVGL Integration (Weeks 5-6)

### Goal

Implement LVGL-based UI rendering thread that displays devices, handles touch input, and updates based on state changes from DisplayClient.

### Milestone 3.1: LVGL Initialization & Screen Setup

**Deliverables**:
- Create `include/os/app/display/DisplayUIThread.hpp`
- Create `src/os/app/display/DisplayUIThread.cpp`

**Implementation Checklist**:
```cpp
class DisplayUIThread {
private:
    // LVGL objects
    lv_disp_t* display;
    lv_indev_t* touch_indev;
    lv_obj_t* homeScreen;
    lv_obj_t* statusBar;      // Time, location, connection status
    lv_obj_t* deviceGrid;     // 3×3 grid of device widgets
    lv_obj_t* footer;         // Logo area
    
    uint8_t currentRoomId;
    std::vector<uint8_t> currentRoomDevices;  // Device indices for current room
    
public:
    static void start();
    static void stop();
    static bool isRunning();
    static uint8_t getCurrentRoom();
    static void switchRoom(uint8_t newRoomId);
};

// LVGL rendering loop (runs in separate thread):
// 1. Initialize display driver (SPI interface to Guition display)
// 2. Initialize touch input device
// 3. Create home screen with status bar, device grid, footer
// 4. Loop forever:
//    a. Process touch input
//    b. Handle internal state updates
//    c. Call lv_task_handler() for LVGL rendering
//    d. Poll for device state updates from DisplayClient
//    e. Re-render affected widgets
```

- [ ] Create `DisplayUIThread::start()` - spawns FreeRTOS task (priority 3)
- [ ] Initialize LVGL with lv_init()
- [ ] Register display driver for SPI → Guition display (480×480, RGB)
- [ ] Register touch input driver for capacitive touch
- [ ] Create home screen layout with all zones (status bar, grid, footer)
- [ ] Load fonts (Montserrat 20pt, 32pt)
- [ ] Set up rendering loop with lv_task_handler()

**Test Criteria**:
- [ ] LVGL initializes without errors
- [ ] Display refreshes at ~60 FPS
- [ ] Touch input detected and logged
- [ ] No memory leaks in LVGL buffers (2× 480×480 = 460KB allocated)

---

### Milestone 3.2: Device Widget Creation & Rendering

**Deliverables**:
- Extend `src/os/app/display/DisplayUIThread.cpp` with device widgets

**Implementation Checklist**:
```cpp
// Device widget structure:
// - 140×90 pixels
// - Rounded border
// - Device name (top, white, 14pt)
// - State indicator (depends on device type):
//   - OnOff: "ON"/"OFF" text (green/red)
//   - LED: Color swatch + brightness bar
//   - Temp: "23.5°C 45%" (temperature + humidity)
//   - Window: Open/Closed icon
// - Background color: green (enabled), dim gray (disabled), orange (active)

void createDeviceWidgets() {
    // 1. Get current room devices from DisplayClient
    // 2. For each device in room (max 9 visible):
    //    a. Create LVGL button/container
    //    b. Add label for device name
    //    c. Add custom widget based on device type
    //    d. Set background color
    //    e. Register touch callback
    // 3. If room > 9 devices: add scroll up/down arrows
}

void updateDeviceWidget(uint8_t deviceId, const DeviceStateWire& newState) {
    // 1. Find widget for device
    // 2. Update state indicator (text, color, etc.)
    // 3. Trigger lv_obj_invalidate() for re-render
}
```

- [ ] Implement device widget creation (3×3 grid)
- [ ] Support all device types (OnOff, LED, Temp, Window, etc.)
- [ ] Use device-specific rendering based on JSON definition
- [ ] Register tap callbacks for device selection
- [ ] Implement long-press detection (for advanced controls)
- [ ] Add scrolling support for rooms > 9 devices

**Test Criteria**:
- [ ] Devices display with correct names and state
- [ ] Device types render with appropriate widgets
- [ ] Touch detected on device widgets
- [ ] Scrolling works for > 9 devices

---

### Milestone 3.3: Room Navigation & Filtering

**Deliverables**:
- Extend `src/os/app/display/DisplayUIThread.cpp` with room navigation

**Implementation Checklist**:
```cpp
// Room navigation:
// 1. Display current room name in status bar
// 2. Create left/right arrow buttons (or area) for room switching
// 3. On left arrow press:
//    a. currentRoomId = (currentRoomId - 1 + totalRooms) % totalRooms
//    b. Re-filter devices by new room
//    c. Clear current grid, recreate with new devices
// 4. On right arrow press:
//    a. currentRoomId = (currentRoomId + 1) % totalRooms
//    b. Re-filter and re-render

void switchRoom(uint8_t newRoomId) {
    currentRoomId = newRoomId;
    
    // Get room name from DisplayClient device list
    const char* roomName = getRoomName(newRoomId);
    lv_label_set_text(roomNameLabel, roomName);
    
    // Filter devices by room ID
    currentRoomDevices.clear();
    auto& allDevices = DisplayClient::getDeviceList();
    for (int i = 0; i < allDevices.size(); i++) {
        if (allDevices[i].roomId == newRoomId) {
            currentRoomDevices.push_back(i);
        }
    }
    
    // Recreate device grid
    clearDeviceGrid();
    createDeviceWidgets();
}
```

- [ ] Implement left/right navigation arrows
- [ ] Store room information from device list
- [ ] Filter devices by room ID
- [ ] Update room name display on switch
- [ ] Re-render device grid on room change
- [ ] Wrap around at boundaries (last room → first)

**Test Criteria**:
- [ ] Room name updates on navigation
- [ ] Devices filtered by room correctly
- [ ] Navigation wraps around
- [ ] Room switch < 50ms
- [ ] No network requests for room switch

---

### Milestone 3.4: Touch Input Handling & User Interactions

**Deliverables**:
- Extend `src/os/app/display/DisplayUIThread.cpp` with touch handling

**Implementation Checklist**:
```cpp
// Touch input handling:
// 1. LVGL handles touch via registered input device
// 2. On tap of device widget:
//    a. Show device info popup with current state
//    b. Provide toggle/action buttons
// 3. On long press (2s):
//    a. Show advanced controls popup
//    b. For LED: brightness slider, color picker
//    c. For other types: custom actions
// 4. On room navigation arrow tap:
//    a. Trigger room switch
//    b. Animate grid transition

void onDeviceTap(uint8_t deviceId) {
    // Show popup with device state and action buttons
    lv_obj_t* popup = createDeviceInfoPopup(deviceId);
    lv_obj_add_state(popup, LV_STATE_DEFAULT);  // Show
}

void onDeviceAction(uint8_t deviceId, ActionType action) {
    // Queue action request to DisplayClient
    if (DisplayClient::requestDeviceAction(deviceId, action, nullptr)) {
        showMessage("Sending...");
    } else {
        showErrorMessage("Failed to send action");
    }
}
```

- [ ] Register touch input callbacks with LVGL
- [ ] Detect tap (short press) on device widgets
- [ ] Detect long press (2+ seconds) for advanced controls
- [ ] Show device info popup on tap
- [ ] Queue action requests to DisplayClient
- [ ] Show visual feedback ("Sending...", "Done", "Error")
- [ ] Handle timeout gracefully

**Test Criteria**:
- [ ] Touch detected on device widgets
- [ ] Popup displayed with device state
- [ ] Action button sends request to DisplayClient
- [ ] Touch latency < 200ms (tap → network message)

---

### Milestone 3.5: State Update Polling & UI Refresh

**Deliverables**:
- Extend `src/os/app/display/DisplayUIThread.cpp` with state updates

**Implementation Checklist**:
```cpp
// In main rendering loop (every ~16ms):
// 1. Poll DisplayClient::pollStateUpdates() (non-blocking)
// 2. For each state update received:
//    a. Find device in currentRoomDevices
//    b. If device in current room:
//       - Update widget display
//       - Trigger lv_obj_invalidate()
//    c. Else:
//       - Silently discard (not visible anyway)
// 3. Update status bar (time, connection status)

void updateDisplayState(const DeviceStateUpdatePayload& update) {
    uint8_t deviceId = update.deviceId;
    
    // Find device in current room
    for (int i = 0; i < currentRoomDevices.size(); i++) {
        if (currentRoomDevices[i] == deviceId) {
            // Device is visible, update widget
            updateDeviceWidget(i, update.newState);
            break;
        }
    }
}
```

- [ ] Poll state updates every render cycle (16ms)
- [ ] Update widgets for devices in current room
- [ ] Discard updates for devices not visible
- [ ] Update status bar (time, connection icon)
- [ ] Smooth transitions (no jarring changes)
- [ ] Log state updates at DEBUG level

**Test Criteria**:
- [ ] State updates applied to visible widgets
- [ ] UI refreshes smoothly on state change
- [ ] Status bar updates (time, connection)
- [ ] Invisible device updates don't cause flicker

---

### Milestone 3.6: Error & Status Display

**Deliverables**:
- Extend `src/os/app/display/DisplayUIThread.cpp` with status messages

**Implementation Checklist**:
```cpp
// Status messages:
// 1. Connection status indicator (top-right corner):
//    - Green: Connected to master
//    - Red: Disconnected (waiting for connection)
//    - Yellow: Connecting...
// 2. Device grid disabled when not connected
// 3. Overlay message: "Waiting for connection..."

// Error messages (popup):
// - "Action timeout" (if action > 5s)
// - "Device not found"
// - "Communication error"
// - "Device offline"

void updateConnectionStatus() {
    bool connected = DisplayClient::isConnected();
    lv_color_t statusColor = connected ? LV_COLOR_GREEN : LV_COLOR_RED;
    lv_obj_set_style_bg_color(statusIcon, statusColor, 0);
    
    if (!connected) {
        lv_label_set_text(overlayLabel, "Waiting for connection...");
        lv_obj_add_flag(deviceGrid, LV_OBJ_FLAG_DISABLED);
    } else {
        lv_obj_clear_flag(overlayLabel, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(deviceGrid, LV_OBJ_FLAG_DISABLED);
    }
}
```

- [ ] Implement connection status icon (color indicator)
- [ ] Show overlay message when disconnected
- [ ] Disable device interactions when offline
- [ ] Show error popups for action failures
- [ ] Auto-dismiss error messages after 3 seconds
- [ ] Log all error messages

**Test Criteria**:
- [ ] Connection status icon visible and correct color
- [ ] Overlay shown when disconnected
- [ ] Error messages display and dismiss
- [ ] Device interactions disabled when offline

---

## Phase 4: DisplayServer Implementation (Weeks 7-8)

### Goal

Implement DisplayServer application on Master device. Manages all connected displays, coordinates state updates, and routes device actions.

### Milestone 4.1: DisplayServer Initialization & Discovery

**Deliverables**:
- Replace stub in `src/os/app/display/DisplayServer.cpp`
- Implement main server logic

**Implementation Checklist**:
```cpp
class DisplayServer {
private:
    struct ConnectedDisplay {
        uint8_t displayId;
        MAC macAddress;
        IPAddr ipAddress;
        uint32_t lastKeepAliveTime;
        uint8_t staleCount;
        bool isStale() { return staleCount > 0; }
    };
    
    std::vector<ConnectedDisplay> knownDisplays;  // Max 20
    uint16_t deviceListHash;
    uint32_t nextDiscoveryTime;
    
public:
    static void init();      // Register handlers, initialize state
    static void deinit();    // Cleanup
    static void cyclic();    // Main loop (call every 100ms)
    
    // Discovery
    static void broadcastDiscovery();
    static void onDisplayPresent(const MAC& mac, const IPAddr& ip);
    
    // Device list distribution
    static void sendDeviceListToDisplay(const ConnectedDisplay& display);
    
    // State updates
    static void broadcastDeviceStateUpdate(const DeviceStateUpdatePayload& update);
    
    // Action handling
    static void onDeviceActionRequest(const MAC& mac, const ActionRequestWire& action);
};

// Initialization:
// 1. Register with NetworkDriver (UDP 9001 reception callback)
// 2. Register with DataContainer (device state change callback)
// 3. Compute initial device list hash
// 4. Set nextDiscoveryTime = now
```

- [ ] Implement DisplayServer::init() with UDP registration
- [ ] Implement DisplayServer::cyclic() state machine
- [ ] Implement discovery broadcast (every 15s)
- [ ] Implement DISPLAY_PRESENT reception (add to knownDisplays)
- [ ] Assign display IDs (0-19) to new displays
- [ ] Track all display metadata (MAC, IP, last keep-alive)

**Test Criteria**:
- [ ] Discovery broadcasts sent every 15 seconds
- [ ] Display announces present, receives handshake
- [ ] Display ID assigned uniquely (0-19)
- [ ] No crashes with 20 simultaneous displays

---

### Milestone 4.2: Device List Distribution

**Deliverables**:
- Extend `src/os/app/display/DisplayServer.cpp` with device list

**Implementation Checklist**:
```cpp
void sendDeviceListToDisplay(const ConnectedDisplay& display) {
    // 1. Get all devices from DeviceProvider
    // 2. Encode each device to DeviceDescriptionWire
    // 3. Split into chunks (max 5 devices per chunk)
    // 4. Send DEVICE_LIST_CHUNK packets sequentially
    // 5. Send DEVICE_LIST_END marker
    // 6. Wait for display to acknowledge (hash match)
    
    auto& allDevices = DeviceProvider::getAllDevices();
    std::vector<DeviceDescriptionWire> encodedDevices;
    
    for (auto& device : allDevices) {
        DeviceDescriptionWire wire;
        if (encodeDeviceDescription(device, wire)) {
            encodedDevices.push_back(wire);
        }
    }
    
    // Chunk and send
    int chunkIndex = 0;
    for (int i = 0; i < encodedDevices.size(); i += 5) {
        int chunkEnd = std::min(i + 5, (int)encodedDevices.size());
        int devicesInChunk = chunkEnd - i;
        
        MessageUDP msg = createDeviceListChunkMessage(
            chunkIndex, 
            (encodedDevices.size() + 4) / 5,  // total chunks
            &encodedDevices[i],
            devicesInChunk
        );
        NetworkDriver::sendUDP(display.ipAddress, 9001, msg);
        chunkIndex++;
    }
    
    // Send end marker
    MessageUDP endMsg = createDeviceListEndMessage();
    NetworkDriver::sendUDP(display.ipAddress, 9001, endMsg);
}

void onDisplayPresent(const MAC& mac, const IPAddr& ip) {
    // 1. Find or create display entry
    // 2. Send DISPLAY_HANDSHAKE with ID, device count, hash
    // 3. Set display state to awaiting device list request
    
    ConnectedDisplay newDisplay;
    newDisplay.displayId = findAvailableDisplayId();
    newDisplay.macAddress = mac;
    newDisplay.ipAddress = ip;
    newDisplay.lastKeepAliveTime = getCurrentTime();
    newDisplay.staleCount = 0;
    
    knownDisplays.push_back(newDisplay);
    
    MessageUDP handshake = createDisplayHandshakeMessage(
        newDisplay.displayId,
        DeviceProvider::getAllDevices().size(),
        deviceListHash
    );
    NetworkDriver::sendUDP(ip, 9001, handshake);
}
```

- [ ] Implement device list encoding (DeviceDescriptionWire)
- [ ] Implement chunking (max 5 devices per chunk)
- [ ] Compute device list hash (CRC16)
- [ ] Send DEVICE_LIST_CHUNK packets
- [ ] Send DEVICE_LIST_END marker
- [ ] Implement handshake with device count and hash

**Test Criteria**:
- [ ] Device list sent in chunks of ≤ 5 devices
- [ ] All devices included (no duplicates, no missing)
- [ ] Hash matches on display side
- [ ] Chunk order preserved

---

### Milestone 4.3: Device State Broadcast

**Deliverables**:
- Extend `src/os/app/display/DisplayServer.cpp` with state updates

**Implementation Checklist**:
```cpp
void onDeviceStateChange(const Device& device, const DeviceState& oldState, 
                         const DeviceState& newState) {
    // 1. Encode state change to DeviceStateUpdatePayload
    // 2. Broadcast to all connected (non-stale) displays
    // 3. Rate-limit: max 10 updates/sec per display
    
    DeviceStateUpdatePayload update;
    update.deviceId = device.id;
    update.nodeMac = device.nodeMac;
    update.roomId = device.roomId;
    update.newState = newState.customBytes;
    update.changeReason = "user_action";  // or "auto_update", "external"
    
    for (auto& display : knownDisplays) {
        if (!display.isStale()) {
            // Rate-limit check
            if (canSendUpdate(display.displayId)) {
                MessageUDP msg = createDeviceStateUpdateMessage(update);
                NetworkDriver::sendUDP(display.ipAddress, 9001, msg);
            }
        }
    }
}

void init() {
    // Register callback with DataContainer
    DataContainer::onDeviceStateChanged([](const Device& d, const DeviceState& old, 
                                          const DeviceState& new) {
        onDeviceStateChange(d, old, new);
    });
}
```

- [ ] Register callback with DataContainer for state changes
- [ ] Encode state to DeviceStateUpdatePayload
- [ ] Broadcast to all connected displays
- [ ] Implement rate limiting (max 10 updates/sec)
- [ ] Include change reason (user_action, auto_update, etc.)
- [ ] Log broadcasts at DEBUG level

**Test Criteria**:
- [ ] State changes trigger broadcasts
- [ ] All displays receive update within 100ms
- [ ] Rate limiting enforced (max 10/sec)
- [ ] Stale displays not sent updates

---

### Milestone 4.4: Device Action Routing

**Deliverables**:
- Extend `src/os/app/display/DisplayServer.cpp` with action handling

**Implementation Checklist**:
```cpp
void onDeviceActionRequest(const MAC& sourceMac, uint16_t sequence,
                          const ActionRequestWire& actionRequest) {
    // 1. Find device by ID and node MAC
    // 2. Validate device exists
    // 3. Call DeviceProvider::applyServiceCall() (same as HTTP)
    // 4. Send DEVICE_ACTION_RESPONSE with status
    // 5. If successful, DEVICE_STATE_UPDATE automatically triggered
    
    try {
        // Find device
        auto* device = DeviceProvider::findDevice(actionRequest.deviceId, 
                                                  actionRequest.nodeMac);
        if (!device) {
            sendActionResponse(sourceMac, sequence, ActionStatus::INVALID_DEVICE, nullptr);
            return;
        }
        
        // Parse action and parameter
        const void* paramData = nullptr;
        uint16_t paramSize = 0;
        parseActionParameter(actionRequest, paramData, paramSize);
        
        // Execute action
        bool success = false;
        if (actionRequest.actionType == ActionType::TOGGLE) {
            success = DeviceProvider::toggleDevice(device->id);
        } else if (actionRequest.actionType == ActionType::SET_VALUE) {
            success = DeviceProvider::setDeviceValue(device->id, 
                                                    *(uint32_t*)paramData);
        } else if (actionRequest.actionType == ActionType::SERVICE_CALL) {
            success = DeviceProvider::callService(device->id, 
                                                 (const char*)paramData);
        }
        
        // Send response
        if (success) {
            sendActionResponse(sourceMac, sequence, ActionStatus::SUCCESS, 
                             &device->state);
        } else {
            sendActionResponse(sourceMac, sequence, ActionStatus::FAILURE, nullptr);
        }
        
    } catch (const std::exception& e) {
        LOG_ERROR("Action processing failed: %s", e.what());
        sendActionResponse(sourceMac, sequence, ActionStatus::FAILURE, nullptr);
    }
}
```

- [ ] Implement action request reception handler
- [ ] Route via DeviceProvider (same as HTTP server)
- [ ] Support TOGGLE, SET_VALUE, SERVICE_CALL actions
- [ ] Send DEVICE_ACTION_RESPONSE with status
- [ ] Handle local and remote devices
- [ ] Log all actions

**Test Criteria**:
- [ ] Action request parsed correctly
- [ ] Device found and action executed
- [ ] Response sent with correct status
- [ ] State update broadcasted on success

---

### Milestone 4.5: Keep-Alive & Disconnection Management

**Deliverables**:
- Extend `src/os/app/display/DisplayServer.cpp` with keep-alive

**Implementation Checklist**:
```cpp
void cyclic() {
    // Main server loop (call every 100ms)
    
    uint32_t now = getCurrentTime();
    
    // Discovery broadcasts (every 15s)
    if (now >= nextDiscoveryTime) {
        broadcastDiscovery();
        nextDiscoveryTime = now + 15000;  // 15 seconds
    }
    
    // Keep-alive monitoring
    for (auto it = knownDisplays.begin(); it != knownDisplays.end(); ) {
        uint32_t timeSinceKeepAlive = now - it->lastKeepAliveTime;
        
        if (timeSinceKeepAlive > 45000) {  // 45 seconds
            // Mark stale
            it->staleCount++;
            if (it->staleCount >= 2) {
                // Remove after 2 stale intervals (90 seconds total)
                LOG_INFO("Display %d disconnected", it->displayId);
                it = knownDisplays.erase(it);
                continue;
            }
        } else {
            // Reset stale count if back online
            it->staleCount = 0;
        }
        
        ++it;
    }
    
    // Process incoming messages
    while (MessageUDP* msg = NetworkDriver::getUDPMessage(9001)) {
        handleIncomingMessage(*msg);
        delete msg;
    }
}

void onKeepAlive(const MAC& mac, uint16_t sequence) {
    // 1. Find display by MAC
    // 2. Update lastKeepAliveTime
    // 3. Send KEEP_ALIVE_ACK
    
    for (auto& display : knownDisplays) {
        if (display.macAddress == mac) {
            display.lastKeepAliveTime = getCurrentTime();
            display.staleCount = 0;  // Reset stale counter
            
            MessageUDP ack = createKeepAliveAckMessage(sequence);
            NetworkDriver::sendUDP(display.ipAddress, 9001, ack);
            break;
        }
    }
}
```

- [ ] Implement keep-alive reception (update timestamp)
- [ ] Mark displays as stale after 45s without keep-alive
- [ ] Remove disconnected displays after 60s
- [ ] Send KEEP_ALIVE_ACK on receipt
- [ ] Log keep-alive events at DEBUG level

**Test Criteria**:
- [ ] Keep-alive ACK sent promptly
- [ ] Display marked stale after 45s
- [ ] Display removed after 60s
- [ ] Stale displays don't receive state updates

---

## Phase 5: Thread Communication & Synchronization (Weeks 9-10)

### Goal

Implement thread-safe communication primitives between DisplayClient, DisplayUIThread, and main platform core.

### Milestone 5.1: Thread-Safe Queues & Mutexes

**Deliverables**:
- Create `include/os/app/display/DisplayThreadSync.hpp`
- Create `src/os/app/display/DisplayThreadSync.cpp`

**Implementation Checklist**:
```cpp
// Thread-safe queue wrapper
template<typename T>
class DisplaySafeQueue {
private:
    std::queue<T> queue;
    mutable std::mutex mutex;
    std::condition_variable cv;
    
public:
    void enqueue(const T& item) {
        {
            std::lock_guard<std::mutex> lock(mutex);
            queue.push(item);
        }
        cv.notify_one();
    }
    
    bool tryDequeue(T& item, uint32_t timeoutMs = 0) {
        std::unique_lock<std::mutex> lock(mutex);
        
        if (queue.empty()) {
            if (timeoutMs > 0) {
                return cv.wait_for(lock, std::chrono::milliseconds(timeoutMs),
                                 [this] { return !queue.empty(); });
            }
            return false;
        }
        
        item = queue.front();
        queue.pop();
        return true;
    }
    
    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex);
        return queue.size();
    }
    
    void clear() {
        std::lock_guard<std::mutex> lock(mutex);
        while (!queue.empty()) queue.pop();
    }
};

// Synchronization event (for matching request/response)
class DisplaySyncEvent {
private:
    std::mutex mutex;
    std::condition_variable cv;
    bool signaled;
    
public:
    void wait(uint32_t timeoutMs = 5000) {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait_for(lock, std::chrono::milliseconds(timeoutMs),
                   [this] { return signaled; });
    }
    
    void signal() {
        {
            std::lock_guard<std::mutex> lock(mutex);
            signaled = true;
        }
        cv.notify_one();
    }
    
    void reset() {
        std::lock_guard<std::mutex> lock(mutex);
        signaled = false;
    }
};
```

- [ ] Implement DisplaySafeQueue template class (enqueue, tryDequeue, size, clear)
- [ ] Implement DisplaySyncEvent for action request/response matching
- [ ] Use FreeRTOS synchronization primitives if available (for consistency)
- [ ] Add unit tests for queue and event

**Test Criteria**:
- [ ] Queue thread-safe (multiple producers/consumers)
- [ ] Event signaling works with timeout
- [ ] No deadlocks or race conditions
- [ ] Performance acceptable (< 1% CPU overhead)

---

### Milestone 5.2: DisplayClient Thread Integration

**Deliverables**:
- Extend `src/os/app/display/DisplayClient.cpp` with threading

**Implementation Checklist**:
```cpp
// DisplayClient internal architecture:
// - Main platform thread (NetworkDriver thread) handles all network I/O
// - DisplayClient runs as state machine in platform thread (not separate thread)
// - Thread-safe queues separate platform thread from UI thread

class DisplayClient {
private:
    // Queues for inter-thread communication
    DisplaySafeQueue<DeviceStateUpdatePayload> stateUpdateQueue;  // Network → UI
    DisplaySafeQueue<ActionRequestWithResponse> pendingActions;   // UI → Network
    
    // For matching request/response
    std::map<uint16_t, DisplaySyncEvent> actionSequenceEvents;
    std::map<uint16_t, ActionResponseWire> actionResponses;
    
public:
    // UI thread calls this (blocks briefly)
    bool requestDeviceAction(uint8_t deviceId, ActionType action, 
                            const void* param) {
        if (!isConnected()) return false;
        
        uint16_t sequence = allocateSequence();
        ActionRequestWire request = { ... };
        
        // Send request
        MessageUDP msg = createActionRequestMessage(sequence, request);
        NetworkDriver::sendUDP(masterIP, 9001, msg);
        
        // Wait for response (5s timeout)
        auto& event = actionSequenceEvents[sequence];
        event.wait(5000);
        
        // Check if response received
        if (actionResponses.count(sequence)) {
            auto response = actionResponses[sequence];
            actionResponses.erase(sequence);
            actionSequenceEvents.erase(sequence);
            
            // Update local state on success
            if (response.status == ActionStatus::SUCCESS) {
                updateLocalDeviceState(deviceId, response.newState);
            }
            return response.status == ActionStatus::SUCCESS;
        }
        
        // Timeout
        actionSequenceEvents.erase(sequence);
        return false;
    }
    
    // Platform thread calls this (in cyclic())
    std::vector<DeviceStateUpdatePayload> pollStateUpdates() {
        std::vector<DeviceStateUpdatePayload> updates;
        DeviceStateUpdatePayload update;
        
        while (stateUpdateQueue.tryDequeue(update, 0)) {
            updates.push_back(update);
        }
        
        return updates;
    }
};
```

- [ ] Add DisplaySafeQueue for state updates (platform → UI)
- [ ] Add DisplaySafeQueue for action requests (UI → platform)
- [ ] Implement request/response matching via sequence numbers
- [ ] Implement timeout on action wait (5 seconds)
- [ ] Add unit tests for request/response flow

**Test Criteria**:
- [ ] State updates queued and retrieved without loss
- [ ] Action requests/responses matched by sequence
- [ ] Timeout after 5 seconds if no response
- [ ] Multiple concurrent actions supported
- [ ] No data corruption with concurrent access

---

### Milestone 5.3: DisplayUIThread Synchronization

**Deliverables**:
- Extend `src/os/app/display/DisplayUIThread.cpp` with threading

**Implementation Checklist**:
```cpp
// DisplayUIThread is separate FreeRTOS task (priority 3)
// Communicates with platform core (priority 2) via queues

void displayUIThreadMain(void* arg) {
    // 1. Initialize LVGL
    // 2. Create screens
    // 3. Main loop:
    //    a. Poll DisplayClient::pollStateUpdates()
    //    b. Update widgets
    //    c. Check for connection status changes
    //    d. Call lv_task_handler()
    //    e. Sleep ~16ms (60 FPS)
    
    while (isRunning) {
        uint32_t startTime = getCurrentTime();
        
        // Poll state updates (non-blocking)
        auto updates = DisplayClient::pollStateUpdates();
        for (auto& update : updates) {
            updateDeviceWidget(update);
        }
        
        // Update connection status
        updateConnectionStatus();
        
        // LVGL rendering
        lv_task_handler();
        
        // Maintain 60 FPS (16ms per frame)
        uint32_t elapsed = getCurrentTime() - startTime;
        if (elapsed < 16) {
            osDelay(16 - elapsed);
        }
    }
}

void start() {
    // Spawn FreeRTOS task
    BaseType_t result = xTaskCreate(
        displayUIThreadMain,        // Function
        "DisplayUIThread",          // Name
        2048,                       // Stack size (words)
        nullptr,                    // Parameter
        3,                          // Priority (higher than network = 2)
        &uiThreadHandle             // Handle
    );
    
    if (result != pdPASS) {
        LOG_ERROR("Failed to create DisplayUIThread");
    }
}
```

- [ ] Create FreeRTOS task for DisplayUIThread (priority 3)
- [ ] Implement main loop with ~16ms tick (60 FPS)
- [ ] Poll DisplayClient state updates every frame
- [ ] Call lv_task_handler() each frame
- [ ] Add priority to platform thread (priority 2 > UI priority 3 = higher priority)
- [ ] Add stack usage monitoring

**Test Criteria**:
- [ ] Thread creates and runs without errors
- [ ] Frame rate ~60 FPS (16ms per frame)
- [ ] State updates processed every frame
- [ ] No priority inversion (platform core not starved)
- [ ] Memory usage < 64KB stack + 600KB heap

---

## Phase 6: NVM & Configuration (Weeks 11-12)

### Goal

Store and retrieve configuration through the standard HCS `ConfigProvider` and `PersistentMemoryAccess` applications. Do not add a display-only NVM implementation.

### Milestone 6.1: NVM Layout & Storage

**Deliverables**:
- Reuse the copied HCS configuration and persistent-memory modules unchanged in responsibility
- Add display-specific data only through the existing HCS NVM block model when needed

**Implementation Checklist**:
```cpp
// NVM layout for display device (typical 4MB flash):
// Standard HCS blocks own WiFi and node configuration.
// Optional display settings use an allocated HCS block; master address is never persisted.
// 0x1000 - 0xFFFF: Reserved for future use

#pragma pack(1)
struct DisplayNVMConfig {
    // WiFi configuration
    char wifiSSID[32];              // 32 bytes
    char wifiPassword[64];          // 64 bytes
    uint8_t wifiMode;               // 1: Station, 2: AccessPoint
    
    // Network configuration
    uint16_t broadcastDiscover;     // Always enabled for automatic discovery
    
    // Display settings
    char displayName[32];           // "Display-1", etc.
    uint8_t displayOrientation;     // 0: Normal, 90/180/270
    uint8_t displayBrightness;      // 0-255
    
    // Metadata
    uint16_t version;               // Config version for compatibility
    uint16_t crc;                   // CRC16 of config
    
    // Padding to reach 4KB
    uint8_t reserved[4096 - 32 - 64 - 1 - 4 - 2 - 32 - 1 - 1 - 2 - 2];
};

class DisplayNVM {
public:
    static bool readConfig(DisplayNVMConfig& config);
    static bool writeConfig(const DisplayNVMConfig& config);
    static bool eraseConfig();  // Clear all settings
    
    static bool isValidConfig(const DisplayNVMConfig& config);
    static void getDefaults(DisplayNVMConfig& config);
};
```

- [ ] Define DisplayNVMConfig structure (4KB)
- [ ] Implement readConfig() with CRC validation
- [ ] Implement writeConfig() with CRC computation
- [ ] Implement eraseConfig() (factory reset)
- [ ] Add default configuration (empty WiFi, broadcast discovery enabled)
- [ ] Add NVM compatibility versioning

**Test Criteria**:
- [ ] Config written and read back correctly
- [ ] CRC validation works
- [ ] Factory reset clears all settings
- [ ] Default config valid

---

### Milestone 6.2: WiFi Configuration & Persistence

**Deliverables**:
- Extend `src/os/app/display/DisplayClient.cpp` with NVM

**Implementation Checklist**:
```cpp
void DisplayClient::init() {
    // 1. Load NVM config
    DisplayNVMConfig config;
    if (DisplayNVM::readConfig(config)) {
        // Config loaded successfully
        masterIP = config.masterIP;
        broadcastDiscover = config.broadcastDiscover;
        displayName = config.displayName;
    } else {
        // No valid config, use defaults
        DisplayNVM::getDefaults(config);
    }
    
    // 2. If WiFi configured, attempt connection
    if (config.wifiMode == WIFI_STATION && config.wifiSSID[0] != '\0') {
        NetworkDriver::connectWiFi(config.wifiSSID, config.wifiPassword);
    } else {
        // No WiFi configured, enter AP mode
        startAccessPointMode(config.displayName);
    }
    
    // 3. Start discovery/connection sequence
    currentState = State::DISCOVERY;
}

void DisplayClient::saveConfiguration() {
    // Called after successful connection
    DisplayNVMConfig config;
    DisplayNVM::readConfig(config);
    
    // Update with current settings
    strncpy(config.masterIP, masterIP, sizeof(config.masterIP));
    config.broadcastDiscover = broadcastDiscover;
    
    DisplayNVM::writeConfig(config);
}
```

- [ ] Load WiFi config on startup
- [ ] Attempt WiFi connection if configured
- [ ] Enter AP mode if WiFi not configured
- [ ] Save configuration after successful connection
- [ ] Support manual WiFi reconfiguration via AP mode

**Test Criteria**:
- [ ] WiFi config loaded and applied
- [ ] AP mode starts if WiFi not configured
- [ ] Configuration persists across reboots

---

### Milestone 6.3: Access Point Configuration Server

**Deliverables**:
- Create `include/os/app/display/DisplayAccessPoint.hpp`
- Create `src/os/app/display/DisplayAccessPoint.cpp`

**Implementation Checklist**:
```cpp
// Access Point HTTP Server
// Started when WiFi not configured or connection fails
// SSID: "HCS_Display-{MAC}" (e.g., "HCS_Display-AABBCC")
// IP: 192.168.4.1
// Port: 80

class DisplayAccessPoint {
private:
    httpd_handle_t serverHandle;
    
public:
    static void start();
    static void stop();
    
private:
    // Handlers
    static esp_err_t handleConfigWiFi(httpd_req_t* req);       // POST /config/wifi
    static esp_err_t handleConfigNetwork(httpd_req_t* req);    // POST /config/network
    static esp_err_t handleConfigStatus(httpd_req_t* req);     // GET /config/status
    static esp_err_t handleConfigRestart(httpd_req_t* req);    // POST /config/restart
    static esp_err_t handleWebpage(httpd_req_t* req);          // GET /
};

// Endpoints:
// POST /config/wifi
//   Request: {"ssid": "...", "password": "..."}
//   Response: 200 OK
//
// POST /config/network
//   Request: {"masterIP": "192.168.1.1", "broadcastDiscover": true}
//   Response: 200 OK
//
// GET /config/status
//   Response: {"ssid": "...", "displayId": 1, "deviceCount": 15}
//
// POST /config/restart
//   Response: 200 OK (device reboots)
```

- [ ] Start HTTP server in AP mode
- [ ] Serve configuration webpage (HTML form)
- [ ] Implement POST /config/wifi endpoint
- [ ] Implement POST /config/network endpoint
- [ ] Implement GET /config/status endpoint
- [ ] Implement POST /config/restart endpoint
- [ ] Store configuration and reboot device

**Test Criteria**:
- [ ] HTTP server accessible at 192.168.4.1:80
- [ ] WiFi config form displays
- [ ] WiFi configuration saved and applied
- [ ] Device reboots after config

---

## Phase 7: Integration & Testing (Weeks 13-14)

### Goal

Integrate all components, run end-to-end tests, and validate the complete system.

### Milestone 7.1: Build Configuration & Compilation

**Deliverables**:
- Update `CMakeLists.txt` for Display system
- Update `platformio.ini` for Guition device

**Implementation Checklist**:
```cmake
# In HomeControlStation/CMakeLists.txt

# Display system components
add_library(display_protocol
    src/os/app/display/DisplaySerialization.cpp
    src/os/app/display/DisplayMessageHandler.cpp
)

add_library(display_client
    src/os/app/display/DisplayClient.cpp
    src/os/app/display/DisplayThreadSync.cpp
    src/os/app/display/DisplayNVM.cpp
)

add_library(display_server
    src/os/app/display/DisplayServer.cpp
)

# Optional: disable display system
option(ENABLE_DISPLAY_TERMINALS "Enable display terminal support" ON)

if(ENABLE_DISPLAY_TERMINALS)
    target_compile_definitions(platform PUBLIC ENABLE_DISPLAY_TERMINALS=1)
    target_link_libraries(platform PUBLIC display_protocol display_server)
endif()

# Unit tests for display system
if(BUILD_TESTS)
    add_executable(test_display_protocol
        test/display/test_serialization.cpp
        test/display/test_message_handler.cpp
    )
    target_link_libraries(test_display_protocol display_protocol)
    add_test(NAME DisplayProtocol COMMAND test_display_protocol)
    
    add_executable(test_display_client
        test/display/test_client_state_machine.cpp
    )
    target_link_libraries(test_display_client display_client)
    add_test(NAME DisplayClient COMMAND test_display_client)
endif()
```

- [ ] Add Display library targets to CMakeLists.txt
- [ ] Create test targets for protocol, client, server
- [ ] Add ENABLE_DISPLAY_TERMINALS option
- [ ] Update Guition platformio.ini to include DisplayUIThread
- [ ] Verify all files compile without warnings
- [ ] Test build with and without ENABLE_DISPLAY_TERMINALS

**Test Criteria**:
- [ ] All source files compile
- [ ] No compiler warnings (C++11 standard)
- [ ] Feature can be disabled via CMAKE flag
- [ ] Test executables build successfully

---

### Milestone 7.2: Unit Tests

**Deliverables**:
- Create test files in `test/display/`

**Implementation Checklist**:
```cpp
// test/display/test_serialization.cpp
TEST(DisplaySerialization, EncodeDecodeDeviceDescription) {
    Device device;
    device.id = 1;
    device.name = "Living Room LED";
    device.type = DEVICE_TYPE_LED_STRIP;
    device.roomId = 0;
    
    DeviceDescriptionWire wire;
    EXPECT_TRUE(encodeDeviceDescription(device, wire));
    
    ParsedDeviceDescription parsed;
    EXPECT_TRUE(decodeDeviceDescription(wire, parsed));
    
    EXPECT_EQ(parsed.id, device.id);
    EXPECT_STREQ(parsed.name, device.name);
}

TEST(DisplaySerialization, CRC16Hash) {
    std::vector<DeviceDescriptionWire> devices;
    // ... add test devices ...
    
    uint16_t hash1 = crc16_devices(devices);
    uint16_t hash2 = crc16_devices(devices);
    
    EXPECT_EQ(hash1, hash2);  // Deterministic
}

// test/display/test_message_handler.cpp
TEST(DisplayMessageHandler, CreateAndParseMessage) {
    MessageUDP original = createDiscoverMessage();
    
    DisplayMessageType type;
    DisplayMessageHeader header;
    EXPECT_TRUE(parseMessage(original, type, header, nullptr));
    
    EXPECT_EQ(type, DisplayMessageType::DISCOVER_DISPLAYS);
}

// test/display/test_client_state_machine.cpp
TEST(DisplayClient, StateTransitions) {
    // Mock NetworkDriver
    // Test IDLE → DISCOVERY (on init)
    // Test DISCOVERY → SYNCING_DEVICES (on handshake)
    // Test SYNCING_DEVICES → CONNECTED (on device list sync)
}
```

- [ ] Create unit tests for serialization/deserialization
- [ ] Create unit tests for message handling
- [ ] Create unit tests for state machine transitions
- [ ] Create unit tests for thread-safe queues
- [ ] Mock external dependencies (NetworkDriver, etc.)
- [ ] Aim for > 80% code coverage

**Test Criteria**:
- [ ] All unit tests pass
- [ ] Code coverage > 80%
- [ ] Tests run in < 5 seconds

---

### Milestone 7.3: Integration Tests

**Deliverables**:
- Create integration test scenarios

**Implementation Checklist**:
```cpp
// test/display/integration_test.cpp

TEST(DisplayIntegration, MasterDiscoveryAndHandshake) {
    // 1. Start mock master (DisplayServer)
    // 2. Start mock display (DisplayClient)
    // 3. Verify discovery messages exchanged
    // 4. Verify handshake completed
    // 5. Verify device list received
    // 6. Verify hash matches
    // Expect: Display transitions to CONNECTED
}

TEST(DisplayIntegration, UserActionFlow) {
    // 1. Display connected and ready
    // 2. Simulate user tap on device widget
    // 3. DisplayUIThread calls DisplayClient::requestDeviceAction()
    // 4. DisplayClient sends DEVICE_ACTION_REQUEST
    // 5. DisplayServer routes via DeviceProvider
    // 6. Device state changes
    // 7. DisplayServer broadcasts DEVICE_STATE_UPDATE
    // 8. Display receives and updates UI
    // Expect: Action completes within 500ms
}

TEST(DisplayIntegration, MultipleDisplaysSync) {
    // 1. Start master with 3 displays
    // 2. Device state changes
    // 3. Verify state update broadcast to all 3 displays
    // 4. Verify displays show consistent state
    // Expect: All displays synchronized
}

TEST(DisplayIntegration, DisplayDisconnectionRecovery) {
    // 1. Display connected and receiving updates
    // 2. Simulate WiFi disconnection
    // 3. Verify display enters DISCONNECTED state
    // 4. Verify backoff retry sequence (5s, 10s, ...)
    // 5. Simulate WiFi reconnection
    // 6. Verify display re-discovers and reconnects
    // 7. Verify device list re-synced
    // Expect: Display fully recovered to CONNECTED
}
```

- [ ] Test master discovery and handshake flow
- [ ] Test device action flow (UI → master → device → broadcast)
- [ ] Test multiple display synchronization
- [ ] Test disconnection and reconnection
- [ ] Test device list hash mismatch recovery
- [ ] Test keep-alive timeout and recovery

**Test Criteria**:
- [ ] All integration tests pass
- [ ] Discovery completes within 30 seconds
- [ ] User actions complete within 500ms
- [ ] Multiple displays stay synchronized
- [ ] Disconnection recovery works

---

### Milestone 7.4: System Testing on Hardware

**Deliverables**:
- Test on actual Guition ESP32-4848S040 display
- Test with real master (HomeControlStation)

**Implementation Checklist**:
```
1. Flash Guition device with DisplayClient firmware
2. Flash master with DisplayServer firmware
3. Verify WiFi connectivity
4. Verify display discovers master
5. Verify device list displayed
6. Verify touch input works
7. Verify user actions control devices
8. Verify state updates display in real-time
9. Test room navigation
10. Test with 3+ displays simultaneously
11. Test WiFi disconnection/reconnection
12. Test master shutdown/restart
13. Verify no crashes after 24 hours operation
14. Profile CPU/memory usage
```

- [ ] Compile for ESP32 (both Master and Display)
- [ ] Flash both devices with binaries
- [ ] Verify device discovers master
- [ ] Verify device list syncs
- [ ] Test touch input on display
- [ ] Test device control (toggle, brightness)
- [ ] Test multiple displays (3+)
- [ ] Test disconnection/reconnection
- [ ] Log performance metrics
- [ ] Stress test (1000+ state updates)

**Test Criteria**:
- [ ] Display discovers master < 30 seconds
- [ ] Device list syncs < 20 seconds
- [ ] Touch latency < 200ms
- [ ] User actions complete < 500ms
- [ ] No crashes in 24-hour test
- [ ] CPU usage < 5%
- [ ] Memory usage < 1MB (display device)

---

## Phase 8: Access Point Mode & Final Polish (Weeks 15-16)

### Goal

Complete access point configuration, add documentation, and prepare for release.

### Milestone 8.1: Access Point Mode Testing

**Deliverables**:
- Test AP configuration flow end-to-end

**Implementation Checklist**:
```
1. Power on Guition display without WiFi config
2. Verify access point starts (SSID: "HCS_Display-AABBCC")
3. Connect to AP with mobile device
4. Open browser to 192.168.4.1
5. Verify configuration webpage displays
6. Enter WiFi credentials (SSID, password)
7. Click "Connect"
8. Verify device saves config and reboots
9. Verify device connects to specified WiFi
10. Verify device discovers master and syncs device list
11. Verify display shows device grid
```

- [ ] Test AP starts after 30s without WiFi
- [ ] Verify SSID contains MAC address
- [ ] Test configuration webpage loads
- [ ] Test WiFi config form submission
- [ ] Test device reboots after config
- [ ] Test device connects to new WiFi
- [ ] Test full flow: AP → WiFi → Discovery → Connected

**Test Criteria**:
- [ ] AP starts within 30 seconds
- [ ] Configuration webpage accessible
- [ ] WiFi config saved and applied
- [ ] Device reconnects to master after AP configuration

---

### Milestone 8.2: Error Recovery & Edge Cases

**Deliverables**:
- Test error scenarios and edge cases

**Implementation Checklist**:
```
1. Invalid WiFi password → AP mode restarts
2. Master not discovered or unreachable → Display repeats broadcast discovery with backoff
3. Device list exceeds 200 devices → Partial sync (handle gracefully)
4. Out-of-order DEVICE_LIST_CHUNK packets → Sequence number recovery
5. Duplicate packets → Sequence number filtering
6. Master sends state update for unknown device → Silently discard
7. User action on stale device → Error message
8. Master disappears mid-action → Timeout, error message
9. WiFi reconnection during action → Retry logic
10. Touch input while offline → Disabled buttons
```

- [ ] Test WiFi connection failures
- [ ] Test master unreachable scenarios
- [ ] Test large device lists (> 100 devices)
- [ ] Test packet loss and duplication
- [ ] Test action timeouts
- [ ] Test offline mode (graceful degradation)
- [ ] Test rapid room switches
- [ ] Test rapid device toggling

**Test Criteria**:
- [ ] All error scenarios handled without crashes
- [ ] Graceful error messages displayed
- [ ] Recovery automatic or user-guided
- [ ] No data corruption

---

### Milestone 8.3: Documentation & Code Comments

**Deliverables**:
- Complete API documentation
- Code comments and design notes

**Implementation Checklist**:
```cpp
// Doxygen-style comments for all public APIs

/**
 * @brief Initialize DisplayServer
 * 
 * Registers UDP message handler with NetworkDriver and device state callback
 * with DataContainer. Broadcasts initial discovery message.
 * 
 * @note Must be called after NetworkDriver and DataContainer initialized
 * @see DisplayServer::cyclic()
 * @see DisplayServer::deinit()
 */
void DisplayServer::init();

/**
 * @brief Main DisplayServer state machine loop
 * 
 * Called periodically (recommended ~100ms interval). Handles:
 * - Discovery broadcast (every 15 seconds)
 * - Keep-alive monitoring
 * - Device state updates
 * - Message reception and routing
 * 
 * @note Non-blocking; runs in platform core thread
 * @warning Maximum 20 displays supported simultaneously
 */
void DisplayServer::cyclic();
```

- [ ] Add Doxygen comments to all public methods
- [ ] Add design notes to key classes
- [ ] Add usage examples in header comments
- [ ] Update README with display setup instructions
- [ ] Create troubleshooting guide
- [ ] Document NVM layout and configuration

**Test Criteria**:
- [ ] All public APIs documented
- [ ] README includes display setup steps
- [ ] Troubleshooting guide covers common issues
- [ ] Doxygen builds without warnings

---

### Milestone 8.4: Performance Optimization & Cleanup

**Deliverables**:
- Optimize critical paths
- Remove debug code and logging

**Implementation Checklist**:
```cpp
// Performance optimization checklist:
// 1. Minimize allocations in cyclic() loops
//    - Pre-allocate queues and buffers
// 2. Optimize serialization (avoid intermediate copies)
// 3. Minimize string operations in state machine
// 4. Profile LVGL rendering (target 60 FPS)
// 5. Reduce memory footprint (< 1MB per display)
// 6. Optimize WiFi connection time (target < 10s)
// 7. Profile master CPU usage (target < 5% for 20 displays)

// Code cleanup:
// - Remove debug printf() calls
// - Replace with LOG_DEBUG() macro
// - Disable verbose logging in release builds
// - Remove unused variables/functions
// - Fix compiler warnings
```

- [ ] Profile cyclic() performance
- [ ] Measure LVGL frame rate (target 60 FPS)
- [ ] Measure memory usage (target < 1MB display)
- [ ] Measure master CPU overhead (target < 5%)
- [ ] Optimize hot paths
- [ ] Remove debug code
- [ ] Fix all compiler warnings
- [ ] Pass static analysis (clang-tidy, etc.)

**Test Criteria**:
- [ ] Frame rate ≥ 60 FPS
- [ ] Memory usage < 1MB display + 10KB per display on master
- [ ] Master CPU < 5% overhead
- [ ] Zero compiler warnings
- [ ] Static analysis passes

---

### Milestone 8.5: Release Preparation

**Deliverables**:
- Final testing checklist
- Release notes

**Implementation Checklist**:
```
Release Checklist:
- [ ] All unit tests pass
- [ ] All integration tests pass
- [ ] 24-hour hardware stress test passed
- [ ] Documentation complete
- [ ] Doxygen builds successfully
- [ ] No critical bugs in GitHub issues
- [ ] CMake build configuration finalized
- [ ] Backward compatibility verified (HCS 2.0 compatible)
- [ ] Access point mode tested end-to-end
- [ ] Multi-display scenarios tested (3, 10, 20 displays)
- [ ] WiFi reconnection tested
- [ ] Master restart/recovery tested
- [ ] Touch latency < 200ms verified
- [ ] User action latency < 500ms verified
- [ ] Memory/CPU profiling complete
- [ ] Build size optimized
- [ ] Flashable binaries prepared
```

- [ ] Run final test suite
- [ ] Generate release notes
- [ ] Tag source code version
- [ ] Prepare binary releases
- [ ] Create user guide (PDF)
- [ ] Create troubleshooting FAQ

**Test Criteria**:
- [ ] All checkboxes checked
- [ ] No critical issues remaining
- [ ] Performance targets met
- [ ] Release ready for deployment

---

## Timeline Summary

| Phase | Duration | Deliverables | Status |
|-------|----------|--------------|--------|
| 1 | Weeks 1-2 | Protocol Infrastructure | Planned |
| 2 | Weeks 3-4 | DisplayClient State Machine | Planned |
| 3 | Weeks 5-6 | DisplayUIThread & LVGL | Planned |
| 4 | Weeks 7-8 | DisplayServer Implementation | Planned |
| 5 | Weeks 9-10 | Thread Communication | Planned |
| 6 | Weeks 11-12 | NVM & Configuration | Planned |
| 7 | Weeks 13-14 | Integration & Testing | Planned |
| 8 | Weeks 15-16 | Access Point & Release | Planned |

**Total**: ~16 weeks (4 months)

---

## Automation & CI/CD Integration

### Continuous Integration Recommendations

```yaml
# .github/workflows/display-system-ci.yml
name: Display System CI

on: [push, pull_request]

jobs:
  build-and-test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      - name: Build Display System
        run: cmake --build build --target display_protocol display_client display_server
      - name: Run Unit Tests
        run: ctest --verbose
      - name: Run Integration Tests
        run: ctest -R Integration --verbose
      - name: Code Coverage
        run: cmake --build build --target coverage
      - name: Upload Coverage
        uses: codecov/codecov-action@v2
      - name: Static Analysis
        run: clang-tidy src/os/app/display/*.cpp --checks=*
```

### Automation Hints for Each Phase

- **Phase 1**: Script to auto-generate enum definitions from requirements table
- **Phase 2**: Auto-generate state machine diagrams from code comments
- **Phase 3**: Auto-generate LVGL widget templates from device type JSON
- **Phase 4**: Auto-generate message type handler from protocol spec
- **Phase 5**: Template-based thread sync primitive generation
- **Phase 6**: Auto-generate NVM layout documentation
- **Phase 7**: Test case generation from SRS requirements
- **Phase 8**: Automated binary size/memory profiling reports

---

## Success Metrics

| Metric | Target | Measurement |
|--------|--------|------------|
| Discovery Time | < 30s | From power-on to device list sync |
| Handshake Time | < 10s | From DISPLAY_PRESENT to CONNECTED |
| Touch Latency | < 200ms | Tap to network message sent |
| User Action Latency | < 500ms | Action sent to state update reflected |
| Room Switch Latency | < 50ms | Arrow press to new room displayed |
| Frame Rate | ≥ 60 FPS | LVGL rendering on Guition display |
| Display Scalability | 20 devices | Simultaneous displays on one master |
| Master CPU Overhead | < 5% | With 20 displays connected |
| Display Memory | < 1MB | UI buffers + device cache |
| Reliability | 99% uptime | Over 24-hour test |
| Code Coverage | > 80% | Unit test code coverage |

---

End of Implementation Roadmap
