# Display Terminal System - Software Requirements Specification (SRS)

**Document Version**: 1.0  
**Date**: 2026-09-07  
**Status**: Design Phase  
**Author**: Architectural Analysis  

---

## 1. Introduction

### 1.1 Purpose

This Software Requirements Specification (SRS) defines all functional and non-functional requirements for the Display Terminal System that extends HomeControlStation with a multi-display UI capability. Display terminals function as sophisticated network clients that provide a second user interface to control HomeControlStation devices, complementing the HTTP webUI.

### 1.2 Scope

- **In Scope**: DisplayServer (Master), DisplayClient (Display device), Display Protocol, UI rendering, device synchronization, user interactions, and full HCS platform integration
- **Out of Scope**: RemoteControl protocol redesign and display ownership of physical HCS end devices
- **Constraints**: Max 20 simultaneous displays, 255 rooms per system, 480×480 display resolution, and independently buildable master and Guition repositories

### 1.3 Document Organization

- **Section 2**: Overall System Requirements
- **Section 3**: Functional Requirements by Component
- **Section 4**: Non-Functional Requirements
- **Section 5**: Protocol Specifications
- **Section 6**: Interface Requirements
- **Section 7**: System Constraints

---

## 2. Overall System Requirements

### 2.1 System Context

```
┌─────────────────┐           UDP Port 9001              ┌────────────────┐
│   HTTP Server   │ ◄─────────────────────────────────► │  DisplayServer │
│   (Dashboard)   │    (existing connection)             │  (Master only) │
└─────────────────┘                                      └────────────────┘
       ▲                                                         ▲
       │                                                         │
       │ User interacts with                   Display Protocol  │
       │ devices via web                       Messages (UDP)    │ Routes requests
       │                                                         │ to DeviceProvider
       │                                                         │
       ▼                                                         ▼
    ┌──────┐        ┌──────┐        ┌──────┐
    │Display│◄──────│Display│◄──────│Display│
    │ Term1 │  UDP  │ Term2 │  UDP  │ Term3 │
    │  Src: │       │  Src: │       │  Src: │
    │  #1   │       │  #2   │       │  #3   │
    └──────┘        └──────┘        └──────┘
    (Slave)         (Slave)         (Slave)
  ESP32+4"Disp    ESP32+4"Disp    ESP32+4"Disp
```

### 2.2 Key Goals

1. **Parity with HTTP WebUI**: Display terminal provides same device control as HTTP dashboard
2. **Distributed Interaction**: Multiple displays can independently or coordinately control devices
3. **Real-Time Synchronization**: State changes on one display reflect immediately on all others
4. **Network Resilience**: Displays gracefully degrade if master/network unavailable
5. **Scalability**: Support up to 20 displays without master performance degradation
6. **Simplicity**: No special cases; display requests flow through same DeviceProvider as HTTP
7. **Repository Independence**: Guition contains physical copies of HCS platform files and has no source link, symlink, or build dependency on the master checkout
8. **Platform Consistency**: Guition uses the standard HCS scheduler, NVM, configuration, WiFi, UDP, HTTP, OTA, diagnostics, and application abstractions; only display-role composition and hardware/UI differ

---

## 3. Functional Requirements

### 3.1 DisplayServer Requirements (Master Device)

#### 3.1.1 Display Discovery & Registration

| REQ-ID | Requirement | Acceptance Criteria |
|--------|-------------|-------------------|
| DS-001 | Broadcast DISCOVER_DISPLAYS every 15 seconds | Server sends discovery message on UDP 9001 broadcast address |
| DS-002 | Accept DISPLAY_PRESENT messages from displays | Server receives and validates display announcement within 1 second |
| DS-003 | Assign unique ID (0-19) to each display | New displays assigned incrementing IDs; ID persists across reconnect |
| DS-004 | Send DISPLAY_HANDSHAKE with metadata | Handshake includes device count, list hash, platform version |
| DS-005 | Track max 20 simultaneous displays | Server rejects connections > 20; oldest/stale displays pruned first |
| DS-006 | Maintain display state table | Server stores MAC, IP, display ID, last keep-alive time, connection status |

#### 3.1.2 Device List Distribution

