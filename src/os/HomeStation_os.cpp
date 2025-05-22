#include <os/HomeStation_os.hpp>


bool OperatingSystem::isHttpServerRunning = false;
bool OperatingSystem::isRCServerRunning = false;
bool OperatingSystem::resetPending = false;
int OperatingSystem::resetCountdown = 50;
uint16_t OperatingSystem::runtimeNodeHash = 0;
uint16_t OperatingSystem::uniqueLifecycleId = 0;

long long OperatingSystem::accessLevelGrantedTimeSnapshot = 0;
SecurityAccessLevelType OperatingSystem::currentAccessLevel = e_ACCESS_LEVEL_NONE;


long long uiBlockTime = 0;

static void displayRamUsage()
{
    size_t free_total = esp_get_free_heap_size();
    Serial.printf("Free heap total: %u bytes\n", free_total);

    // 2. Wolna pamięć o określonych właściwościach (np. DRAM/IRAM/SPIRAM):
    size_t free_dram = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
    size_t free_spiram = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
    Serial.printf("Free DRAM: %u bytes\n", free_dram);
    Serial.printf("Free SPIRAM: %u bytes\n", free_spiram);

    // 3. Największy możliwy blok do zaalokowania w DRAM:
    size_t max_block = heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT);
    Serial.printf("Max single DRAM block: %u bytes\n", max_block);

    // 4. Dla porównania: minimalny stan wolnego heap od resetu
    size_t min_free_ever = heap_caps_get_minimum_free_size(MALLOC_CAP_DEFAULT);
    Serial.printf("Minimum ever free DRAM: %u bytes\n", min_free_ever);
}

void OperatingSystem::init()
{
    pinMode(0, INPUT_PULLUP);
    uniqueLifecycleId = (uint16_t)random(10, 10000);
    DataContainer::setSignalValue(CBK_DISPLAY_RAM_USAGE, static_cast<std::function<void()>>(displayRamUsage));

    DataContainer::setSignalValue(CBK_RESET_DEVICE, static_cast<std::function<void(uint16_t)>>(OperatingSystem::reset));
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
        resetCountdown -= 50;

        if(resetCountdown <= 0)
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

    detectHwMassEraseRequest();
}

void OperatingSystem::reset(uint16_t delay) {
    if(!resetPending){
        resetPending = true;
        resetCountdown = delay;
    }
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


        Serial.println("e_ACCESS_LEVEL_SERVICE_MODE");
        break;      
    
   
    default:
        Serial.println("INVALID");
        break;
    }
}


void OperatingSystem::detectHwMassEraseRequest(){
    static uint8_t activationTimeCounter = 0;
    static bool isMassEraseRequestActivated = false;

    if(isMassEraseRequestActivated){ /* keep it first, to execute reset in the next execution to avoid reset when bootloader btn is pressed */
        if(digitalRead(0) == 1){ /* button was released, reset can be performed */
            std::any_cast<std::function<void(void)>>(DataContainer::getSignalValue(CBK_MASS_ERASE))();
            std::any_cast<std::function<void(uint16_t)>>(DataContainer::getSignalValue(CBK_RESET_DEVICE))(2000);
        }
    }

    if(digitalRead(0) == 0){ /* Button is pressed */
        activationTimeCounter++;
    }else {
        activationTimeCounter = 0; /* reset activation timer */
    }

    if(activationTimeCounter > 5){ /* 5 sec to activate reset */
        isMassEraseRequestActivated = true;
        Serial.println("OS://HW MASS ERASE ACTIVATED.");
    }
}
