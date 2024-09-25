#include <os/app/remoteControl/RemoteControlServer.hpp>


long long requestInitialDataTimer = 0;
long long requestDetailedDataTimer = 0;
long long requestKeepAliveTimer = 0;
long long initialDataExitTimer = 0;


std::map<uint16_t, RemoteNodeInformation> RemoteControlServer::remoteNodes;
std::map<uint8_t, RCTranslation> RemoteControlServer::currentIdMapping;

ServerState RemoteControlServer::currentState = STATE_REQUEST_NODE_INITIAL_DATA;
std::queue<MessageUDP> RemoteControlServer::receivedBuffer;

RequestProcessor RemoteControlServer::requestProcessor;

std::vector<OnOffDevice> vecOnOffDevices = {OnOffDevice(13,"RCDev1",0,8),OnOffDevice(10,"RCDevice2",1,9),OnOffDevice(11,"RCDev3",2,10)};

void RemoteControlServer::deinit() {
    
}

void RemoteControlServer::init(){
    //to robi ze funkcje sa widoczne w datacontainer
    DataContainer::setSignalValue(CBK_REMOTE_DEVICE_ENABLE,"RemoteControlServer", static_cast<std::function<bool(uint8_t, bool)> > (RemoteControlServer::deviceEnable));
    DataContainer::setSignalValue(CBK_REMOTE_DEVICE_BRIGHTNESS_CHANGE,"RemoteControlServer", static_cast<std::function<bool(uint8_t, uint8_t)> > (RemoteControlServer::deviceBrightnessChange));
   
    requestInitialDataTimer = millis();
    requestDetailedDataTimer = millis();
    requestKeepAliveTimer = millis();
    initialDataExitTimer = millis();
}

void RemoteControlServer::cyclic(){   
    
     if(!receivedBuffer.empty()){
        processUDPMessage(receivedBuffer.front());
        receivedBuffer.pop();
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
    Serial.println("---> Wysylam REQUEST_NODE_INITIAL_DATA ---> ");
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

    if(nodesToBeRemoved.size() > 0) { 
        for(auto& nodeID : nodesToBeRemoved) {
            remoteNodes.erase (nodeID);
            Serial.println("Removing Node " + String(nodeID) + " due to lack of communication");
        }

        updateDeviceDescriptionSignal();
    }    
}

void RemoteControlServer::receiveUDP(MessageUDP& msg){
    //Serial.println(" Message Id" + String(msg.getId()));
    receivedBuffer.push(msg);
}

void RemoteControlServer::processUDPMessage(MessageUDP& msg) {
    ServerState stateValidForMsg = mapMsgIDToServerState(msg.getId());
    if(currentState == stateValidForMsg) {
        // is handshake message ?
        if(msg.getId() == RESPONSE_NODE_DETAILED_DATA || msg.getId() == RESPONSE_NODE_INITIAL_DATA) {
            handleHandShakeCommunication(msg);
        }
        if(msg.getId() == RESPONSE_KEEP_ALIVE) {
            handleSlaveAliveMonitoring(msg);
        }
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
            // not found - brak info o node, dodajemy info do mapy
                //Serial.println("Nie jestesmy w mapie i dodajemy do mapy...");
                RemoteNodeInformation nodeInfo {
                    .nodeId = receivedInitialData.nodeId,
                    .roomId = receivedInitialData.roomId,
                    .numberOfOnOffDevices = receivedInitialData.numberOfOnOffDevices,
                    .numberOfLedStrips = receivedInitialData.numberOfLedStrips
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
        }
    }

}

void RemoteControlServer::updateDeviceDescriptionSignal() {

    currentIdMapping.clear();

    static uint8_t uniqueRcId = 0;

    std::vector<OnOffDeviceDescription> vecRemoteOnOffDescription;
    
    
    for(auto& remoteNode : remoteNodes) {
        for(auto device : remoteNode.second.devicesCollectionOnOff) {
            RCTranslation translation = {
                .nodeId = remoteNode.first,
                .onDeviceLocalId = device.deviceId
            };

            device.deviceId = uniqueRcId;
            vecRemoteOnOffDescription.push_back(device);
            currentIdMapping.insert({uniqueRcId,translation});
            uniqueRcId ++;
        }

    }

    DataContainer::setSignalValue(SIG_REMOTE_COLLECTION_ONOFF,"RemoteControlServer", vecRemoteOnOffDescription);
    Serial.println("->RemoteControlServer - Ustawienie sygnalu w Data Container");   
}

RCTranslation RemoteControlServer::getTranslationFromUnique(uint8_t uniqueId) {
    RCTranslation retVal; //oryginalId ma default-owo 255
    
     if (currentIdMapping.find(uniqueId) == currentIdMapping.end()) {
            // not found 
            Serial.println("Remote Control Server-Translation for invalid ID received: " + String(uniqueId));
            Serial.println("---------------------------------------");
        } else {
            // found            
            retVal = currentIdMapping.find(uniqueId)->second;
        }
        
    return retVal;
}


bool RemoteControlServer::deviceEnable(uint8_t deviceId, bool state) {
    //magia z odpaleniem zdalnego urzadzenia  
    Serial.println("->Remote Control Server-Enabling RC Device ID : " + String(deviceId) + " State : " + String(state));
    
    RCTranslation val = getTranslationFromUnique(deviceId);
    Serial.println("->Remote Control Server Translation-Device Enable - NodeId: " + String(val.nodeId) + " Local Id: "+ String(val.onDeviceLocalId));

    RcRequest newRequest;
    newRequest.requestId = 77;
    newRequest.targetNodeId = val.nodeId;
    newRequest.targetDeviceId = val.onDeviceLocalId;
    newRequest.type = DISABLE_REQ;

    newRequest.data[15] = 123;
    newRequest.print();

    //requestProcessor.setCurrentRequest(newRequest);

    return true;
}

bool RemoteControlServer::deviceBrightnessChange(uint8_t deviceId, uint8_t brightnessLevel)
{       
    //magia z jasnoscia zdalnego urzadzenia 

    //Serial.println(" Changing Brightness RC Device ID : " + String(deviceId) + " Level : " + String(brightnessLevel));

    return true;
}

