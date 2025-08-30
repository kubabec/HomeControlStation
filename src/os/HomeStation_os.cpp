#include <os/HomeStation_os.hpp>
#include <DHT.h>

DHT tempSensor(21, DHT11);

bool OperatingSystem::isHttpServerRunning = false;
bool OperatingSystem::isRCServerRunning = false;
bool OperatingSystem::resetPending = false;
int OperatingSystem::resetCountdown = 50;
uint16_t OperatingSystem::runtimeNodeHash = 0;
uint16_t OperatingSystem::uniqueLifecycleId = 0;

bool OperatingSystem::isNvmSaveTimerActive = false;
long long OperatingSystem::nvmSaveTimerValue = 0;

long long OperatingSystem::accessLevelGrantedTimeSnapshot = 0;
SecurityAccessLevelType OperatingSystem::currentAccessLevel = e_ACCESS_LEVEL_NONE;

long long uiBlockTime = 0;

ServiceInformation OperatingSystem::displayRamUsage()
{
    /* prevent session expiration in inspection view */
    accessLevelGrantedTimeSnapshot = millis();

    float T = 0.f;
    float t = tempSensor.readTemperature();
    if (!isnan(t))
    {
        T = t;
    }

    ServiceInformation info;
    info.ramTotal = esp_get_free_heap_size();
    info.ramFree = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
    info.ramUsed = info.ramTotal - info.ramFree;

    info.coreTemperature = T;

    return info;
}

void OperatingSystem::init()
{
    pinMode(0, INPUT_PULLUP);

    uniqueLifecycleId = (uint16_t)random(10, 10000);
    DataContainer::setSignalValue(CBK_DISPLAY_RAM_USAGE, static_cast<std::function<ServiceInformation()>>(OperatingSystem::displayRamUsage));

    DataContainer::setSignalValue(CBK_RESET_DEVICE, static_cast<std::function<void(uint16_t)>>(OperatingSystem::reset));
    DataContainer::setSignalValue(CBK_CALCULATE_RUNTIME_NODE_HASH, static_cast<std::function<uint16_t()>>(OperatingSystem::calculateRuntimeNodeHash));
    DataContainer::setSignalValue(CBK_START_NVM_SAVE_TIMER, static_cast<std::function<void()>>(OperatingSystem::activateNvmSaveTimer));

    DataContainer::setSignalValue(
        CBK_SECURITY_ACCESS_LEVEL_CHANGE_VIA_STRING,
        static_cast<std::function<void(String)>>(OperatingSystem::requestSecurityAccessLevelChangeViaString));

    DataContainer::subscribe(SIG_IS_HTTP_SERVER, [](std::any signal)
                             { isHttpServerRunning = (std::any_cast<bool>(signal)); });

    DataContainer::subscribe(SIG_IS_RC_SERVER, [](std::any signal)
                             { isRCServerRunning = (std::any_cast<bool>(signal)); });

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

    if (isHttpServerRunning)
    {
        HomeLightHttpServer::init();
    }

    if (isRCServerRunning)
    {
        RemoteControlServer::init();
        RemoteDevicesManager::init();
        DigitalButtonReceiver::init();
    }
    else
    {
        RemoteControlClient::init();
        DigitalButtonTransmitter::init();
    }

    DeviceProvider::init();
    TimeMaster::init();

    /* handle security access level grant to SERVICE MODE if there is no valid config loaded */
    NodeConfiguration currentConfig =
        std::any_cast<NodeConfiguration>(DataContainer::getSignalValue(SIG_DEVICE_CONFIGURATION));
    if (currentConfig.nodeType == 255 ||           /* invalid node type  */
        !currentConfig.networkCredentialsAvailable /* no network credentials */
    )
    {
        changeSecurityAccessLevel(e_ACCESS_LEVEL_SERVICE_MODE);

        UserInterfaceNotification notif{
            .title = "Service access granted",
            .body = "Device received service mode access due to one of the following reasons: Invalid configuration / No WiFi configured / No local devices configured",
            .type = UserInterfaceNotification::INFO};
        std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);
    }

    Serial.println("HomeStationOS:// Init completed.");
}

void OperatingSystem::task2ms()
{
    CyclicProfiler::call("NetworkDriver", NetworkDriver::cyclic);
}

