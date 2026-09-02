/**
 * @file HomeStation_os.hpp
 * @brief Central runtime controller for the HomeControlStation operating system.
 */
#ifndef HOME_LIGHT_OK_H
#define HOME_LIGHT_OK_H

#include <WiFiUdp.h>
#include <string>

#include "os/CyclicProfiler.hpp"

#include <os/app/http/HttpServer.hpp>
#include <os/app/DeviceManager.hpp>
#include <os/drivers/NetworkDriver.hpp>
#include <os/datacontainer/DataContainer.hpp>
#include <os/app/remoteControl/RemoteControlServer.hpp>
#include <os/app/RemoteDevicesManager.hpp>
#include <os/app/config/ConfigProvider.hpp>
#include <os/app/remoteControl/RemoteControlClient.hpp>
#include <os/app/DeviceProvider.hpp>
#include <os/app/NotificationHandler.hpp>
#include <os/app/DigitalEvent/DigitalEventReceiver.hpp>
#include <os/app/DigitalEvent/DigitalEventTransmitter.hpp>
#include <os/app/config/ExtendedMemoryManager.hpp>
#include <os/app/SafeResetManager.hpp>
#include <os/app/timeMaster.hpp>
#include <os/app/display/DisplayServer.hpp>

#include <os/HwDependency/RFManager.hpp>

/**
 * @class OperatingSystem
 * @brief Main orchestrator of the firmware runtime and periodic maintenance tasks.
 *
 * This class coordinates initialization, security access, network services,
 * device management, and periodic task execution for the station.
 */
class   OperatingSystem
{

private:
    /** True when the HTTP server is active. */
    static bool isHttpServerRunning;
    /** True when the remote-control server is active. */
    static bool isRCServerRunning;
    /** Runtime hash used to identify the current execution node. */
    static uint16_t runtimeNodeHash;
    /** Unique identifier assigned to the current startup cycle. */
    static uint16_t uniqueLifecycleId;

    /** True when the NVM save timer is currently armed. */
    static bool isNvmSaveTimerActive;
    /** Remaining value of the timer used to trigger NVM persistence. */
    static long long nvmSaveTimerValue;

    /** UI block interval used to throttle user interaction. */
    static long long uIBlockTime;

    /** Signals that a reset is queued. */
    static bool resetPending;
    /** Countdown remaining until system restart. */
    static int resetCountdown;
    
    /** Forces the running system through the reset path. */
    static void performReset();
    /** Calculates a node-specific runtime hash used for diagnostics. */
    static uint16_t calculateRuntimeNodeHash();
    /** Clears expired access-level grants and updates the security state. */
    static void handleSecurityAccessLevelExpiration();
    /** Changes the active authorization level used by the station. */
    static void changeSecurityAccessLevel(SecurityAccessLevelType newAccessLevel);
    
    /** Snapshot of the time when the current access level was granted. */
    static long long accessLevelGrantedTimeSnapshot;
    /** Active authorization level for the current runtime. */
    static SecurityAccessLevelType currentAccessLevel;

    /** Handles expiration of the UI block timer. */
    static void handleUiBlockTimeExpiration();
    /** Detects hardware-triggered mass erase requests. */
    static void detectHwMassEraseRequest();

    /** Persists NVM values when the periodic save timer expires. */
    static void handleNvmSaveMech();
    
public:
    /**
     * @brief Initializes all subsystems and starts the runtime scheduler.
     */
    static void init();
    /**
     * @brief Schedules a delayed system reset.
     * @param delay Delay before restart in milliseconds or scheduler units.
     */
    static void reset(uint16_t delay);

    /**
     * @brief Provides a summary of current RAM usage and allocation health.
     * @return ServiceInformation structure containing memory diagnostics.
     */
    static ServiceInformation displayRamUsage();
    /**
     * @brief Stores the current NVM-backed configuration snapshot.
     */
    static void saveNvmData();
    /**
     * @brief Arms the NVM save timer to trigger deferred persistence.
     */
    static void activateNvmSaveTimer();

    /**
     * @brief Requests a security access-level change using a password string.
     * @param password Password entered by the caller.
     */
    static void requestSecurityAccessLevelChangeViaString(String password);

    /**
     * @brief Executes the 2 ms maintenance tick.
     */
    static void task2ms();
    /**
     * @brief Executes the 10 ms maintenance tick.
     */
    static void task10ms();
    /**
     * @brief Executes the 20 ms maintenance tick.
     */
    static void task20ms();
    /**
     * @brief Executes the 50 ms maintenance tick.
     */
    static void task50ms();
    /**
     * @brief Executes the 100 ms maintenance tick.
     */
    static void task100ms();
    /**
     * @brief Executes the 1 second maintenance tick.
     */
    static void task1s();
};

#endif