| REQ-ID | Requirement | Acceptance Criteria |
|--------|-------------|-------------------|
| DS-010 | Send complete device list on request | DEVICE_LIST_CHUNK packets sent sequentially until all devices delivered |
| DS-011 | Chunk device list for UDP compatibility | Max 5 devices per DEVICE_LIST_CHUNK (fits within ~1400 byte UDP limit) |
| DS-012 | Compute device list hash | CRC16 hash computed over all device descriptions for integrity validation |
| DS-013 | Send DEVICE_LIST_END marker | Displays know when device list complete |
| DS-014 | Re-sync on hash mismatch | If display hash ≠ server hash, server re-sends full list |
| DS-015 | Include room names with devices | Room ID and name translation sent during handshake or in device list |

#### 3.1.3 Device State Management & Broadcasts

| REQ-ID | Requirement | Acceptance Criteria |
|--------|-------------|-------------------|
| DS-020 | Listen to device state changes | Server registers callback with DataContainer for all device state updates |
| DS-021 | Broadcast DEVICE_STATE_UPDATE to all displays | State change triggers update sent to all connected displays within 100ms |
| DS-022 | Include device identifiers in update | Update contains node MAC, device ID, device type, new state bytes |
| DS-023 | Include state change reason | Update includes reason: user action, auto-update, external change |
| DS-024 | Rate-limit state updates | No more than 10 updates/second per display to prevent UDP flood |

#### 3.1.4 Device Action Request Handling

| REQ-ID | Requirement | Acceptance Criteria |
|--------|-------------|-------------------|
| DS-030 | Receive DEVICE_ACTION_REQUEST from display | Server parses action containing device ID, node MAC, action type, parameter |
| DS-031 | Route via DeviceProvider (same as HTTP) | Use existing DeviceProvider::applyServiceCall() method |
| DS-032 | Support toggle, set_value, and service_call actions | Display can toggle, set numeric value, or invoke custom service |
| DS-033 | Handle local device actions | If device on Master, call DeviceManager directly |
| DS-034 | Handle remote device actions | If device on Slave, route via RemoteDevicesManager/RemoteControlClient |
| DS-035 | Send DEVICE_ACTION_RESPONSE with result | Response includes sequence number, status (success/failure), updated device state |
| DS-036 | Broadcast updated state to all displays | After action succeeds, DEVICE_STATE_UPDATE sent to all displays |

#### 3.1.5 Keep-Alive & Disconnection Management

| REQ-ID | Requirement | Acceptance Criteria |
|--------|-------------|-------------------|
| DS-040 | Process KEEP_ALIVE messages from displays | Server updates last-keep-alive timestamp on receipt |
| DS-041 | Mark displays as stale after 45 seconds | Display not responding to keep-alive marked as STALE; state updates not sent |
| DS-042 | Remove disconnected displays after 60 seconds | Display not responding for 60+ seconds removed from knownDisplays |
| DS-043 | Send KEEP_ALIVE_ACK to displays | Server acknowledges receipt with matching sequence number |
| DS-044 | Log display disconnections | Each display disconnect event logged with timestamp |

#### 3.1.6 Error Handling

| REQ-ID | Requirement | Acceptance Criteria |
|--------|-------------|-------------------|
| DS-050 | Validate all incoming messages | Message ID, payload size, sequence number, IP address validated |
| DS-051 | Silently drop invalid messages | Malformed packets logged but not cause server crash/state corruption |
| DS-052 | Handle action failures gracefully | If action fails (e.g., device not found), send error response; don't broadcast |
| DS-053 | Recover from transient network issues | Lost UDP packets handled via keep-alive retry; no special recovery needed |

---

### 3.2 DisplayClient Requirements (Display Device)

#### 3.2.1 Initialization & Boot

| REQ-ID | Requirement | Acceptance Criteria |
|--------|-------------|-------------------|
| DC-001 | Initialize NetworkDriver (WiFi mode) | WiFi connection attempted; if fails, Access Point mode starts |
| DC-002 | Load NVM configuration | WiFi SSID, password, display settings loaded from NVM |
| DC-003 | Register UDP reception callback | DisplayClient registers with NetworkDriver for port 9001 |
| DC-004 | Start DisplayClient application | Entry point: DisplayClient::init() called after platform core ready |
| DC-005 | Begin discovery sequence | DISPLAY_PRESENT announcements are broadcast until a master responds |

