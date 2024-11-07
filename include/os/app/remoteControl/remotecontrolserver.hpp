#ifndef REMOTECONTROLSERVER_H
#define REMOTECONTROLSERVER_H
#include <Arduino.h>
#include <os/drivers/NetworkDriver.hpp>
#include <os/datacontainer/DataContainer.hpp>
#include <map>
#include <os/datacontainer/SigMessages.hpp>
#include <os/app/remoteControl/rc_DataTypes.hpp>
#include <devices/OnOffDevice.hpp>
#include <os/app/remoteControl/ReqestProcessor.hpp>

#define TIME_TO_REPEAT_INITIAL_DATA_REQEST 500
#define TIME_TO_REPEAT_DETAILED_DATA_REQEST 500
#define TIME_TO_REPEAT_KEEP_ALIVE_REQEST 500
#define TIME_TO_SWITCH_FROM_INITIAL_TO_DETAILED 2000


/* Typ danych opisujacy zadalnego Node*/
typedef struct {
    uint16_t nodeId = 255;
    uint8_t roomId = 255;
    uint8_t numberOfOnOffDevices = 255;
    uint8_t numberOfLedStrips = 255;
    std::vector<OnOffDeviceDescription> devicesCollectionOnOff;
    bool isOnOffCollectionCompleted = false;
    uint64_t lastKeepAliveReceivedTime =0;

    void printLn(){
        Serial.println("------- Remote Node Information --------------");
        Serial.println("Node Id : " + String(nodeId));
        Serial.println("Room Id : " + String(roomId));
        Serial.println("numberOfOnOffDevices : " + String(numberOfOnOffDevices));
        Serial.println("numberOfLedStrips : " + String(numberOfLedStrips));
        for(auto& device: devicesCollectionOnOff) {
            device.print();
        }
        Serial.println("isOnOffCollectionCompleted : " + String(isOnOffCollectionCompleted));
        Serial.println("------- ----------------------- --------------");                
    }
} RemoteNodeInformation;

struct RCTranslation {
    uint16_t nodeId = 255;
    uint8_t onDeviceLocalId = 255;

    void print() {
        Serial.println(String(nodeId) + ", " + String((int) onDeviceLocalId));
    }
};

class RemoteControlServer 
{
   
    static ServerState currentState;
    static std::queue<MessageUDP> receivedBuffer;
    static std::queue<RcRequest> pendingRequestsQueue;

    static std::vector<OnOffDevice> vecRemoteOnOffDevices;
    static std::array<std::function<bool(SystemResponse&)>, REQ_COUNT> responseReceivers;

    static RequestProcessor requestProcessor;

    /* Kontener na informacje o zdalnych Nodach*/
    static std::map<uint16_t, RemoteNodeInformation> remoteNodes;
    static std::map<uint8_t, RCTranslation> currentIdMapping;

    static void requestNodeInitialData();
    static void requestNodeDetailedData();
    static void requestKeepAliveData();
    
   
    static void processUDPMessage(MessageUDP& msg);
    static bool processPendingRequest(RcRequest& request);
    static void processReceivedRcResponse(MessageUDP& msg);

    static void handleRequestNodeInitialDataState();
    static void handleRequestNodeDetailedDataState();
    static void handleKeepAliveState();
    static void handleHandShakeCommunication(MessageUDP& msg);
    static void handleSlaveAliveMonitoring(MessageUDP& msg);

    static ServerState mapMsgIDToServerState(int msgID); // zwraca server state w którym powinniśmy obsłużyć message o zadanym msgID
    static NodeInitialData getInitialDataFromPayload(MessageUDP& msg);

    static void updateDeviceDescriptionSignal();

    static RCTranslation getTranslationFromUnique(uint8_t uniqueId);
    static void printTranslationMap();

public:
    static void init();
    static void deinit();
    static void cyclic();
    static void receiveUDP(MessageUDP& msg);

    static bool deviceEnable(uint8_t deviceId, bool state);
    static bool deviceBrightnessChange(uint8_t deviceId, uint8_t brightnessLevel);

    static bool registerResponseReceiver(SystemRequestType request, std::function<bool(SystemResponse&)> receiverCallback);
    
};


#endif