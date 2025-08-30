#ifndef DATA_CONTAINER_TYPES_H
#define DATA_CONTAINER_TYPES_H

#include "DeviceControlApi.hpp"
#include "os/app/remoteControl/RC_publicDataTypes.hpp"
#include "devices/Device.hpp"
#include "os/datacontainer/NvmConfigSlotDefinition.hpp"
#include "os/tools/messageUDP.hpp"
#include <ArduinoJson.h>

#define PERSISTENT_DATABLOCK_SIZE 50
#define NUMBER_OF_CONFIG_SLOTS 6

// aktualny stan przerabianego requesta
/* State of request processing */
enum RequestProcessingState {
    eNO_REQUEST,
    eREQUEST_PENDING,
    eREQUEST_COMPLETED
};

/* Basic node (ESP32) configuration data */
typedef struct {
    bool isHttpServer = 1;
    bool isRcServer = 1;
    bool isDefaultUserAdmin = 0;
    bool networkCredentialsAvailable = false;
    uint8_t nodeType = 0;
    String networkSSID = "";
    String networkPassword = "";
    String panelPassword = "";
}NodeConfiguration;

/* NVM datablocks identifiers */
typedef enum
{
    e_PERSISTENT_BLOCK_FIRST = 0,
    e_BLOCK_DEVICE_1 = e_PERSISTENT_BLOCK_FIRST,
    e_BLOCK_DEVICE_2,
    e_BLOCK_DEVICE_3,
    e_BLOCK_DEVICE_4,
    e_BLOCK_DEVICE_5,
    e_BLOCK_DEVICE_6,
    e_BLOCK_HTTP_FIRST,
    e_BLOCK_HTTP_1 = e_BLOCK_HTTP_FIRST,
    e_BLOCK_HTTP_2,
    e_BLOCK_HTTP_3,
    e_BLOCK_HTTP_4,
    e_BLOCK_HTTP_5,
    e_BLOCK_HTTP_6,
    e_BLOCK_HTTP_7,
    e_BLOCK_HTTP_8,
    e_BLOCK_HTTP_LAST = e_BLOCK_HTTP_8,
    e_BLOCK_EXT_MEMORY_METADATA,
    e_PERSISTENT_BLOCK_LAST = e_BLOCK_EXT_MEMORY_METADATA,
    e_NUMBER_OF_PERSISTENT_BLOCKS = (e_PERSISTENT_BLOCK_LAST + 1)
}PersistentDatablockID;

/* Array of available configuration slots wrapper */
typedef struct {
    std::array<DeviceConfigSlotType, NUMBER_OF_CONFIG_SLOTS> slots;
}ConfigSlotsDataType;

/* Generic datatype for NVM PERSISTENT_DATABLOCK_SIZE-bytes long block */
typedef struct
{
    uint8_t data[PERSISTENT_DATABLOCK_SIZE];

    static size_t getSize() {
        return PERSISTENT_DATABLOCK_SIZE;
    }
}PersistentDataBlock;

typedef struct
{
    uint8_t occurrenceCount = 0;
    String comment = "none";
    uint64_t lastOccurrenceTime = 0;
    String timeOfOccurrence = "";
}SystemErrorType;


typedef struct
{
    std::function<ServiceRequestErrorCode(uint8_t, DeviceServicesType)> serviceCall_NoParams;
    std::function<ServiceRequestErrorCode(uint8_t, DeviceServicesType, ServiceParameters_set1 param)> serviceCall_set1;
    std::function<ServiceRequestErrorCode(uint8_t, DeviceServicesType, ServiceParameters_set2 param)> serviceCall_set2;
    std::function<ServiceRequestErrorCode(uint8_t, DeviceServicesType, ServiceParameters_set3 param)> serviceCall_set3;
}DeviceServicesAPI;


typedef struct{
    std::function<bool(uint8_t, uint16_t)>  requestNewExtendedMemorySpace;
    std::function<void(uint8_t)>            releaseExtendedMemorySpace;
    std::function<uint8_t*(uint8_t, uint16_t*)>  getExtMemoryPtrByDeviceId;
    std::function<uint16_t(void)>           getCurrentMemoryUsage;
}ExtendedMemoryCtrlAPI;

typedef enum
{
    e_ACCESS_LEVEL_NONE = 10,
    e_ACCESS_LEVEL_AUTH_USER,
    e_ACCESS_LEVEL_SERVICE_MODE
}SecurityAccessLevelType;

typedef struct
{
    enum Type{
        INFO,
        WARNING,
        ERROR
    };

    String title;
    String body;
    String time;
    Type type = INFO;

    String toJson(){
        String json = "";
        json += "{";
        json += "\"title\": \""+ title +"\",";
        json += "\"body\": \""+ body +"\",";
        json += "\"time\": \""+ time +"\",";
        json += "\"type\": "+ String((int)type);
        json += "}";
        return json;
    }
}UserInterfaceNotification;

typedef struct{
    std::function<bool(UserInterfaceNotification&)> createNotification;
    std::function<uint8_t(void)> getActiveNotificationsCount;
    std::function<UserInterfaceNotification(void)> getOldestNotification;
}UINotificationsControlAPI;


typedef struct
{
    std::function<bool(String&)> setDeviceCfgViaJson;
    std::function<String()> getDeviceCfgJson;
    std::function<bool(JsonDocument&)> loadConfigFromFile;
}DeviceConfigManipulationAPI;

struct NetworkNodeInfo{
    enum NodeType{
        Master,
        Slave,
        Unknown
    } nodeType;
    MessageUDP::IPAddr nodeIP;
};

struct ServiceInformation{
    uint32_t ramTotal = 0;
    uint32_t ramFree = 0;
    uint32_t ramUsed = 0;
    float coreTemperature = 0.0f;
};

namespace DigitalButton{
    enum AffectedType {
        ROOM = 11,
        DEVICE
    };

    enum ActionType {
        ON = 21,
        OFF,
        TOGGLE
    };

    struct ButtonEvent{
        AffectedType affectedType;
        uint32_t affectedId;
        ActionType actionType;
    };
}

#endif 