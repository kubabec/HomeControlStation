#include <os/HomeStation_os.hpp>


bool OperatingSystem::isHttpServerRunning = false;
bool OperatingSystem::isRCServerRunning = false;
bool OperatingSystem::resetPending = false;
uint8_t OperatingSystem::resetCountdown = 10;
uint16_t OperatingSystem::runtimeNodeHash = 0;
uint16_t OperatingSystem::uniqueLifecycleId = 0;

void OperatingSystem::init()
{
    uniqueLifecycleId = (uint16_t)random(10, 10000);

    DataContainer::setSignalValue(CBK_RESET_DEVICE, static_cast<std::function<void()>>(OperatingSystem::reset));
    DataContainer::setSignalValue(SIG_RUNTIME_NODE_HASH, static_cast<uint16_t>(runtimeNodeHash));
    DataContainer::setSignalValue(
        CBK_SECURITY_ACCESS_LEVEL_CHANGE_VIA_STRING,
        static_cast<std::function<void(String)>>(OperatingSystem::requestSecurityAccessLevelChangeViaString)
    );

    DataContainer::subscribe(SIG_IS_HTTP_SERVER, [](std::any signal) {
        isHttpServerRunning = (std::any_cast<bool>(signal));
    });

    DataContainer::subscribe(SIG_IS_RC_SERVER, [](std::any signal) {
        isRCServerRunning = (std::any_cast<bool>(signal));
    });

    ErrorMonitor::init();
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
        RemoteDevicesManager::init();
    }else{
        RemoteControlClient::init();
    }

    DeviceProvider::init();
    

    // std::any_cast<std::function<void(ERR_MON_ERROR_TYPE errorCode, uint16_t extendedData)>>(
    //     DataContainer::getSignalValue(CBK_ERROR_REPORT))(ERR_MON_UNEXPECTED_RESET, 2);
}


void OperatingSystem::task10ms()
{
    NetworkDriver::cyclic();
    ErrorMonitor::cyclic();

    if(isHttpServerRunning){
        HomeLightHttpServer::cyclic();
    }

    if(isRCServerRunning){
        RemoteControlServer::cyclic();
        RemoteDevicesManager::cyclic();
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

    if(resetPending){
        resetCountdown --;

        if(resetCountdown == 0)
        {
            performReset();
        }
    }
}

void OperatingSystem::task1s()
{
    runtimeNodeHash = calculateRuntimeNodeHash();
    DataContainer::setSignalValue(SIG_RUNTIME_NODE_HASH, static_cast<uint16_t>(runtimeNodeHash));
}

void OperatingSystem::reset() {
    resetPending = true;
}


void OperatingSystem::performReset()
{
    DeviceProvider::deinit();

    if(isRCServerRunning){
        RemoteDevicesManager::deinit();
        RemoteControlServer::deinit();
    }else{
        RemoteControlClient::deinit();
    }

    DeviceManager::deinit();

    if(isHttpServerRunning){
        HomeLightHttpServer::deinit();
    }

    NetworkDriver::deinit();

    ErrorMonitor::deinit();
    ConfigProvider::deinit();

    ESP.restart();
}

/* Funkcja do obliczenia unikalnego identyfikatora (hasha) dla bieżącego stanu systemu.*/
uint16_t OperatingSystem::calculateRuntimeNodeHash()
{
    uint16_t hash = 0;

    hash += uniqueLifecycleId;

    /* Get configuration */
    try{
        NodeConfiguration configuration = 
            std::any_cast<NodeConfiguration>(
                DataContainer::getSignalValue(SIG_DEVICE_CONFIGURATION)
            );
        hash += (uint8_t) configuration.isHttpServer;
        hash += (uint8_t) configuration.isRcServer;
        hash += (uint8_t) configuration.networkCredentialsAvailable;
        hash += (uint8_t) configuration.nodeId;
        hash += (uint8_t) configuration.nodeType;
        for(uint8_t idx = 0 ; idx < configuration.networkSSID.length(); idx ++)
        {
            hash += (uint8_t) configuration.networkSSID.charAt(idx);
        }
        for(uint8_t idx = 0 ; idx < configuration.networkPassword.length(); idx ++)
        {
            hash += (uint8_t) configuration.networkPassword.charAt(idx);
        }
    }catch (std::bad_any_cast ex){}


    /* Get devices data */
    try{
        std::vector<OnOffDeviceDescription> devicesVector = 
            std::any_cast<std::vector<OnOffDeviceDescription>>(
                DataContainer::getSignalValue(SIG_COLLECTION_ONOFF)
            );
        for(auto& device : devicesVector)
        {
            hash += (uint8_t) device.deviceId;
            hash += (uint8_t) device.nodeId;
            hash += (uint8_t) device.isEnabled;
            hash += (uint8_t) device.currentBrightness;
            hash += (uint8_t) device.brightnessIsAdjustable;
            for(uint8_t idx = 0 ; idx < device.deviceName.length(); idx ++)
            {
                hash += (uint8_t) device.deviceName.charAt(idx);
            }
        }
    }catch (std::bad_any_cast ex){}

    //Serial.println("Hash : " + String((int)hash));

    return hash;
}

void OperatingSystem::requestSecurityAccessLevelChangeViaString(String password)
{
    if(password == "admin"){
        DataContainer::setSignalValue(SIG_SECURITY_ACCESS_LEVEL, e_ACCESS_LEVEL_SERVICE_MODE);
        Serial.println("Access level granted: e_ACCESS_LEVEL_SERVICE_MODE");
    }
}