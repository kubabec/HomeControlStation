#include <os/app/config/ConfigProvider.hpp>
#include "os/Logger.hpp"

#include <os/app/config/PersistentMemoryAccess.hpp>
#include <os/app/config/ExtendedMemoryManager.hpp>
#include <os/drivers/networkdriver.hpp>
#include <ArduinoJson.h>
#include "os/datacontainer/datacontainertypes.hpp"
#include <Regexp.h>

/**
 * @file src/os/app/config/configprovider.cpp
 * @brief Configuration persistence and runtime configuration logic for the Home Control Station.
 */

ConfigData ConfigProvider::configRamMirror = {};
PersistentDataBlock ConfigProvider::dataBlocksRamMirror[PersistentDatablockID::e_NUMBER_OF_PERSISTENT_BLOCKS] = {'\0'};
uint16_t ConfigProvider::totalNvmSize = 0;
bool ConfigProvider::nvmDataAvailable = false;
uint8_t ConfigProvider::restoredPersistentBlockCount = e_NUMBER_OF_PERSISTENT_BLOCKS;

void ConfigProvider::init()
{
    Logger::log("ConfigProvider init ...");
    totalNvmSize = configRamMirror.getSize() + PersistentDatablockID::e_NUMBER_OF_PERSISTENT_BLOCKS * PersistentDataBlock::getSize();

    PersistentMemoryAccess::init(totalNvmSize, MAX_EXT_MEMORY_SIZE_TOTAL);

    if (readRamMirrorFromNvm())
    {
        Logger::log("safe flag : " + String((int)configRamMirror.safeShutdownFlag));
        if (configRamMirror.safeShutdownFlag != 37)
        {
            /* Unexpected reset detected! */
            UserInterfaceNotification notif{
                .title = "Unexpected reset",
                .body = "Device was restarted unexpectedly.",
                .type = UserInterfaceNotification::ERROR};
            std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);
        }

        configRamMirror.safeShutdownFlag = 0;

        // Share WifiInitialConnection flag with NetworkDriver
        NetworkDriver::isWifiFirstConnectionTry = configRamMirror.wifiCredentialsChanged;
        configRamMirror.wifiCredentialsChanged = 0; // reset the flag after sharing with NetworkDriver
        saveRamMirrorToNvm();
        configRamMirror.serialPrint();

        UserInterfaceNotification notif;
        if (configRamMirror.networkSSID[0] == '\0')
        {
            notif.title = "Missing WiFi credentials";
            notif.body = "To connect to the WiFi network, navigate to 'Settings', enter network name and password.";
            notif.type = UserInterfaceNotification::INFO;
            std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);
        }
    }
    else
    {

        Logger::log("ConfigProvider:: Reading EEPROM failed, loading default configuration ...");
        NodeConfiguration emptyConfiguration;
        // Master device as default;
        emptyConfiguration.isHttpServer = true;
        emptyConfiguration.isRcServer = true;
        emptyConfiguration.networkCredentialsAvailable = false;
        emptyConfiguration.nodeType = compiledHardwareNodeType();

        emptyConfiguration.panelPassword = "admin";

        /* Write DEFAULT configuration to DataContainer */
        // Obsolete signal to be removed
        DataContainer::setSignalValue(SIG_IS_HTTP_SERVER, static_cast<bool>(true));
        // Obsolete signal to be removed
        DataContainer::setSignalValue(SIG_IS_RC_SERVER, static_cast<bool>(true));
        // New signal to be used in the future implementation
        DataContainer::setSignalValue(SIG_DEVICE_CONFIGURATION, emptyConfiguration);

        /* notification */
        UserInterfaceNotification notif{
            .title = "Configuration problem",
            .body = "Looks like this device was never configured or there was some problem during configuration restore process. Please go to 'Settings' and create device configuration.",
            .type = UserInterfaceNotification::WARNING};
        std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);

        // /* As device cannot restore correct configuration, Service Mode is granted by default */
        // DataContainer::setSignalValue(SIG_SECURITY_ACCESS_LEVEL, e_ACCESS_LEVEL_SERVICE_MODE); /* moved to OS logic */
    }

    DeviceConfigManipulationAPI cfgControls = {
        .setDeviceCfgViaJson = ConfigProvider::setConfigViaString,
        .getDeviceCfgJson = ConfigProvider::getConfigJson,
        .loadConfigFromFile = ConfigProvider::loadConfigFromFile};

    DataContainer::setSignalValue(
        SIG_SET_CONFIG_VIA_JSON_STRING,
        static_cast<DeviceConfigManipulationAPI>(cfgControls));

    DataContainer::setSignalValue(
        CBK_SET_NVM_DATABLOCK,
        static_cast<std::function<bool(PersistentDatablockID, uint8_t *)>>(ConfigProvider::setDatablock));

    DataContainer::setSignalValue(
        CBK_GET_NVM_DATABLOCK,
        static_cast<std::function<bool(PersistentDatablockID, uint8_t *)>>(ConfigProvider::getDatablock));

    DataContainer::setSignalValue(
        CBK_MASS_ERASE,
        static_cast<std::function<void(void)>>(ConfigProvider::massErase));

    DataContainer::setSignalValue(
        CBK_RESTORE_OLD_WIFI_CREDENTIALS_AND_REBOOT,
        static_cast<std::function<void(void)>>(ConfigProvider::restoreOldWiFiCredentialsAndReboot));

    DataContainer::setSignalValue(
        CBK_SET_DAY_TIME_CONFIG,
        static_cast<std::function<bool(const DayTimeRanges &, int8_t)>>(ConfigProvider::setDayTimeConfig));

    Logger::log("... done");
}

