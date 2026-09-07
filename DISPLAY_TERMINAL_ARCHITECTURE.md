# HomeControlStation Display Terminal Architecture

## 1. Executive Summary

This document defines the architecture for a multi-display terminal system integrated into HomeControlStation. Display terminals (ESP32 devices running LVGL) function as sophisticated UI clients that mirror the HTTP webUI functionality without access to the device layer. The system employs:

- **DisplayServer** on Master: Coordinates up to 20 display terminals, manages device state distribution, and routes user interactions
- **DisplayClient** on Display: Runs in a complete, locally copied HCS platform with a parallel UI rendering thread
- **Display Protocol**: Custom UDP-based protocol optimized for display state synchronization and user input routing
- **Thread Architecture**: Safe data exchange between HCS platform core (network, services) and display UI thread

The master and display are separate firmware repositories and are flashed independently. The Guition repository contains physical copies of the HCS platform and application files; it has no source link, symlink, or build dependency on the master repository. Tailoring is limited to application composition and display hardware: the master schedules `DisplayServer`, while Guition schedules `DisplayClient` and `DisplayUIThread`. Standard HCS NVM, configuration, WiFi, UDP, HTTP, OTA, diagnostics, and scheduler abstractions remain unchanged in responsibility.

---

## 2. Display Terminal UI Layout Specification

### 2.1 Physical Display & Screen Reference

The target display is a **4" Guition ESP32-4848S040** (480×480 pixels, square):

```
┌──────────────────────────────────────────────────┐
│  Warsza.      LIVING ROOM        12:00           │ ← Status Bar (50px height)
├──────────────────────────────────────────────────┤
│                                           [OFF]   │ ← Master power button (optional)
│  < ┌───────────┐ ┌───────────┐ ┌───────────┐ >  │
│    │  Main     │ │    TV     │ │  Meble    │     │ ← Device grid (140×90 each)
│    │  lamp     │ │           │ │           │     │
│    └───────────┘ └───────────┘ └───────────┘     │
│  < ┌───────────┐ ┌───────────┐ ┌───────────┐ >  │
│    │  Okno     │ │LED Strip▯ │ │  Temp     │     │
│    │           │ │▓▓▓▓▓▓     │ │ 23.5°C    │     │
│    └───────────┘ └───────────┘ │ 45%       │     │
│                                 └───────────┘     │
│                                                   │
│                 [Scrollable area]                 │
│                                                   │
│                  ⌂ HomeControlStation             │ ← Footer logo (40px)
└──────────────────────────────────────────────────┘
```

**Display Specifications**:
- **Resolution**: 480×480 pixels (square format)
- **Size**: 4" (approximately 100mm diagonal)
- **Color**: Full RGB color support
- **Touch**: Capacitive touchscreen
- **Interface**: SPI/GPIO connection to ESP32
- **Usable area**: ~430×430 pixels (accounting for margins)

### 2.2 UI Components & Layout Rules

**Status Bar (Top 50px)**:
- Left: Location name ("Warsza.") — from settings
- Center: Current room name ("LIVING ROOM") — updates when room changes
- Right: Current time ("12:00") — synced from master or RTC
- Background: Dark gradient overlay

**Device Grid (Main area, 380×320 pixels)**:
- 3 columns × up to 3 rows visible
- Each widget: ~140×90 pixels
- Scroll up/down for rooms > 9 devices
- Left/Right arrows: Navigate between rooms
- Padding: 6px between items, 10px margins

**Device Widget Appearance**:
- Border: Subtle outline with rounded corners
- Dimensions: 140×90 pixels (compact square format)
- Text: Device name (top, white, 14pt)
- State indicator: Icon or color
  - LED Strip: Shows color swatch or brightness bar (compact)
  - Temp Sensor: Shows numeric value (e.g., "23.5°C 45%", small text)
  - OnOff: Text "ON" (green) or "OFF" (red/gray)
  - Window sensor: Icon (open/closed)
- Colors:
  - Enabled device: Colored background (green, blue, etc.)
  - Disabled device: Dim gray
  - Active control: Highlighted (orange accent)

**Master Power Button** (Top right, optional):
- Shows overall network/master status
- Red "OFF" state indicator
- Dimensions: 50×30 pixels

**Footer (Bottom 40px)**:
- Logo: "⌂ HomeControlStation"
- Centered, small text (10pt)
- Indicates system name/brand

### 2.3 Device Info Popup / Advanced Controls

When a device is **long-pressed**, an overlay popup appears:

```
┌──────────────────────────────────┐
│      Device Info                 │ ← Header
├──────────────────────────────────┤
│ Device: LED Strip                │
│ Room: LIVING ROOM                │
│ State: ON                        │
│ Action: Long press               │
│                                  │
│        ◐  (Loading spinner)      │ ← Waiting for response
│                                  │
│    [▓▓▓ Sending... ▓▓▓]          │ ← Status message
│                                  │
│  [────────────────────────────]  │ ← Action button (e.g., "Set Brightness")
│                                  │
└──────────────────────────────────┘
```

**Popup Features**:
- Modal overlay (darkens background)
- Shows device metadata:
  - Device name
  - Room assignment
  - Current state (ON/OFF)
  - Last interaction type
- Loading spinner during action
- Device type-specific control (if applicable):
  - **OnOff devices**: Toggle button
  - **LED Strip**: Brightness slider + color picker (if supported)
  - **Temp Sensor**: Read-only display
  - **Window/Door sensor**: Read-only status
- Cancel/Back button or tap outside to close

**Interactions**:
- Single tap on device widget: Open device info popup
- Long press (2s): Show advanced control popup
- Tap action button: Send DEVICE_ACTION_REQUEST to master, show spinner
- Receive DEVICE_ACTION_RESPONSE: Update state, close popup

---

## 3. Communication Protocol Specification

### 3.1 Message Structure

All Display Protocol messages use the existing `MessageUDP` wrapper with custom payload structures. Messages share a common header followed by type-specific data.

```
MessageUDP Wrapper:
  ├─ ID: uint8_t (Display-specific message type)
  ├─ Source/Destination IP + Port (UDP metadata)
  └─ Payload: Variable-length binary data
     └─ Display Protocol Payload:
        ├─ SequenceNumber: uint16_t (for reliability tracking)
        ├─ Timestamp: uint32_t (milliseconds, for keep-alive/sync)
        └─ MessageData: Type-specific structure
```

