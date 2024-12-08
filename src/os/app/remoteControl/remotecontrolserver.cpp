#include <os/app/remoteControl/RemoteControlServer.hpp>


long long requestInitialDataTimer = 0;
long long requestDetailedDataTimer = 0;
long long requestKeepAliveTimer = 0;
long long initialDataExitTimer = 0;


std::map<uint16_t, RemoteNodeInformation> RemoteControlServer::remoteNodes;
std::array<std::function<bool(RcResponse&)>, REQ_COUNT> RemoteControlServer::responseReceivers;

ServerState RemoteControlServer::currentState = STATE_REQUEST_NODE_INITIAL_DATA;
std::queue<MessageUDP> RemoteControlServer::receivedBuffer;

std::queue<RcRequest> RemoteControlServer::pendingRequestsQueue;
std::queue<uint8_t> RemoteControlServer::pendingDDRefreshNodeIdentifiers;

RequestProcessor RemoteControlServer::requestProcessor;
uint8_t RemoteControlServer::detailedDataPendingNodeID = 255;

uint8_t RemoteControlServer::requestIdCounter = 0; // Inicjalizacja zmiennej statycznej poza klasą^^^^^^^^^^^^^^^^^^^^^^

std::vector<OnOffDevice> vecOnOffDevices = {OnOffDevice(13,"RCDev1",0,8),OnOffDevice(10,"RCDevice2",1,9),OnOffDevice(11,"RCDev3",2,10)};

void RemoteControlServer::deinit() {
    
}

void RemoteControlServer::init(){
    Serial.println("RemoteControlServer init ...");
    DataContainer::setSignalValue(
        CBK_REGISTER_RESPONSE_RECEIVER,
        "RemoteControlServer", 
            static_cast<std::function<bool(RequestType, std::function<bool(RcResponse&)>)> >(RemoteControlServer::registerResponseReceiver)
        );
    
    DataContainer::setSignalValue(
        CBK_CREATE_RC_REQUEST,
        "RemoteControlServer", 
            static_cast<std::function<void(RcRequest&)>>(RemoteControlServer::createRcRequest)
        );
    
    
    requestIdCounter = 0; //inicjalizacja zmiennej do trzymania countera requestów^^^^^^^^^^^^^^^^^^^
    
    requestInitialDataTimer = millis();
    requestDetailedDataTimer = millis();
    requestKeepAliveTimer = millis();
    initialDataExitTimer = millis();

    Serial.println("... done");
}

void RemoteControlServer::cyclic(){   
    /* did we receive any UDP ? */
    if(!receivedBuffer.empty()){
        processUDPMessage(receivedBuffer.front());
        receivedBuffer.pop();
    }


    /* is there anything we can send ? */
    if(!pendingRequestsQueue.empty()){
        /* This call will return true as long as message is being processed, otherwise it will return false */
        if(processPendingRequest(pendingRequestsQueue.front()) == false){
            /* In this case processing returned true due to timeouted request */
            /* it must be removed from pendign requests queue */
            pendingRequestsQueue.pop();
            DataContainer::setSignalValue(SIG_IS_UI_BLOCKED, "RCDevManager", static_cast<bool>(false));
        }
        
    }

    switch (currentState) {
        case STATE_REQUEST_NODE_INITIAL_DATA:
            handleRequestNodeInitialDataState();
            break;

        case STATE_REQUEST_NODE_DETAILED_DATA:
            handleRequestNodeDetailedDataState();            
            break;

        case STATE_KEEP_ALIVE:
            handleKeepAliveState();
            break;
    }

}


