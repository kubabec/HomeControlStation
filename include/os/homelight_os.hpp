#ifndef HOME_LIGHT_OK_H
#define HOME_LIGHT_OK_H

#include <os/app/http/httpserver.hpp>
#include <os/app/devicemanager.hpp>
#include <os/drivers/networkdriver.hpp>
#include <os/datacontainer/datacontainer.hpp>
#include <os/app/remoteControl/remotecontrolserver.hpp>
#include <os/app/config/configprovider.hpp>
#include <os/app/remoteControl/remoteControlClient.hpp>
#include <os/app/deviceProvider.hpp>

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