#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H
#include <WiFi.h>
#include <any>
#include <map>
#include <os/datacontainer/DataContainer.hpp>
#include <os/app/http/HttpAsyncRequestHandler.hpp>

#define MAX_NUMBER_OF_ROOM_NAME_TRANSLATIONS 15
/**
 * @class HomeLightHttpServer
 * @brief Serves the web UI, configuration pages, and device-control endpoints for the station.
 *
 * The HTTP server converts device descriptions and room metadata into JSON and HTML pages, processes
 * operational requests from the browser, and dispatches them into the rest of the OS.
 */

class HomeLightHttpServer
{
    /**
     * Metadata persisted in NVM for the HTTP server state.
     */
    typedef struct {
        /**
         * true when the metadata was restored from persistent storage successfully.
         */
        uint8_t restoredSuccessfully = 0;

        /**
         * Number of room-name translations currently stored in the server metadata.
         */
        uint8_t numberOfTranslations = 0;
    }HttpServerNvmMetadata;

    /**
     * Persistent metadata describing the current HTTP-server setup.
     */
    static HttpServerNvmMetadata nvmMetadata;

    /**
     * Wi-Fi server instance opened for browser requests.
     */
    static WiFiServer server;

    /**
     * HTTP request header currently being processed.
     */
    static String header;

    /**
     * Current millisecond timestamp used while reading client requests.
     */
    static unsigned long currentTime;

    /**
     * Previous timestamp used to compute HTTP timeout conditions.
     */
    static unsigned long previousTime;

    /**
     * Request timeout used before a client connection is considered stale.
     */
    static const long timeoutTime;

    /**
     * Temporary parsing indexes used while resolving request URLs.
     */
    static int pos1;
    static int pos2;
    static int pos3;

    /**
     * Current list of device descriptions prepared for UI rendering.
     */
    static std::vector<DeviceDescription> descriptionVector;

    /**
     * Maps each room ID to the list of device pointers assigned to that room.
     */
    static std::map<uint8_t, std::vector<DeviceDescription*>> deviceToRoomMappingList;

    /**
     * Maps room IDs to their user-friendly names as displayed by the UI.
     */
    static std::map<uint8_t, String> roomNamesMapping;

    /**
     * String representation of the station IP used in generated pages.
     */
    static String ipAddressString;

    /**
     * Security access level associated with the current client session.
     */
    static SecurityAccessLevelType secAccessLevel;

    /**
     * Reads and dispatches the next client request from the server socket.
     */
    static void handleClientRequest();

    /**
     * Callback invoked when a device-enabled state change must be propagated to the OS.
     */
    static std::function<bool(uint8_t, bool)> deviceEnableCallback;

    /**
     * Callback invoked when a brightness change request must be propagated to the device layer.
     */
    static std::function<bool(uint8_t, uint8_t)> deviceBrightnessChangeCallback;

    /**
     * Escapes special characters before embedding a string in JSON output.
     * @param request String to sanitize.
     */
    static void escapeSpecialCharsInJson(String& request);

    /**
     * Generates JSON for the asynchronously refreshed page content.
     * @param client Connected HTTP client receiving the update.
     */
    static void generateAsyncPageContentJson(WiFiClient& client);

    /**
     * Prints the HTML configuration page.
     * @param client Connected HTTP client receiving the page.
     */
    static void printConfigPage(WiFiClient& client);

    /**
     * Prints the slots configuration page used by the setup UI.
     * @param client Connected HTTP client receiving the page.
     */
    static void printSlotsConfigPage(WiFiClient& client);

    /**
     * Extracts link-related request data from the incoming request.
     * @param client Connected HTTP client.
     */
    static void processLinkRequestData(WiFiClient& client);

    /**
     * Handles an async link request.
     * @param client Connected HTTP client.
     * @return true when the request was processed successfully.
     */
    static bool processLinkAsyncRequest(WiFiClient& client);

    /**
     * Dispatches all constant-value HTTP requests.
     * @param request HTTP request string.
     * @param client Connected HTTP client.
     * @return true when a matching constant handler handled the request.
     */
    static bool processConstantRequests(const String& request, WiFiClient& client);

    /**
     * Dispatches parameterized request handlers for device or setup commands.
     * @param request HTTP request string.
     * @param client Connected HTTP client.
     * @return true when a parameterized request was handled successfully.
     */
    static bool processParameterizedRequests(String& request, WiFiClient& client);

