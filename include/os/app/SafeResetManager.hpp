#ifndef SAFE_RESET_MANAGER_H
#define SAFE_RESET_MANAGER_H
#include <Arduino.h>
#include <os/datacontainer/DataContainer.hpp>
/**
 * @class SafeResetManager
 * @brief Protects the system from unintended resets during periods when the installation is active.
 *
 * The manager waits for a long cooldown window and then checks whether the system is in the
 * configured morning maintenance interval and whether no local device is still active. If both
 * conditions are true, it triggers the reset callback registered inside the data container.
 */

class SafeResetManager
{
    /**
     * Delay before a reset is considered safe. The value is intentionally long so the node does
     * not reboot repeatedly when the system is still servicing devices.
     */
    static const long long RESET_DELAY; // 30h in ms

    /**
     * Earliest hour of the allowed maintenance window for a scheduled reset.
     */
    static const uint8_t morningStartHour;

    /**
     * Exclusive end hour of the allowed maintenance window for a scheduled reset.
     */
    static const uint8_t morningEndHour;

    /**
     * Checks whether the current time falls inside the configured morning maintenance window.
     * @return true when the clock is in the allowed reset interval; otherwise false.
     */
    static bool isMorning();

    /**
     * Verifies that no local device is still marked as enabled before allowing a reset.
     * @return true when the local device collection is considered safe for reset; otherwise false.
     */
    static bool localDevicesConditionFilled();

public:
    /**
     * Initializes the reset manager and prepares its runtime state.
     */
    static void init();

    /**
     * Runs the periodic safety check and executes the reset callback only when all conditions are met.
     */
    static void cyclic();

    /**
     * Releases any runtime state owned by the reset manager.
     */
    static void deinit();
};

#endif