#include <os/app/remoteControl/RemoteControlClient.hpp>
#include <os/datacontainer/DataContainer.hpp>
#include <os/datacontainer/SigMessages.hpp>

//static uint16_t nodeId = 10;
static ClientState currentState;
std::queue<MessageUDP> RemoteControlClient::receivedBuffer;
std::array<std::function<bool(RcRequest&)>, REQ_COUNT> RemoteControlClient::requestReceivers;
std::queue<RcResponse> RemoteControlClient::vecResponseMessage;
std::queue<MessageUDP> RemoteControlClient::pendingTxQueue;

uint8_t RemoteControlClient::localNodeId =255;

void RemoteControlClient::deinit() {
    
}

void RemoteControlClient::init()
{       
    Serial.println("RemoteControlClient init ...");
    DataContainer::setSignalValue(CBK_REGISTER_REQUEST_RECEIVER, static_cast<std::function<bool(RequestType, std::function<bool(RcRequest&)>)> >(RemoteControlClient::registerRequestReceiver));

    DataContainer::setSignalValue(CBK_RESPONSE, static_cast<std::function<bool(RcResponse&)> > (RemoteControlClient::sendResponse));

    currentState = STATE_NODE_INITIAL_DATA;
    localNodeId = std::any_cast<NodeConfiguration>(DataContainer::getSignalValue(SIG_DEVICE_CONFIGURATION)).nodeId;

    Serial.println("... done");
}

void RemoteControlClient::cyclic()
{
    // Sprawdzenie czy jest coś w receivedBuffer
    if(!receivedBuffer.empty()){
        processUDPRequest(receivedBuffer.front());
        receivedBuffer.pop();
    }

    processResponse();
    processPendingTxData();
}

void RemoteControlClient::processPendingTxData()
{
    static long lastTxSendTime = 0;
    const uint8_t frameTransmissionDelay = 20; /* ms */

    if(!pendingTxQueue.empty())
    {
        /* Tx data waiting to be sent */
        if(millis() - lastTxSendTime > frameTransmissionDelay)
        {
            NetworkDriver::sendBroadcast(pendingTxQueue.front());
            // Serial.println("Sending broadcast with message id " + String((int)pendingTxQueue.front().getId()));
            pendingTxQueue.pop();
            
            lastTxSendTime = millis();
        }
    }    
}

void RemoteControlClient::processUDPRequest(MessageUDP& msg){
    switch(msg.getId()) {
        case RC_REQUEST:
            Serial.println("-> Dostałem UDP type RC_REQUEST");
            processGenericRequest(msg);
            
            break;

        case REQUEST_NODE_INITIAL_DATA:
            Serial.println("-> Dostałem UDP REQUEST_NODE_INITIAL_DATA");
            sendInitialDataResponse();


            break;
        case REQUEST_NODE_DETAILED_DATA:
            Serial.println("-> Dostałem UDP REQUEST_NODE_DETAILED_DATA");
            sendDetailedDataResponse();
            break;
        
        case REQUEST_KEEP_ALIVE:
            Serial.println("-> Dostałem UDP REQUEST_KEEP_ALIVE");
            sendKeepAlive();
            break;
        
        case REQUEST_NODE_DETAILED_DATA_FROM_SPECIFIC_SLAVE:
            Serial.println("-> Dostałem UDP REQUEST_NODE_DETAILED_DATA_FROM_SPECIFIC_SLAVE");
            sendDetailedDataResponseFromNode();
            break;


        default: break;

    }
    
}


void RemoteControlClient::processGenericRequest(MessageUDP& msg) {
    if(msg.getPayload().size() == REQEST_SIZE) {
        RcRequest newRequest;
        memcpy(&newRequest, &msg.getPayload().at(0),REQEST_SIZE); //memcopy(dokad, co, wielkosc)
        newRequest.print();
        if(newRequest.targetNodeId == localNodeId) {
            if(newRequest.type >= REQ_FIRST && newRequest.type < UNKNOWN_REQ) {
                //sprawdzenie czy istnieje funkcja w tablicy do obslugi danego typu requestu
                if(requestReceivers.at(newRequest.type)) {
                    //requestReceivers to tablica, newRequest.type to typ zadania, requestReceivers.at(newRequest.type) pobiera odpowiednią funkcję z tablicy requestReceivers na podstawie typu żądania.
                    (requestReceivers.at(newRequest.type))(newRequest);

                }
            }
        }

    }else {
        Serial.println("Request with invalid lenght received, received : " + String((int)msg.getPayload().size()) + ", expected: " + String((int)REQEST_SIZE) );
    }

}

void RemoteControlClient::receiveUDP(MessageUDP& msg){
    receivedBuffer.push(msg);

    if(msg.getId() == RC_REQUEST) {
        MessageUDP::serialPrintMessageUDP(msg);
    }
}

void RemoteControlClient::handleNodeInitialDataState(){    
    currentState = STATE_NODE_DETAILED_DATA;
     

}

void RemoteControlClient::handleNodeDetailedDataState(){
    currentState = STATE_SLAVE_KEEP_ALIVE;
} 

void RemoteControlClient::handleKeepAliveState(){    
    
}

