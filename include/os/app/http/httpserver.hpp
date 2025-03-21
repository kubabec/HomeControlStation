#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H
#include <WiFi.h>
#include <any>
#include <map>
#include <os/datacontainer/DataContainer.hpp>
#include <os/app/http/HttpAsyncRequestHandler.hpp>

#define MAX_NUMBER_OF_ROOM_NAME_TRANSLATIONS 15

class HomeLightHttpServer
{
    typedef struct {
        uint8_t restoredSuccessfully = 0;
        uint8_t numberOfTranslations = 0;

        //uint8_t reserved[PERSISTENT_DATABLOCK_SIZE - 2];
    }HttpServerNvmMetadata;

    
    static HttpServerNvmMetadata nvmMetadata;
    static WiFiServer server;
    static String header;
    static unsigned long currentTime;
    // Previous time
    static unsigned long previousTime;
    // Define timeout time in milliseconds (example: 2000ms = 2s)
    static const long timeoutTime;
    static int pos1;
    static int pos2;
    static int pos3;
    
    static std::vector<DeviceDescription> descriptionVector;
    static std::map<uint8_t, std::vector<DeviceDescription*>> deviceToRoomMappingList;
    static std::map<uint8_t, String> roomNamesMapping;

    static std::array<SystemErrorType, ERR_MONT_ERROR_COUNT> systemErrorList;
    static uint8_t activeErrorsCount;
    
    static String ipAddressString;
    static SecurityAccessLevelType secAccessLevel;
    static void handleClientRequest();
    static std::function<bool(uint8_t, bool)> deviceEnableCallback;
    static std::function<bool(uint8_t, uint8_t)> deviceBrightnessChangeCallback;
    static void requestErrorList();

    static void generateAsyncPageContentJson(WiFiClient& client);
    static void printConfigPage(WiFiClient& client);
    static void printSlotsConfigPage(WiFiClient& client);
    static void printErrorTable(WiFiClient& client);

    static void processLinkRequestData(WiFiClient& client);
    static bool processLinkAsyncRequest(WiFiClient& client);
    static bool processConstantRequests(const String& request, WiFiClient& client);
    static bool processParameterizedRequests(String& request, WiFiClient& client);
    static bool processParameterizedAsyncRequests(String& request, WiFiClient& client);

    static void restoreNvmData(uint8_t* nvmData, uint16_t length);
    static bool packNvmData(uint8_t* nvmData, uint16_t length);

public:
    static void cyclic();
    static void init();
    static void deinit();
    static void onDeviceDescriptionChange(std::any newDescriptionVector);

    static void generateOnOffUi(DeviceDescription& description, WiFiClient& client);
    static void generateConfigSlotUi(uint8_t slotNumber, DeviceConfigSlotType& slot, WiFiClient& client);

    static String getRoomsCfgJson();
    static bool loadConfigFromFile(JsonDocument& doc);


    static void constantHandler_mainPage(WiFiClient& client);
    static void constantHandler_clearErrors(WiFiClient& client);
    static void constantHandler_configPage(WiFiClient& client);
    static void constantHandler_resetDevice(WiFiClient& client);
    static void constantHandler_devicesSetup(WiFiClient& client);
    static void constantHandler_roomAssignment(WiFiClient& client);
    static void constantHandler_massErase(WiFiClient& client);
    static void constantHandler_asyncTest(WiFiClient& client);
    static void constantHandler_asyncGetPageContent(String& request, WiFiClient& client);
    static void constantHandler_asyncGetNotifications(String& request, WiFiClient& client);
    static void pending(WiFiClient& client);

    static void parameterizedHandler_newConfigApply(String& request, WiFiClient& client);
    static void parameterizedHandler_deviceSwitch(String& request, WiFiClient& client);
    static void parameterizedHandler_deviceBrightnessChange(String& request, WiFiClient& client);
    static void parameterizedHandler_roomNameMappingApply(String& request, WiFiClient& client);
    static void parameterizedHandler_passwordApply(String& request, WiFiClient& client);
    static void parameterizedHandler_ledStripColor(String& request, WiFiClient& client);
    static void parameterizedHandler_ledColor(String& request, WiFiClient& client);
    static void parameterizedHandler_newSetupJson(String& request, WiFiClient& client);
    static void parameterizedHandler_downloadDeviceConfiguration(String& request, WiFiClient& client);
    static void parameterizedHandler_loadDeviceConfiguration(String& request, WiFiClient& client);
    static void parameterizedHandler_getExtendedControls(String& request, WiFiClient& client);
    
};

#endif