#### 3.2.2 Discovery & Handshake

| REQ-ID | Requirement | Acceptance Criteria |
|--------|-------------|-------------------|
| DC-010 | Respond to DISCOVER_DISPLAYS | Display learns the master endpoint from the datagram source and replies with DISPLAY_PRESENT |
| DC-011 | Send DISPLAY_PRESENT announcement | While disconnected, display periodically announces its MAC, IP, and screen size |
| DC-012 | Receive DISPLAY_HANDSHAKE | Display accepts handshake containing display ID, device count, list hash |
| DC-013 | Transition to device list sync | After handshake, display enters SYNCING_DEVICES state |
| DC-014 | Request device list | Display sends DEVICE_LIST_REQUEST to Master |
| DC-015 | Receive device list chunks | Display accumulates DEVICE_LIST_CHUNK packets sequentially |
| DC-016 | Validate device list hash | After all chunks, computed hash compared with handshake hash |
| DC-017 | Enter connected state | Display transitions to CONNECTED after successful sync |
| DC-018 | Timeout and retry | If no handshake response in 5 seconds, return to DISCOVERY |

#### 3.2.3 Device Synchronization

| REQ-ID | Requirement | Acceptance Criteria |
|--------|-------------|-------------------|
| DC-020 | Store device descriptions locally | All received devices cached in knownDevices vector |
| DC-021 | Map devices by room | Device list queryable by room ID for UI filtering |
| DC-022 | Maintain device state map | Each device has current state (customBytes) cached and updateable |
| DC-023 | Process DEVICE_STATE_UPDATE | On update received, device state map updated; UI thread notified |
| DC-024 | Queue state updates for UI | State updates placed in thread-safe queue for DisplayUIThread |
| DC-025 | Re-sync on hash mismatch | If device list hash doesn't match, request full re-sync |

#### 3.2.4 User Action Processing

| REQ-ID | Requirement | Acceptance Criteria |
|--------|-------------|-------------------|
| DC-030 | Receive toggle request from UI | DisplayUIThread calls DisplayClient::requestDeviceAction() |
| DC-031 | Send DEVICE_ACTION_REQUEST | Request packed and sent to Master via UDP with sequence number |
| DC-032 | Wait for DEVICE_ACTION_RESPONSE | DisplayClient waits (non-blocking via event) for response up to 5 seconds |
| DC-033 | Handle action timeout | If no response in 5 seconds, display error; notify UI thread |
| DC-034 | Update local state on success | Device state updated from response; queued for UI thread |
| DC-035 | Support multiple simultaneous actions | At least 2 concurrent action requests queued (e.g., fast user clicks) |

#### 3.2.5 Keep-Alive & Connection Management

| REQ-ID | Requirement | Acceptance Criteria |
|--------|-------------|-------------------|
| DC-040 | Send KEEP_ALIVE every 20 seconds | While in CONNECTED state, keep-alive sent with incrementing sequence |
| DC-041 | Detect keep-alive timeout | If no KEEP_ALIVE_ACK for 3 seconds, retry up to 3 times |
| DC-042 | Transition to DISCONNECTED | After 3 failed keep-alive retries, enter DISCONNECTED state |
| DC-043 | Implement exponential backoff | Reconnection attempts: 5s, 10s, 20s, 40s, 60s delays |
| DC-044 | Notify UI of connection status | DisplayUIThread can query DisplayClient::getConnectionStatus() |
| DC-045 | Clear device cache on final disconnect | After max retries, knownDevices optionally cleared (or cached) |

#### 3.2.6 Error Handling & Resilience

| REQ-ID | Requirement | Acceptance Criteria |
|--------|-------------|-------------------|
| DC-050 | Validate received messages | Check message ID, sequence number, payload size before processing |
| DC-051 | Detect duplicate/out-of-order packets | Sequence number validation; duplicates silently dropped |
| DC-052 | Handle WiFi disconnection | NetworkDriver notifies DisplayClient; state → DISCONNECTED |
| DC-053 | Re-establish WiFi connection | If WiFi reconnects, DisplayClient re-enters DISCOVERY |
| DC-054 | Device list corruption detection | Hash mismatch triggers re-sync; max 3 retries before error |

---

