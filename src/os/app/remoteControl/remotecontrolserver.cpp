#include <os/app/remoteControl/RemoteControlServer.hpp>
#include "os/Logger.hpp"


long long requestInitialDataTimer = 0;
long long requestDetailedDataTimer = 0;
long long requestKeepAliveTimer = 0;
long long initialDataExitTimer = 0;


std::map<uint64_t, RemoteNodeInformation> RemoteControlServer::remoteNodes;
std::array<std::function<bool(RcResponse&)>, REQ_COUNT> RemoteControlServer::responseReceivers;

ServerState RemoteControlServer::currentState = STATE_REQUEST_NODE_INITIAL_DATA;
std::queue<MessageUDP> RemoteControlServer::receivedBuffer;

std::queue<RcRequest> RemoteControlServer::pendingRequestsQueue;
std::queue<uint64_t> RemoteControlServer::pendingDDRefreshNodeMACs;

RequestProcessor RemoteControlServer::requestProcessor;
uint64_t RemoteControlServer::detailedDataPendingNodeMAC;

uint8_t RemoteControlServer::requestIdCounter = 0; // Inicjalizacja zmiennej statycznej poza klasą^^^^^^^^^^^^^^^^^^^^^^
bool RemoteControlServer::slaveMonitoringBlockedDueToRequestProcessing = false;

void RemoteControlServer::deinit() {
    
}

void RemoteControlServer::init(){
    /* Stub test code START */
    // DeviceDescription fakeDescription;
    // fakeDescription.deviceType = type_LED_STRIP;
    // fakeDescription.macAddress = 765432;
    // fakeDescription.deviceId = 1;
    // fakeDescription.roomId = 2;
    // fakeDescription.isEnabled = true;
    // fakeDescription.deviceName = "FakeDevice";
    // fakeDescription.customBytes[0] = 50; // Leds count 
    // fakeDescription.customBytes[2] = 43; // average color R
    // fakeDescription.customBytes[3] = 150; // average color G
    // fakeDescription.customBytes[4] = 180; // average color B
    
    // RemoteNodeInformation fakeNode;
    // fakeNode.numberOfDevices = 1;
    // fakeNode.devicesCollection.push_back(fakeDescription);
    // fakeNode.isDeviceCollectionCompleted = true;
    // fakeNode.lastKeepAliveReceivedTime = millis();
    // fakeNode.lastKnownNodeHash = 1000;

    // remoteNodes.insert({765432, fakeNode});

    /* Stub test code END */

    Logger::log("RemoteControlServer init ...");
    DataContainer::setSignalValue(
        CBK_REGISTER_RESPONSE_RECEIVER,
            static_cast<std::function<bool(RequestType, std::function<bool(RcResponse&)>)> >(RemoteControlServer::registerResponseReceiver)
        );
    
    DataContainer::setSignalValue(
        CBK_CREATE_RC_REQUEST,
            static_cast<std::function<uint8_t(RcRequest&)>>(RemoteControlServer::createRcRequest)
        );
    DataContainer::setSignalValue(
        CBK_UPDATE_RC_SLAVE_INFORMATION,
            static_cast<std::function<void(DeviceDescription&, uint16_t)>>(RemoteControlServer::updateSlaveInformation)
        );

    
    
    requestIdCounter = 0; //inicjalizacja zmiennej do trzymania countera requestów^^^^^^^^^^^^^^^^^^^
    
    requestInitialDataTimer = millis();
    requestDetailedDataTimer = millis();
    requestKeepAliveTimer = millis();
    initialDataExitTimer = millis();

    updateNetworkNodesInformationSignal();

    Logger::log("... done");
}

