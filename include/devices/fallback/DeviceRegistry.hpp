#ifndef DEVICE_REGISTRY_FALLBACK_HPP
#define DEVICE_REGISTRY_FALLBACK_HPP

#include <functional>
#include <memory>
#include "SystemDefinition.hpp"
#include "devices/device.hpp"
#include "os/datacontainer/NvmConfigSlotDefinition.hpp"

namespace GeneratedDeviceRegistry
{
struct RuntimeContext
{
    std::function<void()> persistentDataChanged;
    std::function<RtcTime()> getRtcTime;
    std::function<void(uint16_t)> toggleLocalDevice;
    std::function<void(uint64_t)> fireDigitalEvent;
};

struct Registration
{
    uint8_t typeId;
    const char* name;
    bool includedInDeviceCollection;
    uint32_t cycleIntervalMs;
};

inline constexpr Registration kEnabledTypes[] = {};
inline const Registration* find(uint8_t) { return nullptr; }
inline std::unique_ptr<Device> create(uint8_t, DeviceConfigSlotType, const RuntimeContext&)
{
    return nullptr;
}
}

#endif