### 3.2 Message Types

```c++
enum class DisplayMessageIds : uint8_t
{
    // Discovery & Lifecycle
    DISCOVER_DISPLAYS = 200,           // Server → Broadcast: "Which displays are alive?"
    DISPLAY_PRESENT = 201,             // Display → Server: "I am here" + metadata
    DISPLAY_HANDSHAKE = 202,           // Server → Display: "I acknowledge you" + subscription
    
    // Device Synchronization
    DEVICE_LIST_REQUEST = 210,         // Display → Server: "Send device list"
    DEVICE_LIST_CHUNK = 211,           // Server → Display: Device batch (chunked for large lists)
    DEVICE_LIST_END = 212,             // Server → Display: "No more devices"
    DEVICE_STATE_UPDATE = 213,         // Server → Display: Device state changed (broadcast to all displays)
    
    // User Interaction & Control
    DEVICE_ACTION_REQUEST = 220,       // Display → Server: User toggled/controlled device
    DEVICE_ACTION_RESPONSE = 221,      // Server → Display: Action executed (or failed)
    
    // State Management
    DISPLAY_STATE_REFRESH = 230,       // Server → Display: "Refresh all device states"
    DISPLAY_HASH_SYNC = 231,           // Server ↔ Display: Device list hash validation
    
    // Keep-Alive
    KEEP_ALIVE = 240,                  // Bidirectional: Heartbeat + timestamp
    KEEP_ALIVE_ACK = 241,              // Bidirectional: Acknowledge heartbeat
};
```

### 3.3 Room Navigation & Device Grouping

**Room Organization**:
- Each room is a separate view (accessed via left/right arrows)
- Maximum 255 rooms supported (0-254)
- Room 0: Special case, may represent "All" or "System"
- Room names stored in master's HTTP server metadata
- Display requests room name translation during handshake

**Room Switching Flow**:
1. User presses LEFT arrow (`<`)
2. DisplayUIThread detects input
3. Rotates backward: roomId = (roomId - 1) or wraps to last room
4. Queries DisplayClient::getDeviceList()
5. Filters: device.roomId == currentRoomId
6. Calls HomeControlApp::reloadDevices(filteredList)
7. LVGL re-renders grid with new room's devices (no network request)

**Device Sorting & Display Order**:
- Sorted by deviceId within room (deterministic across displays)
- First two columns: rows 0, 1, 2
- Third column: rows 0, 1, 2 (max 9 visible without scroll)
- Devices 10+: Scrollable vertically with up/down indicators

**Room Name Display**:
- From HTTP server's roomNamesMapping
- Broadcast to all displays during handshake
- Fallback: "Room {roomId}" if name not available

### 3.4 Payload Structures

#### DISPLAY_PRESENT (Display → Server)
```c++
struct DisplayPresencePayload {
    uint16_t sequenceNumber;           // For ordering
    uint32_t timestamp;                // Display clock
    uint8_t firmwareVersion;           // Display firmware version
    uint8_t screenWidth;               // Physical width in pixels (e.g., 480)
    uint8_t screenHeight;              // Physical height in pixels (e.g., 320)
    uint8_t maxSimultaneousDevices;    // Capability hint (e.g., 30 widgets)
    uint8_t reserved[8];               // For future use
    // Total: 16 bytes
};
```

#### DISPLAY_HANDSHAKE (Server → Display)
```c++
struct DisplayHandshakePayload {
    uint16_t sequenceNumber;           // Echo from DISPLAY_PRESENT
    uint32_t timestamp;                // Server time
    uint8_t displayAssignedId;         // Unique ID in master's list (0-19)
    uint8_t numberOfDevices;           // Total devices to expect
    uint16_t deviceListHash;           // CRC16 of device list
    uint32_t masterPlatformVersion;    // Platform version for protocol compatibility
    uint8_t reserved[8];               // For future use
    // Total: 20 bytes
};
```

#### DEVICE_LIST_CHUNK (Server → Display)
```c++
struct DeviceListChunkPayload {
    uint16_t sequenceNumber;           // Packet sequence
    uint16_t chunkIndex;               // Which chunk (0, 1, 2, ...)
    uint16_t totalChunks;              // Total chunks expected
    uint8_t devicesInThisChunk;        // Number of devices in payload
    uint8_t reserved;
    // Followed by N × DeviceDescriptionWire (packed, no padding)
    // DeviceDescriptionWire defined below
};

struct DeviceDescriptionWire {
    uint8_t deviceType;                // From JSON definition
    uint64_t macAddress;               // Master's MAC that owns device
    uint8_t deviceId;                  // Local ID on owning master
    uint8_t roomId;                    // Room assignment (0-255)
    uint8_t isEnabled;                 // 1 = enabled, 0 = disabled
    uint8_t customBytesSize;           // How many custom bytes follow
    char deviceName[32];               // Null-terminated name
    uint8_t customBytes[50];           // Type-specific state
    // Total per device: ~98 bytes (packed)
};
```

#### DEVICE_STATE_UPDATE (Server → All Displays)
```c++
struct DeviceStateUpdatePayload {
    uint16_t sequenceNumber;           // For ordering
    uint32_t timestamp;                // When change occurred
    uint8_t sourceDeviceType;          // Device that changed
    uint64_t sourceNodeMac;            // Which node owns it
    uint8_t sourceDeviceId;            // Device ID on that node
    uint8_t newStateCustomBytes[50];   // New state data
    uint8_t stateChangeReason;         // 0=user_action, 1=auto_update, 2=external_change
    uint8_t reserved[7];
    // Total: ~65 bytes
};
```

#### DEVICE_ACTION_REQUEST (Display → Server)
```c++
struct DeviceActionRequestPayload {
    uint16_t sequenceNumber;           // Unique ID for this request
    uint32_t requestTimestamp;         // When user acted
    uint8_t sourceDisplayId;           // Which display initiated (from handshake)
    uint8_t targetDeviceType;          // What kind of device
    uint64_t targetNodeMac;            // Which node hosts it
    uint8_t targetDeviceId;            // Device ID on that node
    uint8_t actionType;                // 0=toggle, 1=set_value, 2=call_service
    uint8_t actionServiceCode;         // Service ID if actionType=2
    uint32_t actionParameter;          // Value if actionType=1, or service param
    uint8_t reserved[8];
    // Total: ~30 bytes
};
```

