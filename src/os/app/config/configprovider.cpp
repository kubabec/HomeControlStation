#include <os/app/config/ConfigProvider.hpp>

#include <os/app/config/PersistentMemoryAccess.hpp>
#include <ArduinoJson.h>
#include <Regexp.h>



ConfigData ConfigProvider::configRamMirror = {255, 0, 0, 1, 255, "\0", "\0", "\0"};
PersistentDataBlock ConfigProvider::dataBlocksRamMirror[NUMBER_OF_PERSISTENT_DATABLOCKS] = {'\0'};
uint16_t ConfigProvider::totalNvmSize = 0;
bool ConfigProvider::nvmDataAvailable = false;

void ConfigProvider::init()
{
    Serial.println("ConfigProvider init ...");
    totalNvmSize = configRamMirror.getSize() + NUMBER_OF_PERSISTENT_DATABLOCKS * PersistentDataBlock::getSize();

    PersistentMemoryAccess::init(totalNvmSize);

/* TEST BLOCK TO SAVE EEPROM DATA ONCE */
    // eraseDatablockMemory();

    // configRamMirror.isHttpServer = true;
    // configRamMirror.isRcServer = true;
    // String ssid("Orange_Swiatlowod_DA2C");
    // String pwd("2FYXFG6MAGVZ");
    // configRamMirror.setSSID(ssid);
    // configRamMirror.setPassword(pwd);

    // saveRamMirrorToNvm();
/* TEST BLOCK TO SAVE EEPROM DATA ONCE */


    if(readRamMirrorFromNvm())
    {
        Serial.println("safe flag : " + String((int)configRamMirror.safeShutdownFlag ));
        if(configRamMirror.safeShutdownFlag != 37)
        {
            /* Unexpected reset detected! */
            std::any_cast<std::function<void(ERR_MON_ERROR_TYPE, String)>>(
                DataContainer::getSignalValue(CBK_ERROR_REPORT))(
                    ERR_MON_UNEXPECTED_RESET, 
                    "Unexpected reset occured"
                );
        }

        configRamMirror.safeShutdownFlag = 0;
        saveRamMirrorToNvm();
        configRamMirror.serialPrint();

        UserInterfaceNotification notif;
        if(configRamMirror.networkSSID[0] == '\0'){
            notif.title = "Missing WiFi credentials";
            notif.body = "To connect to the WiFi network, navigate to 'Settings', enter network name and password.";
            notif.type = UserInterfaceNotification::INFO;
            std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);
        }

    }else 
    {
        /* error unused anymore */
        // std::any_cast<std::function<void(ERR_MON_ERROR_TYPE, String)>>(
        //     DataContainer::getSignalValue(CBK_ERROR_REPORT)
        //     )(
        //         ERR_MON_INVALID_NVM_DATA,
        //         "Unable to restore NVM data"
        //     );

    
        Serial.println("ConfigProvider:: Reading EEPROM failed, loading default configuration ...");
        NodeConfiguration emptyConfiguration;
        // Master device as default;
        emptyConfiguration.isHttpServer = true;
        emptyConfiguration.isRcServer = true;
        emptyConfiguration.isDefaultUserAdmin = true;
        emptyConfiguration.networkCredentialsAvailable = false;
        emptyConfiguration.nodeType = 255;

        emptyConfiguration.panelPassword = "user";
        
        /* Write DEFAULT configuration to DataContainer */
        // Obsolete signal to be removed
        DataContainer::setSignalValue(SIG_IS_HTTP_SERVER, static_cast<bool> (true));
        // Obsolete signal to be removed
        DataContainer::setSignalValue(SIG_IS_RC_SERVER, static_cast<bool> (true));
        // New signal to be used in the future implementation
        DataContainer::setSignalValue(SIG_DEVICE_CONFIGURATION, emptyConfiguration);

        /* notification */
        UserInterfaceNotification notif{
            .title = "Configuration problem",
            .body = "Looks like this device was never configured or there was some problem during configuration restore process. Please go to 'Settings' and create device configuration.",
            .type = UserInterfaceNotification::WARNING
        };
        std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);


        // /* As device cannot restore correct configuration, Service Mode is granted by default */
        // DataContainer::setSignalValue(SIG_SECURITY_ACCESS_LEVEL, e_ACCESS_LEVEL_SERVICE_MODE); /* moved to OS logic */
    }

    
    DataContainer::setSignalValue(
        CBK_SET_CONFIG_VIA_JSON_STRING,
        static_cast<std::function<bool(String&)>>(ConfigProvider::setConfigViaString));

    DataContainer::setSignalValue(
        CBK_SET_NVM_DATABLOCK,
        static_cast<std::function<bool(PersistentDatablockID, uint8_t*)>>(ConfigProvider::setDatablock));

    DataContainer::setSignalValue(
        CBK_GET_NVM_DATABLOCK,
        static_cast<std::function<bool(PersistentDatablockID, uint8_t*)>>(ConfigProvider::getDatablock));

    DataContainer::setSignalValue(
        CBK_MASS_ERASE,
        static_cast<std::function<void(void)>>(ConfigProvider::massErase));


    Serial.println("... done");
}