### 3.3 DisplayUIThread Requirements (Display Device)

#### 3.3.1 Initialization & LVGL Setup

| REQ-ID | Requirement | Acceptance Criteria |
|--------|-------------|-------------------|
| UI-001 | Initialize LVGL library | lv_init() called; display buffer allocated (480×480 RGB) |
| UI-002 | Create display driver | lv_disp_drv registered for SPI interface to Guition display |
| UI-003 | Setup touch input device | lv_indev_drv registered for capacitive touch |
| UI-004 | Create display screens | Home screen, device info popup, and settings screens created |
| UI-005 | Load font resources | Montserrat 20pt and 32pt fonts loaded for rendering |
| UI-006 | Start at home screen | DisplayUIThread launches showing first room's devices |

#### 3.3.2 Device Display & Rendering

| REQ-ID | Requirement | Acceptance Criteria |
|--------|-------------|-------------------|
| UI-010 | Display current room name | Room name shown in status bar at top of screen |
| UI-011 | Display device grid | 3 columns × 3 rows of device widgets (max 9 visible) |
| UI-012 | Render device widgets | Each device widget displays name, state indicator, custom info |
| UI-013 | Show device states based on type | OnOff: ON/OFF text; LED: color + brightness bar; Temp: temperature + humidity |
| UI-014 | Update device colors | Device background color reflects enabled/disabled/active state |
| UI-015 | Support vertical scrolling | If room > 9 devices, scroll up/down with visual feedback |
| UI-016 | Display status bar | Time, location name, connection status shown at top |
| UI-017 | Display footer | HomeControlStation logo shown at bottom |

#### 3.3.3 User Interaction & Input

| REQ-ID | Requirement | Acceptance Criteria |
|--------|-------------|-------------------|
| UI-020 | Detect single tap on device | Tap opens device info popup showing current state |
| UI-021 | Detect long press on device | Long press (2+ seconds) shows advanced controls popup |
| UI-022 | Detect room navigation arrows | Left/right arrows detected and trigger room switch |
| UI-023 | Toggle device from popup | Button in popup sends toggle action to DisplayClient |
| UI-024 | Support brightness slider | LED strip widget shows brightness slider; dragging sends set_value actions |
| UI-025 | Support color picker | LED strip long press shows color picker (if available) |
| UI-026 | Provide visual feedback | Action buttons show "Sending..." during request; update on response |

#### 3.3.4 Room Navigation

| REQ-ID | Requirement | Acceptance Criteria |
|--------|-------------|-------------------|
| UI-030 | Filter devices by room | Only devices with current roomId displayed |
| UI-031 | Switch rooms with left arrow | Pressing left goes to previous room; wraps to last |
| UI-032 | Switch rooms with right arrow | Pressing right goes to next room; wraps to first |
| UI-033 | Show room count indicator | Optional: display "Room X of Y" at top |
| UI-034 | Update room name immediately | Room name updates instantly when switching (no network delay) |
| UI-035 | Cache device list locally | Room switches don't require network requests |

#### 3.3.5 State Update Processing

| REQ-ID | Requirement | Acceptance Criteria |
|--------|-------------|-------------------|
| UI-040 | Poll device state updates queue | DisplayUIThread polls queue every render cycle (16ms) |
| UI-041 | Update widget on state change | Device widget re-rendered when state received |
| UI-042 | Handle out-of-sync updates | If update for device not in currentRoomDevices, silently discard |
| UI-043 | Smooth UI transitions | State changes don't cause jarring visual shifts (use animations where appropriate) |
| UI-044 | Update custom bytes | Temperature, LED color, and other state data reflected immediately |

#### 3.3.6 Error & Offline Handling

| REQ-ID | Requirement | Acceptance Criteria |
|--------|-------------|-------------------|
| UI-050 | Show connection status | Visual indicator (icon, color) shows master connection status |
| UI-051 | Display offline message | If not connected, show "Waiting for connection..." message |
| UI-052 | Disable action buttons offline | Device widgets show but actions disabled with visual feedback |
| UI-053 | Show action errors | If action fails, popup displays error message (e.g., "Failed to toggle") |
| UI-054 | Handle long action timeouts | If action takes > 5 seconds, timeout and display error |

---

### 3.4 Display Protocol Requirements