bool ConfigProvider::setDayTimeConfig(const DayTimeRanges &ranges, int8_t timeZone)
{
    if (!DayTime::isTimeZoneValid(timeZone) || !(DayTime::isUnset(ranges) || DayTime::isValid(ranges)))
    {
        Logger::log("ConfigProvider:// Invalid day time configuration rejected");
        return false;
    }

    memcpy(configRamMirror.dayTimeRanges, &ranges, sizeof(DayTimeRanges));
    configRamMirror.timeZoneOffset = (uint8_t)(timeZone + DayTime::TIME_ZONE_NVM_BIAS);
    updateNodeConfigurationSignal();

    if (!flushNvmData())
    {
        Logger::log("ConfigProvider:// Day time configuration NVM commit failed");
        return false;
    }

    /* TimeMaster registers this callback; it is absent on nodes without time service */
    std::any notify = DataContainer::getSignalValue(CBK_DAY_TIME_CONFIG_CHANGED);
    if (auto cbk = std::any_cast<std::function<void(const DayTimeRanges &, int8_t)>>(&notify))
    {
        (*cbk)(ranges, timeZone);
    }
    return true;
}

void ConfigProvider::updateNodeConfigurationSignal()
{
    NodeConfiguration validConfiguration;
    validConfiguration.isHttpServer = configRamMirror.isHttpServer;
    validConfiguration.isRcServer = configRamMirror.isRcServer;
    validConfiguration.networkSSID = String(configRamMirror.networkSSID);
    validConfiguration.networkPassword = String(configRamMirror.networkPassword);
    validConfiguration.nodeType = configRamMirror.nodeType;
    validConfiguration.configurationInitialized = isConfigurationInitialized();
    validConfiguration.panelPassword = String(configRamMirror.panelPassword);
    memcpy(&validConfiguration.dayTimeRanges, configRamMirror.dayTimeRanges, sizeof(DayTimeRanges));
    {
        const int zone = (int)configRamMirror.timeZoneOffset - DayTime::TIME_ZONE_NVM_BIAS;
        validConfiguration.timeZoneOffset = DayTime::isTimeZoneValid(zone) ? (int8_t)zone : DayTime::DEFAULT_TIME_ZONE;
    }

    if (validConfiguration.networkSSID.length() > 0)
    {
        validConfiguration.networkCredentialsAvailable = true;
    }
    Logger::log(validConfiguration.networkPassword);

    /* Write valid  configuration to DataContainer */
    // Obsolete signal to be removed
    DataContainer::setSignalValue(SIG_IS_HTTP_SERVER, static_cast<bool>(validConfiguration.isHttpServer));
    // Obsolete signal to be removed
    DataContainer::setSignalValue(SIG_IS_RC_SERVER, static_cast<bool>(validConfiguration.isRcServer));

    // New signal to be used in the future implementation
    DataContainer::setSignalValue(SIG_DEVICE_CONFIGURATION, validConfiguration);

    Logger::log("NVM configuration restored successfully.");
}

