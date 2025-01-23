#ifndef HTTP_ASYNC_REQUEST_HANDLER_H
#define HTTP_ASYNC_REQUEST_HANDLER_H
#include <Arduino.h>
#include <map>
#include <devices/device.hpp>
#include <os/datacontainer/DataContainer.hpp>


#define DEVICE_ID_IN_ASYNC_REQUEST_SERVICE_CALL 0
#define MAX_PARAM_LENGTH_FOR_ASYNC_REQUEST 20

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
    ASYNC_GET_NOTIFICATION_LIST,
    ASYNC_TYPE_LAST = ASYNC_GET_PAGE_CONTENT
}AsyncRequestType;

class HTTPAsyncRequestHandler{
    /* Request type definition */
    typedef struct {
        AsyncRequestState state = ASYNC_NO_REQUEST;
        long receivedTime = 0;
        AsyncRequestType type = ASYNC_TYPE_INVALID;
        uint8_t requestData[MAX_PARAM_LENGTH_FOR_ASYNC_REQUEST];

        void print(){
            Serial.println("Request type: " + String((int)type));
            Serial.println("requestData[0]: " + String((int)requestData[0]));
            Serial.println("requestData[1]: " + String((int)requestData[1]));
            Serial.println("requestData[2]: " + String((int)requestData[2]));
            Serial.println("requestData[3]: " + String((int)requestData[3]));
        }
    }AsyncHttpRequest;


    /* variables */
    static AsyncHttpRequest currentRequest;
    static String jsonResponse;
    static std::map<uint8_t, String>* roomMapping_ptr;
    static std::map<uint8_t, std::vector<DeviceDescription*>>* deviceToRoomMappingList_ptr;

    static void currentRequestClear();
    static void processRequest();
    static void createServiceCall();
    static void createJsonResponse();

    static void createMainPageContentJson();
    static void createNotificationListContentJson();

    public :

    static void init(std::map<uint8_t, String>*  roomMapping, std::map<uint8_t, std::vector<DeviceDescription*>>* deviceToRoomMappingList);
    static HttpErrorCode createRequest(AsyncRequestType requestType,
                                       uint8_t* requestParamsPtr,
                                       uint8_t paramsLength);

    static void mainFunction();
    static AsyncRequestState getProcessingState();
    static String getJsonResponse();


};


#endif