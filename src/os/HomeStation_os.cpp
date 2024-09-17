#include <os/HomeStation_os.hpp>


bool OperatingSystem::isHttpServerRunning = false;
bool OperatingSystem::isRCServerRunning = false;

void OperatingSystem::init()
{
    DataContainer::setSignalValue(CBK_RESET_DEVICE, "OperatingSystem", static_cast<std::function<void()>>(OperatingSystem::reset));

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

    
}

void OperatingSystem::reset() {

    DeviceProvider::deinit();

    if(isRCServerRunning){
        RemoteControlServer::deinit();
    }else{
        RemoteControlClient::deinit();
    }

    DeviceManager::deinit();

    if(isHttpServerRunning){
        HomeLightHttpServer::deinit();
    }

    NetworkDriver::deinit();
    ConfigProvider::deinit();

    ESP.restart();

}