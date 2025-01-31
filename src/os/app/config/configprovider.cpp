#include <os/app/config/ConfigProvider.hpp>

#include <os/app/config/PersistentMemoryAccess.hpp>

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
        CBK_SET_CONFIG_VIA_STRING,
        static_cast<std::function<void(String&)>>(ConfigProvider::setConfigViaString));

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


void ConfigProvider::setConfigViaString(String& configString)
{
    SecurityAccessLevelType currentAccessLevel = 
        std::any_cast<SecurityAccessLevelType>(DataContainer::getSignalValue(SIG_SECURITY_ACCESS_LEVEL));

    /* Check if device is unlocked that config can be modified */
    if(currentAccessLevel > e_ACCESS_LEVEL_NONE){
        String str_isHttpServer, str_isRcServer, str_isUserAdmin, str_Ssid, str_Passwd, str_PanelPassword, str_nodeType;
        MatchState matcher;
        matcher.Target(const_cast<char*>(configString.c_str()));
        /* GET /apply?isHTTPServer=yes&isRCServer=no&SSID=NetworkSSID&Password=SomeRandomPassword HTTP/1.1 */
        Serial.println("New config received :");
        const String part1 = "apply?isHTTPServer=";
        const String part2 = "&isRCServer=";
        const String part3 = "&isUserAdmin=";
        const String part4 = "&SSID=";
        const String part5 = "&Password=";
        const String part6 = "&PanelPassword=";
        const String part7 = "&nodeType=";
        const String part8 = "&end";

        char searchResult = matcher.Match(part2.c_str());
        uint16_t readStartIndex = 0;
        Serial.println((uint16_t)searchResult);
        if(searchResult > 0)
        {
            readStartIndex = part1.length();
            // Read HTTP server config
            str_isHttpServer = configString.substring(readStartIndex, matcher.MatchStart);
            //Serial.println("isHttpServer : " + str_isHttpServer);

            // Move start read index by part2 length
            readStartIndex = matcher.MatchStart + part2.length();
            
            // Search for next value
            searchResult = matcher.Match(part3.c_str());
            if(searchResult > 0)
            {
                // Read RC server config
                str_isRcServer = configString.substring(readStartIndex, matcher.MatchStart);
                //Serial.println("str_isRcServer : " + str_isRcServer);
            }

            // Move start read index by part3 length
            readStartIndex = matcher.MatchStart + part3.length();
            // Search for next value
            searchResult = matcher.Match(part4.c_str());
            if(searchResult > 0)
            {
                // Read RC server config
                str_isUserAdmin = configString.substring(readStartIndex, matcher.MatchStart);
                //Serial.println("str_Ssid : " + str_Ssid);
            }


            // Move start read index by part3 length
            readStartIndex = matcher.MatchStart + part4.length();
            // Search for next value
            searchResult = matcher.Match(part5.c_str());
            if(searchResult > 0)
            {
                // Read RC server config
                str_Ssid = configString.substring(readStartIndex, matcher.MatchStart);
                str_Ssid.replace("%20", " ");
                //Serial.println("str_Ssid : " + str_Ssid);
            }



            // Move start read index by part4 length
            readStartIndex = matcher.MatchStart + part5.length();
            // Search for next value
            searchResult = matcher.Match(part6.c_str());
            if(searchResult > 0)
            {
                // Read RC server config
                str_Passwd = configString.substring(readStartIndex, matcher.MatchStart);
                //Serial.println("str_Passwd : " + str_Passwd);
            }


            // Move start read index by part4 length
            readStartIndex = matcher.MatchStart + part6.length();
            // Search for next value
            searchResult = matcher.Match(part7.c_str());
            if(searchResult > 0)
            {
                // Read RC server config
                str_PanelPassword = configString.substring(readStartIndex, matcher.MatchStart);
                //Serial.println("////////str_nodeType : " + str_nodeType);
            }


            // Move start read index by part4 length
            readStartIndex = matcher.MatchStart + part7.length();
            // Search for next value
            searchResult = matcher.Match(part8.c_str());
            if(searchResult > 0)
            {
                // Read RC server config
                str_nodeType = configString.substring(readStartIndex, matcher.MatchStart);
                //Serial.println("////////str_nodeType : " + str_nodeType);
            }
            

            /* Prepare configuration data */
            /* Some of configs are only allowed to be changed in Service mode */
            if(currentAccessLevel >= e_ACCESS_LEVEL_SERVICE_MODE)   {
                configRamMirror.isHttpServer = str_isHttpServer == "yes" ? 1 : 0;
                configRamMirror.isRcServer = str_isRcServer == "yes" ? 1 : 0;
                configRamMirror.isDefaultUserAdmin = str_isUserAdmin == "yes" ? 1 : 0;
                configRamMirror.nodeType = str_nodeType.toInt() < 10 ? str_nodeType.toInt() : 255;

                Serial.println("String values:");
                Serial.println("str_isHttpServer " + str_isHttpServer) ;
                Serial.println("str_isRcServer " + str_isRcServer) ;
                Serial.println("str_IsUserAdmin " + str_isUserAdmin) ;
                Serial.println("str_Ssid " + str_Ssid) ;
                Serial.println("str_Passwd " + str_Passwd) ;
                Serial.println("str_PanelPasswd " + str_PanelPassword) ;
                Serial.println("str_nodeType " + str_nodeType) ;
            }

            configRamMirror.setSSID(str_Ssid);
            configRamMirror.setPassword(str_Passwd);
            configRamMirror.setPanelPassword(str_PanelPassword);

            Serial.println("Applying following configuration :");
            configRamMirror.serialPrint();

            /* Save changed configuration + all the rest of RAM mirrors to NVM */
            //saveRamMirrorToNvm();

        }else
        {
            std::any_cast<std::function<void(ERR_MON_ERROR_TYPE, String)>>(
                DataContainer::getSignalValue(CBK_ERROR_REPORT)
                )(
                    ERR_MON_WRONG_CONFIG_STRING_RECEIVED,
                    "Invalid configuration string received"
                );

            Serial.println("Invalid config passed! Rejecting...");
        }

        Serial.println(configString);

    } else 
    {
        /* No access level to apply */
    }
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



