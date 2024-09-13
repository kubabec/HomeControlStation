#ifndef SIGNALS_H
#define SIGNALS_H

#define NUMBER_OF_SIGNALS 21
enum Signal
{
    /* Type: bool , somme description */
    SIG_KEEP_ALIVE_RECEIVED = 0,
    /* Type: int , not used yet */
    SIG_CONNECTION_STATUS,
    
    /* Type: std:function<void(uin8_t, bool)> - callback do lokalnych on/off int - ID urządzenia, bool - on/off */
    CBK_LOCAL_DEVICE_ENABLE,
    /* Type: std:function<void(uin8_t, uin8_t)> - callback do lokalnych brightness uin8_t - ID urządzenia, uin8_t - poziom brightness */
    CBK_LOCAL_DEVICE_BRIGHTNESS_CHANGE,
    /* Type: std::vector<OnOffDeviceDescription> - zbiór lokalnych OnOffDevices information*/
    SIG_LOCAL_COLLECTION_ONOFF,


    /* Type: std:function<void(uin8_t, bool)> - callback do urzadzeń na innych nodach on/off int - ID urządzenia, bool - on/off */
    CBK_REMOTE_DEVICE_ENABLE,
    /* Type: std:function<void(uin8_t, uin8_t)> - callback do urzadzeń na innych nodach do brightness uin8_t - ID urządzenia, uin8_t - poziom brightness */
    CBK_REMOTE_DEVICE_BRIGHTNESS_CHANGE,
    /* Type: std::vector<OnOffDeviceDescription> - zbior urządzeń na innych nodach OnOffDevices information*/
    SIG_REMOTE_COLLECTION_ONOFF,
    
    
    /* Type: std:function<void(uin8_t, bool)> - callback do on/off int - ID urządzenia, bool - on/off */
    CBK_DEVICE_ENABLE,
    /* Type: std:function<void(uin8_t, uin8_t)> - callback do brightness uin8_t - ID urządzenia, uin8_t - poziom brightness */
    CBK_DEVICE_BRIGHTNESS_CHANGE,
    /* Type: std::vector<OnOffDeviceDescription> - OnOffDevices information*/
    SIG_COLLECTION_ONOFF,
    /* Type: bool - is http server running*/
    SIG_IS_HTTP_SERVER,
    /* Type: bool - is controll remote server or client, TRUE-server, FALSE-client */
    SIG_IS_RC_SERVER,

    /* Type: RequestProcessingState - current request processing state */
    SIG_CURRENT_REQUEST_PROCESSING_STATE,

    /* Type: NodeConfiguration - value of device configuration provided by ConfigProvider when EEPROM data read succeeded */
    SIG_DEVICE_CONFIGURATION,

    /* Type: std:function<void(String&)> - callback to parse and apply device configuration using String class */
    CBK_SET_CONFIG_VIA_STRING,

    /* Type: String - current IP Address */
    SIG_IP_ADDRESS_STRING,

    /* Type: uint32 - current IP Address  as 4 bytes */
    SIG_IP_ADDRESS,

    /* Type: std:function<bool(PersistentDatablockID, uint8_t*)> - change/set NVM datablock with given ID */
    CBK_SET_NVM_DATABLOCK,

    /* Type: std:function<bool(PersistentDatablockID, uint8_t*)> - read NVM datablock with given ID */
    CBK_GET_NVM_DATABLOCK,

    /* TYpe: ConfigSlotsDataType - container with hardware pins configuration and logical devices extraction */
    SIG_CONFIG_SLOTS
};

const std::string signalNames[NUMBER_OF_SIGNALS] = {
    "KEEP_ALIVE_RECEIVED",
    "CONNECTION_STATUS",
    "CBK_LOCAL_DEVICE_ENABLE",
    "CBK_LOCAL_DEVICE_BRIGHTNESS_CHANGE",
    "SIG_LOCAL_COLLECTION_ONOFF",
    "CBK_REMOTE_DEVICE_ENABLE",
    "CBK_REMOTE_DEVICE_BRIGHTNESS_CHANGE",
    "SIG_REMOTE_COLLECTION_ONOFF",
    "CBK_DEVICE_ENABLE",
    "CBK_DEVICE_BRIGHTNESS_CHANGE",
    "SIG_COLLECTION_ONOFF",
    "SIG_IS_HTTP_SERVER",
    "SIG_IS_RC_SERVER",
    "SIG_CURRENT_REQUEST_PROCESSING_STATE",
    "SIG_DEVICE_CONFIGURATION",
    "CBK_SET_CONFIG_VIA_STRING",
    "SIG_IP_ADDRESS_STRING",
    "SIG_IP_ADDRESS",
    "CBK_SET_NVM_DATABLOCK",
    "CBK_GET_NVM_DATABLOCK",
    "SIG_CONFIG_SLOTS"
};


#endif