#ifndef CONFIG_PROVIDER_H
#define CONFIG_PROVIDER_H
#include <Arduino.h>
#include <os/datacontainer/DataContainer.hpp>


#define SSID_LENGTH 30
#define PASSWORD_LENGTH 30
#define NUMBER_OF_PERSISTENT_DATABLOCKS 15
typedef struct
{
    uint8_t safeShutdownFlag = 255;
    uint8_t isHttpServer = 0;
    uint8_t isRcServer = 0;
    uint8_t isDefaultUserAdmin = 1;
    uint8_t nodeType = 255;
    char networkSSID[SSID_LENGTH] = {'\0'};
    char networkPassword[PASSWORD_LENGTH] = {'\0'};
    char panelPassword[PASSWORD_LENGTH] = {'\0'};

    uint16_t getSize()
    {
        return sizeof(uint8_t)  + //isHttpServer
            sizeof(uint8_t) + //isRcServer
            sizeof(uint8_t) + //isDefaultUserAdmin
            sizeof(uint8_t) + //safeShutdownFlag
            sizeof(uint8_t) + //nodeType
            SSID_LENGTH + // networkSSID
            PASSWORD_LENGTH + // Password
            PASSWORD_LENGTH; // user password
    }

    void serialPrint()
    {
        Serial.println("<<<<< - - - - CONFIGURATION - - - - >>>>>");
        Serial.println("isHttpServer : " + String(isHttpServer));
        Serial.println("isRcServer : " + String(isRcServer));
        Serial.println("isDefaultUserAdmin : " + String(isDefaultUserAdmin));
        Serial.println("Type : " + String((int)nodeType));
        Serial.println("network SSID : " + String(networkSSID));
        Serial.println("network Password : " + String(networkPassword));
        Serial.println("Panel Password : " + String(panelPassword));
        Serial.println("<<<<< - - - - - - - - - - - - - - - >>>>>");

    }

    void setSSID(String& str)
    {
        const char* c_str = str.c_str();
        memset(networkSSID, '\0', SSID_LENGTH);
        if(str.length() < SSID_LENGTH){
            for(uint8_t i = 0; i < str.length(); i++)
            {
                networkSSID[i] = c_str[i];
            }
        }
        else
        {
            Serial.println("Invalid string length given for SSID!");
        }
    }

    void setPassword(String& str)
    {
        const char* c_str = str.c_str();
        memset(networkPassword, '\0', PASSWORD_LENGTH);
        if(str.length() < PASSWORD_LENGTH){
            for(uint8_t i = 0; i < str.length(); i++)
            {
                networkPassword[i] = c_str[i];
            }
        }
        else
        {
            Serial.println("Invalid string length given for Password!");
        }
    }

    void setPanelPassword(String& str)
    {
        const char* c_str = str.c_str();
        memset(panelPassword, '\0', PASSWORD_LENGTH);
        if(str.length() < PASSWORD_LENGTH){
            for(uint8_t i = 0; i < str.length(); i++)
            {
                panelPassword[i] = c_str[i];
            }
        }
        else
        {
            Serial.println("Invalid string length given for Password!");
        }
    }
}ConfigData;





class ConfigProvider 
{

    static ConfigData configRamMirror;
    static PersistentDataBlock dataBlocksRamMirror[e_NUMBER_OF_PERSISTENT_BLOCKS];
    static uint16_t totalNvmSize;

    static bool nvmDataAvailable;

    static void updateNodeConfigurationSignal();
    static bool saveRamMirrorToNvm();
    static bool readRamMirrorFromNvm();
public:
    static void init();
    static void deinit();
    static void cyclic();

    static bool loadConfigFromFile(JsonDocument& doc);
    static bool setConfigViaString(String& configString);
    static String getConfigJson();

    static bool setDatablock(PersistentDatablockID blockID, uint8_t* data);
    static bool getDatablock(PersistentDatablockID blockID, uint8_t* buffer);

    static void eraseDatablockMemory();
    static void massErase();
        
};


#endif