#### DEVICE_ACTION_RESPONSE (Server → Display)
```c++
struct DeviceActionResponsePayload {
    uint16_t sequenceNumber;           // Echo from DEVICE_ACTION_REQUEST
    uint32_t responseTimestamp;        // When executed
    uint8_t status;                    // 0=success, 1=failed, 2=timeout, 3=invalid
    uint8_t resultCode;                // Device-specific result
    uint8_t newStateCustomBytes[50];   // Updated state if successful
    uint8_t reserved[8];
    // Total: ~60 bytes
};
```

#### KEEP_ALIVE (Bidirectional)
```c++
struct KeepAlivePayload {
    uint16_t sequenceNumber;           // Ping sequence
    uint32_t timestamp;                // Sender's clock
    uint8_t sender;                    // 0=server, 1=display
    uint8_t displayId;                 // Display ID (for server-to-display)
    uint8_t reserved[10];
    // Total: 16 bytes
};
```

---

## 4. Display Device Software Architecture

### 4.1 Overall Thread Model

The Display device runs two independent but coordinated threads:

```
┌──────────────────────────────────────────────────────────────────────┐
│                          ESP32 Display Device                        │
└──────────────────────────────────────────────────────────────────────┘

┌────────────────────────────────┐    ┌─────────────────────────────────┐
│    HCS Platform Core Thread    │    │    Display UI Rendering Thread  │
│     (FreeRTOS Priority 2)      │    │      (FreeRTOS Priority 3)      │
├────────────────────────────────┤    ├─────────────────────────────────┤
│ • Platform initialization      │    │ • LVGL tick handler             │
│ • Operating System (scheduler) │    │ • Input event processing        │
│ • NetworkDriver (UDP/WiFi)     │    │ • Screen rendering              │
│ • ConfigProvider (NVM)         │    │ • Animation/transitions         │
│ • DisplayClient app            │    │ • User interaction callbacks    │
│ • RemoteControl Client (stub)  │    │ • Queued state updates          │
│ • No DeviceManager             │    │ • No platform access directly   │
│ • No RemoteDevicesManager      │    │                                 │
└────────────────────────────────┘    └─────────────────────────────────┘
         ↕                                         ↕
    UDP port 9001              Thread-Safe Message Queue (FIFO)
    ↔ DisplayServer                    ↔ UI State & Actions
```

### 4.2 Display Client Application

`DisplayClient` is a new application in the DisplayServer category that replaces the dummy implementation:

```c++
// include/os/app/display/DisplayClient.hpp
class DisplayClient
{
public:
    /**
     * Initializes connection state machine and UDP reception handler.
     */
    static void init();
    
    /**
     * Releases resources and disconnects from server.
     */
    static void deinit();
    
    /**
     * Core cyclic routine: discovery, handshake, state sync, action dispatch.
     */
    static void cyclic();
    
    /**
     * Called by DisplayUIThread to request device action.
     * Queues request → waits for response → signals thread with result.
     */
    static void requestDeviceAction(const DeviceActionRequestPayload& action);
    
    /**
     * Called by DisplayUIThread to retrieve latest device list.
     * Returns only if list is complete; blocks until available.
     */
    static const std::vector<DeviceDescriptionWire>& getDeviceList();
    
    /**
     * Called by DisplayUIThread for current device state.
     */
    static std::optional<DeviceDescriptionWire> getDeviceState(uint64_t nodeMac, uint8_t deviceId);
    
private:
    // State machine states
    enum ClientState {
        IDLE,               // Waiting to discover master
        DISCOVERY,          // Sending DISCOVER_DISPLAYS
        WAITING_HANDSHAKE,  // Sent DISPLAY_PRESENT, waiting DISPLAY_HANDSHAKE
        HANDSHOOK,          // Received handshake, requesting device list
        SYNCING_DEVICES,    // Receiving DEVICE_LIST_CHUNKs
        SYNCED,             // Ready for action dispatch
        CONNECTED,          // Stable, receiving updates
        DISCONNECTED        // Lost connection to master
    };
    
    static ClientState currentState;
    static uint8_t assignedDisplayId;           // From handshake
    static std::vector<DeviceDescriptionWire> knownDevices;
    static std::map<std::pair<uint64_t, uint8_t>, DeviceDescriptionWire> deviceStateMap;
    static std::queue<MessageUDP> receptionUdpQueue;
    static std::queue<DeviceActionRequestPayload> pendingActions;
    static std::queue<DeviceActionResponsePayload> actionResponses;
    
    static unsigned long lastDiscoveryTime;
    static unsigned long lastKeepAliveTime;
    static unsigned long lastHandshakeTime;
    static uint16_t currentChunkIndex;
    static uint16_t expectedTotalChunks;
    
    // Threading
    static Mutex deviceListMutex;               // Protects knownDevices
    static Mutex actionMutex;                   // Protects action queues
    static Event deviceListReadyEvent;          // Signals when list complete
};
```

### 4.3 Display UI Thread

The display UI thread (running LVGL) is launched after platform initialization:

```c++
// include/os/app/display/DisplayUIThread.hpp
class DisplayUIThread
{
public:
    /**
     * Launch the UI thread (called after platform init completes).
     * Creates FreeRTOS task at appropriate priority.
     */
    static void start();
    
    /**
     * Signal graceful shutdown of UI thread.
     */
    static void stop();
    
private:
    /**
     * Main thread loop: runs LVGL tick, processes input, renders.
     * Entry point: displayUIThreadEntry()
     */
    static void threadLoop();
    
    // Callbacks from LVGL widgets
    static void onDeviceTogglePressed(lv_event_t* event);
    static void onDeviceValueChanged(lv_event_t* event);
    static void onRoomSwitch(uint8_t direction);  // Left/right arrow
    
    // Internal state
    static std::vector<DeviceDescriptionWire> currentRoomDevices;
    static uint8_t currentRoomId;
    static bool isWaitingForActionResponse;
    static uint16_t lastSentActionSequence;
    
    // Timing
    static unsigned long lastDeviceListRefresh;
    static unsigned long lastScreenUpdate;
};
```

### 4.4 Thread Communication Channels

#### Channel 1: Device State Updates (DisplayClient → DisplayUIThread)
```c++
// Thread-safe queue in DisplayClient
static std::queue<DeviceStateUpdatePayload> deviceStateUpdates;
static Mutex deviceStateUpdateMutex;

// DisplayUIThread polls periodically:
{
    Mutex::Lock lock(displayStateUpdateMutex);
    while (!deviceStateUpdates.empty()) {
        auto update = deviceStateUpdates.front();
        deviceStateUpdates.pop();
        uiUpdateDeviceState(update);
    }
}
```

