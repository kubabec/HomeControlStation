#ifndef SIGNALS_H
#define SIGNALS_H

#define NUMBER_OF_SIGNALS 35
enum Signal
{
    /* Type: bool , somme description */
    SIG_KEEP_ALIVE_RECEIVED = 0,
    /* Type: int , not used yet */
    SIG_CONNECTION_STATUS,    
    
    /* Type: std::vector<OnOffDeviceDescription> - zbiór lokalnych OnOffDevices information*/
    SIG_LOCAL_COLLECTION_ONOFF,

    /* Type: std::vector<DeviceDescription> - zbiór lokalnych Devices information*/
    SIG_LOCAL_COLLECTION,

    /* Type: std::vector<DeviceDescription> - Devices information*/
    SIG_DEVICE_COLLECTION,
    
    /* Type: std::vector<OnOffDeviceDescription> - zbior urządzeń na innych nodach OnOffDevices information*/
    SIG_REMOTE_COLLECTION_ONOFF,
    
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
    /* Type: std:function<void(String&)> - callback to parse and apply local devices configuration using String class */
    CBK_SET_DEVICES_CONFIG_VIA_STRING,
    /* Type: String - current IP Address */
    SIG_IP_ADDRESS_STRING,
    /* Type: uint32 - current IP Address  as 4 bytes */
    SIG_IP_ADDRESS,
    /* Type: std:function<bool(PersistentDatablockID, uint8_t*)> - change/set NVM datablock with given ID */
    CBK_SET_NVM_DATABLOCK,
    /* Type: std:function<bool(PersistentDatablockID, uint8_t*)> - read NVM datablock with given ID */
    CBK_GET_NVM_DATABLOCK,
    /* TYpe: ConfigSlotsDataType - container with hardware pins configuration and logical devices extraction */
    SIG_CONFIG_SLOTS,
    /* Type: std:function<void()> - callback to reboot */
    CBK_RESET_DEVICE,
    /* Type: std:function<void(ERR_MON_ERROR_TYPE errorCode, String)> - callback to reboot */
    CBK_ERROR_REPORT,
    /* Type: std:function<void(ERR_MON_ERROR_TYPE errorCode)> - callback to reboot */
    CBK_ERROR_CLEAR,
    /* TYpe: std::array<SystemErrorType, ERR_MONT_ERROR_COUNT> - container with actual system error data */
    SIG_SYSTEM_ERROR_LIST,
    /* Type: std:function<void(void)> - callback to erase NVM data */
    CBK_MASS_ERASE,
    /* Type: std:function<bool(RequestType, bool)> - callback to signal receivers */
    CBK_REGISTER_REQUEST_RECEIVER,
    /* Type: std:function<bool(ResponseType, bool)> - callback to signal receivers */
    CBK_REGISTER_RESPONSE_RECEIVER,


    /* Type: std:function<void(uin8_t, bool)> - callback to response RcRequest uint8_t - response ID, bool - on/off */
    CBK_RESPONSE,
    /* TYpe: uint16_t - hash representing current runtime state including configuration and devices states */
    SIG_RUNTIME_NODE_HASH,
    /* Type: std::vector<OnOffDeviceDescription> - Internal tunel!! only to be used between RCServer <-> RCDevicesManager*/
    SIG_RC_DEVICES_INTERNAL_TUNNEL,
    /* Type: std:function<void(RcRequest&)> - callback add new generic RcRequest to RcServer for processing */
    CBK_CREATE_RC_REQUEST,
    /* TYpe: bool - represents information if UI can be displayed to the user or not */
    SIG_IS_UI_BLOCKED,
    /* Type: DeviceServicesAPI - set of services functions provided by Device Manager to Device Provider */
    SIG_LOCAL_DEVICE_SERVICES,
    /* Type: DeviceServicesAPI - set of services functions provided by RemooteDevicesManager to Device Provider */
    SIG_REMOTE_DEVICE_SERVICES,
    /* Type: DeviceServicesAPI - set of services functions provided by DeviceProvider to users */
    SIG_DEVICE_SERVICES,
    /* Type: SecurityAccessLevelType - security access to prevent unauthorized changes */
    SIG_SECURITY_ACCESS_LEVEL,
    /* Type: std:function<void(String)> - callback to try access level change in OS */
    CBK_SECURITY_ACCESS_LEVEL_CHANGE_VIA_STRING

};
#endif