void RemoteControlServer::cyclic(){   
    /* stub test code */
    // auto pos = remoteNodes.find(765432);
    // pos->second.lastKeepAliveReceivedTime = millis();
    /*stub test code */

    /* did we receive any UDP ? */
    
    if(!receivedBuffer.empty()){
        // Logger::log("Starting processing of udp message");
        processUDPMessage(receivedBuffer.front());
        receivedBuffer.pop();
    }


    /* is there anything we can send ? */
    if(!pendingRequestsQueue.empty()){
        // Logger::log("Pending request processing ongoing");
        /* This call will return true as long as message is being processed, otherwise it will return false */
        if(processPendingRequest(pendingRequestsQueue.front()) == false){
            /* In this case processing returned true due to timeouted request */

            /* we must notify the request sender that processing of the request is completed */
            /* TODO : return status relevant to message timeout */

            RcResponse emptyResponse(
                pendingRequestsQueue.front().getRequestId(),
                0LL,
                pendingRequestsQueue.front().getRequestType(),
                INVALID_REQ_RESP
            );
            
            if(emptyResponse.getRequestType() >= REQ_FIRST && emptyResponse.getRequestType() < UNKNOWN_REQ) {
                if(responseReceivers.at(emptyResponse.getRequestType()) != nullptr){
                    /* forward response in a callback to the request sender */
                    responseReceivers.at(emptyResponse.getRequestType())(emptyResponse);
                }else {

                }
            }

            /* it must be removed from pendign requests queue */
            pendingRequestsQueue.pop();
        }

        // Logger::log("Pending request processing ongoing");
        
    }else {
        slaveMonitoringBlockedDueToRequestProcessing = false;
    }

    switch (currentState) {
        case STATE_REQUEST_NODE_INITIAL_DATA:
            handleRequestNodeInitialDataState();
            break;

        case STATE_REQUEST_NODE_DETAILED_DATA:
            handleRequestNodeDetailedDataState();            
            break;

        case STATE_KEEP_ALIVE:
            if(!slaveMonitoringBlockedDueToRequestProcessing){
                handleKeepAliveState();
            }
            break;
    }

}


void RemoteControlServer::requestNodeInitialData(){
    static uint8_t msgCount=0;
    MessageUDP msg(REQUEST_NODE_INITIAL_DATA, NETWORK_BROADCAST, 9001);
    msg.pushData(msgCount);
    NetworkDriver::sendBroadcast(msg);
    //Logger::log("---> Wysylam REQUEST_NODE_INITIAL_DATA ---> ");
    msgCount++;
}
void RemoteControlServer::requestNodeDetailedData(){
    MessageUDP msg(REQUEST_NODE_DETAILED_DATA, NETWORK_BROADCAST, 9001);
    // Logger::log("---> Wysylam REQUEST_NODE_DETAILED_DATA ---> ");
    NetworkDriver::sendBroadcast(msg);

}
void RemoteControlServer::requestKeepAliveData(){
    MessageUDP msg(REQUEST_KEEP_ALIVE, NETWORK_BROADCAST, 9001);
    NetworkDriver::sendBroadcast(msg);
}
// Funkcja obsługująca stan zadznia danych poczatkowych
void RemoteControlServer::handleRequestNodeInitialDataState() {
    // Sprawdzenie, czy minęło 2 sekundy od ostatniego żądania danych początkowych  
    

    if(abs(millis() - requestInitialDataTimer) > TIME_TO_REPEAT_INITIAL_DATA_REQEST) 
    {
        requestNodeInitialData();
        requestInitialDataTimer = millis();
    }

    if(abs(millis() - initialDataExitTimer) > TIME_TO_SWITCH_FROM_INITIAL_TO_DETAILED) 
    {
        //Logger::log("Zmieniam stan na DETAILED_DATA================");
        currentState = STATE_REQUEST_NODE_DETAILED_DATA;
        initialDataExitTimer = millis();
    }


}

