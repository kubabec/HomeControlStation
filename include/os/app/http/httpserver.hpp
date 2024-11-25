#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H
#include <WiFi.h>
#include <any>
#include <os/datacontainer/DataContainer.hpp>


class HomeLightHttpServer
{
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
    
    static std::vector<OnOffDeviceDescription> onOffDescriptionVector;
    static std::array<SystemErrorType, ERR_MONT_ERROR_COUNT> systemErrorList;
    static uint8_t activeErrorsCount;
    static ConfigSlotsDataType pinConfigSlotsCopy_HttpServer;

    static bool isUserInterfaceBlocked;

    static String ipAddressString;
    static void handleClientRequest();
    static std::function<bool(uint8_t, bool)> deviceEnableCallback;
    static std::function<bool(uint8_t, uint8_t)> deviceBrightnessChangeCallback;
    static void requestErrorList();

    


    static void printConfigPage(WiFiClient& client);
    static void printSlotsConfigPage(WiFiClient& client);
    static void printErrorTable(WiFiClient& client);

    static void processLinkRequestData(WiFiClient& client);
    static bool processConstantRequests(const String& request, WiFiClient& client);
    static bool processParameterizedRequests(String& request, WiFiClient& client);

public:
    static void cyclic();
    static void init();
    static void deinit();
    static void onDeviceDescriptionChange(std::any newDescriptionVector);
    static void onSlotConfigChange(std::any newSlotConfig);
    static void onUiBlockedSignalChange(std::any isBlockedValue);

    static void generateOnOffUi(OnOffDeviceDescription& description, WiFiClient& client);
    static void generateConfigSlotUi(uint8_t slotNumber, DeviceConfigSlotType& slot, WiFiClient& client);


    static void constantHandler_mainPage(WiFiClient& client);
    static void constantHandler_clearErrors(WiFiClient& client);
    static void constantHandler_configPage(WiFiClient& client);
    static void constantHandler_devicesSetup(WiFiClient& client);
    static void constantHandler_massErase(WiFiClient& client);
    static void pending(WiFiClient& client);

    static void parameterizedHandler_newConfigApply(String& request, WiFiClient& client);
    static void parameterizedHandler_newDevicesSetup(String& request, WiFiClient& client);
    static void parameterizedHandler_deviceSwitch(String& request, WiFiClient& client);
    static void parameterizedHandler_deviceBrightnessChange(String& request, WiFiClient& client);
    
};

#endif