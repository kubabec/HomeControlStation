#include <os/app/remoteControl/remoteControlClient.hpp>
#include <os/datacontainer/datacontainer.hpp>
#include <os/datacontainer/sigmessages.hpp>
#include <os/app/remoteControl/rc_dataTypes.hpp>



static uint16_t nodeId = 10;
static ClientState currentState;
std::queue<MessageUDP> RemoteControlClient::receivedBuffer;

void RemoteControlClient::init()
{       
    currentState = STATE_NODE_INITIAL_DATA;
         
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
}

void RemoteControlClient::processUDPRequest(MessageUDP& msg){
    switch(msg.getId()) {
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

void RemoteControlClient::receiveUDP(MessageUDP& msg){
    receivedBuffer.push(msg);
    MessageUDP::serialPrintMessageUDP(msg);
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
        .nodeId = nodeId,
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
        deviceDescription.nodeId = nodeId;
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
    keepAlive.nodeId = nodeId;
    MessageUDP keepAliveResponse(RESPONSE_KEEP_ALIVE, NETWORK_BROADCAST, 9001);
    keepAliveResponse.pushData((byte*)&keepAlive, sizeof(keepAlive));
    NetworkDriver::sendBroadcast(keepAliveResponse);
}