bool ConfigProvider::isConfigurationInitialized()
{
    return configRamMirror.configurationState == CONFIGURATION_INITIALIZED_MARKER;
}

void ConfigProvider::cyclic()
{
}

String ConfigProvider::getConfigJson()
{
    String nodeCfgJson = "\"NodeConfig\":[";

    /* Values initialization */
    String isHttpServerActive = configRamMirror.isHttpServer == true ? "true" : "false";
    String isRcServerActive = configRamMirror.isRcServer == true ? "true" : "false";
    String hasUserAdminRights = "true";
    String nodeType = String((int)configRamMirror.nodeType);
    String networkSSID = String(configRamMirror.networkSSID);
    String networkPassword = String(configRamMirror.networkPassword);
    String panelPassword = String(configRamMirror.panelPassword);

    /* Add values to JSON */
    nodeCfgJson += "{";
    nodeCfgJson += "\"isHttpActive\":" + isHttpServerActive + ",";
    nodeCfgJson += "\"isRcServer\":" + isRcServerActive + ",";
    nodeCfgJson += "\"isUsrAdmin\":" + hasUserAdminRights + ",";
    nodeCfgJson += "\"nodeType\":\"" + nodeType + "\",";
    nodeCfgJson += "\"ssid\":\"" + networkSSID + "\",";
    nodeCfgJson += "\"wifiPass\":\"" + networkPassword + "\",";
    nodeCfgJson += "\"usrPass\":\"" + panelPassword + "\"";
    nodeCfgJson += "}";

    nodeCfgJson += "]";

    return nodeCfgJson;
}

bool isNotNull(const String &str)
{
    return (str != "null");
}

bool ConfigProvider::loadConfigFromFile(JsonDocument &doc)
{

    String isHttpServerActive = String(doc["NodeConfig"][0]["isHttpActive"]);
    String isRcServerActive = String(doc["NodeConfig"][0]["isRcServer"]);
    String networkSSID = String(doc["NodeConfig"][0]["ssid"]);
    String networkPassword = String(doc["NodeConfig"][0]["wifiPass"]);
    String panelPassword = String(doc["NodeConfig"][0]["usrPass"]);

    // Logger::log(isHttpServerActive + " " + isRcServerActive + " " + hasUserAdminRights+ " " +  nodeType+ " " +  networkSSID+ " " +  networkPassword+ " " +  panelPassword);

    /*Assign parameters to RAM mirror whenever there was valid value in the configuration file */
    if (isNotNull(isHttpServerActive))
    {
        configRamMirror.isHttpServer = isHttpServerActive == "true" ? 1 : 0;
    }
    if (isNotNull(isRcServerActive))
    {
        configRamMirror.isRcServer = isRcServerActive == "true" ? 1 : 0;
    }

    configRamMirror.nodeType = compiledHardwareNodeType();
    if (isNotNull(networkSSID))
    {
        configRamMirror.setSSID(networkSSID);
    }
    if (isNotNull(networkPassword))
    {
        configRamMirror.setPassword(networkPassword);
    }
    if (isNotNull(panelPassword))
    {
        configRamMirror.setPanelPassword(panelPassword);
    }
    if (isNotNull(networkSSID) && networkSSID.length() > 0 &&
        isNotNull(panelPassword) && panelPassword.length() > 0)
    {
        configRamMirror.configurationState = CONFIGURATION_INITIALIZED_MARKER;
    }

    /* no failure */
    return false;
}