#### Channel 2: User Actions (DisplayUIThread → DisplayClient)
```c++
// DisplayUIThread calls (thread-safe through mutex):
DeviceActionRequestPayload action;
action.targetDeviceType = device.deviceType;
action.targetDeviceId = device.deviceId;
action.targetNodeMac = device.macAddress;
action.actionType = ACTION_TOGGLE;

DisplayClient::requestDeviceAction(action);
// Blocks briefly while DisplayClient dispatches UDP request

// DisplayClient updates device state when response arrives
displayStateUpdates.push(response);  // Next UI update cycle will refresh
```

#### Channel 3: Device List Sync (DisplayClient → DisplayUIThread)
```c++
// On connection/handshake, DisplayUIThread polls:
const auto& devices = DisplayClient::getDeviceList();  // Blocks until ready

// If device list changes (hash mismatch), UI thread queries again:
std::optional<auto> device = DisplayClient::getDeviceState(mac, id);
```

---

## 5. Initialization & Handshake Flow

### 5.1 Platform Initialization (DisplayClient Thread)

```
Time    Action
────────────────────────────────────────────────────────────
T0      Platform boot:
        ├─ NetworkDriver init (WiFi mode: try to join HCS, fallback to AP)
        ├─ ConfigProvider init (load MAC, WiFi creds, display settings)
        ├─ OperatingSystem init (scheduler, tick handler)
        ├─ DisplayClient init
        │  ├─ Set state = DISCOVERY
        │  ├─ Register UDP reception callback (port 9001)
        │  ├─ Queue first DISCOVER_DISPLAYS broadcast
        │  └─ Set lastDiscoveryTime = now()
        └─ (No DeviceManager or RemoteDevicesManager)

T1      Application thread stack runs
        ├─ DisplayClient::cyclic() periodically runs
        │  ├─ Check if DISCOVER_DISPLAYS timeout → resend
        │  ├─ Process receptionUdpQueue
        │  └─ State machine transitions (DISCOVERY → WAITING_HANDSHAKE, etc.)
        └─ Continue until handshake complete

T2      DisplayClient discovers Master:
        ├─ DisplayClient sends DISPLAY_PRESENT (with MAC, screen size, capabilities)
        ├─ Server receives → adds to knownDisplays list
        └─ State → WAITING_HANDSHAKE
```

### 5.2 Handshake & Device List Sync Flow

```
Sequence:

1. DisplayClient sends DISPLAY_PRESENT
   ├─ Payload: firmware version, screen dimensions, max devices
   └─ State machine → WAITING_HANDSHAKE

2. DisplayServer receives DISPLAY_PRESENT
   ├─ Extracts DisplayInfo
   ├─ Adds to knownDisplays vector
   ├─ Sends DISPLAY_HANDSHAKE
   │  ├─ assignedDisplayId (0-19)
   │  ├─ numberOfDevices (total count)
   │  └─ deviceListHash (CRC16 of all device data)
   └─ State on server: tracking as ACTIVE

3. DisplayClient receives DISPLAY_HANDSHAKE
   ├─ Stores assignedDisplayId
   ├─ Knows to expect numberOfDevices
   ├─ Enters state → SYNCING_DEVICES
   └─ Sends DEVICE_LIST_REQUEST

4. DisplayServer processes DEVICE_LIST_REQUEST
   ├─ Calls DeviceProvider::getAllDeviceDescriptions()
   ├─ Splits into chunks (max 3-5 devices per packet to fit UDP)
   ├─ Sends DEVICE_LIST_CHUNK packets sequentially
   │  ├─ chunkIndex = 0, totalChunks = N
   │  ├─ Contains DeviceDescriptionWire array
   │  └─ Device data pulled from HTTP server's existing descriptionVector
   ├─ After all chunks, sends DEVICE_LIST_END
   └─ Begins KEEP_ALIVE cycle

5. DisplayClient receives DEVICE_LIST_CHUNKs
   ├─ Accumulates devices in knownDevices vector
   ├─ Validates chunkIndex and totalChunks
   ├─ On DEVICE_LIST_END:
   │  ├─ Computes hash of received data
   │  ├─ Compares with handshake hash → if mismatch, reissues request
   │  └─ Signals deviceListReadyEvent
   └─ State → CONNECTED

6. DisplayClient enters KEEP_ALIVE cycle
   ├─ Sends KEEP_ALIVE every 20s
   ├─ Waits for KEEP_ALIVE_ACK
   ├─ Timeout after 3 seconds → retry
   └─ 3 retries → state → DISCONNECTED, restart discovery
```

---

## 6. Device State Update Flow

### 6.1 Proactive Update (External Action)

User on HTTP webUI toggles device → Master's DeviceProvider updates local device.

```
Sequence:

1. HTTP Server (on Master) processes toggle action:
   ├─ Calls DataContainer::setDeviceEnabled(...)
   ├─ DeviceProvider::applyServiceCall(...)
   └─ Device state changes locally

2. DisplayServer is notified via DataContainer callback:
   ├─ Listens to device state change signals
   ├─ Packs DeviceStateUpdatePayload
   └─ Broadcasts to ALL connected displays:
      for each display in knownDisplays:
         if display.isConnected():
             NetworkDriver::send(DEVICE_STATE_UPDATE, display.ipAddress)

3. Each DisplayClient receives DEVICE_STATE_UPDATE:
   ├─ Extracts DeviceStateUpdatePayload
   ├─ Updates internal deviceStateMap
   ├─ Queues update in deviceStateUpdates queue
   └─ DisplayUIThread polls queue on next refresh cycle → renders new state
```

### 6.2 Reactive Update (Device Action from Display)

User on Display presses button → display requests action → master executes → display updates.