#### 3.4.1 Message Structure & Format

| REQ-ID | Requirement | Acceptance Criteria |
|--------|-------------|-------------------|
| DP-001 | Use MessageUDP wrapper | All display messages wrapped with existing MessageUDP structure |
| DP-002 | Assign message IDs 200-249 | Display protocol messages use reserved ID range |
| DP-003 | Include sequence numbers | All payloads include uint16_t sequence number for ordering |
| DP-004 | Include timestamps | All payloads include uint32_t timestamp (milliseconds) |
| DP-005 | Use packed binary format | All structures use #pragma pack(1) for exact binary representation |
| DP-006 | Support max 1400 byte payloads | All messages fit within UDP MTU ~1400 bytes |

#### 3.4.2 Message Types

| REQ-ID | Requirement | Acceptance Criteria |
|--------|-------------|-------------------|
| DP-010 | Implement DISCOVER_DISPLAYS | Server broadcasts, displays respond |
| DP-011 | Implement DISPLAY_PRESENT | Displays announce MAC, screen size, firmware version |
| DP-012 | Implement DISPLAY_HANDSHAKE | Server acknowledges with display ID, device count, hash |
| DP-013 | Implement DEVICE_LIST_CHUNK | Device data sent in chunks with index/total tracking |
| DP-014 | Implement DEVICE_STATE_UPDATE | Broadcast updates when device state changes |
| DP-015 | Implement DEVICE_ACTION_REQUEST | Display requests action with sequence number for matching response |
| DP-016 | Implement DEVICE_ACTION_RESPONSE | Server responds with sequence number, status, updated state |
| DP-017 | Implement KEEP_ALIVE / KEEP_ALIVE_ACK | Bidirectional heartbeat for connection monitoring |

#### 3.4.3 Reliability & Validation

| REQ-ID | Requirement | Acceptance Criteria |
|--------|-------------|-------------------|
| DP-020 | Validate message IDs | Invalid IDs rejected; logging at DEBUG level |
| DP-021 | Validate payload sizes | Message size must fit declared size; rejected if mismatch |
| DP-022 | Validate IP addresses | Source IP checked to belong to known display/master |
| DP-023 | Handle out-of-order packets | Sequence numbers allow detection; older packets dropped |
| DP-024 | Handle duplicate packets | Sequence number tracking prevents duplicate processing |
| DP-025 | Compute message hashes | CRC16 for device list; validates data integrity |

---

### 3.5 Integration Requirements

#### 3.5.1 Integration with Existing Platform

| REQ-ID | Requirement | Acceptance Criteria |
|--------|-------------|-------------------|
| INT-001 | Hook into DataContainer | DisplayServer listens to device state change signals |
| INT-002 | Use DeviceProvider for routing | All device actions routed through existing DeviceProvider API |
| INT-003 | Support RemoteDevicesManager | Actions on slave devices properly routed via RemoteControl |
| INT-004 | Share HTTP device descriptions | DisplayServer uses same device list as HTTP server |
| INT-005 | Respect access control (future) | If HTTP has access control, display respects same rules |
| INT-006 | Coordinate with HTTP server | Both HTTP and Display read from same device state; no conflicts |

#### 3.5.2 NVM & Configuration

| REQ-ID | Requirement | Acceptance Criteria |
|--------|-------------|-------------------|
| INT-010 | Use standard HCS persistence | ConfigProvider and PersistentMemoryAccess retain ownership of all NVM operations |
| INT-011 | Store WiFi configuration | Standard HCS configuration stores SSID, password, and IP mode across reboot |
| INT-012 | Store display settings | Display settings use allocated standard HCS configuration blocks when introduced |
| INT-013 | Discover the master automatically | Display responds to DISCOVER_DISPLAYS and learns the master endpoint from the source address |
| INT-014 | Support factory reset | NVM clear command resets all display settings to defaults |

#### 3.5.3 Access Point Configuration