bool ConfigProvider::setConfigViaString(String &configString)
{
    bool retVal = false;

    SecurityAccessLevelType currentAccessLevel =
        std::any_cast<SecurityAccessLevelType>(DataContainer::getSignalValue(SIG_SECURITY_ACCESS_LEVEL));

    UserInterfaceNotification notification;
    notification.title = "Configuration change failure";
    notification.type = UserInterfaceNotification::ERROR;

    /* Check if device is unlocked that config can be modified */
    if (currentAccessLevel > e_ACCESS_LEVEL_NONE)
    {
        configString.replace("%7B", "{");
        configString.replace("%22", "\"");
        configString.replace("%7D", "}");
        configString.replace("%20", " ");
        configString.replace("newCfgApply&", "");

        JsonDocument doc;
        DeserializationError success = deserializeJson(doc, configString.c_str());
        if (success == DeserializationError::Code::Ok)
        {
            String isHttpServerActive = String("yes" /*doc["httpActive"]*/);
            String isRcServerActive = String(doc["rcServerActive"]);
            String networkSSID = String(doc["network"]);
            String networkPassword = String(doc["netPwd"]);
            String panelPassword = String(doc["cfgPwd"]);

            if (isHttpServerActive.length() > 0 && isRcServerActive.length() > 0 &&
                networkSSID.length() > 0 && panelPassword.length() > 0)
            {
                /* Some of configs are only allowed to be changed in Service mode */
                if (currentAccessLevel >= e_ACCESS_LEVEL_SERVICE_MODE)
                {
                    configRamMirror.isHttpServer = isHttpServerActive == "yes" ? 1 : 0;
                    configRamMirror.isRcServer = isRcServerActive == "yes" ? 1 : 0;
                    configRamMirror.nodeType = compiledHardwareNodeType();
                }

                configRamMirror.setSSID(networkSSID);
                configRamMirror.setPassword(networkPassword);
                configRamMirror.setPanelPassword(panelPassword);
                configRamMirror.configurationState = CONFIGURATION_INITIALIZED_MARKER;
                configRamMirror.wifiCredentialsChanged = 1; // Flag to mark the change after reboot

                Logger::log("Applying following configuration :");
                configRamMirror.serialPrint();

                /* return success */
                retVal = true;
            }
            else
            {
                /* Some of JSON fields was not fulfilled */
                notification.body = "Some configuration parameters are missing";
                std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notification);
            }
        }
        else
        {
            Logger::log("ConfigProvider://Problem with JSON parsing.");
            notification.body = "Config JSON content cannot be correctly evaluated";
            std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notification);
        }
    }
    else
    {
        /* No access level to apply */
        notification.body = "Device cannot be locked to apply new configuration";
        std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notification);
    }

    return retVal;
}

bool ConfigProvider::setCredentialsOnly(const String &ssid, const String &wifiPassword, const String &panelPassword)
{
    SecurityAccessLevelType currentAccessLevel =
        std::any_cast<SecurityAccessLevelType>(DataContainer::getSignalValue(SIG_SECURITY_ACCESS_LEVEL));
    if (currentAccessLevel < e_ACCESS_LEVEL_SERVICE_MODE ||
        ssid.length() == 0 || ssid.length() >= SSID_LENGTH ||
        panelPassword.length() == 0 || panelPassword.length() >= PASSWORD_LENGTH ||
        wifiPassword.length() >= PASSWORD_LENGTH)
    {
        return false;
    }

    String ssidCopy = ssid;
    String wifiPasswordCopy = wifiPassword;
    String panelPasswordCopy = panelPassword;
    configRamMirror.setSSID(ssidCopy);
    configRamMirror.setPassword(wifiPasswordCopy);
    configRamMirror.setPanelPassword(panelPasswordCopy);
    configRamMirror.configurationState = CONFIGURATION_INITIALIZED_MARKER;
    configRamMirror.wifiCredentialsChanged = 1; // Flag to mark the change after reboot
    return true;
}