    /**
     * Dispatches async parameterized request handlers.
     * @param request HTTP request string.
     * @param client Connected HTTP client.
     * @return true when an async request was handled successfully.
     */
    static bool processParameterizedAsyncRequests(String& request, WiFiClient& client);

    /**
     * Restores server metadata from the persistent NVM block.
     * @param nvmData Stored data block to decode.
     * @param length Number of bytes in the block.
     */
    static void restoreNvmData(uint8_t* nvmData, uint16_t length);

    /**
     * Packs the current metadata into a binary block for NVM persistence.
     * @param nvmData Output buffer to fill.
     * @param length Maximum size of the output block.
     * @return true when the metadata was packed successfully.
     */
    static bool packNvmData(uint8_t* nvmData, uint16_t length);

public:
    /**
     * Runs the periodic HTTP-server loop, processing any queued requests and refreshes.
     */
    static void cyclic();

    /**
     * Starts the HTTP server and registers the protocol handlers.
     */
    static void init();

    /**
     * Stops the HTTP service and clears the runtime server state.
     */
    static void deinit();

    /**
     * Flushes the HTTP metadata back to the persistent store.
     */
    static void flushNvmData();

    /**
     * Reacts to a device-description vector update and refreshes the UI model.
     * @param newDescriptionVector Updated device description snapshot.
     */
    static void onDeviceDescriptionChange(std::any newDescriptionVector);

    /**
     * Builds the UI fragment used to configure a single device slot.
     * @param slotNumber Slot index to render.
     * @param slot Device configuration being displayed.
     * @param client Connected HTTP client receiving the HTML payload.
     */
    static void generateConfigSlotUi(uint8_t slotNumber, DeviceConfigSlotType& slot, WiFiClient& client);

    /**
     * Returns the JSON representation of the room configuration.
     * @return Serialized room configuration JSON.
     */
    static String getRoomsCfgJson();

    /**
     * Loads the configuration from a JSON document.
     * @param doc Input document containing the server configuration.
     * @return true when the configuration was accepted.
     */
    static bool loadConfigFromFile(JsonDocument& doc);

    /**
     * Renders the main dashboard page for the browser.
     * @param client HTTP client receiving the response.
     */
    static void constantHandler_mainPage(WiFiClient& client);

    /**
     * Renders the configuration page.
     * @param client HTTP client receiving the response.
     */
    static void constantHandler_configPage(WiFiClient& client);

    /**
     * Resets a device through the HTTP layer.
     * @param client HTTP client receiving the response.
     */
    static void constantHandler_resetDevice(WiFiClient& client);

    /**
     * Renders the device setup page.
     * @param client HTTP client receiving the response.
     */
    static void constantHandler_devicesSetup(WiFiClient& client);

    /**
     * Renders the room-assignment page.
     * @param client HTTP client receiving the response.
     */
    static void constantHandler_roomAssignment(WiFiClient& client);

    /**
     * Performs a full mass-erase action triggered by the UI.
     * @param client HTTP client receiving the response.
     */
    static void constantHandler_massErase(WiFiClient& client);

    /**
     * Provides the network inspection summary page.
     * @param client HTTP client receiving the response.
     */
    static void constantHandler_networkInspecion(WiFiClient& client);

    /**
     * Provides the system-details page.
     * @param client HTTP client receiving the response.
     */
    static void constantHandler_systemDetails(WiFiClient& client);

    /**
     * Generates a testing endpoint used to validate async behavior.
     * @param client HTTP client receiving the response.
     */
    static void constantHandler_asyncTest(WiFiClient& client);

    /**
     * Retrieves async page content for the front-end.
     * @param request Raw request string.
     * @param client HTTP client receiving the response.
     */
    static void constantHandler_asyncGetPageContent(String& request, WiFiClient& client);

    /**
     * Retrieves notification information for the UI.
     * @param request Raw request string.
     * @param client HTTP client receiving the response.
     */
    static void constantHandler_asyncGetNotifications(String& request, WiFiClient& client);

    /**
     * Renders the digital-events page.
     * @param client HTTP client receiving the response.
     */
    static void constantHandler_digitalEvents(WiFiClient& client);

    /**
     * Handles a fallback pending response route.
     * @param client HTTP client receiving the response.
     */
    static void pending(WiFiClient& client);