| REQ-ID | Requirement | Acceptance Criteria |
|--------|-------------|-------------------|
| INT-020 | Start AP if WiFi unavailable | After 30 seconds without WiFi, enter AP mode |
| INT-021 | Use standard HCS AP naming | Display uses the access-point identity generated by WiFiAdapter |
| INT-022 | Host standard HCS HTTP server | Shared configuration server is available at the AP address |
| INT-023 | Provide standard HCS WiFi configuration | Credentials are submitted through the shared HCS configuration flow |
| INT-024 | Provide reboot and factory reset controls | POST /config/reboot reboots; POST /config/reset clears settings and reboots |
| INT-025 | Trigger WiFi reconnect | After WiFi configuration, device reboots and attempts to connect |

---

## 4. Non-Functional Requirements

### 4.1 Performance

| REQ-ID | Requirement | Metric | Target |
|--------|-------------|--------|--------|
| PER-001 | Touch input latency | Time from user tap to network message sent | < 200ms |
| PER-002 | State update latency | Time from action response to UI re-render | < 500ms |
| PER-003 | Room switch latency | Time from arrow press to new room displayed | < 50ms |
| PER-004 | Screen refresh rate | Frames per second | ≥ 60 FPS (16ms per frame) |
| PER-005 | Display discovery time | Time to discover master from cold boot | < 30 seconds |
| PER-006 | Handshake time | Time from DISPLAY_PRESENT to CONNECTED state | < 10 seconds |
| PER-007 | Device list sync time | Time to download and validate device list | < 20 seconds |
| PER-008 | Keep-alive response time | Time to receive KEEP_ALIVE_ACK | < 1 second (typical) |
| PER-009 | Message processing latency | Time from UDP reception to state update | < 100ms |

### 4.2 Reliability & Availability

| REQ-ID | Requirement | Metric | Target |
|--------|-------------|--------|--------|
| REL-001 | System availability | Uptime when both master and display connected | ≥ 99.0% |
| REL-002 | Message delivery | UDP messages successfully received | ≥ 98% (WiFi networks) |
| REL-003 | Connection recovery | Time to reconnect after WiFi drop | < 30 seconds |
| REL-004 | Display robustness | No crashes after 24 hours operation | Fail-safe behavior required |
| REL-005 | Master robustness | DisplayServer survives 20 displays on/off | No state corruption |
| REL-006 | Graceful degradation | Display functions offline as terminal | No critical errors logged |
| REL-007 | Data integrity | Device state never corrupted by protocol errors | CRC validation required |

### 4.3 Scalability

| REQ-ID | Requirement | Metric | Target |
|--------|-------------|--------|--------|
| SCA-001 | Simultaneous displays | Max concurrent displays supported | 20 |
| SCA-002 | Master CPU overhead | CPU usage increase from DisplayServer | < 5% at max load |
| SCA-003 | Memory usage (Master) | RAM for display tracking | < 10KB per display |
| SCA-004 | Memory usage (Display) | RAM for UI + device cache | < 1MB total |
| SCA-005 | Message throughput | Devices controllable per second | ≥ 10 actions/sec |
| SCA-006 | Broadcast efficiency | Time to broadcast state to 20 displays | < 1 second |

### 4.4 Security & Privacy

| REQ-ID | Requirement | Metric | Target |
|--------|-------------|--------|--------|
| SEC-001 | WiFi encryption | WiFi connection security | WPA2/WPA3 minimum |
| SEC-002 | Access control | Control over display access (future) | Authenticated with master |
| SEC-003 | Data privacy | Device data not exposed in transit | Encrypted or local network only |
| SEC-004 | Denial of Service protection | Malformed messages don't crash system | Validation + rate limiting |
| SEC-005 | Memory safety | No buffer overflows from network data | Bounds checking on all copies |

### 4.5 Maintainability & Code Quality

| REQ-ID | Requirement | Metric | Target |
|--------|-------------|--------|--------|
| MAINT-001 | Code documentation | API and internal structures documented | 100% of public methods |
| MAINT-002 | Error logging | All errors logged with context | DEBUG/INFO/ERROR levels |
| MAINT-003 | State tracing | State machine transitions logged | Only in DEBUG builds |
| MAINT-004 | Protocol debugging | Message dumps available for debugging | With DEBUG_DISPLAY_PROTOCOL flag |
| MAINT-005 | Modularity | Display system decoupled from platform | Can be disabled via CMake flag |

### 4.6 Compatibility

