#ifndef DEVICE_TYPES_FALLBACK_HPP
#define DEVICE_TYPES_FALLBACK_HPP

#include <cstdint>

enum DevType : uint8_t
{
};

namespace GeneratedDeviceTypes
{
inline bool isKnown(uint8_t) { return false; }
inline const char* nameOf(uint8_t) { return "UNKNOWN"; }
}

#endif