void ConfigProvider::updateNodeConfigurationSignal()
{
    NodeConfiguration validConfiguration;
    validConfiguration.isHttpServer = configRamMirror.isHttpServer;
    validConfiguration.isRcServer = configRamMirror.isRcServer;
    validConfiguration.networkSSID = String(configRamMirror.networkSSID);
    validConfiguration.isDefaultUserAdmin = configRamMirror.isDefaultUserAdmin;
    validConfiguration.networkPassword = String(configRamMirror.networkPassword);
    validConfiguration.nodeType = configRamMirror.nodeType;
    validConfiguration.panelPassword = String(configRamMirror.panelPassword);

    if(validConfiguration.networkPassword.length() > 0  && validConfiguration.networkSSID.length() > 0){
        validConfiguration.networkCredentialsAvailable = true;
    }
    Serial.println(validConfiguration.networkPassword);

    /* Write valid  configuration to DataContainer */
    // Obsolete signal to be removed
    DataContainer::setSignalValue(SIG_IS_HTTP_SERVER, static_cast<bool> (validConfiguration.isHttpServer));
    // Obsolete signal to be removed
    DataContainer::setSignalValue(SIG_IS_RC_SERVER, static_cast<bool> (validConfiguration.isRcServer));
    
    // New signal to be used in the future implementation
    DataContainer::setSignalValue(SIG_DEVICE_CONFIGURATION, validConfiguration);

    Serial.println("NVM configuration restored successfully.");
}

void ConfigProvider::cyclic()
{
    
}


bool ConfigProvider::setConfigViaString(String& configString)
{
    bool retVal = false;

    SecurityAccessLevelType currentAccessLevel = 
        std::any_cast<SecurityAccessLevelType>(DataContainer::getSignalValue(SIG_SECURITY_ACCESS_LEVEL));

    UserInterfaceNotification notification;
    notification.title = "Configuration change failure";
    notification.type = UserInterfaceNotification::ERROR;

    /* Check if device is unlocked that config can be modified */
    if(currentAccessLevel > e_ACCESS_LEVEL_NONE){
        configString.replace("%7B", "{");
        configString.replace("%22", "\"");
        configString.replace("%7D", "}");
        configString.replace("newCfgApply&", "");

        JsonDocument doc;
        DeserializationError success = deserializeJson(doc, configString.c_str());
        if(success == DeserializationError::Code::Ok){
            String isHttpServerActive       = String(doc["httpActive"]);
            String isRcServerActive         = String(doc["rcServerActive"]);
            String hasUserAdminRights       = String(doc["usrAdmin"]);
            String nodeType                 = String(doc["type"]);
            String networkSSID              = String(doc["network"]);
            String networkPassword          = String(doc["netPwd"]);
            String panelPassword            = String(doc["cfgPwd"]);

            if(isHttpServerActive.length() > 0 && isRcServerActive.length() > 0 && hasUserAdminRights.length() > 0 &&
               nodeType.length() > 0 && networkSSID.length() > 0 && networkPassword.length() > 0 && panelPassword.length() > 0 ) {
                /* Some of configs are only allowed to be changed in Service mode */
                if(currentAccessLevel >= e_ACCESS_LEVEL_SERVICE_MODE)   {
                    configRamMirror.isHttpServer = isHttpServerActive == "yes" ? 1 : 0;
                    configRamMirror.isRcServer = isRcServerActive == "yes" ? 1 : 0;
                    configRamMirror.isDefaultUserAdmin = hasUserAdminRights == "yes" ? 1 : 0;
                    configRamMirror.nodeType = nodeType.toInt() < 10 ? nodeType.toInt() : 255;
                }

                configRamMirror.setSSID(networkSSID);
                configRamMirror.setPassword(networkPassword);
                configRamMirror.setPanelPassword(panelPassword);

                Serial.println("Applying following configuration :");
                configRamMirror.serialPrint();

                /* return success */
                retVal = true;
            }else {
                /* Some of JSON fields was not fulfilled */
                notification.body = "Some configuration parameters are missing";
                std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notification);
            }
            
        }else {
            Serial.println("ConfigProvider://Problem with JSON parsing.");
            notification.body = "Config JSON content cannot be correctly evaluated";
            std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notification);
        }
    } else 
    {
        /* No access level to apply */
        notification.body = "Device cannot be locked to apply new configuration";
        std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notification);
    }


    return retVal;
}