void OperatingSystem::task10ms()
{

    CyclicProfiler::call("DeviceProvider", DeviceProvider::cyclic);

    if (isRCServerRunning)
    {
        CyclicProfiler::call("RemoteControlServer", RemoteControlServer::cyclic);
        CyclicProfiler::call("RemoteDevicesManager", RemoteDevicesManager::cyclic);
        CyclicProfiler::call("DigitalButtonReceiver", DigitalButtonReceiver::cyclic);
    }
    else
    {
        CyclicProfiler::call("RemoteControlClient", RemoteControlClient::cyclic);
        CyclicProfiler::call("DigitalButtonTransmitter", DigitalButtonTransmitter::cyclic);
    }
    CyclicProfiler::call("DeviceManager", DeviceManager::cyclic);
}

void OperatingSystem::task20ms()
{
}

void OperatingSystem::task50ms()
{
    CyclicProfiler::call("NotificationHandler", NotificationHandler::cyclic);
    CyclicProfiler::call("ExtendedMemoryManager", ExtendedMemoryManager::cyclic);

    if (resetPending)
    {
        resetCountdown -= 50;

        if (resetCountdown <= 0)
        {
            performReset();
        }
    }

    calculateRuntimeNodeHash();
}

void OperatingSystem::task100ms()
{
    if (isHttpServerRunning)
    {
        CyclicProfiler::call("HomeLightHttpServer", HomeLightHttpServer::cyclic);
    }
}

void OperatingSystem::activateNvmSaveTimer()
{
    Serial.println("NVM save timer activated.");
    isNvmSaveTimerActive = true;
    nvmSaveTimerValue = millis();
}

void OperatingSystem::handleNvmSaveMech()
{
    if (isNvmSaveTimerActive)
    {
        if (abs(millis() - nvmSaveTimerValue) > (1000 * 60 * 30))
        { /* 30 minutes */
            isNvmSaveTimerActive = false;
            nvmSaveTimerValue = 0;
            Serial.println("NVM save timer expired, saving NVM data.");
            saveNvmData();
        }
        else
        {
            nvmSaveTimerValue -= 1; // - 1 sec
        }
    }
}

void OperatingSystem::task1s()
{
    Serial.print(".");

    handleSecurityAccessLevelExpiration();
    CyclicProfiler::call("TimeMaster", TimeMaster::cyclic);

    detectHwMassEraseRequest();
    handleNvmSaveMech();

    static long long lastCheck = 0;

    if (millis() - lastCheck > 3000)
    {
        // displayRamUsage();
        lastCheck = millis();
    }
}

void OperatingSystem::reset(uint16_t delay)
{
    if (!resetPending)
    {
        resetPending = true;
        resetCountdown = delay;
    }
}

void OperatingSystem::saveNvmData()
{
    DeviceManager::flushNvmData();
    if (isHttpServerRunning)
    {
        HomeLightHttpServer::flushNvmData();
    }
    ExtendedMemoryManager::flushNvmData();
    ConfigProvider::flushNvmData();
}

void OperatingSystem::performReset()
{
    DeviceProvider::deinit();

    if (isRCServerRunning)
    {
        RemoteDevicesManager::deinit();
        RemoteControlServer::deinit();
        DigitalButtonReceiver::deinit();
    }
    else
    {
        RemoteControlClient::deinit();
        DigitalButtonTransmitter::deinit();
    }

    DeviceManager::deinit();

    if (isHttpServerRunning)
    {
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
    if (currentAccessLevel > e_ACCESS_LEVEL_NONE)
    {
        if (abs(millis() - accessLevelGrantedTimeSnapshot) > timeToExpireAccessLevel)
        {
            accessLevelGrantedTimeSnapshot = 0;
            currentAccessLevel = e_ACCESS_LEVEL_NONE;
            DataContainer::setSignalValue(SIG_SECURITY_ACCESS_LEVEL, currentAccessLevel);
            Serial.println("Access level unlock expired.");

            /* notification */
            UserInterfaceNotification notif{
                .title = "Device lock/unlock",
                .body = "Unlock time expired, device is locked again.",
                .type = UserInterfaceNotification::INFO};
            std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);
        }
    }
}