// Funkcja obsługująca stan żądania danych szczegółowych 
void RemoteControlServer::handleRequestNodeDetailedDataState() { 

    //unsigned long currentTime = millis();

    if(remoteNodes.size() > 0) {

        if(abs(millis() - requestDetailedDataTimer) > TIME_TO_REPEAT_DETAILED_DATA_REQEST)
        {
            requestNodeDetailedData();
            requestDetailedDataTimer = millis();
        } else {
            /* sprawdzamy cyklicznie czy zebralismy wszystkie dane dla kazdego node */
            for(auto& node: remoteNodes){
                /* kolekcja zawiera tyle urzadzen ile zadeklarowano w initial data */
                if(!node.second.isDeviceCollectionCompleted && node.second.numberOfDevices == node.second.devicesCollection.size()){
                    node.second.isDeviceCollectionCompleted = true;
                    node.second.printLn();
                }
                
                
            }

            /* sprawdzamy czy wszystkie nody sa zebrane*/
            bool isHandshakeCompleted = true;
            for(auto& node: remoteNodes) {
                if(!node.second.isDeviceCollectionCompleted) {
                    isHandshakeCompleted = false;
                    break;
                }
            }

            if(isHandshakeCompleted) {
                updateDeviceDescriptionSignal();
                currentState = STATE_KEEP_ALIVE;
                for(auto& node: remoteNodes) {
                    node.second.lastKeepAliveReceivedTime = millis();
                }
            }
        }
    }
    else {
        /* brak initial data w mapie wracamy do initial data collection*/
        currentState = STATE_REQUEST_NODE_INITIAL_DATA;
    }

    // Po upływie 2 minut przełącz w stan STATE_KEEP_ALIVE
    // if (currentTime - lastStateChangeTime >= 120000) {
    //     currentState = STATE_KEEP_ALIVE;
    //     lastStateChangeTime = currentTime;
    // }
}

// Funkcja obsługująca stan keep alive
void RemoteControlServer::handleKeepAliveState() {
    if(abs(millis() - requestKeepAliveTimer) > TIME_TO_REPEAT_KEEP_ALIVE_REQEST)
    {
        requestKeepAliveData();
        requestKeepAliveTimer = millis();
    }

    std::vector <uint64_t> nodesToBeRemoved;
    for(auto& node: remoteNodes) {
        if(millis() - node.second.lastKeepAliveReceivedTime > (6*TIME_TO_REPEAT_KEEP_ALIVE_REQEST)) {
            nodesToBeRemoved.push_back(node.first);
        }
        
    }

    /* Handle refresh of specific node detailed data update if ongoing */
    handleDetailedDataRefreshMech(nodesToBeRemoved);

    if(nodesToBeRemoved.size() > 0) { 
        for(auto MAC : nodesToBeRemoved) {
            remoteNodes.erase(MAC);
            Logger::log("Removing Node due to lack of communication MAC" + String((int)MAC));
            UserInterfaceNotification notif;
            notif.title = "Node disconnected";
            notif.body = "Node with MAC" + String(MAC) + " disconnected.";
            notif.type = UserInterfaceNotification::WARNING;
            // std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);
        }

        updateDeviceDescriptionSignal();
    }  

}

void RemoteControlServer::handleDetailedDataRefreshMech(std::vector <uint64_t>& nodesToBeRemoved)
{
    if( detailedDataPendingNodeMAC == 0 &&  /* No refresh ongoing */
        !pendingDDRefreshNodeMACs.empty() /* There is at least some node waiting for refresh */)
    {
        detailedDataPendingNodeMAC = pendingDDRefreshNodeMACs.front(); /* Start refresh for first in the queue */
        pendingDDRefreshNodeMACs.pop(); /* Remove already processed */

        remoteNodes.find(detailedDataPendingNodeMAC)->second.devicesCollection.clear(); // czyścimy wektor z urzadzeniami na danym MAC
    }

    
    /* Time snapshot to do not overload the network and pause between following frames */
    static long lastDetailedDataRequestTime = 0;
    /* Timeout retry counter */
    static uint8_t detailedDataRefresh_retryCount = 0;

    /* Detailed data from specific slave must be collected ?*/  
    if(detailedDataPendingNodeMAC != 0 ){
        /* Enough time passed since last request try? */
        if(millis() - lastDetailedDataRequestTime > 3000){
            if(detailedDataRefresh_retryCount > 5){
                /* Maximum number of times to ask for detailed data exceeded */
                /* Node to be removed from the list */
                nodesToBeRemoved.push_back(detailedDataPendingNodeMAC);
                detailedDataPendingNodeMAC = 0 ;
                Logger::log("Removing slave due to detailed data refresh failure");
            }else {
                refreshRemoteNodeInfo(detailedDataPendingNodeMAC);
                lastDetailedDataRequestTime = millis();
                
                detailedDataRefresh_retryCount ++;
            }
        }
    }else {
        detailedDataRefresh_retryCount = 0;
        lastDetailedDataRequestTime = 0;
    }
}