```
Sequence:

1. DisplayUIThread detects user button press (LVGL callback)
   ├─ Creates DeviceActionRequestPayload
   ├─ Calls DisplayClient::requestDeviceAction(payload)
   └─ Waits (non-blocking via event) for response

2. DisplayClient sends DEVICE_ACTION_REQUEST to Master:
   ├─ Message includes device identifiers, action type, parameters
   ├─ Increments sequenceNumber for tracking
   └─ Stores in pendingActions queue

3. DisplayServer receives DEVICE_ACTION_REQUEST:
   ├─ Extracts target device identifiers
   ├─ Routes via DeviceProvider (same as HTTP server):
   │  ├─ Looks up device by MAC + ID
   │  ├─ Calls service (toggle, set value, etc.)
   │  └─ Collects response
   ├─ Packs DEVICE_ACTION_RESPONSE
   ├─ Sends back to requesting display
   └─ ALSO broadcasts DEVICE_STATE_UPDATE to all displays

4. DisplayClient receives DEVICE_ACTION_RESPONSE:
   ├─ Matches sequenceNumber to track request
   ├─ Updates deviceStateMap with new state
   ├─ Queues DEVICE_STATE_UPDATE for UI thread
   └─ Signals completion (if UI thread was waiting)

5. DisplayUIThread wakes from wait:
   ├─ Receives updated state from queue
   ├─ Re-renders affected widget
   └─ Cycle completes
```

---

## 7. User Interaction Flow (Detailed)

### 7.1 Room Navigation

```
User presses LEFT arrow on display:

1. LVGL detects touch → callback onRoomSwitch(LEFT)

2. DisplayUIThread::onRoomSwitch(LEFT):
   ├─ currentRoomId = previousRoomId
   ├─ Queries DisplayClient::getDeviceList()
   ├─ Filters devices by currentRoomId
   ├─ Calls HomeControlApp::reloadDevices(filteredList)
   ├─ LVGL re-renders widgets for new room
   └─ No network interaction needed
```

### 7.2 Device Toggle (OnOff Device)

```
User presses ON/OFF button for lamp in Living Room:

1. LVGL detects button → callback onDeviceTogglePressed()
   ├─ Device data from currentRoomDevices[i]
   ├─ Creates action:
   │  action.targetDeviceType = 43 (OnOff)
   │  action.targetNodeMac = 0x112233445566 (device's node)
   │  action.targetDeviceId = 5
   │  action.actionType = ACTION_TOGGLE
   └─ Calls DisplayClient::requestDeviceAction(action)

2. DisplayClient (platform thread):
   ├─ Queues action in pendingActions
   ├─ On next cyclic:
   │  ├─ Creates MessageUDP with DEVICE_ACTION_REQUEST
   │  ├─ Sends via NetworkDriver to Master's IP (port 9001)
   │  └─ Waits for response (stores in actionResponses queue)
   └─ DisplayUIThread may show "Sending..." briefly

3. DisplayServer (Master):
   ├─ Receives DEVICE_ACTION_REQUEST
   ├─ Extracts device identifiers
   ├─ Calls DeviceProvider::applyServiceCall(...)
   │  ├─ If on Master: direct call to DeviceManager
   │  ├─ If on Slave: routes via RemoteDevicesManager → RemoteControlClient
   │  └─ Waits for result
   ├─ Updates device in HTTP server's descriptionVector
   ├─ Packs DEVICE_ACTION_RESPONSE
   ├─ Sends back to requesting display
   └─ Broadcasts DEVICE_STATE_UPDATE to all displays

4. DisplayClient receives DEVICE_ACTION_RESPONSE:
   ├─ Validates sequenceNumber matches
   ├─ On success:
   │  ├─ Updates deviceStateMap
   │  ├─ Queues state update for UI thread
   │  └─ Signals event
   └─ On failure: queues error notification

5. DisplayUIThread (UI thread):
   ├─ Polls deviceStateUpdates queue
   ├─ Receives update with new state
   ├─ Calls HomeControlApp::onDeviceStateChanged(...)
   ├─ LVGL widget updates (button state, LED color, etc.)
   └─ User sees immediate feedback (toggle reflected on display)
```

### 7.3 Device Control with Value (LED Brightness)

```
User drags brightness slider for LED strip:

1. LVGL slider reports continuous events
   ├─ onDeviceValueChanged() called with new value (0-255)
   └─ Creates action:
      action.actionType = ACTION_SET_VALUE
      action.actionParameter = newBrightness

2. DisplayClient processes similarly to toggle
   ├─ Sends DEVICE_ACTION_REQUEST to Master
   ├─ Master routes via DeviceProvider
   ├─ LED strip service invoked with brightness parameter
   └─ Response sent back

3. State update flows back to UI
   ├─ New brightness reflected in LED state bytes
   ├─ UI renders updated color/brightness
   └─ Display shows visual confirmation
```

---

## 8. Keep-Alive & Connection Management

### 8.1 Keep-Alive Cycle

```
DisplayClient cyclic() loop:

Every 20 seconds:
├─ If state == CONNECTED:
│  ├─ Create KeepAlivePayload
│  │  ├─ timestamp = millis()
│  │  └─ displayId = assignedDisplayId
│  ├─ Send KEEP_ALIVE to Master
│  └─ Set lastKeepAliveTime = now()
│
├─ Every 3 seconds:
│  └─ Check: (now() - lastKeepAliveTime) > 3000?
│     ├─ If yes: haven't received ACK → retryCount++
│     ├─ If retryCount >= 3: state → DISCONNECTED
│     └─ If retryCount < 3: resend KEEP_ALIVE

Master cyclic() loop (DisplayServer):

┌──────────────────────────────────────────────┐
│ For each display in knownDisplays:           │
│  ├─ Check (now() - lastKeepAliveReceived)   │
│  ├─ If > 45 seconds:                        │
│  │  ├─ Mark as STALE                        │
│  │  └─ Stop sending state updates           │
│  ├─ If > 60 seconds:                        │
│  │  ├─ Remove from knownDisplays            │
│  │  └─ Log disconnection                    │
│  ├─ On receiving KEEP_ALIVE from display:   │
│  │  ├─ Update lastKeepAliveReceived         │
│  │  └─ Send KEEP_ALIVE_ACK                  │
│  └─ Display marked CONNECTED                │
└──────────────────────────────────────────────┘
```

### 8.2 Reconnection Scenario