bool ConfigProvider::saveRamMirrorToNvm()
{
    bool retVal = false;

    /* Get size of : Device configuration + all the datablocks to save merged data buffer into NvM */
    uint16_t mergedBufferSize = totalNvmSize;

    /* Create merged buffer */
    uint8_t* mergedBuffer = (uint8_t*) malloc (mergedBufferSize);

    /* Dynamic buffer allocation succeeded */
    if(mergedBuffer != nullptr) {
        /* Copy device config to the buffer */
        memcpy(mergedBuffer, &configRamMirror, configRamMirror.getSize());

        /* Copy datablocks to the buffer */
        memcpy(
            mergedBuffer + configRamMirror.getSize(),
            dataBlocksRamMirror->data,
            NUMBER_OF_PERSISTENT_DATABLOCKS * PersistentDataBlock::getSize()
        );

        /* Try to save entire mergedBuffer into persistent memory */
        if(PersistentMemoryAccess::saveData(mergedBuffer, mergedBufferSize))
        {
            /* Update data container signals */
            updateNodeConfigurationSignal();
            retVal = true;
        }else 
        {
            Serial.println("Saving config failed!");
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
    uint8_t* mergedBuffer = (uint8_t*) malloc (mergedBufferSize);

    /* Dynamic buffer allocation succeeded */
    if(mergedBuffer != nullptr) {

        /* Try to read NvM data to mergedBuffer */
        if(PersistentMemoryAccess::readData(mergedBuffer, mergedBufferSize))
        {
            nvmDataAvailable = true;
            retVal = true;

            /* Copy device configuration */
            memcpy(&configRamMirror, mergedBuffer, configRamMirror.getSize());

            /* Copy datablocks to ram mirror */
            memcpy(
                dataBlocksRamMirror->data,
                mergedBuffer + configRamMirror.getSize(),
                NUMBER_OF_PERSISTENT_DATABLOCKS * PersistentDataBlock::getSize()
            );

            updateNodeConfigurationSignal();
        }
        else 
        {
            nvmDataAvailable = false;
        }

        /* Release resources */
        free(mergedBuffer);
    }

    return retVal;
}



bool ConfigProvider::setDatablock(PersistentDatablockID blockID, uint8_t* data)
{
    bool retVal = false;
    if(blockID <= e_PERSISTENT_BLOCK_LAST && data != nullptr)
    {
        memcpy(
            &dataBlocksRamMirror[blockID],
            data,
            PersistentDataBlock::getSize()
        );

        //retVal = saveRamMirrorToNvm();
        retVal = true;
    }

    return retVal;
}

bool ConfigProvider::getDatablock(PersistentDatablockID blockID, uint8_t* buffer)
{
    bool retVal = false;
    /* Is nvm data read and available? */
    if(nvmDataAvailable && (blockID <= e_PERSISTENT_BLOCK_LAST) && buffer != nullptr)
    {
        retVal = true;

        memcpy(
            buffer,
            &dataBlocksRamMirror[blockID],
            PersistentDataBlock::getSize()
        );

    }

    return retVal;
}


void ConfigProvider::eraseDatablockMemory()
{
    Serial.println("Datablock erase start...");
    uint8_t eraserData[PERSISTENT_DATABLOCK_SIZE] = {0xFF};

    for(uint8_t datablock = e_PERSISTENT_BLOCK_FIRST; datablock <= e_PERSISTENT_BLOCK_LAST; datablock++)
    {
        if(!setDatablock((PersistentDatablockID)datablock, eraserData))
        {
            Serial.println("Erasing failed!");
            return;
        }
    }

    nvmDataAvailable = false;

    Serial.println("Datablock erase done.");
}

void ConfigProvider::deinit() {
    configRamMirror.safeShutdownFlag = 37;
    saveRamMirrorToNvm();
}


void ConfigProvider::massErase()
{
    PersistentMemoryAccess::massErase(totalNvmSize);
}