void RemoteControlServer::receiveUDP(MessageUDP& msg){
    /* Received UDP Message */
    receivedBuffer.push(std::move(msg));
}

void RemoteControlServer::processUDPMessage(MessageUDP& msg) {
    /* validate handshake state before processing */
    ServerState stateValidForMsg = mapMsgIDToServerState(msg.getId());
    if(currentState == stateValidForMsg) {
        // is handshake message ?
        if(msg.getId() == RESPONSE_NODE_DETAILED_DATA || msg.getId() == RESPONSE_NODE_INITIAL_DATA) {
            handleHandShakeCommunication(msg);
            //Logger::log("<-RESPONSE_NODE_INITIAL/DETAILED_DATA");
        }
        if(msg.getId() == RESPONSE_KEEP_ALIVE) {
            if(!slaveMonitoringBlockedDueToRequestProcessing){
                handleSlaveAliveMonitoring(msg);
            }
            //Logger::log("RESPONSE_KEEP_ALIVE");
        }
        if(msg.getId() == RESPONSE_NODE_DETAILED_DATA_FROM_SPECIFIC_SLAVE) {
            handleDetailedDataUpdate(msg);
            //Logger::log("<-RESPONSE_NODE_DETAILED_DATA_FROM_SPECIFIC_SLAVE");
        }
    }


    /* Process response from the slave */
    if(msg.getId() == RC_RESPONSE)
    {
        /* Process RcResponse */
        Logger::log("->Device Provider received Message with ID: " + String((int)msg.getId()));
        processReceivedRcResponse(msg);
        //Logger::log("<-RC_RESPONSE");
    }


}

ServerState RemoteControlServer::mapMsgIDToServerState(int msgID) {
    if(msgID == RESPONSE_NODE_INITIAL_DATA){
        return ServerState::STATE_REQUEST_NODE_INITIAL_DATA;
    }
    if(msgID == RESPONSE_NODE_DETAILED_DATA){
        return ServerState::STATE_REQUEST_NODE_DETAILED_DATA;
    }
    if(msgID == RESPONSE_KEEP_ALIVE){
        return ServerState::STATE_KEEP_ALIVE;
    }
    if(msgID == RESPONSE_NODE_DETAILED_DATA_FROM_SPECIFIC_SLAVE){
        return ServerState::STATE_KEEP_ALIVE; 
    }


    return ServerState::STATE_INVALID;
}