```
Network loses connection → DisplayClient loses WiFi:

1. DisplayClient::cyclic() detects no UDP responses
   ├─ Keep-alive timeout threshold exceeded
   ├─ State → DISCONNECTED
   └─ Clears knownDevices (optional, or keeps for cache)

2. WiFi reconnects:
   ├─ NetworkDriver::onWiFiReconnect() callback
   ├─ DisplayClient detects WiFi up
   ├─ State → DISCOVERY (restart handshake)
   └─ Sends DISCOVER_DISPLAYS broadcasts

3. Master receives DISCOVER_DISPLAYS from previously-known display:
   ├─ Recognizes MAC address
   ├─ Sends DISPLAY_HANDSHAKE with same assignedDisplayId
   ├─ Sends DEVICE_LIST (may have changed during disconnect)
   └─ Display re-syncs state

4. Display re-enters CONNECTED state
   └─ Continues normal operation
```

---

## 9. Display Server Implementation (Master)

### 9.1 DisplayServer Core Responsibilities

```c++
class DisplayServer
{
private:
    // Known display terminals
    static std::vector<Display> knownDisplays;  // Max 20
    
    // State tracking for each display
    struct DisplayConnectionState {
        uint8_t displayId;
        MessageUDP::IPAddr ipAddress;
        uint64_t macAddress;
        unsigned long lastKeepAliveReceived;
        unsigned long lastStateUpdateSent;
        bool isConnected;
        uint16_t lastKnownDeviceListHash;
        std::queue<MessageUDP> outgoingQueue;
    };
    
    static std::map<uint8_t, DisplayConnectionState> displayStates;
    
    // Callbacks hooked into device layer
    static std::function<void(const DeviceDescription&)> onDeviceStateChanged;
    
public:
    static void init()
    {
        // Register with DataContainer for state-change signals
        DataContainer::registerStateChangeCallback(onDeviceStateChanged);
        
        // Start broadcasting DISCOVER_DISPLAYS
        scheduleDiscoveryBroadcast();
    }
    
    static void cyclic()
    {
        // 1. Discover new displays
        broadcastDiscoveryIfNeeded();
        
        // 2. Process incoming UDP messages
        while (!receptionUdpQueue.empty()) {
            auto msg = receptionUdpQueue.front();
            receptionUdpQueue.pop();
            
            switch ((DisplayMessageIds)msg.getId()) {
                case DISPLAY_PRESENT:
                    handleDisplayPresent(msg);
                    break;
                case KEEP_ALIVE:
                    handleKeepAlive(msg);
                    break;
                case DEVICE_ACTION_REQUEST:
                    handleDeviceActionRequest(msg);
                    break;
                case DISPLAY_HASH_SYNC:
                    handleHashSync(msg);
                    break;
            }
        }
        
        // 3. Send keep-alives to connected displays
        sendKeepAlivesToConnected();
        
        // 4. Broadcast state updates to displays
        while (!pendingStateUpdates.empty()) {
            auto update = pendingStateUpdates.front();
            pendingStateUpdates.pop();
            broadcastStateUpdate(update);
        }
        
        // 5. Clean up stale displays
        pruneDisconnectedDisplays();
    }

    static void onDeviceStateChanged(const DeviceDescription& device)
    {
        // Called by DeviceProvider when any device state changes
        DeviceStateUpdatePayload update;
        update.sourceDeviceType = device.deviceType;
        update.sourceNodeMac = device.macAddress;
        update.sourceDeviceId = device.deviceId;
        memcpy(update.newStateCustomBytes, device.customBytes, 50);
        
        // Queue for broadcast to all displays
        pendingStateUpdates.push(update);
    }
    
private:
    static std::queue<DeviceStateUpdatePayload> pendingStateUpdates;
    
    static void handleDisplayPresent(MessageUDP& msg)
    {
        // Extract DisplayInfo from payload
        // Check if already known (by MAC)
        // If new: assign displayId, send DISPLAY_HANDSHAKE + device list
        // If known: re-handshake (display rebooted or reconnected)
    }
    
    static void handleDeviceActionRequest(MessageUDP& msg)
    {
        DeviceActionRequestPayload request;
        // Unpack from msg.getPayload()
        
        // Route through DeviceProvider (same as HTTP server)
        DeviceDescription targetDevice;
        if (DeviceProvider::lookupDevice(request.targetNodeMac, 
                                          request.targetDeviceId, 
                                          targetDevice)) {
            auto result = DeviceProvider::applyServiceCall(
                request.actionType, 
                request.actionParameter, 
                targetDevice
            );
            
            // Pack DEVICE_ACTION_RESPONSE
            DeviceActionResponsePayload response;
            response.sequenceNumber = request.sequenceNumber;
            response.status = result.success ? 0 : 1;
            memcpy(response.newStateCustomBytes, 
                   targetDevice.customBytes, 50);
            
            // Send back to requesting display
            MessageUDP responseMsg(DEVICE_ACTION_RESPONSE, 
                                    displayStates[request.sourceDisplayId].ipAddress);
            // ... pack and send
        }
    }
    
    static void handleKeepAlive(MessageUDP& msg)
    {
        // Update lastKeepAliveReceived for that display
        // Send KEEP_ALIVE_ACK back
    }
    
    static void broadcastStateUpdate(const DeviceStateUpdatePayload& update)
    {
        // Send to all connected displays
        for (auto& [displayId, state] : displayStates) {
            if (state.isConnected && 
                millis() - state.lastStateUpdateSent > 100) { // Rate limit
                MessageUDP msg(DEVICE_STATE_UPDATE, state.ipAddress);
                // Pack payload and send
                state.lastStateUpdateSent = millis();
            }
        }
    }
    
    static void pruneDisconnectedDisplays()
    {
        unsigned long now = millis();
        auto it = displayStates.begin();
        while (it != displayStates.end()) {
            if (now - it->second.lastKeepAliveReceived > 60000) {
                // 60 seconds timeout
                Logger::log("Display " + String(it->first) + " disconnected");
                it = displayStates.erase(it);
            } else {
                ++it;
            }
        }
    }
};
```

### 9.2 Integration with HTTP Server

DisplayServer mirrors the HTTP server's device retrieval:

```c++
// Both HTTP and Display use same source
std::vector<DeviceDescription> descriptions = DeviceProvider::getAllDeviceDescriptions();

// For HTTP: render as HTML/JSON
// For Display: pack as DeviceDescriptionWire packets and send via UDP
```

---

## 10. Display Device NVM Layout

The display uses the standard HCS `ConfigProvider`, `PersistentMemoryAccess`, and NVM block layout. It does not introduce a display-only preferences namespace or a parallel persistence implementation. Display-specific settings may be added through normal HCS configuration blocks when required.

