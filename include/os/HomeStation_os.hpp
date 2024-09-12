#ifndef HOME_LIGHT_OK_H
#define HOME_LIGHT_OK_H

#include <os/app/http/HttpServer.hpp>
#include <os/app/DeviceManager.hpp>
#include <os/drivers/NetworkDriver.hpp>
#include <os/datacontainer/DataContainer.hpp>
#include <os/app/remoteControl/RemoteControlServer.hpp>
#include <os/app/config/ConfigProvider.hpp>
#include <os/app/remoteControl/RemoteControlClient.hpp>
#include <os/app/DeviceProvider.hpp>

class   OperatingSystem
{

private:
    static SystemNetworkModeEnum networkMode;
    static bool isHttpServerRunning;
    static bool isRCServerRunning;

public:
    static void init();


    static void task10ms();
    static void task20ms();
    static void task50ms();

};



#endif