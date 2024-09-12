#include <os/homelight_os.hpp>

SystemNetworkModeEnum OperatingSystem::networkMode = SystemNetworkModeEnum::eWIFI_UNKNOWN;

bool OperatingSystem::isHttpServerRunning = false;
bool OperatingSystem::isRCServerRunning = false;

void OperatingSystem::init()
{
    DataContainer::subscribe(SIG_IS_HTTP_SERVER, "OperatingSystem", [](std::any signal) {
        isHttpServerRunning = (std::any_cast<bool>(signal));
    });

    DataContainer::subscribe(SIG_IS_RC_SERVER, "OperatingSystem", [](std::any signal) {
        isRCServerRunning = (std::any_cast<bool>(signal));
    });

    ConfigProvider::init();

    // Initialize network settings such as 
    // WiFi network connection etc.
    NetworkDriver::init();

    if(isHttpServerRunning){
        HomeLightHttpServer::init();
    }




    // Inicialize device manager, devices settings etc.
    DeviceManager::init();

    if(isRCServerRunning){
        RemoteControlServer::init();
    }else{
        RemoteControlClient::init();
    }

    DeviceProvider::init();
    
}


void OperatingSystem::task10ms()
{
    NetworkDriver::cyclic();

    if(isHttpServerRunning){
        HomeLightHttpServer::cyclic();
    }

    if(isRCServerRunning){
        RemoteControlServer::cyclic();
    }else{
        RemoteControlClient::cyclic();
    }
}

void OperatingSystem::task20ms()
{
    DeviceManager::cyclic();
    DeviceProvider::cyclic();
}

void OperatingSystem::task50ms()
{
    DataContainer::setSignalValue(SIG_SYSTEM_NETWORK_MODE, "OperatingSystem", networkMode);
    //DataContainer::setSignalValue(CBK_DEVICE_ENABLE, "OperatingSystem", ....);
    
}