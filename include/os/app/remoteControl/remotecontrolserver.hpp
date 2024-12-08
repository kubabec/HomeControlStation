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
    uint8_t numberOfOnOffDevices = 255;
    uint8_t numberOfLedStrips = 255;
    std::vector<OnOffDeviceDescription> devicesCollectionOnOff;
    bool isOnOffCollectionCompleted = false;
    uint64_t lastKeepAliveReceivedTime =0;
    uint16_t lastKnownNodeHash = 0;


    void printLn(){
        Serial.println("------- Remote Node Information --------------");
        Serial.println("Node Id : " + String(nodeId));
        Serial.println("numberOfOnOffDevices : " + String(numberOfOnOffDevices));
        Serial.println("numberOfLedStrips : " + String(numberOfLedStrips));
        for(auto& device: devicesCollectionOnOff) {
            device.print();
        }
        Serial.println("Device Hash : " + String((int)lastKeepAliveReceivedTime));
        Serial.println("isOnOffCollectionCompleted : " + String(isOnOffCollectionCompleted));
        Serial.println("------- ----------------------- --------------");                
    }
} RemoteNodeInformation;


class RemoteControlServer 
{  
    static uint8_t requestIdCounter; //zmienna dla trzymania requestId pakietu UDP ^^^^^^^^^^^^^^

    static ServerState currentState;
    static std::queue<MessageUDP> receivedBuffer;
    static std::queue<RcRequest> pendingRequestsQueue;
    static std::queue<uint8_t> pendingDDRefreshNodeIdentifiers;

    static std::vector<OnOffDevice> vecRemoteOnOffDevices;
    static std::array<std::function<bool(RcResponse&)>, REQ_COUNT> responseReceivers;

    static RequestProcessor requestProcessor;

    static uint8_t detailedDataPendingNodeID;
    
    /* Kontener na informacje o zdalnych Nodach*/
    static std::map<uint16_t, RemoteNodeInformation> remoteNodes;

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
    static void handleDetailedDataUpdate(MessageUDP& msg);

    static ServerState mapMsgIDToServerState(int msgID); // zwraca server state w którym powinniśmy obsłużyć message o zadanym msgID
    static NodeInitialData getInitialDataFromPayload(MessageUDP& msg);

    static void updateDeviceDescriptionSignal();
    static void triggerDDRefresh(uint8_t nodeID); /* Trigger detailed data refresh */
    static void handleDetailedDataRefreshMech(std::vector <uint16_t>& nodesToBeRemoved);

    static uint8_t generateRequestId(); // funkcja do generowania Request Id

public:

    static void init();
    static void deinit();
    static void cyclic();
    static void receiveUDP(MessageUDP& msg);

    static void createRcRequest(RcRequest& newRequest);

    static bool registerResponseReceiver(RequestType request, std::function<bool(RcResponse&)> receiverCallback);
    static void refreshRemoteNodeInfo(uint8_t nodeId);
    
};


#endif