| REQ-ID | Requirement | Metric | Target |
|--------|-------------|--------|--------|
| COMPAT-001 | Platform version compatibility | Supports HCS platform 2.x.x | Version check in handshake |
| COMPAT-002 | Multi-device type support | Works with all device types in JSON | No hardcoded device logic |
| COMPAT-003 | Custom bytes interpretation | Display respects device-specific state format | From device JSON definitions |
| COMPAT-004 | Future protocol extension | Protocol allows new message types | ID range reserved for future |
| COMPAT-005 | WiFi standards | Supports 802.11 b/g/n/ax | Tested on common ESP32 boards |

---

## 5. Protocol Specifications

### 5.1 Message Type Reference

| Message ID | Type | Direction | Purpose | Response |
|-----------|------|-----------|---------|----------|
| 200 | DISCOVER_DISPLAYS | Server → Broadcast | Find available displays | DISPLAY_PRESENT |
| 201 | DISPLAY_PRESENT | Display → Server | Announce presence | DISPLAY_HANDSHAKE |
| 202 | DISPLAY_HANDSHAKE | Server → Display | Accept & subscribe | DEVICE_LIST_REQUEST |
| 210 | DEVICE_LIST_REQUEST | Display → Server | Request device data | DEVICE_LIST_CHUNK* |
| 211 | DEVICE_LIST_CHUNK | Server → Display | Device batch | (none) |
| 212 | DEVICE_LIST_END | Server → Display | List complete | (none) |
| 213 | DEVICE_STATE_UPDATE | Server → All Displays | State changed | (none, broadcast) |
| 220 | DEVICE_ACTION_REQUEST | Display → Server | User action | DEVICE_ACTION_RESPONSE |
| 221 | DEVICE_ACTION_RESPONSE | Server → Display | Action result | (none) |
| 230 | DISPLAY_STATE_REFRESH | Server → Display | Force re-sync | (none) |
| 231 | DISPLAY_HASH_SYNC | Bidirectional | Validate device list | (depends) |
| 240 | KEEP_ALIVE | Bidirectional | Heartbeat | KEEP_ALIVE_ACK |
| 241 | KEEP_ALIVE_ACK | Bidirectional | Ack heartbeat | (none) |

### 5.2 Payload Structure Reference

**Common header** (all payloads):
```c
uint16_t sequenceNumber;    // Increments per message type
uint32_t timestamp;         // Milliseconds (local clock)
uint8_t  reserved[4];       // For future use
```

**DISPLAY_PRESENT payload** (~24 bytes):
- Display firmware version, screen resolution, max devices, MAC

**DEVICE_LIST_CHUNK payload** (~400+ bytes):
- Chunk index/total, device count
- Array of DeviceDescriptionWire (per device: ~98 bytes)

**DEVICE_ACTION_REQUEST payload** (~30 bytes):
- Target device (MAC, ID), action type, parameter

**DEVICE_ACTION_RESPONSE payload** (~60 bytes):
- Status (success/failure), device state bytes

---

## 6. Interface Requirements

### 6.1 DisplayServer Public API

```cpp
class DisplayServer {
    static void init();                          // Register with platform
    static void deinit();                        // Cleanup
    static void cyclic();                        // Run state machine
    static void receiveUDP(MessageUDP& msg);     // UDP reception callback
    static int getConnectedDisplayCount();       // Query connected count
    static bool isDisplayConnected(uint8_t id);  // Query specific display
};
```

### 6.2 DisplayClient Public API

```cpp
class DisplayClient {
    static void init();                          // Register with NetworkDriver
    static void deinit();                        // Cleanup
    static void cyclic();                        // Run state machine
    static bool isConnected();                   // Query connection status
    static bool requestDeviceAction(...);        // Queue action (blocks briefly)
    static const std::vector<DeviceDescriptionWire>& getDeviceList();  // Blocks until ready
    static std::vector<DeviceStateUpdatePayload> getStateUpdates();    // Non-blocking
};
```

### 6.3 DisplayUIThread Public API

```cpp
class DisplayUIThread {
    static void start();                         // Spawn UI thread
    static void stop();                          // Shutdown UI thread
    static ConnectionStatus getStatus();         // Query current status
};
```

### 6.4 Configuration Interface (Access Point HTTP)

