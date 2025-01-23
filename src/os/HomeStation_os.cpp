#include <os/HomeStation_os.hpp>


bool OperatingSystem::isHttpServerRunning = false;
bool OperatingSystem::isRCServerRunning = false;
bool OperatingSystem::resetPending = false;
uint8_t OperatingSystem::resetCountdown = 10;
uint16_t OperatingSystem::runtimeNodeHash = 0;
uint16_t OperatingSystem::uniqueLifecycleId = 0;

long long OperatingSystem::accessLevelGrantedTimeSnapshot = 0;
SecurityAccessLevelType OperatingSystem::currentAccessLevel = e_ACCESS_LEVEL_NONE;

long long uiBlockTime = 0;

void OperatingSystem::init()
{
    changeSecurityAccessLevel(e_ACCESS_LEVEL_NONE);

    uniqueLifecycleId = (uint16_t)random(10, 10000);

    DataContainer::setSignalValue(CBK_RESET_DEVICE, static_cast<std::function<void()>>(OperatingSystem::reset));
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
    NotificationHandler::init();


    calculateRuntimeNodeHash();
    DataContainer::setSignalValue(SIG_RUNTIME_NODE_HASH, static_cast<uint16_t>(runtimeNodeHash));


    /* handle security access level grant to SERVICE MODE if there is no valid config loaded */
    NodeConfiguration currentConfig = 
        std::any_cast<NodeConfiguration>(DataContainer::getSignalValue(SIG_DEVICE_CONFIGURATION));
    if(currentConfig.nodeType == 255){
        changeSecurityAccessLevel(e_ACCESS_LEVEL_SERVICE_MODE);
    }
    
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
    


    /* notification test */
    UserInterfaceNotification notif{
        .title = "Title!",
        .body = "Notification testing",
        .type = UserInterfaceNotification::ERROR
    };
    std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);
    std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);
    std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);
    /* notification test */
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
    NotificationHandler::cyclic();

    if(resetPending){
        resetCountdown --;

        if(resetCountdown == 0)
        {
            performReset();
        }
    }

    calculateRuntimeNodeHash();
}

void OperatingSystem::task1s()
{
    handleSecurityAccessLevelExpiration();
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
    NotificationHandler::deinit();


    /* This app must be last, as it saves NVM data */
    ConfigProvider::deinit();

    ESP.restart();
}

void OperatingSystem::handleSecurityAccessLevelExpiration()
{
    const unsigned timeToExpireAccessLevel = (1000 * 60) * 5; /* 5 minutes */
    /* access level is granted? */
    if(currentAccessLevel > e_ACCESS_LEVEL_NONE){
        if(abs(millis() - accessLevelGrantedTimeSnapshot) > timeToExpireAccessLevel){
            accessLevelGrantedTimeSnapshot = 0;
            currentAccessLevel = e_ACCESS_LEVEL_NONE;
            DataContainer::setSignalValue(SIG_SECURITY_ACCESS_LEVEL, currentAccessLevel);
            Serial.println("Access level unlock expired.");
        }
    }
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
        std::vector<DeviceDescription> devicesVector = 
            std::any_cast<std::vector<DeviceDescription>>(
                DataContainer::getSignalValue(SIG_DEVICE_COLLECTION)
            );
        for(auto& device : devicesVector)
        {
            hash += (uint8_t) device.deviceId;
            hash += (uint8_t) device.macAddress;
            hash += (uint8_t) device.isEnabled;
            for(uint8_t idx = 0 ; idx < NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION ; idx ++)
            {
                hash += device.customBytes[idx];
            }
            for(uint8_t idx = 0 ; idx < device.deviceName.length(); idx ++)
            {
                hash += (uint8_t) device.deviceName.charAt(idx);
            }
        }
    }catch (std::bad_any_cast ex){}

    //Serial.println("Hash : " + String((int)hash));

    runtimeNodeHash = hash;
    DataContainer::setSignalValue(SIG_RUNTIME_NODE_HASH, static_cast<uint16_t>(hash));
    // Serial.println("New hash : " + String((int)hash));

    return hash;
}

void OperatingSystem::requestSecurityAccessLevelChangeViaString(String password)
{
    if(password == "admin"){
        changeSecurityAccessLevel(e_ACCESS_LEVEL_SERVICE_MODE);
    }

    if(password == "user"){
        changeSecurityAccessLevel(e_ACCESS_LEVEL_AUTH_USER);
    }
}

void OperatingSystem::changeSecurityAccessLevel(SecurityAccessLevelType newAccessLevel)
{
    currentAccessLevel = newAccessLevel;
    accessLevelGrantedTimeSnapshot = millis();
    DataContainer::setSignalValue(SIG_SECURITY_ACCESS_LEVEL, currentAccessLevel);

    Serial.print("Access level granted: ");
    switch (currentAccessLevel)
    {
    case e_ACCESS_LEVEL_NONE:
        Serial.println("e_ACCESS_LEVEL_NONE");
        break;
    
    case e_ACCESS_LEVEL_AUTH_USER:
        Serial.println("e_ACCESS_LEVEL_AUTH_USER");
        break;

    case e_ACCESS_LEVEL_SERVICE_MODE:
        Serial.println("e_ACCESS_LEVEL_SERVICE_MODE");
        break;
    default:
        Serial.println("INVALID");
        break;
    }
}