void RemoteControlServer::handleHandShakeCommunication(MessageUDP& msg) {
    if(msg.getId() == RESPONSE_NODE_INITIAL_DATA) {
        Logger::log("dostalismy msg i ID RESPONSE_NODE_INITIAL_DATA");
        NodeInitialData receivedInitialData = getInitialDataFromPayload(msg);

        if(receivedInitialData.isValid() ) {
            //Logger::log("Sprawdzamy czy node jest w mapie.......................");
            //czy w mapie jest już node od ktorego dostalismy wiadomosc receivedInitialData
            if (remoteNodes.find(receivedInitialData.macAddress) == remoteNodes.end()) {
                // not found - brak info o node, dodajemy info do mapy
                RemoteNodeInformation nodeInfo {
                    .numberOfDevices = receivedInitialData.numberOfDevices,
                    .nodeIpAddress = msg.getIPAddress(),
                    .lastKnownNodeHash = receivedInitialData.nodeHash
                };

                remoteNodes.insert({receivedInitialData.macAddress, nodeInfo});
                nodeInfo.printLn();
                updateNetworkNodesInformationSignal();
                Logger::log("New Node Added, current remote nodes content:");                
                for(auto& node:remoteNodes){ // pętla iterująca przez wszystkie elementy w mapie remoteNodes
                    node.second.printLn(); // node.second oznacza, że korzystamy z drugiego elementu pary z każdego wpisu.
                }               
                //Logger::log("=========== Rozmiar mapy: " + String(RemoteControlServer::remoteNodes.size()));

            } else {
                

            }

        }
        
    }
    else if(msg.getId() == RESPONSE_NODE_DETAILED_DATA) {
        DeviceDescription receivedDescription;
        /* Size of payload (DeviceDescription only) and receivedDescription must be equal, otherwise message is incorrect */
        if(msg.getPayload().size() == receivedDescription.getSize()){
            /* Try to deserialize DeviceDescription from received payload */
            if(receivedDescription.fromByteArray(msg.getPayload().data(), receivedDescription.getSize())){
        
                /* sprawdz czy jest w mapie Node dla ktorego dostalismy description*/
                if (remoteNodes.find(receivedDescription.macAddress) == remoteNodes.end()) {
                    // not found
                    Logger::log("Received Node ID Not Found");
                } else {
                    // found
                    /* do zmiennej collectionVecRef przypisujemy referencje do wlasciwej kolekcji (wektora)device'ow dla otrzymanego MAC */
                    std::vector<DeviceDescription>& collectionVecRef = remoteNodes.find(receivedDescription.macAddress)->second.devicesCollection;
                    bool isDeviceAlreadyInCollection = false;
                    /* prawdz czy otrzymanego dvice nie ma juz w kolekcji*/
                    for(auto& device: collectionVecRef) {
                        if(device.deviceId == receivedDescription.deviceId){
                            isDeviceAlreadyInCollection = true;
                            //Logger::log("Mam go w kolekcji");
                            break;
                        }
                    }
                    /* dodaj do kolekcji jesli nie istnieje*/
                    if(!isDeviceAlreadyInCollection) {
                        collectionVecRef.push_back(receivedDescription);
                        //Logger::log("Dodaje do kolekcji");
                    }
                    else{
                        Logger::log("Device allready PRESENT");
                    }   
                }
            }else {
                Logger::log("RemoteControlServer:// Problem during DeviceDescription deserialization");
            }
        }else {
            Logger::log("RemoteControlServer:// Invalid length of received DeviceDescription message");
        }
    }
}

void RemoteControlServer::handleDetailedDataUpdate(MessageUDP& msg){

    DeviceDescription receivedDescription;
    /* Size of payload (DeviceDescription only) and receivedDescription must be equal, otherwise message is incorrect */
    if(msg.getPayload().size() == receivedDescription.getSize()){
        /* Try to deserialize DeviceDescription from received payload */
        if(receivedDescription.fromByteArray(msg.getPayload().data(), receivedDescription.getSize())){
        
            /* sprawdz czy jest w mapie Node dla ktorego dostalismy description*/
            if (remoteNodes.find(receivedDescription.macAddress) == remoteNodes.end()) {
                // not found
                Logger::log("Received Node ID Not Found");
            } else {
                // found
                /* do zmiennej collectionVecRef przypisujemy referencje do wlasciwej kolekcji (wektora) device'ow dla otrzymanego MAC */
                std::vector<DeviceDescription>& collectionVecRef = remoteNodes.find(receivedDescription.macAddress)->second.devicesCollection;
                bool isDeviceAlreadyInCollection = false;
                /* prawdz czy otrzymanego dvice nie ma juz w kolekcji*/
                for(auto& device: collectionVecRef) {
                    if(device.deviceId == receivedDescription.deviceId){
                        isDeviceAlreadyInCollection = true;
                        //Logger::log("Mam go w kolekcji");
                        break;
                    }
                }
                /* dodaj do kolekcji jesli nie istnieje*/
                if(!isDeviceAlreadyInCollection){
                    collectionVecRef.push_back(receivedDescription);
                    if(remoteNodes.find(receivedDescription.macAddress)->second.numberOfDevices == collectionVecRef.size()) {
                        detailedDataPendingNodeMAC = 0 ; /* reset ID to do not repeat the requests anymore */
                        updateDeviceDescriptionSignal();
                    }
                }
                else{
                    Logger::log("Device allready PRESENT");
                }   
            }
        }else {
            Logger::log("RemoteControlServer:// Problem during DeviceDescription deserialization");
        }
    }else {
        Logger::log("RemoteControlServer:// Invalid length of received DeviceDescription message");
    }
}

NodeInitialData RemoteControlServer::getInitialDataFromPayload(MessageUDP& msg) {
    NodeInitialData returnValue;
    std::vector<uint8_t>& payload = msg.getPayload();
    if(payload.size() ==  sizeof(NodeInitialData))  {
        memcpy(&returnValue, &(payload.at(0)),sizeof(NodeInitialData)); //gdzie, skad, wielkosc

    }
    
    return returnValue;
}

