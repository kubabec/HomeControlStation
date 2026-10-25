#pragma once


#include "os/drivers/networkdriver.hpp"
#include "devices/device.hpp"


enum class DisplayMessageIds : uint8_t {
    DISCOVER_DISPLAYS = PacketRangeDefinition::DISPLAY_CONTROLS_RANGE + 1,
    DISPLAY_PRESENT,
    DISPLAY_HANDSHAKE,
    DEVICE_LIST_REQUEST,
    DEVICE_LIST_CHUNK,
    DEVICE_LIST_END,
    DEVICE_ACTION_REQUEST,
    DEVICE_ACTION_RESPONSE,
    DEVICE_STATE_UPDATE,
    KEEP_ALIVE,
    KEEP_ALIVE_ACK,
    ROOM_NAMES_CHUNK,
    ROOM_STATE_CHUNK,
    ROOM_TOGGLE_REQUEST,
};

static constexpr uint8_t DISPLAY_PROTOCOL_VERSION = 1;
static constexpr uint8_t DISPLAY_MAX_DEVICES_PER_CHUNK = 5;
static constexpr uint8_t DISPLAY_MAX_ROOMS_PER_CHUNK = 8;
static constexpr uint8_t DISPLAY_ROOM_NAME_MAX_LENGTH = 24;
static constexpr uint8_t DISPLAY_MAX_ROOM_STATES_PER_CHUNK = 16;

/** Aggregated room state computed by the DisplayServer (same rule as the web UI room switch). */
enum class DisplayRoomState : uint8_t {
    OFF = 0,
    ON = 1,
    /** Room contains only devices which cannot be switched (e.g. temperature sensors). */
    NOT_TOGGLABLE = 2,
};
static constexpr uint16_t DISPLAY_UDP_PORT = 9001;

enum class DisplayAction : uint8_t {
    TOGGLE = 0,
    ENABLE = 1,
    DISABLE = 2,
    SET_VALUE = 3,
    /** parameter[0] carries the requested level in percent (0-100). */
    SET_BRIGHTNESS = 4,
    /** parameter[0] carries the LED strip memory slot index (1-3). */
    LOAD_LED_SLOT = 5,
    /** parameter[0] carries the segment index, parameter[1] the requested state (0/1). */
    SET_SEGMENT_STATE = 6,
};

#pragma pack(push, 1)
struct DisplayMessageHeader {
    uint8_t version = DISPLAY_PROTOCOL_VERSION;
    uint16_t sequence = 0;
};

struct DisplayHandshake {
    DisplayMessageHeader header;
    uint8_t displayId = 0;
    uint16_t deviceCount = 0;
    uint16_t deviceHash = 0;
};

struct DisplayDeviceWire {
    uint8_t deviceType = 0;
    uint64_t macAddress = 0;
    uint8_t deviceId = 0;
    uint8_t roomId = 0;
    uint8_t isEnabled = 0;
    uint8_t nameLength = 0;
    char deviceName[DEVICE_NAME_MAX_LENGHT] = {};
    uint8_t customBytes[NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION] = {};
};

struct DisplayDeviceListChunk {
    DisplayMessageHeader header;
    uint8_t chunkIndex = 0;
    uint8_t totalChunks = 0;
    uint8_t deviceCount = 0;
    DisplayDeviceWire devices[DISPLAY_MAX_DEVICES_PER_CHUNK] = {};
};

struct DisplayActionRequest {
    DisplayMessageHeader header;
    uint8_t deviceId = 0;
    uint8_t service = 0;
    uint8_t parameter[5] = {};
};

struct DisplayActionResponse {
    DisplayMessageHeader header;
    uint8_t status = 0;
    uint8_t deviceId = 0;
    DisplayDeviceWire device = {};
};

struct DisplayStateUpdate {
    DisplayMessageHeader header;
    DisplayDeviceWire device = {};
};

struct DisplayRoomNameWire {
    uint8_t roomId = 0;
    uint8_t nameLength = 0;
    char roomName[DISPLAY_ROOM_NAME_MAX_LENGTH] = {};
};

struct DisplayRoomNamesChunk {
    DisplayMessageHeader header;
    uint8_t chunkIndex = 0;
    uint8_t totalChunks = 0;
    uint8_t roomCount = 0;
    DisplayRoomNameWire rooms[DISPLAY_MAX_ROOMS_PER_CHUNK] = {};
};

struct DisplayRoomStateWire {
    uint8_t roomId = 0;
    uint8_t state = 0;
};

struct DisplayRoomStatesChunk {
    DisplayMessageHeader header;
    uint8_t chunkIndex = 0;
    uint8_t totalChunks = 0;
    uint8_t roomCount = 0;
    DisplayRoomStateWire rooms[DISPLAY_MAX_ROOM_STATES_PER_CHUNK] = {};
};

struct DisplayRoomToggleRequest {
    DisplayMessageHeader header;
    uint8_t roomId = 0;
};
#pragma pack(pop)
/**
 * @struct DisplayInfo
 * @brief Description of a display endpoint discovered on the LAN.
 */

struct DisplayInfo{
    /**
     * MAC address identifying the display endpoint uniquely.
     */
    uint64_t macAddress = 0;

    /**
     * Width of the display in pixels.
     */
    uint16_t resWidth = 480;

    /**
     * Height of the display in pixels.
     */
    uint16_t resHeight = 480;

    /**
     * IPv4 address used to contact the display over the local network.
     */
    MessageUDP::IPAddr ip{0, 0, 0, 0};
};