bool ConfigProvider::saveRamMirrorToNvm()
{
    bool retVal = false;

    /* Get size of : Device configuration + all the datablocks to save merged data buffer into NvM */
    uint16_t mergedBufferSize = totalNvmSize;

    /* Create merged buffer */
    uint8_t *mergedBuffer = (uint8_t *)malloc(mergedBufferSize);

    /* Dynamic buffer allocation succeeded */
    if (mergedBuffer != nullptr)
    {
        /* Copy device config to the buffer */
        memcpy(mergedBuffer, &configRamMirror, configRamMirror.getSize());

        /* Copy datablocks to the buffer */
        for (uint8_t block = 0; block < PersistentDatablockID::e_NUMBER_OF_PERSISTENT_BLOCKS; ++block)
        {
            memcpy(mergedBuffer + configRamMirror.getSize() + block * PersistentDataBlock::getSize(),
                   dataBlocksRamMirror[block].data, PersistentDataBlock::getSize());
        }

        /* Try to save entire mergedBuffer into persistent memory */
        const uint16_t restoredDataSize = configRamMirror.getSize() +
                                          restoredPersistentBlockCount * PersistentDataBlock::getSize();
        const bool saved = restoredPersistentBlockCount < e_NUMBER_OF_PERSISTENT_BLOCKS
                               ? PersistentMemoryAccess::saveDataMigratingExtendedMemory(
                                     mergedBuffer, mergedBufferSize, restoredDataSize, MAX_EXT_MEMORY_SIZE_TOTAL)
                               : PersistentMemoryAccess::saveData(mergedBuffer, mergedBufferSize);
        if (saved)
        {
            restoredPersistentBlockCount = e_NUMBER_OF_PERSISTENT_BLOCKS;
            /* Update data container signals */
            updateNodeConfigurationSignal();
            retVal = true;
        }
        else
        {
            Logger::log("Saving config failed!");
        }

        /* Release merged buffer resources */
        free(mergedBuffer);
    }

    return retVal;
}

bool ConfigProvider::readRamMirrorFromNvm()
{
    bool retVal = false;

    /* Get size of : Device configuration + all the datablocks to save merged data buffer into NvM */
    uint16_t mergedBufferSize = totalNvmSize;

    /* Create merged buffer */
    uint8_t *mergedBuffer = (uint8_t *)malloc(mergedBufferSize);

    /* Dynamic buffer allocation succeeded */
    if (mergedBuffer != nullptr)
    {

        /* Try to read NvM data to mergedBuffer */
        if (PersistentMemoryAccess::readData(mergedBuffer, mergedBufferSize))
        {
            nvmDataAvailable = true;
            retVal = true;

            /* Copy device configuration */
            memcpy(&configRamMirror, mergedBuffer, configRamMirror.getSize());

            const bool legacyConfigurationExists =
                (!isHardwareNodeType(configRamMirror.nodeType) &&
                 configRamMirror.nodeType != UNDEFINED_HARDWARE_NODE_TYPE) ||
                configRamMirror.networkSSID[0] != '\0' ||
                configRamMirror.panelPassword[0] != '\0';
            const bool requiresMigration =
                configRamMirror.nodeType != compiledHardwareNodeType() ||
                (!isConfigurationInitialized() && legacyConfigurationExists);
            if (requiresMigration)
            {
                configRamMirror.nodeType = compiledHardwareNodeType();
                if (legacyConfigurationExists)
                {
                    configRamMirror.configurationState = CONFIGURATION_INITIALIZED_MARKER;
                }
            }

            /* Copy datablocks to ram mirror */
            for (uint8_t block = 0; block < PersistentDatablockID::e_NUMBER_OF_PERSISTENT_BLOCKS; ++block)
            {
                memcpy(dataBlocksRamMirror[block].data,
                       mergedBuffer + configRamMirror.getSize() + block * PersistentDataBlock::getSize(),
                       PersistentDataBlock::getSize());
            }

            updateNodeConfigurationSignal();
        }
        else
        {
            constexpr uint8_t historicalBlockCounts[] = {
                CONDITION_LAYOUT_BLOCK_COUNT,
                LEGACY_PERSISTENT_BLOCK_COUNT};
            for (const uint8_t historicalBlockCount : historicalBlockCounts)
            {
                const uint16_t historicalBufferSize = configRamMirror.getSize() +
                                                      historicalBlockCount * PersistentDataBlock::getSize();
                uint8_t *historicalBuffer = static_cast<uint8_t *>(malloc(historicalBufferSize));
                if (historicalBuffer != nullptr &&
                    PersistentMemoryAccess::readData(historicalBuffer, historicalBufferSize))
                {
                    memcpy(&configRamMirror, historicalBuffer, configRamMirror.getSize());
                    const bool legacyConfigurationExists =
                        (!isHardwareNodeType(configRamMirror.nodeType) &&
                         configRamMirror.nodeType != UNDEFINED_HARDWARE_NODE_TYPE) ||
                        configRamMirror.networkSSID[0] != '\0' ||
                        configRamMirror.panelPassword[0] != '\0';
                    configRamMirror.nodeType = compiledHardwareNodeType();
                    if (legacyConfigurationExists)
                    {
                        configRamMirror.configurationState = CONFIGURATION_INITIALIZED_MARKER;
                    }
                    for (uint8_t block = 0; block < historicalBlockCount; ++block)
                    {
                        memcpy(dataBlocksRamMirror[block].data,
                               historicalBuffer + configRamMirror.getSize() + block * PersistentDataBlock::getSize(),
                               PersistentDataBlock::getSize());
                    }
                    for (uint8_t block = historicalBlockCount;
                         block < PersistentDatablockID::e_NUMBER_OF_PERSISTENT_BLOCKS; ++block)
                    {
                        memset(dataBlocksRamMirror[block].data, 0, PersistentDataBlock::getSize());
                    }
                    nvmDataAvailable = true;
                    restoredPersistentBlockCount = historicalBlockCount;
                    retVal = true;
                    updateNodeConfigurationSignal();
                    Logger::log("ConfigProvider:// Migrating persistent layout with " +
                                String((int)historicalBlockCount) + " blocks");
                }
                free(historicalBuffer);
                if (retVal)
                {
                    break;
                }
            }
            if (!retVal)
                nvmDataAvailable = false;
        }

        /* Release resources */
        free(mergedBuffer);
    }

    return retVal;
}