```
POST /config/wifi
  Body: {"ssid": "...", "password": "..."}
  Response: 200 (on success)

GET /config/status
  Response: JSON with WiFi status, display ID, device count

POST /config/reboot
  Response: 200 (then device reboots)

POST /config/reset
  Response: 200 (settings are cleared and device reboots)
```

---

## 7. System Constraints

### 7.1 Hardware Constraints

- **Master**: ESP32 (existing HCS master)
- **Display**: ESP32 + 480×480 capacitive touchscreen
- **Network**: WiFi (2.4GHz, 802.11 b/g/n)
- **Flash**: ~4MB partition for program + 4MB for OTA (typical)
- **RAM**: ~4MB available (display uses ~600KB for UI)

### 7.2 Network Constraints

- **Protocol**: UDP on port 9001
- **Bandwidth**: Assumed > 1 Mbps (typical WiFi)
- **Latency**: Assumed < 50ms LAN round-trip
- **Packet Loss**: WiFi typical 1-5% loss expected
- **Range**: Same as WiFi (typical 30-50m indoor)

### 7.3 Device Constraints

- **Max Displays**: 20 simultaneous (hard limit)
- **Max Rooms**: 255 (0-254)
- **Max Devices per Display**: Limited by available RAM (~200 device descriptions)
- **Max Device Types**: Support all types defined in JSON device packages

### 7.4 Protocol Constraints

- **UDP Payload Size**: Max ~1400 bytes (TCP/IP header overhead)
- **Device List Chunk Size**: Max 5 devices per chunk (~400 bytes)
- **Message Retries**: Max 3 retries for critical messages
- **Timeout Values**:
  - Handshake: 5 seconds
  - Device list sync: 20 seconds
  - Action response: 5 seconds
  - Keep-alive: 20 seconds + 3 second ACK timeout

### 7.5 Functional Constraints

- **No Local Control Offline**: Display is terminal only; requires master for actions
- **No Peer-to-Peer**: Displays communicate only with master, not with each other
- **No Action Queueing**: If master unavailable, actions fail immediately (no queue)
- **No State Persistence**: Display cache not saved to NVM (lost on reboot if offline)

---

## 8. Acceptance Criteria Checklist

### System-Level Acceptance

- [ ] All 13 message types implemented and tested
- [ ] DisplayServer manages up to 20 displays simultaneously
- [ ] Display discovers master within 30 seconds of boot
- [ ] Device list syncs within 20 seconds of connection
- [ ] User actions execute within 500ms (action sent + response + UI update)
- [ ] Room switches occur immediately (< 50ms)
- [ ] State updates broadcast to all displays within 1 second
- [ ] Displays gracefully handle master disconnection/reconnection
- [ ] No crashes after 48 hours of stress testing
- [ ] Touch latency < 200ms on all user interactions
- [ ] Access Point configuration works end-to-end
- [ ] All device types (OnOff, LED, Temp, Window, etc.) render correctly

### Code Quality Acceptance

- [ ] All public APIs documented in comments
- [ ] All error cases logged with INFO or ERROR severity
- [ ] Memory usage < 1MB on display device
- [ ] No buffer overflows or memory leaks (static analysis + testing)
- [ ] Code compiles without warnings (on GCC)
- [ ] CMake build configuration supports disable/enable of feature
- [ ] Unit tests exist for protocol serialization/deserialization
- [ ] Integration tests exist for handshake and device sync flows

---

## Appendix A: Glossary

| Term | Definition |
|------|-----------|
| **DisplayServer** | Application running on Master device that coordinates display terminals |
| **DisplayClient** | Application running on Display device that communicates with DisplayServer |
| **DisplayUIThread** | LVGL rendering thread on Display device |
| **Display Terminal** | ESP32 device running DisplayClient + DisplayUIThread with 480×480 display |
| **Device Description** | Metadata about a device (name, type, state, custom bytes) |
| **Custom Bytes** | Device-specific state data (e.g., LED color as RGB, temperature as int16_t) |
| **Room** | Logical grouping of devices (e.g., Living Room, Bedroom) |
| **Master** | ESP32 device running HCS with DeviceManager and HTTP server |
| **Slave** | Remote ESP32 device in HCS network with devices but no HTTP server |
| **DeviceProvider** | HCS platform component that routes service calls to devices |
| **MessageUDP** | Wrapper structure for UDP packets in HCS |

---

End of Software Requirements Specification
