#include <os/app/remoteControl/RemoteControlClient.hpp>
#include <os/datacontainer/DataContainer.hpp>
#include <os/datacontainer/SigMessages.hpp>

//static uint16_t nodeId = 10;
static ClientState currentState;
std::queue<MessageUDP> RemoteControlClient::receivedBuffer;
std::array<std::function<bool(SystemRequest&)>, REQ_COUNT> RemoteControlClient::requestReceivers;
std::queue<SystemResponse> RemoteControlClient::vecResponseMessage;

uint8_t RemoteControlClient::localNodeId =255;

void RemoteControlClient::deinit() {
    
}

void RemoteControlClient::init()
{       
    Serial.println("RemoteControlClient init ...");
    DataContainer::setSignalValue(CBK_REGISTER_REQUEST_RECEIVER,"RemoteControlClient", static_cast<std::function<bool(SystemRequestType, std::function<bool(SystemRequest&)>)> >(RemoteControlClient::registerRequestReceiver));

    DataContainer::setSignalValue(CBK_RESPONSE, "RemoteControlClient",static_cast<std::function<bool(SystemResponse&)> > (RemoteControlClient::sendResponse));

    currentState = STATE_NODE_INITIAL_DATA;
    localNodeId = std::any_cast<NodeConfiguration>(DataContainer::getSignalValue(SIG_DEVICE_CONFIGURATION)).nodeId;

    Serial.println("... done");
}

void RemoteControlClient::cyclic()
{
    // switch (currentState) {

    //     case STATE_NODE_INITIAL_DATA:
    //         handleNodeInitialDataState();
    //         break;

    //     case STATE_NODE_DETAILED_DATA:
    //         handleNodeDetailedDataState();            
    //         break;

    //     case STATE_KEEP_ALIVE:
    //          handleKeepAliveState();
    //         break;
    // }

    // Sprawdzenie czy jest coś w receivedBuffer
    if(!receivedBuffer.empty()){
        processUDPRequest(receivedBuffer.front());
        receivedBuffer.pop();
    }

    processResponse();
}

void RemoteControlClient::processUDPRequest(MessageUDP& msg){
    switch(msg.getId()) {
        case RC_REQUEST:
            processGenericRequest(msg);
            break;

        case REQUEST_NODE_INITIAL_DATA:
            sendInitialDataResponse();

            break;
        case REQUEST_NODE_DETAILED_DATA:
            sendDetailedDataResponse();
            break;
        
        case REQUEST_KEEP_ALIVE:
            sendKeepAlive();
            break;


        default: break;

    }
    
}

SystemRequest mapRcToSystemRequest(RcRequest& request){
    SystemRequest retVal;
    retVal.requestId = request.requestId;
    retVal.type = request.type;
    memcpy(retVal.data, request.data,request.getSize());
    return retVal;
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
                    // jeżeli istnieje to ja wywoluje z parametrem newRequest
                    SystemRequest newSystemRequest = mapRcToSystemRequest(newRequest);
                    //requestReceivers to tablica, newRequest.type to typ zadania, requestReceivers.at(newRequest.type) pobiera odpowiednią funkcję z tablicy requestReceivers na podstawie typu żądania.
                    (requestReceivers.at(newRequest.type))(newSystemRequest);

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
    NetworkDriver::sendBroadcast(initialDataResponse);
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
        NetworkDriver::sendBroadcast(detailedDataResponse);

        Serial.println("->Remote Control Client - ! Wysyłam Detaile Data!");
        delay(10);

      }
      
    }catch (const std::bad_any_cast& e){ }

}

void RemoteControlClient::sendKeepAlive() {
    KeepAliveData keepAlive;
    keepAlive.nodeId = localNodeId;
    MessageUDP keepAliveResponse(RESPONSE_KEEP_ALIVE, NETWORK_BROADCAST, 9001);
    keepAliveResponse.pushData((byte*)&keepAlive, sizeof(keepAlive));
    NetworkDriver::sendBroadcast(keepAliveResponse);
}

bool RemoteControlClient::registerRequestReceiver(SystemRequestType request, std::function<bool(SystemRequest&)> receiverCallback) {
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

bool RemoteControlClient::sendResponse(SystemResponse& response) {
    //Serial.println("!!! RemoteControlClient - sendResponse do vektora - ");
    
    vecResponseMessage.push(response);

    return true;
   

}
// checking if the vector containing the response to the request has an entry 
bool RemoteControlClient::processResponse() {
    if (!vecResponseMessage.empty()) {
        RcResponse remoteControlResponse;
        SystemResponse& currentResponse = vecResponseMessage.front();
        remoteControlResponse.responseId = currentResponse.responseId;
        remoteControlResponse.requestType = currentResponse.type;
        remoteControlResponse.responceNodeId = localNodeId;
        if(currentResponse.isPositive){
            remoteControlResponse.responseType = POSITIVE_RESP;        
        }
        else{
            remoteControlResponse.responseType = NEGATIVE_RESP;
        }
        memcpy(remoteControlResponse.data, currentResponse.data, REQUEST_DATA_SIZE);
        vecResponseMessage.pop();

        Serial.println("!!! Remote Control Client Response - processResponse : ");
        remoteControlResponse.print();

        MessageUDP msg(RC_RESPONSE, NETWORK_BROADCAST, 9001);
        msg.pushData((byte*)&remoteControlResponse, remoteControlResponse.getSize());
        
        NetworkDriver::sendBroadcast(msg);

        Serial.println("!!! RemoteControlClient - UDP Packet Response Send : ");
        MessageUDP::serialPrintMessageUDP(msg);

        return true;
    } else {        

        return false;
    }
    return true;
}