bool ConfigProvider::setDatablock(PersistentDatablockID blockID, uint8_t *data)
{
    bool retVal = false;
    if (blockID <= e_PERSISTENT_BLOCK_LAST && data != nullptr)
    {
        memcpy(
            &dataBlocksRamMirror[blockID],
            data,
            PersistentDataBlock::getSize());

        // retVal = saveRamMirrorToNvm();
        retVal = true;
    }

    return retVal;
}

bool ConfigProvider::getDatablock(PersistentDatablockID blockID, uint8_t *buffer)
{
    bool retVal = false;
    /* Is nvm data read and available? */
    if (nvmDataAvailable && (blockID <= e_PERSISTENT_BLOCK_LAST) && buffer != nullptr)
    {
        retVal = true;

        memcpy(
            buffer,
            &dataBlocksRamMirror[blockID],
            PersistentDataBlock::getSize());
    }

    return retVal;
}

void ConfigProvider::eraseDatablockMemory()
{
    Logger::log("Datablock erase start...");
    uint8_t eraserData[PERSISTENT_DATABLOCK_SIZE] = {0xFF};

    for (uint8_t datablock = e_PERSISTENT_BLOCK_FIRST; datablock <= e_PERSISTENT_BLOCK_LAST; datablock++)
    {
        if (!setDatablock((PersistentDatablockID)datablock, eraserData))
        {
            Logger::log("Erasing failed!");
            return;
        }
    }

    nvmDataAvailable = false;

    Logger::log("Datablock erase done.");
}

bool ConfigProvider::flushNvmData()
{
    return saveRamMirrorToNvm();
}

void ConfigProvider::deinit()
{
    configRamMirror.safeShutdownFlag = 37;
    flushNvmData();
}

void ConfigProvider::massErase()
{
    PersistentMemoryAccess::massErase(totalNvmSize);
}

void ConfigProvider::restoreOldWiFiCredentialsAndReboot()
{
    memcpy(configRamMirror.networkSSID, configRamMirror.legacySSID, SSID_LENGTH);
    memcpy(configRamMirror.networkPassword, configRamMirror.legacyPassword, PASSWORD_LENGTH);

    // get OS reboot function signal CBK_RESET_DEVICE
    std::any_cast<std::function<void(uint16_t)>>(DataContainer::getSignalValue(CBK_RESET_DEVICE))(1000);
}