void RemoteControlServer::requestNodeInitialData(){
    static uint8_t msgCount=0;
    MessageUDP msg(REQUEST_NODE_INITIAL_DATA, NETWORK_BROADCAST, 9001);
    msg.pushData(msgCount);
    NetworkDriver::sendBroadcast(msg);
    //Serial.println("---> Wysylam REQUEST_NODE_INITIAL_DATA ---> ");
    msgCount++;
}
void RemoteControlServer::requestNodeDetailedData(){
    MessageUDP msg(REQUEST_NODE_DETAILED_DATA, NETWORK_BROADCAST, 9001);
    Serial.println("---> Wysylam REQUEST_NODE_DETAILED_DATA ---> ");
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
        //Serial.println("Zmieniam stan na DETAILED_DATA================");
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
                if(!node.second.isOnOffCollectionCompleted && node.second.numberOfOnOffDevices == node.second.devicesCollectionOnOff.size()){
                    node.second.isOnOffCollectionCompleted = true;
                    node.second.printLn();
                }
                
            }

            /* sprawdzamy czy wszystkie nody sa zebrane*/
            bool isHandshakeCompleted = true;
            for(auto& node: remoteNodes) {
                if(!node.second.isOnOffCollectionCompleted) {
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

    std::vector <uint16_t> nodesToBeRemoved;
    for(auto& node: remoteNodes) {
        if(millis() - node.second.lastKeepAliveReceivedTime > (10*TIME_TO_REPEAT_KEEP_ALIVE_REQEST)) {
            nodesToBeRemoved.push_back(node.first);
        }
        
    }

    /* Handle refresh of specific node detailed data update if ongoing */
    handleDetailedDataRefreshMech(nodesToBeRemoved);

    if(nodesToBeRemoved.size() > 0) { 
        for(auto& nodeID : nodesToBeRemoved) {
            remoteNodes.erase (nodeID);
            Serial.println("Removing Node " + String(nodeID) + " due to lack of communication");
        }

        updateDeviceDescriptionSignal();
    }  

}

void RemoteControlServer::handleDetailedDataRefreshMech(std::vector <uint16_t>& nodesToBeRemoved)
{
    if(detailedDataPendingNodeID == 255 && /* No refresh ongoing */
        !pendingDDRefreshNodeIdentifiers.empty() /* There is at least some node waiting for refresh */)
    {
        detailedDataPendingNodeID = pendingDDRefreshNodeIdentifiers.front(); /* Start refresh for first in the queue */
        pendingDDRefreshNodeIdentifiers.pop(); /* Remove already processed */
        remoteNodes.find(detailedDataPendingNodeID)->second.devicesCollectionOnOff.clear(); // czyścimy wektor z urzadzeniami na danym nodeId
    }

    
    /* Time snapshot to do not overload the network and pause between following frames */
    static long lastDetailedDataRequestTime = 0;
    /* Timeout retry counter */
    static uint8_t detailedDataRefresh_retryCount = 0;

    /* Detailed data from specific slave must be collected ?*/  
    if(detailedDataPendingNodeID != 255){
        /* Enough time passed since last request try? */
        if(millis() - lastDetailedDataRequestTime > 3000){
            if(detailedDataRefresh_retryCount > 5){
                /* Maximum number of times to ask for detailed data exceeded */
                /* Node to be removed from the list */
                nodesToBeRemoved.push_back(detailedDataPendingNodeID);
                detailedDataPendingNodeID = 255;
                Serial.println("Removing slave due to detailed data refresh failure");
            }else {
                refreshRemoteNodeInfo(detailedDataPendingNodeID);
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
    //Serial.println(" ->RCS Push to buffer receive UDP Message Id: " + String(msg.getId()));
    receivedBuffer.push(msg);
}

void RemoteControlServer::processUDPMessage(MessageUDP& msg) {
    /* validate handshake state before processing */
    ServerState stateValidForMsg = mapMsgIDToServerState(msg.getId());
    if(currentState == stateValidForMsg) {
        // is handshake message ?
        if(msg.getId() == RESPONSE_NODE_DETAILED_DATA || msg.getId() == RESPONSE_NODE_INITIAL_DATA) {
            handleHandShakeCommunication(msg);
            //Serial.println("<-RESPONSE_NODE_INITIAL/DETAILED_DATA");
        }
        if(msg.getId() == RESPONSE_KEEP_ALIVE) {
            handleSlaveAliveMonitoring(msg);
            //Serial.println("RESPONSE_KEEP_ALIVE");
        }
        if(msg.getId() == RESPONSE_NODE_DETAILED_DATA_FROM_SPECIFIC_SLAVE) {
            handleDetailedDataUpdate(msg);
            //Serial.println("<-RESPONSE_NODE_DETAILED_DATA_FROM_SPECIFIC_SLAVE");
        }
    }


    /* Process response from the slave */
    if(msg.getId() == RC_RESPONSE)
    {
        /* Process RcResponse */
        processReceivedRcResponse(msg);
        //Serial.println("<-RC_RESPONSE");
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
        //Serial.println("dostalismy msg i ID RESPONSE_NODE_INITIAL_DATA");
        NodeInitialData receivedInitialData = getInitialDataFromPayload(msg);

        if(receivedInitialData.isValid() ) {
            //Serial.println("Sprawdzamy czy node jest w mapie.......................");
            //czy w mapie jest już node od ktorego dostalismy wiadomosc receivedInitialData
            if (remoteNodes.find(receivedInitialData.nodeId) == remoteNodes.end()) {
                DataContainer::setSignalValue(SIG_IS_UI_BLOCKED, "RCDevManager", static_cast<bool>(true));
            // not found - brak info o node, dodajemy info do mapy
                //Serial.println("Nie jestesmy w mapie i dodajemy do mapy...");
                RemoteNodeInformation nodeInfo {
                    .nodeId = receivedInitialData.nodeId,
                    .numberOfOnOffDevices = receivedInitialData.numberOfOnOffDevices,
                    .numberOfLedStrips = receivedInitialData.numberOfLedStrips,
                    .lastKnownNodeHash = receivedInitialData.nodeHash
                };

                remoteNodes.insert({receivedInitialData.nodeId,nodeInfo});
                Serial.println("New Node Added, current remote nodes content:");                
                for(auto& node:remoteNodes){ // pętla iterująca przez wszystkie elementy w mapie remoteNodes
                    node.second.printLn(); // node.second oznacza, że korzystamy z drugiego elementu pary z każdego wpisu.
                }               
                //Serial.println("=========== Rozmiar mapy: " + String(RemoteControlServer::remoteNodes.size()));

            } else {
                

            }

        }
        
    }
    else if(msg.getId() == RESPONSE_NODE_DETAILED_DATA) {
        OnOffDeviceDescription receivedDescription;
        memcpy(&receivedDescription, &(msg.getPayload().at(0)), msg.getPayload().size()); //gdzie, skąd (getpayload to jest wektor databuffer), wielkosc
        
        /* sprawdz czy jest w mapie Node dla ktorego dostalismy description*/
        if (remoteNodes.find(receivedDescription.nodeId) == remoteNodes.end()) {
            // not found
            Serial.println("Received Node ID Not Found");
        } else {
            // found
            /* do zmiennej collectionVecRef przypisujemy referencje do wlasciwej kolekcji (wektora) onOff-ow dla otrzymanego nodeID */
            std::vector<OnOffDeviceDescription>& collectionVecRef = remoteNodes.find(receivedDescription.nodeId)->second.devicesCollectionOnOff;
            bool isDeviceAlreadyInCollection = false;
            /* prawdz czy otrzymanego dvice nie ma juz w kolekcji*/
            for(auto& device: collectionVecRef) {
                if(device.deviceId == receivedDescription.deviceId){
                    isDeviceAlreadyInCollection = true;
                    //Serial.println("Mam go w kolekcji");
                    break;
                }
            }
            /* dodaj do kolekcji jesli nie istnieje*/
            if(!isDeviceAlreadyInCollection) {
                collectionVecRef.push_back(receivedDescription);
                //Serial.println("Dodaje do kolekcji");
            }
            else{
                Serial.println("Device allready PRESENT");
            }   
        }
    }
}

void RemoteControlServer::handleDetailedDataUpdate(MessageUDP& msg){

    OnOffDeviceDescription receivedDescription;
        memcpy(&receivedDescription, &(msg.getPayload().at(0)), msg.getPayload().size()); //gdzie, skąd (getpayload to jest wektor databuffer), wielkosc
        
        /* sprawdz czy jest w mapie Node dla ktorego dostalismy description*/
        if (remoteNodes.find(receivedDescription.nodeId) == remoteNodes.end()) {
            // not found
            Serial.println("Received Node ID Not Found");
        } else {
            // found
            /* do zmiennej collectionVecRef przypisujemy referencje do wlasciwej kolekcji (wektora) onOff-ow dla otrzymanego nodeID */
            std::vector<OnOffDeviceDescription>& collectionVecRef = remoteNodes.find(receivedDescription.nodeId)->second.devicesCollectionOnOff;
            bool isDeviceAlreadyInCollection = false;
            /* prawdz czy otrzymanego dvice nie ma juz w kolekcji*/
            for(auto& device: collectionVecRef) {
                if(device.deviceId == receivedDescription.deviceId){
                    isDeviceAlreadyInCollection = true;
                    //Serial.println("Mam go w kolekcji");
                    break;
                }
            }
            /* dodaj do kolekcji jesli nie istnieje*/
            if(!isDeviceAlreadyInCollection) {
                collectionVecRef.push_back(receivedDescription);
                if(remoteNodes.find(receivedDescription.nodeId)->second.numberOfOnOffDevices == collectionVecRef.size()) {
                    detailedDataPendingNodeID = 255; /* reset ID to do not repeat the requests anymore */
                    updateDeviceDescriptionSignal();
                }
                //Serial.println("Dodaje do kolekcji");
            }
            else{
                Serial.println("Device allready PRESENT");
            }   
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
        if (remoteNodes.find(receivedKeepAlive.nodeId) == remoteNodes.end()) {
            // not found 
            remoteNodes.clear();
            currentState = STATE_REQUEST_NODE_INITIAL_DATA;
            Serial.println("Reinit started unknow received");
        } else {
            // found
            remoteNodes.find(receivedKeepAlive.nodeId)->second.lastKeepAliveReceivedTime = millis();
            //Serial.println("<-Received Node ID :" + String(receivedKeepAlive.nodeId) + " Received Hash :" + String(receivedKeepAlive.nodeHash));

            /* Node hash validation */
            if(remoteNodes.find(receivedKeepAlive.nodeId)->second.lastKnownNodeHash != receivedKeepAlive.nodeHash)
            {
                Serial.println("RCServer//: Detected slave state change, DD refresh start ...");
                /* Detailed data collection refresh needed */
                triggerDDRefresh(receivedKeepAlive.nodeId);

                /* Update last known hash */
                remoteNodes.find(receivedKeepAlive.nodeId)->second.lastKnownNodeHash = receivedKeepAlive.nodeHash;
            }   
        }
    }

}

void RemoteControlServer::updateDeviceDescriptionSignal() {
    std::vector<OnOffDeviceDescription> mergedTunneledDevices;
    for(auto& remoteNode : remoteNodes) {
        for(auto device : remoteNode.second.devicesCollectionOnOff) {
            mergedTunneledDevices.push_back(device);
        }
    }
    DataContainer::setSignalValue(
        SIG_RC_DEVICES_INTERNAL_TUNNEL, 
        "RcServer", 
        mergedTunneledDevices
    );

}

void RemoteControlServer::triggerDDRefresh(uint8_t nodeID)
{
    /* Add node id for which DetailedData shall be refreshed ,
    Refresh procedure takes place in handleDetailedDataRefreshMech() */
    pendingDDRefreshNodeIdentifiers.push(nodeID);
}

bool RemoteControlServer::processPendingRequest(RcRequest& request){
    return requestProcessor.processReqest(request);
    //request.print();
    //return false;
    
}

void RemoteControlServer::processReceivedRcResponse(MessageUDP& msg)
{
    /* TODO : Unpack payload MessageUdp -> RcResponse */
    RcResponse response;
    /* Is received response in valid length ? */
    if(msg.getPayload().size() == response.getSize()){
        memcpy(&response, &msg.getPayload().at(0), response.getSize());
        
        /*PRINT*/
        //msg.serialPrintMessageUDP(msg);
        /* Does received response match currently processed request? */
        if(pendingRequestsQueue.front().requestId == response.responseId &&
           pendingRequestsQueue.front().type == response.requestType){
            /* remove processed request as we received the response */
            pendingRequestsQueue.pop();
  
            /* Do we have receiver registered for this type of the request ? */
            if(responseReceivers.at(response.requestType)){
                /* forward response in a callback to the request sender */
                responseReceivers.at(response.requestType)(response);
            }

            //refreshRemoteNodeInfo(response.responseNodeId);
            if(response.responseType == POSITIVE_RESP){
                if(response.responseNodeId != 255){
                    /* Trigger Detailed Data refresh */
                    //triggerDDRefresh(response.responseNodeId);
                    /* Handling of new device description will be detected by hash check mechanism ,
                    no explicit trigger needed here anymore ! */
                }else{
                    Serial.println("Response from invalid slave with ID 255 received");
                    DataContainer::setSignalValue(SIG_IS_UI_BLOCKED, "RCDevManager", static_cast<bool>(false));
                }
            }else 
            {
                /* Unlock UI immediately as there is no detailed data refresh for NegativeResponse */
                DataContainer::setSignalValue(SIG_IS_UI_BLOCKED, "RCDevManager", static_cast<bool>(false));
            }
        }
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
void RemoteControlServer::refreshRemoteNodeInfo(uint8_t nodeId){
    
    MessageUDP msg(REQUEST_NODE_DETAILED_DATA_FROM_SPECIFIC_SLAVE,NETWORK_BROADCAST, 9001);
    msg.pushData(nodeId);
    NetworkDriver::sendBroadcast(msg);
    Serial.println("->RCS - wysylam o detailed data dla node Id : " + String(nodeId));
    msg.serialPrintMessageUDP(msg);
}


void RemoteControlServer::createRcRequest(RcRequest& newRequest)
{
    newRequest.requestId = generateRequestId();

    /* Creating new request */
    Serial.println("RCServer| Creating new request with ID "+ String((int)newRequest.requestId));
    pendingRequestsQueue.push(newRequest);
}