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
#include <os/app/timeMaster.hpp>

#include <os/HwDependency/RFManager.hpp>


class   OperatingSystem
{

private:
    static bool isHttpServerRunning;
    static bool isRCServerRunning;
    static uint16_t runtimeNodeHash;
    static uint16_t uniqueLifecycleId;

    static bool isNvmSaveTimerActive;
    static long long nvmSaveTimerValue;

    static long long uIBlockTime;

    static bool resetPending;
    static int resetCountdown;
    
    static void performReset();
    static uint16_t calculateRuntimeNodeHash();
    static void handleSecurityAccessLevelExpiration();
    static void changeSecurityAccessLevel(SecurityAccessLevelType newAccessLevel);
    
    static long long accessLevelGrantedTimeSnapshot;
    static SecurityAccessLevelType currentAccessLevel;

    static void handleUiBlockTimeExpiration();
    static void detectHwMassEraseRequest();

    static void handleNvmSaveMech();
    
    

public:

    static void init();
    static void reset(uint16_t delay);

    static ServiceInformation displayRamUsage();
    static void saveNvmData();
    static void activateNvmSaveTimer();

    static void requestSecurityAccessLevelChangeViaString(String password);


    static void task2ms();
    static void task10ms();
    static void task20ms();
    static void task50ms();
    static void task100ms();
    static void task1s();

    
};



#endif