```
NVM Structure (4MB partition):

Offset    Size    Purpose
────────────────────────────────────────
0x0000    4KB     NVM Metadata
          - Magic: 0xCAFEBABE
          - Version: 2
          - Checksum
          
0x1000    8KB     WiFi Configuration
          - SSID (32 bytes)
          - Password (64 bytes)
          - IP Mode (DHCP/Static)
          - Static IP config (if applicable)
          
0x3000    4KB     Display Configuration
          - Display name
          - Screen orientation
          - Brightness settings
          - Custom UI settings
          
0x4000    2KB     Device Cache (optional)
          - Last known device list hash
          - Last sync timestamp
          - Compression: TBD
          
0x4800    4KB     Network Configuration
          - Automatic broadcast discovery state
          - UDP port (9001)
          - Keep-alive interval
          
0x5800    2KB     OTA & Update
          - Firmware version
          - Update URL
          - Last update check
          
0x6000    Remaining  Free space
          - Expansion for future use
          - NVM wear leveling
```

### 10.1 NVM Access Patterns

```c++
// Display's ConfigProvider works with new slots:
class ConfigProvider
{
    enum NVMSlot {
        METADATA = 0,
        WIFI_CONFIG = 1,
        DISPLAY_CONFIG = 2,
        DEVICE_CACHE = 3,
        NETWORK_CONFIG = 4,
        OTA_INFO = 5,
        // ... (Device slots 6-11 are unused on display)
    };
};

// Initialize on first boot
if (!ConfigProvider::isInitialized()) {
    ConfigProvider::initializeSlots(METADATA, WIFI_CONFIG, DISPLAY_CONFIG, 
                                    NETWORK_CONFIG, OTA_INFO);
}

// Access WiFi settings (same pattern as Master)
auto wifiConfig = ConfigProvider::readNVM(WIFI_CONFIG);

// Access display-specific settings
auto displayConfig = ConfigProvider::readNVM(DISPLAY_CONFIG);

// Cache device list hash for validation
ConfigProvider::writeNVM(DEVICE_CACHE, hash, timestamp);
```

---

## 11. State Management & Thread Safety

### 11.1 Critical Sections

```c++
// Display Client must protect:
// 1. knownDevices vector (read by UI thread, written by platform thread)
// 2. deviceStateMap (read/write by both threads)
// 3. actionQueues (read/write by both threads)

// Protection mechanisms:
// - Mutex for device lists
// - Event flags for synchronization
// - Atomic variables for simple counters

class DisplayClient
{
    static std::mutex deviceListMutex;
    static std::condition_variable deviceListReady;
    static std::atomic<uint16_t> lastDeviceListHash;
    
public:
    static const std::vector<DeviceDescriptionWire>& getDeviceList()
    {
        // Acquire lock
        std::unique_lock<std::mutex> lock(deviceListMutex);
        
        // Wait if list not ready
        while (!isDeviceListReady) {
            deviceListReady.wait(lock);
        }
        
        // Return (still locked)
        return knownDevices;
    }
};
```

### 11.2 Deadlock Prevention

```
Rule: Always acquire locks in same order:
  1. deviceListMutex (if both needed)
  2. actionMutex

Rule: Never call blocking operations while holding a lock:
  ✗ WRONG: Lock, then send UDP (blocking)
  ✓ RIGHT: Copy data under lock, release lock, then send UDP
```

---

## 12. Error Handling & Resilience

### 12.1 Display Failure Modes

| Scenario | Handling |
|----------|----------|
| WiFi disconnects | DisplayClient stays in DISCONNECTED state, broadcasts recovery attempts, retries discovery |
| Master reboots | DisplayClient detects keep-alive timeout, returns to DISCOVERY, re-handshakes |
| Device list changes on master | DisplayServer broadcasts DEVICE_LIST_CHUNK with new data, display updates cache |
| Action times out | DisplayClient queues error notification, UI displays "Failed" |
| UDP packet loss | Retry logic built into keep-alive and action requests |
| Master network unreachable | Display functions as terminal-only, UI shows "Offline" |
| Memory exhaustion on display | Graceful degradation: limit device count, drop old state updates |

### 12.2 Validation & Integrity

```c++
// Message validation
bool validateDisplayPayload(const MessageUDP& msg, DisplayMessageIds expected)
{
    if (msg.getId() != (uint8_t)expected) return false;
    if (msg.getPayload().size() < MIN_PAYLOAD_SIZE) return false;
    
    // Sequence number ordering for reliability
    static std::map<uint8_t, uint16_t> lastSeqNumbers;
    uint16_t seq = *(uint16_t*)msg.getPayload().data();
    
    if (seq <= lastSeqNumbers[msg.getSourceIP()]) {
        // Duplicate or out-of-order → drop
        return false;
    }
    
    lastSeqNumbers[msg.getSourceIP()] = seq;
    return true;
}

// Device list integrity
uint16_t computeDeviceListHash()
{
    // CRC16 over all device data
    uint16_t crc = 0;
    for (const auto& device : knownDevices) {
        crc = crc16(crc, (uint8_t*)&device, sizeof(DeviceDescriptionWire));
    }
    return crc;
}
```

---

## 13. Configuration & Access Point

### 13.1 Access Point Mode

When display starts:

```c++
// In NetworkDriver::init() for display:

if (ConfigProvider::isWiFiConfigured()) {
    // Try to connect to known network
    WiFi.mode(WIFI_STA);
    WiFi.connect(ssid, password);
    
    // With timeout: if can't connect in 30s, fall through to AP
    unsigned long apFallbackTime = millis() + 30000;
    while (millis() < apFallbackTime && WiFi.status() != WL_CONNECTED) {
        delay(100);
    }
}

if (WiFi.status() != WL_CONNECTED) {
    // Start Access Point for configuration
    uint64_t mac = WiFi.macAddress();
    String apName = "HCS_Display-" + String(mac, HEX);  // e.g., HCS_Display-AABBCCDD
    
    WiFi.mode(WIFI_AP);
    WiFi.softAP(apName);
    // mDNS is enabled only in AP mode: hcs-display-XXXXXX.local
    
    // HTTP configuration server on 192.168.4.1
    startConfigurationServer();
}
```

### 13.2 Configuration Server Routes

