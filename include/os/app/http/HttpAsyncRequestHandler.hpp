#ifndef HTTP_ASYNC_REQUEST_HANDLER_H
#define HTTP_ASYNC_REQUEST_HANDLER_H
#include <Arduino.h>
#include <map>
#include <devices/device.hpp>
#include <os/datacontainer/DataContainer.hpp>
#include "os/Logger.hpp"


#define DEVICE_ID_IN_ASYNC_REQUEST_SERVICE_CALL 2
#define DYNAMIC_REQUEST_MEMORY_LENGTH_IDX (DEVICE_ID_IN_ASYNC_REQUEST_SERVICE_CALL + 1)
#define DYNAMIC_REQUEST_DIRECTION_IDX (DYNAMIC_REQUEST_MEMORY_LENGTH_IDX + 2)
#define DYNAMIC_REQUEST_ADDITIONAL_PARAM_IDX (DYNAMIC_REQUEST_DIRECTION_IDX + 2)
#define DYNAMIC_REQUEST_START_OF_DATA_IDX (DYNAMIC_REQUEST_ADDITIONAL_PARAM_IDX + 1)
#define MAX_PARAM_LENGTH_FOR_ASYNC_REQUEST 400

/**
 * @file include/os/app/http/HttpAsyncRequestHandler.hpp
 * @brief HTTP request and UI handling for the Home Control Station.
 */


typedef enum {
    e_HTTP_OK,
    e_HTTP_FAILED,
    e_HTTP_BUSY
}HttpErrorCode;

 /* Type to handle different request processing state */
typedef enum
{
    ASYNC_NO_REQUEST,
    ASYNC_REQUEST_RECEIVED,
    ASYNC_REQUEST_PROCESSING,
    ASYNC_REQUEST_COMPLETED
}AsyncRequestState;

/* Type describing async request possible actions to be performed internally by a device */
typedef enum
{
    ASYNC_TYPE_INVALID,
    ASYNC_TYPE_DEVICE_SERVICE_CALL,
    ASYNC_GET_PAGE_CONTENT,
    ASYNC_GET_HASH,
    ASYNC_GET_SYSTEM_DETAILS,
    ASYNC_GET_NOTIFICATION_LIST,
    ASYNC_DOWNLOAD_CONFIGURATION,
    ASYNC_REDIRECT_TO_MAIN_PAGE,
    ASYNC_GET_ADVANCED_CONTROLS,
    ASYNC_TYPE_LAST = ASYNC_GET_ADVANCED_CONTROLS
}AsyncRequestType;
/**
 * @class HTTPAsyncRequestHandler
 * @brief Manages asynchronous page and device requests served by the HTTP UI layer.
 *
 * The handler stores the current request, decides which payload to generate, and returns the final JSON
 * response used by the front-end to refresh the dashboard without forcing a full page reload.
 */

class HTTPAsyncRequestHandler{
    /**
     * Container describing the active async HTTP request and its timing metadata.
     */
    typedef struct {
        /**
         * Current finite-state used by the request processing pipeline.
         */
        AsyncRequestState state = ASYNC_NO_REQUEST;

        /**
         * Timestamp when the request was first received.
         */
        long receivedTime = 0;

        /**
         * Type of the asynchronous request being processed.
         */
        AsyncRequestType type = ASYNC_TYPE_INVALID;

        /**
         * Raw parameter data associated with the request.
         */
        uint8_t requestData[MAX_PARAM_LENGTH_FOR_ASYNC_REQUEST];

        /**
         * Logs the basic fields of the request for debugging.
         */
        void print(){
            Logger::log("Request type: " + String((int)type));
            Logger::log("requestData[0]: " + String((int)requestData[0]));
            Logger::log("requestData[1]: " + String((int)requestData[1]));
            Logger::log("requestData[2]: " + String((int)requestData[2]));
            Logger::log("requestData[3]: " + String((int)requestData[3]));
        }
    }AsyncHttpRequest;

    /**
     * Active async request currently being processed by the handler.
     */
    static AsyncHttpRequest currentRequest;

    /**
     * JSON payload created for the asynchronous response.
     */
    static String jsonResponse;

    /**
     * Room-name mapping used while building UI replies.
     */
    static std::map<uint8_t, String>* roomMapping_ptr;

    /**
     * Device-to-room mapping list used by the async UI responses.
     */
    static std::map<uint8_t, std::vector<DeviceDescription*>>* deviceToRoomMappingList_ptr;

    /**
     * Clears the current request state after completion or cancellation.
     */
    static void currentRequestClear();

    /**
     * Executes the active async request and prepares the response payload.
     */
    static void processRequest();

    /**
     * Produces a device-service call payload for async execution.
     */
    static void createServiceCall();

    /**
     * Downloads the advanced-control UI data for the front-end.
     */
    static void downloadAdvancedControls();

    /**
     * Generates the JSON payload that will be returned to the requester.
     */
    static void createJsonResponse();

    /**
     * Creates JSON representing the main page content.
     */
    static void createMainPageContentJson();

    /**
     * Creates JSON describing the page hash used by the client to invalidate stale UI data.
     */
    static void createHashJson();

    /**
     * Creates JSON containing the notification count used by the dashboard.
     */
    static void createNotificationCountJson();

    /**
     * Creates JSON containing the notification list for the client.
     */
    static void createNotificationListContentJson();

    /**
     * Creates JSON containing the device configuration snapshot.
     */
    static void createDeviceConfigurationJson();

    /**
     * Creates JSON redirecting the client back to the main page.
     */
    static void createRedirectToMainJson();

    /**
     * Creates JSON containing the extended controls UI section.
     */
    static void createExtendedControls();

    /**
     * Creates JSON exposing system details for UI display.
     */
    static void createSystemDetailsJson();

    public :
    /**
     * Initializes the handler with the room and device-to-room mapping tables.
     * @param roomMapping Pointer to the room-name mapping table.
     * @param deviceToRoomMappingList Pointer to the device-room assignment table.
     */
    static void init(std::map<uint8_t, String>*  roomMapping, std::map<uint8_t, std::vector<DeviceDescription*>>* deviceToRoomMappingList);

    /**
     * Queues a new async request for processing.
     * @param requestType Type of asynchronous action to process.
     * @param requestParamsPtr Pointer to the related request parameters.
     * @param paramsLength Number of bytes in the parameter buffer.
     * @return Result code describing whether the request was accepted.
     */
    static HttpErrorCode createRequest(AsyncRequestType requestType,
                                       uint8_t* requestParamsPtr,
                                       uint16_t paramsLength);

    /**
     * Runs the handler main loop and advances the request state machine.
     */
    static void mainFunction();

    /**
     * Returns the processing state of the active async request.
     * @return Current request state.
     */
    static AsyncRequestState getProcessingState();

    /**
     * Returns the JSON response prepared by the async request handler.
     * @return Completed JSON payload ready to send to the client.
     */
    static String getJsonResponse();


};


#endif