/* Funkcja do obliczenia unikalnego identyfikatora (hasha) dla bieżącego stanu systemu.*/
uint16_t OperatingSystem::calculateRuntimeNodeHash()
{
    uint64_t hash = 0;

    // hash += uniqueLifecycleId;

    /* Get configuration */
    try
    {
        NodeConfiguration configuration =
            std::any_cast<NodeConfiguration>(
                DataContainer::getSignalValue(SIG_DEVICE_CONFIGURATION));
        hash += configuration.isHttpServer;
        hash += configuration.isRcServer;
        hash += configuration.isDefaultUserAdmin;
        hash += configuration.networkCredentialsAvailable;
        hash += configuration.nodeType;
        for (uint8_t idx = 0; idx < configuration.networkSSID.length(); idx++)
        {
            hash += configuration.networkSSID.charAt(idx);
        }
        for (uint8_t idx = 0; idx < configuration.networkPassword.length(); idx++)
        {
            hash += configuration.networkPassword.charAt(idx);
        }
    }
    catch (std::bad_any_cast ex)
    {
    }

    /* Get devices data */
    try
    {
        std::vector<DeviceDescription> devicesVector =
            std::any_cast<std::vector<DeviceDescription>>(
                DataContainer::getSignalValue(SIG_DEVICE_COLLECTION));
        for (auto &device : devicesVector)
        {
            hash += device.deviceId;
            hash += device.macAddress;
            hash += device.isEnabled;
            for (uint8_t idx = 0; idx < NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION; idx++)
            {
                hash += device.customBytes[idx];
            }
            for (uint8_t idx = 0; idx < device.deviceName.length(); idx++)
            {
                hash += device.deviceName.charAt(idx);
            }
        }
    }
    catch (std::bad_any_cast ex)
    {
    }

    // Serial.println("Hash : " + String((int)hash));

    memcpy(&runtimeNodeHash, &hash, sizeof(uint16_t));
    DataContainer::setSignalValue(SIG_RUNTIME_NODE_HASH, static_cast<uint16_t>(runtimeNodeHash));
    // Serial.println("New hash : " + String((int)hash));

    return hash;
}

void OperatingSystem::requestSecurityAccessLevelChangeViaString(String password)
{
    NodeConfiguration currentConfig = std::any_cast<NodeConfiguration>(DataContainer::getSignalValue(SIG_DEVICE_CONFIGURATION));
    if (password == "admin")
    {
        changeSecurityAccessLevel(e_ACCESS_LEVEL_SERVICE_MODE);
    }
    else if (password == currentConfig.panelPassword)
    { /* check is password for the user matches */
        SecurityAccessLevelType userRights = e_ACCESS_LEVEL_AUTH_USER;

        if (currentConfig.isDefaultUserAdmin)
        { /* verify if default user can have admin rights */
            userRights = e_ACCESS_LEVEL_SERVICE_MODE;
        }
        changeSecurityAccessLevel(userRights);
    }
    else
    {
        /* notification */
        UserInterfaceNotification notif{
            .title = "Wrong password",
            .body = "Device unlock try failed due to invalid password.",
            .type = UserInterfaceNotification::WARNING};
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
        .type = UserInterfaceNotification::WARNING};

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

void OperatingSystem::detectHwMassEraseRequest()
{
    static uint8_t activationTimeCounter = 0;
    static bool isMassEraseRequestActivated = false;

    if (isMassEraseRequestActivated)
    { /* keep it first, to execute reset in the next execution to avoid reset when bootloader btn is pressed */
        if (digitalRead(0) == 1)
        { /* button was released, reset can be performed */
            std::any_cast<std::function<void(void)>>(DataContainer::getSignalValue(CBK_MASS_ERASE))();
            std::any_cast<std::function<void(uint16_t)>>(DataContainer::getSignalValue(CBK_RESET_DEVICE))(1000);
        }
    }

    if (digitalRead(0) == 0)
    { /* Button is pressed */
        activationTimeCounter++;
    }
    else
    {
        activationTimeCounter = 0; /* reset activation timer */
    }

    if (activationTimeCounter > 3)
    { /* 5 sec to activate reset */
        isMassEraseRequestActivated = true;
        Serial.println("OS://HW MASS ERASE ACTIVATED.");
    }
}
