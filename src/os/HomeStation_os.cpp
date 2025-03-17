#include <os/HomeStation_os.hpp>


bool OperatingSystem::isHttpServerRunning = false;
bool OperatingSystem::isRCServerRunning = false;
bool OperatingSystem::resetPending = false;
uint8_t OperatingSystem::resetCountdown = 50;
uint16_t OperatingSystem::runtimeNodeHash = 0;
uint16_t OperatingSystem::uniqueLifecycleId = 0;

long long OperatingSystem::accessLevelGrantedTimeSnapshot = 0;
SecurityAccessLevelType OperatingSystem::currentAccessLevel = e_ACCESS_LEVEL_NONE;


long long uiBlockTime = 0;

void OperatingSystem::init()
{
    
    uniqueLifecycleId = (uint16_t)random(10, 10000);

    DataContainer::setSignalValue(CBK_RESET_DEVICE, static_cast<std::function<void()>>(OperatingSystem::reset));
    DataContainer::setSignalValue(CBK_CALCULATE_RUNTIME_NODE_HASH, static_cast<std::function<uint16_t()>>(OperatingSystem::calculateRuntimeNodeHash));

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
    NotificationHandler::init();

    changeSecurityAccessLevel(e_ACCESS_LEVEL_NONE);
    // changeSecurityAccessLevel(e_ACCESS_LEVEL_SERVICE_MODE);
    ConfigProvider::init();
    ExtendedMemoryManager::init();


    calculateRuntimeNodeHash();
    DataContainer::setSignalValue(SIG_RUNTIME_NODE_HASH, static_cast<uint16_t>(runtimeNodeHash));
    
    // Inicialize device manager, devices settings etc.
    DeviceManager::init();

    // Initialize network settings such as 
    // WiFi network connection etc.
    NetworkDriver::init();

    if(isHttpServerRunning){
        HomeLightHttpServer::init();
    }

    if(isRCServerRunning){
        RemoteControlServer::init();
        RemoteDevicesManager::init();
    }else{
        RemoteControlClient::init();
    }

    DeviceProvider::init();
    TimeMaster::init(); 
    

    /* handle security access level grant to SERVICE MODE if there is no valid config loaded */
    NodeConfiguration currentConfig = 
        std::any_cast<NodeConfiguration>(DataContainer::getSignalValue(SIG_DEVICE_CONFIGURATION));
    if( currentConfig.nodeType == 255 || /* invalid node type  */
        !currentConfig.networkCredentialsAvailable || /* no network credentials */
        (std::any_cast<std::vector<DeviceDescription>>(DataContainer::getSignalValue(SIG_LOCAL_COLLECTION)).size() == 0)){ /* no devices configured yet */
        changeSecurityAccessLevel(e_ACCESS_LEVEL_SERVICE_MODE);


        UserInterfaceNotification notif {
            .title = "Service access granted",
            .body = "Device received service mode access due to one of the following reasons: Invalid configuration / No WiFi configured / No local devices configured",
            .type = UserInterfaceNotification::INFO
        };
        std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);
    }


    Serial.println("HomeStationOS:// Init completed.");
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
    ExtendedMemoryManager::cyclic();

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
    Serial.print(".");
    handleSecurityAccessLevelExpiration();        
    TimeMaster::cyclic();
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
    TimeMaster::deinit();

    ExtendedMemoryManager::deinit();
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

            /* notification */
            UserInterfaceNotification notif{
                .title = "Device lock/unlock",
                .body = "Unlock time expired, device is locked again.",
                .type = UserInterfaceNotification::INFO
            };
            std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);
        }
    }
}

