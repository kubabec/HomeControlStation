#ifndef REMOTECONTROLSERVER_H
#define REMOTECONTROLSERVER_H
#include <Arduino.h>
#include <os/drivers/NetworkDriver.hpp>
#include <os/datacontainer/DataContainer.hpp>
#include <map>
#include <os/datacontainer/SigMessages.hpp>
#include <os/app/remoteControl/rc_DataTypes.hpp>
#include <os/app/remoteControl/ReqestProcessor.hpp>
#include "os/Logger.hpp"

#define TIME_TO_REPEAT_INITIAL_DATA_REQEST 500
#define TIME_TO_REPEAT_DETAILED_DATA_REQEST 500
#define TIME_TO_REPEAT_KEEP_ALIVE_REQEST 15000
#define TIME_TO_SWITCH_FROM_INITIAL_TO_DETAILED 2000


/* Typ danych opisujacy zadalnego Node*/
typedef struct {
    uint8_t numberOfDevices = 255;          /* Count of on-Node devices */
    MessageUDP::IPAddr nodeIpAddress{0, 0, 0, 0}; /* Node IP address */
    std::vector<DeviceDescription> devicesCollection;
    bool isDeviceCollectionCompleted = false;
    uint64_t lastKeepAliveReceivedTime =0;
    uint16_t lastKnownNodeHash = 0;


    void printLn(){
        Logger::log("Remote Node Info | Devices: " + String(numberOfDevices) +
                " | IP: " + String(nodeIpAddress.octet1) + "." + String(nodeIpAddress.octet2) + "." +
                String(nodeIpAddress.octet3) + "." + String(nodeIpAddress.octet4) +
                " | Hash: " + String((int)lastKnownNodeHash) +
                " | Completed: " + String(isDeviceCollectionCompleted) + "] | ---");
        for(auto& device: devicesCollection) {
            device.print();
        }

    }
} RemoteNodeInformation;


class RemoteControlServer 
{  
    static uint8_t requestIdCounter; //zmienna dla trzymania requestId pakietu UDP ^^^^^^^^^^^^^^
    static bool slaveMonitoringBlockedDueToRequestProcessing;
    static ServerState currentState;
    static std::queue<MessageUDP> receivedBuffer;
    static std::queue<RcRequest> pendingRequestsQueue;
    static std::queue<uint64_t> pendingDDRefreshNodeMACs;

    static std::array<std::function<bool(RcResponse&)>, REQ_COUNT> responseReceivers;

    static RequestProcessor requestProcessor;

    static uint64_t detailedDataPendingNodeMAC;
    
    /* Kontener na informacje o zdalnych Nodach*/
    static std::map<uint64_t, RemoteNodeInformation> remoteNodes;

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
    static void triggerDDRefresh(uint64_t mac); /* Trigger detailed data refresh */
    static void handleDetailedDataRefreshMech(std::vector <uint64_t>& nodesToBeRemoved);

    static uint8_t generateRequestId(); // funkcja do generowania Request Id

    static void updateNetworkNodesInformationSignal();

public:

    static void init();
    static void deinit();
    static void cyclic();
    static void receiveUDP(MessageUDP& msg);

    static uint8_t createRcRequest(RcRequest& newRequest);

    static bool registerResponseReceiver(RequestType request, std::function<bool(RcResponse&)> receiverCallback);
    static void refreshRemoteNodeInfo(uint64_t mac);

    static void updateSlaveInformation(DeviceDescription& deviceDescription, uint16_t newNodeHash);
    
};


#endif