void RemoteControlClient::sendInitialDataResponse(){
    MessageUDP initialDataResponse(RESPONSE_NODE_INITIAL_DATA, NETWORK_BROADCAST, 9001);
    //get onOffCollection Signal Value from DataCointainer 
    std::any onOffCollection = DataContainer::getSignalValue(SIG_COLLECTION_ONOFF);

    NodeInitialData initialData = {
        .nodeId = localNodeId,
        .nodeHash = std::any_cast<uint16_t>(DataContainer::getSignalValue(SIG_RUNTIME_NODE_HASH)),
        .numberOfOnOffDevices = 0,
        .numberOfLedStrips = 0                 
    };


    try
    {
     // try to read onOff collection if signal exist in data container
      std::vector<OnOffDeviceDescription> onOffDescriptionVector  = std::any_cast<std::vector<OnOffDeviceDescription>>(onOffCollection);
      // assign number of devices in collection to initial data payload
      initialData.numberOfOnOffDevices = onOffDescriptionVector.size(); // do struktury initialData utworzonej w linii 79 dodaje ilość OnOff Device istniejących w wektorze onOffDescriptionVector

    }catch (const std::bad_any_cast& e){ }
    
    initialDataResponse.pushData((byte*)&initialData, sizeof(NodeInitialData)); //wkleja do payload

    /* TX transmission will be handled in the available time from cyclic() context */
    pendingTxQueue.push(initialDataResponse);
}

void RemoteControlClient::sendDetailedDataResponse() {
   
    std::any onOffCollection = DataContainer::getSignalValue(SIG_COLLECTION_ONOFF);

    try
    {
     // try to read onOff collection if signal exist in data container
      std::vector<OnOffDeviceDescription> onOffDescriptionVector  = std::any_cast<std::vector<OnOffDeviceDescription>>(onOffCollection);
      for(OnOffDeviceDescription& deviceDescription: onOffDescriptionVector) {
        deviceDescription.nodeId = localNodeId;
        MessageUDP detailedDataResponse(RESPONSE_NODE_DETAILED_DATA, NETWORK_BROADCAST, 9001);
        
        detailedDataResponse.pushData((byte*)&deviceDescription, sizeof(deviceDescription));

        /* TX transmission will be handled in the available time from cyclic() context */
        pendingTxQueue.push(detailedDataResponse);

        Serial.println("->Remote Control Client - ! Wysyłam Detaile Data!");
        //delay(10);

      }
      
    }catch (const std::bad_any_cast& e){ }

}

void RemoteControlClient:: sendKeepAlive() {
    KeepAliveData keepAlive;
    keepAlive.nodeId = localNodeId;

    /* pobranie wartości Hash informujacej czy cos na ESP sie nie zmienilo*/
    keepAlive.nodeHash = std::any_cast<uint16_t>(DataContainer::getSignalValue(SIG_RUNTIME_NODE_HASH));
    // Serial.print("Node sending Keep Alive response local ID :" + String(keepAlive.nodeId));
    // Serial.println(" Node Hash :" + String(keepAlive.nodeHash));

    MessageUDP keepAliveResponse(RESPONSE_KEEP_ALIVE, NETWORK_BROADCAST, 9001);
    keepAliveResponse.pushData((byte*)&keepAlive, sizeof(keepAlive));
    
    /* TX transmission will be handled in the available time from cyclic() context */
    pendingTxQueue.push(keepAliveResponse);
}

bool RemoteControlClient::registerRequestReceiver(RequestType request, std::function<bool(RcRequest&)> receiverCallback) {
    if(request >= REQ_FIRST && request < UNKNOWN_REQ) {
        if(requestReceivers.at(request)) { 
            //receiver allready registered
            return false;
            

        } else {
            requestReceivers.at(request) = receiverCallback;
            return true;
        }
    }
    return false;
}

bool RemoteControlClient::sendResponse(RcResponse& response) {
    //Serial.println("!!! RemoteControlClient - sendResponse do vektora - ");
    
    vecResponseMessage.push(response);

    return true;
   

}
// checking if the vector containing the response to the request has an entry 
bool RemoteControlClient::processResponse() {
    if (!vecResponseMessage.empty()) {
        RcResponse remoteControlResponse = vecResponseMessage.front();
        remoteControlResponse.responseNodeId = localNodeId;
        vecResponseMessage.pop();

        MessageUDP msg(RC_RESPONSE, NETWORK_BROADCAST, 9001);
        msg.pushData((byte*)&remoteControlResponse, remoteControlResponse.getSize());
        

        /* TX transmission will be handled in the available time from cyclic() context */
        pendingTxQueue.push(msg);

        return true;
    } else {        

        return false;
    }
    return true;
}

void RemoteControlClient::sendDetailedDataResponseFromNode(){
    std::any onOffCollection = DataContainer::getSignalValue(SIG_COLLECTION_ONOFF);

    try
    {
     // try to read onOff collection if signal exist in data container
      std::vector<OnOffDeviceDescription> onOffDescriptionVector  = std::any_cast<std::vector<OnOffDeviceDescription>>(onOffCollection);
      for(OnOffDeviceDescription& deviceDescription: onOffDescriptionVector) {
        deviceDescription.nodeId = localNodeId;
        MessageUDP detailedDataResponse(RESPONSE_NODE_DETAILED_DATA_FROM_SPECIFIC_SLAVE, NETWORK_BROADCAST, 9001);
        
        detailedDataResponse.pushData((byte*)&deviceDescription, sizeof(deviceDescription));
        
        /* TX transmission will be handled in the available time from cyclic() context */
        pendingTxQueue.push(detailedDataResponse);

        Serial.println("->Remote Control Client - Wysyłam Detailed data z Node Id : " + String(localNodeId));
        delay(10);
      }
      
    }catch (const std::bad_any_cast& e){ }
}