/* Funkcja do obliczenia unikalnego identyfikatora (hasha) dla bieżącego stanu systemu.*/
uint16_t OperatingSystem::calculateRuntimeNodeHash()
{
    uint64_t hash = 0;

    //hash += uniqueLifecycleId;

    /* Get configuration */
    try{
        NodeConfiguration configuration = 
            std::any_cast<NodeConfiguration>(
                DataContainer::getSignalValue(SIG_DEVICE_CONFIGURATION)
            );
        hash +=  configuration.isHttpServer;
        hash +=  configuration.isRcServer;
        hash +=  configuration.isDefaultUserAdmin;
        hash +=  configuration.networkCredentialsAvailable;
        hash +=  configuration.nodeType;
        for(uint8_t idx = 0 ; idx < configuration.networkSSID.length(); idx ++)
        {
            hash += configuration.networkSSID.charAt(idx);
        }
        for(uint8_t idx = 0 ; idx < configuration.networkPassword.length(); idx ++)
        {
            hash += configuration.networkPassword.charAt(idx);
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
            hash += device.deviceId;
            hash += device.macAddress;
            hash += device.isEnabled;
            for(uint8_t idx = 0 ; idx < NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION ; idx ++)
            {
                hash += device.customBytes[idx];
            }
            for(uint8_t idx = 0 ; idx < device.deviceName.length(); idx ++)
            {
                hash += device.deviceName.charAt(idx);
            }
        }
    }catch (std::bad_any_cast ex){}

    //Serial.println("Hash : " + String((int)hash));

    memcpy(&runtimeNodeHash, &hash, sizeof(uint16_t));
    DataContainer::setSignalValue(SIG_RUNTIME_NODE_HASH, static_cast<uint16_t>(runtimeNodeHash));
    // Serial.println("New hash : " + String((int)hash));

    return hash;
}

void OperatingSystem::requestSecurityAccessLevelChangeViaString(String password)
{
    NodeConfiguration currentConfig = std::any_cast<NodeConfiguration>(DataContainer::getSignalValue(SIG_DEVICE_CONFIGURATION));
    if(password == "admin"){
        changeSecurityAccessLevel(e_ACCESS_LEVEL_SERVICE_MODE);
    }else if(password == currentConfig.panelPassword){ /* check is password for the user matches */
        SecurityAccessLevelType userRights = e_ACCESS_LEVEL_AUTH_USER;

        if(currentConfig.isDefaultUserAdmin){ /* verify if default user can have admin rights */
            userRights = e_ACCESS_LEVEL_SERVICE_MODE;
        }
        changeSecurityAccessLevel(userRights);
    }else {
        /* notification */
        UserInterfaceNotification notif{
            .title = "Wrong password",
            .body = "Device unlock try failed due to invalid password.",
            .type = UserInterfaceNotification::WARNING
        };
        std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);
    }
}

void OperatingSystem::changeSecurityAccessLevel(SecurityAccessLevelType newAccessLevel)
{
    DataAndTime currentTime;

    currentAccessLevel = newAccessLevel;
    accessLevelGrantedTimeSnapshot = millis();
    DataContainer::setSignalValue(SIG_SECURITY_ACCESS_LEVEL, currentAccessLevel);

    /* notification */
    UserInterfaceNotification notif{
        .title = "Device lock/unlock",
        .body = "",
        .type = UserInterfaceNotification::WARNING
    };

    Serial.print("Access level granted: ");
    switch (currentAccessLevel)
    {
    case e_ACCESS_LEVEL_NONE:
        Serial.println("e_ACCESS_LEVEL_NONE");
        break;
    
    case e_ACCESS_LEVEL_AUTH_USER:
        Serial.println("e_ACCESS_LEVEL_AUTH_USER");
        notif.type = UserInterfaceNotification::INFO;
        notif.body = "Device is unlocked to authorized user level and will be automatically locked.";
        std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);
        break;

    case e_ACCESS_LEVEL_SERVICE_MODE:

        // currentTime = std::any_cast<DataAndTime>(DataContainer::getSignalValue(SIG_CURRENT_TIME));

        char timeBuffer[20]; // Bufor na czas (np. "2023-10-05 12:34:56")
        snprintf(timeBuffer, sizeof(timeBuffer), "%04d-%02d-%02d %02d:%02d:%02d",
                 currentTime.year, currentTime.month, currentTime.day,
                 currentTime.hour, currentTime.minute, currentTime.second);
        

        Serial.println("e_ACCESS_LEVEL_SERVICE_MODE");
        notif.body = "Device is running in service mode ";

        std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);
        break;      
    
   
    default:
        Serial.println("INVALID");
        break;
    }
}
