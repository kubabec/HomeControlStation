#pragma once

#include <cstdint>

enum DevType : uint8_t
{
    type_ONOFFDEVICE = 43,
    type_LED_STRIP = 44,
    type_TEMP_SENSOR = 45,
    type_HARDWARE_BUTTON = 48,
    type_WINDOW_BLINDER = 49,
    type_WINDOW_DOOR_SENSOR = 50,
    type_GATE = 51,
    type_AQUARIUM_CONTROLLER = 52
};

namespace GeneratedDeviceTypes
{
inline bool isKnown(uint8_t id) { return id >= 43 && id <= 52 && id != 46 && id != 47; }
inline const char* nameOf(uint8_t id)
{
    switch (id)
    {
    case type_ONOFFDEVICE: return "OnOff";
    case type_LED_STRIP: return "LedStrip";
    case type_TEMP_SENSOR: return "TempSensor";
    case type_HARDWARE_BUTTON: return "HwButton";
    case type_WINDOW_BLINDER: return "WindowBlinder";
    case type_WINDOW_DOOR_SENSOR: return "WindowDoorSensor";
    case type_GATE: return "Gate";
    case type_AQUARIUM_CONTROLLER: return "AquariumController";
    default: return "UNKNOWN";
    }
}
}
