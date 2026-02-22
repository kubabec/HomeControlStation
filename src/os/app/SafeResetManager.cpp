#include <os/app/SafeResetManager.hpp>
#include "os/Logger.hpp"

const long long SafeResetManager::RESET_DELAY{1000 * 60 * 60 * 30}; // 30h in ms
const uint8_t SafeResetManager::morningStartHour{7};
const uint8_t SafeResetManager::morningEndHour{9};

void SafeResetManager::init()
{
    Logger::log("SafeResetManager init ...");
    
    Logger::log("... done");
}

void SafeResetManager::cyclic()
{
    unsigned long currentTime = millis();

    // Check if reset delay has been exceeded
    if (currentTime > RESET_DELAY)
    {
        // Check if it's morning before performing reset
        if (isMorning() && localDevicesConditionFilled())
        {
            // Get device reset function from DataContainer and call it to perform reset
            std::any_cast<std::function<void(uint16_t)>>(DataContainer::getSignalValue(CBK_RESET_DEVICE))(500);
        }
        else
        {
            Logger::log("SafeResetManager:// Reset delay exceeded but it's not morning or local devices are active, skipping reset.");
        }
    }
}

void SafeResetManager::deinit()
{
}

bool SafeResetManager::isMorning()
{
    bool isMorningTime = false;
    std::any localAny = DataContainer::getSignalValue(CBK_GET_CURRENT_TIME);
    if (auto p = std::any_cast<std::function<RtcTime()>>(&localAny))
    {
        auto timeCallback = *p;
        RtcTime currentTime = timeCallback();
        if (currentTime.year < 1990)
        {
            Logger::log("SafeResetManager:// Current time is not set, unable to determine if it's morning. Assuming it's not morning.");
            return false;
        }

        if (currentTime.hour >= morningStartHour && currentTime.hour < morningEndHour)
        {
            isMorningTime = true;
        }
    }
    else
    {
        Logger::log("SafeResetManager:// Unable to determine current time, assuming it's not morning.");
    }
}

bool SafeResetManager::localDevicesConditionFilled()
{
    std::any localAny = DataContainer::getSignalValue(SIG_LOCAL_COLLECTION);
    bool anyLocalDeviceActive = false;
    if (auto p = std::any_cast<std::vector<DeviceDescription>>(&localAny))
    {
        std::vector<DeviceDescription> localDevices = *p;
        // Check if any local device is ENABLED
        for (const auto &device : localDevices)
        {
            // If at least one local device is enabled, we can consider the condition for safe reset as not filled, to avoid resetting when user has active devices at home
            if (device.isEnabled == 1)
            {
                anyLocalDeviceActive = true;
                break;
            }
        }
    }
    else
    {
        Logger::log("SafeResetManager:// Unable to access local devices collection, assuming conditions not filled.");
        return false;
    }

    // If there are local devices but none is active, we can consider it as a condition for safe reset
    return !anyLocalDeviceActive;
}