```
GET /config
    └─ Returns current configuration as JSON
    
POST /config/wifi
    ├─ Body: { "ssid": "...", "password": "..." }
    ├─ Saves to NVM
    └─ Restarts display

GET /config/status
    └─ Returns: WiFi status, Display ID, Device count, Connection state

POST /config/restart
    └─ Reboots device

POST /config/reset
    └─ Clears display settings and reboots device

// The display never stores a master IP. It answers the master's broadcast
// DISCOVER_DISPLAYS message and learns the master endpoint from the packet source.
```

---

## 14. Development & Testing Strategy

### 14.1 Protocol Testing

1. **Unit Tests** (Desktop/Pytest):
   - Message packing/unpacking
   - State machine transitions
   - Hash computation
   - Thread-safe queue behavior

2. **Integration Tests** (Emulator):
   - Master ↔ Display message flows
   - Device state updates
   - Action dispatches and responses

3. **Hardware Tests**:
   - Real ESP32 display + master
   - Network failure scenarios
   - Large device counts (20+ devices)

### 14.2 Simulation & Debug Mode

```c++
// For desktop development:
class DisplayServerSimulator
{
    // In-memory master state
    // Fake device list
    // Simulated user actions
    // Protocol message capture/replay
};

// Build with TEST_MODE=1 to enable:
// - Verbose logging of all messages
// - Display protocol dumping
// - Memory statistics
```

---

## 15. Future Enhancements

1. **Partial Device Sync**: Display requests only devices from specific rooms
2. **Delta Updates**: Instead of full state, send only changed fields
3. **Compression**: GZIP device list for slow networks
4. **Display Clustering**: Multiple displays coordinated without master (P2P fallback)
5. **Telemetry**: Display reports touch statistics, error rates
6. **OTA Updates**: Display receives firmware updates from master
7. **Advanced Scheduling**: Display-initiated scheduled actions (store locally, trigger locally)
8. **Gesture Recognition**: Swipe patterns for quick room navigation
9. **Voice Control**: Audio input on display routed to master
10. **Multi-Master**: Display can fail over to secondary master

---

## 16. Summary of Key Design Decisions

| Decision | Rationale |
|----------|-----------|
| **UDP-based, not TCP** | Lightweight, broadcast-friendly, fits ESP32 UDP stack already in use |
| **Stateless displays** | No DeviceManager simplifies hardware requirements, reduces NVM usage |
| **Chunked device list** | Avoids large UDP packets, handles slow networks gracefully |
| **Dual-thread UI** | Responsive display UI, non-blocking network I/O |
| **Keep-alive handshake** | Detects stale displays, prevents orphaned connections |
| **State hash validation** | Automatic recovery from packet loss or desync |
| **Access point mode** | Self-configuration, no need for pre-provisioning |
| **Mirror HTTP routing** | Consistency, no special-case device control logic |
| **Room-based UI caching** | Efficient rendering, supports up to 255 rooms |
| **Broadcast state updates** | All displays stay in sync without polling |

---

## 17. Display Layout Implementation Notes

### 17.1 LVGL Widget Mapping

The HomeControlApp (from Guition project) should map display protocol concepts to LVGL widgets:

```
Display Protocol              LVGL Implementation
─────────────────────────────────────────────────
Room Name                  → lv_label (centered, top area)
Device List                → lv_tileview (3×N grid) or lv_obj array
Device Widget              → lv_btn (custom) + lv_label (name)
                             Color based on state
Room Navigation (arrows)   → lv_btn (left/right)
Device Info Popup          → lv_msgbox or custom container
Loading Spinner            → lv_spinner
State Indicator            → lv_label or lv_bar (for brightness/progress)
```

### 17.2 Color Scheme & Styling

Based on actual display layout:

```
Enabled Device:         Color theme (blue, green, etc.) + 80% opacity
Disabled Device:        Gray (0x808080) + 50% opacity
Hovered Device:         Lighter shade + border glow
OFF State Button:       Orange (#FF6600) background, white text
Widget Text:            White (#FFFFFF)
Popup Background:       Dark overlay (0x000000 with 70% transparency)
Accent Colors:          Orange (#FF9900), cyan (#00CCFF)
```

### 17.3 Responsiveness & Performance

- **Touch latency**: < 200ms (LVGL input → DisplayClient message)
- **State update**: < 500ms (master responds → display re-renders)
- **Room switch**: Immediate (< 50ms, no network)
- **Screen refresh**: ~16ms (60 FPS target)
- **Memory**: ~600KB RAM for UI buffers (2× 480×480 = 460KB + overhead) + ~200KB for device list cache
- **Display update**: Full screen refresh via SPI (typically 30-50ms per frame)

### 17.4 Device Widget State Indicators

Based on observed display behavior:

**OnOff Devices**:
- ON: Colored background (green, blue, etc.) + white text
- OFF: Gray background + dim text
- Long press: Shows device info popup with toggle option

**LED Strip Devices**:
- State: Colored background reflecting LED color
- Indicator: Horizontal progress bar for brightness level (0-100%)
- Shows color swatch in custom bytes

**Temperature Sensors**:
- State: Green background (read-only)
- Display: Temperature (e.g., "23.5°C")
- Humidity: Secondary text (e.g., "45%")

**Window/Door Sensors**:
- OPEN: Red/alert color
- CLOSED: Green/safe color
- Icon: Can use text symbol or bitmap

---

## 18. Appendix A: Protocol Message Reference

### Message Size Estimates

```
DISPLAY_PRESENT:              ~16 bytes
DISPLAY_HANDSHAKE:            ~20 bytes
DEVICE_LIST_CHUNK:            ~400 bytes (3 devices × ~98 bytes + header)
DEVICE_STATE_UPDATE:          ~65 bytes
DEVICE_ACTION_REQUEST:        ~30 bytes
DEVICE_ACTION_RESPONSE:       ~60 bytes
KEEP_ALIVE:                   ~16 bytes

Typical handshake overhead:     ~100 bytes
Typical device update cycle:    ~65 bytes × N displays
Typical action-response cycle:  ~100 bytes
```

### Sequence Number Strategy

- Each display gets unique sequence counter (0-65535, wraps)
- Master validates ordering: each new seq > previous seq
- Allows detection of duplicate or out-of-order packets
- Retries use same sequence number (idempotent)

### Payload Packing Strategy

- All structures use `#pragma pack(1)` (no padding)
- Endianness: All multi-byte integers in network byte order (big-endian)
- Strings: Null-terminated, fixed-length buffers
- Custom bytes: Opaque binary, device-type-specific interpretation

---

End of Document