void RemoteControlServer::handleSlaveAliveMonitoring(MessageUDP& msg) {
    KeepAliveData receivedKeepAlive;

    std::vector<uint8_t>& payload = msg.getPayload();
    if(payload.size() ==  sizeof(receivedKeepAlive))  {
        memcpy(&receivedKeepAlive, &(payload.at(0)),sizeof(receivedKeepAlive)); //gdzie, skad, wielkosc
        if (remoteNodes.find(receivedKeepAlive.mac) == remoteNodes.end()) {
            // not found 
            remoteNodes.clear();
            currentState = STATE_REQUEST_NODE_INITIAL_DATA;
            Logger::log("Reinit started unknow received");
        } else {
            // found
            remoteNodes.find(receivedKeepAlive.mac)->second.lastKeepAliveReceivedTime = millis();
            //Logger::log("<-Received Node ID :" + String(receivedKeepAlive.mac) + " Received Hash :" + String(receivedKeepAlive.nodeHash));

            /* Node hash validation */
            if(remoteNodes.find(receivedKeepAlive.mac)->second.lastKnownNodeHash != receivedKeepAlive.nodeHash)
            {
                Logger::log("Old hash : " + String((int)remoteNodes.find(receivedKeepAlive.mac)->second.lastKnownNodeHash));
                Logger::log("New hash : " + String((int)receivedKeepAlive.nodeHash));
                Logger::log("RCServer//: Detected slave state change, DD refresh start ...");
                /* Detailed data collection refresh needed */
                triggerDDRefresh(receivedKeepAlive.mac);

                /* Update last known hash */
                remoteNodes.find(receivedKeepAlive.mac)->second.lastKnownNodeHash = receivedKeepAlive.nodeHash;
            }   
        }
    }

}

void RemoteControlServer::updateDeviceDescriptionSignal() {
    std::vector<DeviceDescription> mergedTunneledDevices;
    for(auto& remoteNode : remoteNodes) {
        for(auto device : remoteNode.second.devicesCollection) {
            mergedTunneledDevices.push_back(device);
        }
    }
    DataContainer::setSignalValue(
        SIG_RC_DEVICES_INTERNAL_TUNNEL, 
        mergedTunneledDevices
    );

}

void RemoteControlServer::triggerDDRefresh(uint64_t macAddr)
{
    /* Add node id for which DetailedData shall be refreshed ,
    Refresh procedure takes place in handleDetailedDataRefreshMech() */
    pendingDDRefreshNodeMACs.push(macAddr);
}

bool RemoteControlServer::processPendingRequest(RcRequest& request){
    /* fake update of slaves activity to do not lost communication when other requests are being processed */
    for(auto& remoteNode : remoteNodes){
        remoteNode.second.lastKeepAliveReceivedTime = millis();
    }


    slaveMonitoringBlockedDueToRequestProcessing = true;
    // Logger::log("RCServer| Processing request with ID "+ String((int)request.getRequestId()));
    return requestProcessor.processReqest(request);
}

void RemoteControlServer::processReceivedRcResponse(MessageUDP& msg)
{
    /* TODO : Unpack payload MessageUdp -> RcResponse */
    RcResponse response;

    /* Try to construct response from the payload data */
    if(response.fromByteArray(msg.getPayload().data(), msg.getPayload().size())){
        // Logger::log("Receiver response, checking validity ...");
        // response.print();

        if(response.isValid()){
            /* Does received response match currently processed request? */
            if(pendingRequestsQueue.front().getRequestId() == response.getResponseId() &&
                pendingRequestsQueue.front().getRequestType() == response.getRequestType()){
                /* remove processed request as we received the response */
                pendingRequestsQueue.pop();

                /* Do we have receiver registered for this type of the request ? */
                if(responseReceivers.at(response.getRequestType())){
                    Logger::log("RCS:// Forwarding response to the request author...");
                    /* forward response in a callback to the request sender */
                    responseReceivers.at(response.getRequestType())(response);
                }
            }
        }else {
            Logger::log("Invalid response received!");
        }
    }else {
        Logger::log("Unable to unpack the response");
    }
      
    /* TODO : Drop incorrect response (e.g. wrong service type, wrong slave ID, wrong CRC ) */
    /* TODO : Call the request source application (e.g. DeviceProvider) to provide the response but remember to
              map the type into RcResponse -> SystemResponse */
}