    /**
     * Applies a new configuration payload submitted by the UI.
     * @param request Raw request string.
     * @param client HTTP client receiving the response.
     */
    static void parameterizedHandler_newConfigApply(String& request, WiFiClient& client);

    /**
     * Switches a device state from the web interface.
     * @param request Raw request string.
     * @param client HTTP client receiving the response.
     */
    static void parameterizedHandler_deviceSwitch(String& request, WiFiClient& client);

    /**
     * Adjusts device brightness through the HTTP interface.
     * @param request Raw request string.
     * @param client HTTP client receiving the response.
     */
    static void parameterizedHandler_deviceBrightnessChange(String& request, WiFiClient& client);

    /**
     * Applies room-name mappings submitted by the browser.
     * @param request Raw request string.
     * @param client HTTP client receiving the response.
     */
    static void parameterizedHandler_roomNameMappingApply(String& request, WiFiClient& client);

    /**
     * Applies a password update from the UI.
     * @param request Raw request string.
     * @param client HTTP client receiving the response.
     */
    static void parameterizedHandler_passwordApply(String& request, WiFiClient& client);

    /**
     * Sets an LED-strip color value from the web interface.
     * @param request Raw request string.
     * @param client HTTP client receiving the response.
     */
    static void parameterizedHandler_ledStripColor(String& request, WiFiClient& client);

    /**
     * Sets a single LED color from the web interface.
     * @param request Raw request string.
     * @param client HTTP client receiving the response.
     */
    static void parameterizedHandler_ledColor(String& request, WiFiClient& client);

    /**
     * Stores a new device setup JSON payload from the client.
     * @param request Raw request string.
     * @param client HTTP client receiving the response.
     */
    static void parameterizedHandler_newSetupJson(String& request, WiFiClient& client);

    /**
     * Downloads a device configuration payload to the browser.
     * @param request Raw request string.
     * @param client HTTP client receiving the response.
     */
    static void parameterizedHandler_downloadDeviceConfiguration(String& request, WiFiClient& client);

    /**
     * Loads a device configuration payload sent by the browser.
     * @param request Raw request string.
     * @param client HTTP client receiving the response.
     */
    static void parameterizedHandler_loadDeviceConfiguration(String& request, WiFiClient& client);

    /**
     * Fetches the extended controls section for the current UI.
     * @param request Raw request string.
     * @param client HTTP client receiving the response.
     */
    static void parameterizedHandler_getExtendedControls(String& request, WiFiClient& client);

    /**
     * Sets the strip color state from the requested data payload.
     * @param request Raw request string.
     * @param client HTTP client receiving the response.
     */
    static void parameterizedHandler_setStripColor(String& request, WiFiClient& client);

    /**
     * Loads a strip configuration from the persistent memory block.
     * @param request Raw request string.
     * @param client HTTP client receiving the response.
     */
    static void parameterizedHandler_stripLoadFromMemory(String& request, WiFiClient& client);

    /**
     * Saves the current strip state to persistent memory.
     * @param request Raw request string.
     * @param client HTTP client receiving the response.
     */
    static void parameterizedHandler_stripSaveCurrent(String& request, WiFiClient& client);

    /**
     * Changes the room state through the UI trigger.
     * @param request Raw request string.
     * @param client HTTP client receiving the response.
     */
    static void parameterizedHandler_roomStateChange(String& request, WiFiClient& client);

    /**
     * Toggles the room state for all associated devices.
     * @param request Raw request string.
     * @param client HTTP client receiving the response.
     */
    static void parameterizedHandler_roomToggle(String& request, WiFiClient& client);

    /**
     * Switches the segment state from the UI.
     * @param request Raw request string.
     * @param client HTTP client receiving the response.
     */
    static void parameterizedHandler_segmentStateSwitch(String& request, WiFiClient& client);

    /**
     * Toggles live LED state from the UI.
     * @param request Raw request string.
     * @param client HTTP client receiving the response.
     */
    static void parameterizedHandler_ledsLiveSwitch(String& request, WiFiClient& client);

    /**
     * Returns the current hash used by the client to validate UI state.
     * @param request Raw request string.
     * @param client HTTP client receiving the response.
     */
    static void parameterizedHandler_getHash(String& request, WiFiClient& client);

    /**
     * Creates a new digital-event tab requested by the UI.
     * @param request Raw request string.
     * @param client HTTP client receiving the response.
     */
    static void parameterizedHandler_newDigEvntTab(String& request, WiFiClient& client);
    
};

#endif