bool RemoteControlServer::registerResponseReceiver(RequestType request, std::function<bool(RcResponse&)> receiverCallback) {
    if(request >= REQ_FIRST && request < UNKNOWN_REQ) {
        if(responseReceivers.at(request)) { 
            //receiver allready registered
            return false;
            

        } else {
            responseReceivers.at(request) = receiverCallback;
            return true;
        }
    }
    return false;
}

uint8_t RemoteControlServer::generateRequestId() {
    return requestIdCounter++;
}
/* funkcja służy do update urzadzen i stanow na danym node (np po otrzymaniu pakietu response) */
void RemoteControlServer::refreshRemoteNodeInfo(uint64_t macAddr){
    
    MessageUDP msg(REQUEST_NODE_DETAILED_DATA_FROM_SPECIFIC_SLAVE,NETWORK_BROADCAST, 9001);
    msg.pushData((byte*)&macAddr, sizeof(uint64_t));
    NetworkDriver::sendBroadcast(msg);
    Logger::log("->RCS - wysylam o detailed data dla node, MAC:"+ String(int(macAddr)));
    // msg.serialPrintMessageUDP(msg);
}


uint8_t RemoteControlServer::createRcRequest(RcRequest& newRequest)
{
    newRequest.setID(generateRequestId());

    /* Creating new request */
    Logger::log("RCServer| Creating new request with ID "+ String((int)newRequest.getRequestId()));
    pendingRequestsQueue.push(std::move(newRequest));
    // Logger::log("RCServer| Request with ID "+ String((int)newRequest.getRequestId()) + " added to the queue");

    return newRequest.getRequestId();
}


void RemoteControlServer::updateSlaveInformation(DeviceDescription& deviceDescription, uint16_t newNodeHash)
{
    uint64_t macOfUpdatedSlave = deviceDescription.macAddress;
    Logger::log(String((int)macOfUpdatedSlave));

    if (remoteNodes.find(macOfUpdatedSlave) != remoteNodes.end()) {
        std::vector<DeviceDescription>& slaveDevices = remoteNodes.find(macOfUpdatedSlave)->second.devicesCollection;
        remoteNodes.find(macOfUpdatedSlave)->second.lastKnownNodeHash = newNodeHash; /* update hash */
        for(auto& device : slaveDevices){
            if(device.deviceId == deviceDescription.deviceId) { /* device found in the list */
                device = deviceDescription; /* overwrite description */
                updateDeviceDescriptionSignal();
                break;
            }
        }


    }else {
        /* Slave not found */
        Logger::log("Trying to update information about non existing slave");
    }

}

void RemoteControlServer::updateNetworkNodesInformationSignal()
{
    std::vector<NetworkNodeInfo> networkNodes;
    try {
        uint32_t localIpAddress = std::any_cast<uint32_t>(DataContainer::getSignalValue(SIG_IP_ADDRESS));

        NetworkNodeInfo localNodeInfo;
        localNodeInfo.nodeType = NetworkNodeInfo::NodeType::Master;
        uint8_t* ip = (uint8_t*)&localIpAddress;
        localNodeInfo.nodeIP = {
            ip[0],
            ip[1],
            ip[2],
            ip[3]
        };
        networkNodes.push_back(localNodeInfo);
    }
    catch (const std::bad_any_cast& e) {
        Logger::log("Error: " + String(e.what()));
        // return;
    }
    
    for(auto& node: remoteNodes) {
        NetworkNodeInfo nodeInfo;
        nodeInfo.nodeType = NetworkNodeInfo::NodeType::Slave;
        nodeInfo.nodeIP = node.second.nodeIpAddress;
        networkNodes.push_back(nodeInfo);
    }

    DataContainer::setSignalValue(SIG_NETWORK_NODES_INFO, static_cast<std::vector<NetworkNodeInfo>>(networkNodes));

}