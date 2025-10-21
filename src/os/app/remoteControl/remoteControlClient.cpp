#include <os/app/remoteControl/RemoteControlClient.hpp>
#include <os/datacontainer/DataContainer.hpp>
#include <os/datacontainer/SigMessages.hpp>
#include "os/Logger.hpp"
#include "esp_heap_caps.h"

static ClientState currentState;
std::queue<MessageUDP> RemoteControlClient::receivedBuffer;
std::array<std::function<bool(RcRequest &)>, REQ_COUNT> RemoteControlClient::requestReceivers;
std::queue<RcResponse> RemoteControlClient::vecResponseMessage;
std::queue<MessageUDP> RemoteControlClient::pendingTxQueue;
uint8_t RemoteControlClient::lastReceivedRequestId = 0xFF;
unsigned long RemoteControlClient::lastMasterActivityTime = 0;
MessageUDP::IPAddr RemoteControlClient::lastKnownMasterIp{0, 0, 0, 0};
Transaction RemoteControlClient::currentTransaction {};

uint64_t RemoteControlClient::localNodeMACAddress;

void RemoteControlClient::deinit()
{
}

void RemoteControlClient::init()
{
    Logger::log("RemoteControlClient init ...");
    DataContainer::setSignalValue(CBK_REGISTER_REQUEST_RECEIVER, static_cast<std::function<bool(RequestType, std::function<bool(RcRequest &)>)>>(RemoteControlClient::registerRequestReceiver));

    DataContainer::setSignalValue(CBK_RESPONSE, static_cast<std::function<bool(RcResponse &)>>(RemoteControlClient::sendResponse));

    currentState = STATE_NODE_INITIAL_DATA;
    localNodeMACAddress = std::any_cast<uint64_t>(DataContainer::getSignalValue(SIG_MAC_ADDRESS));

    Logger::log("... done");
}

void RemoteControlClient::cyclic()
{
    // Sprawdzenie czy jest coś w receivedBuffer
    if (!receivedBuffer.empty())
    {
        processUDPRequest(receivedBuffer.front());
        receivedBuffer.pop();
    }

    processResponse();
    processPendingTxData();


    if(millis() - lastMasterActivityTime > TIME_TO_ASK_FOR_DISCOVERY){
        Logger::log("RemoteControlClient:// No master activity detected, asking for discovery ...");
        MessageUDP discoverMeMsg(DISCOVER_ME_MESSAGE, NETWORK_BROADCAST, 9001);
        NetworkDriver::sendBroadcast(discoverMeMsg);

        lastMasterActivityTime = millis(); // reset timer after sending discovery
    }
}

void RemoteControlClient::processPendingTxData()
{
    static long lastTxSendTime = 0;
    const uint8_t frameTransmissionDelay = 20; /* ms */

    if (!pendingTxQueue.empty())
    {
        /* Tx data waiting to be sent */
        if (millis() - lastTxSendTime > frameTransmissionDelay)
        {
            NetworkDriver::sendBroadcast(pendingTxQueue.front());
            // Logger::log("Sending broadcast with message id " + String((int)pendingTxQueue.front().getId()));
            pendingTxQueue.pop();

            lastTxSendTime = millis();
        }
    }
}

void RemoteControlClient::updateLastKnownMasterIp(MessageUDP::IPAddr address){
    lastMasterActivityTime = millis();
    lastKnownMasterIp = address;
}

void RemoteControlClient::processUDPRequest(MessageUDP &msg)
{
    switch (msg.getId())
    {
    case RC_REQUEST:
        // Logger::log("-> Dostałem UDP type RC_REQUEST");
        Logger::log("RemoteControlClient:// Received RC_REQUEST message.");
        updateLastKnownMasterIp(msg.getIPAddress());
        processGenericRequest(msg);

        break;

    case REQUEST_NODE_INITIAL_DATA:
        // Logger::log("-> Dostałem UDP REQUEST_NODE_INITIAL_DATA");
        Logger::log("RemoteControlClient:// Received REQUEST_NODE_INITIAL_DATA message.");
        updateLastKnownMasterIp(msg.getIPAddress());
        sendInitialDataResponse();

        break;

    case REQUEST_KEEP_ALIVE:
        // Logger::log("-> Dostałem UDP REQUEST_KEEP_ALIVE");
        Logger::log("RemoteControlClient:// Received REQUEST_KEEP_ALIVE message.");
        updateLastKnownMasterIp(msg.getIPAddress());
        sendKeepAlive();
        break;

    case REQUEST_NODE_DETAILED_DATA_FROM_SPECIFIC_SLAVE:
        // Logger::log("-> Dostałem UDP REQUEST_NODE_DETAILED_DATA_FROM_SPECIFIC_SLAVE");
        Logger::log("RemoteControlClient:// Received REQUEST_NODE_DETAILED_DATA_FROM_SPECIFIC_SLAVE message.");
        sendDetailedDataResponse(RESPONSE_NODE_DETAILED_DATA_FROM_SPECIFIC_SLAVE);
        updateLastKnownMasterIp(msg.getIPAddress());
        break;

    default:
        break;
    }
}

void RemoteControlClient::processGenericRequest(MessageUDP &msg)
{
    RcRequest newRequest;
    newRequest.fromByteArray(msg.getPayload().data(), msg.getPayload().size());

    // newRequest.print();
    // Is request targeted to us AND we did not start the processing of this one already (repeat request received)
    if (newRequest.getRequestNodeMAC() == localNodeMACAddress)
    {
        if (lastReceivedRequestId != newRequest.getRequestId())
        {
            if (newRequest.getRequestType() >= REQ_FIRST && newRequest.getRequestType() < UNKNOWN_REQ)
            {
                currentTransaction.request = newRequest;
                currentTransaction.response.reset();

                lastReceivedRequestId = newRequest.getRequestId();
                // sprawdzenie czy istnieje funkcja w tablicy do obslugi danego typu requestu
                if (requestReceivers.at(newRequest.getRequestType()))
                {
                    // requestReceivers to tablica, newRequest.type to typ zadania, requestReceivers.at(newRequest.type) pobiera odpowiednią funkcję z tablicy requestReceivers na podstawie typu żądania.
                    (requestReceivers.at(newRequest.getRequestType()))(newRequest);
                }
            }
        }
        else
        {
            // We probably received repeated request, resend last response
            if(currentTransaction.response.has_value()){
                Logger::log("RemoteControlClient:// Repeated request received, resending last response.");
                sendResponse(currentTransaction.response.value());
            }else {
                // Retry to ask the application for the responce once again
                if (requestReceivers.at(newRequest.getRequestType()))
                {
                    // requestReceivers to tablica, newRequest.type to typ zadania, requestReceivers.at(newRequest.type) pobiera odpowiednią funkcję z tablicy requestReceivers na podstawie typu żądania.
                    (requestReceivers.at(newRequest.getRequestType()))(newRequest);
                }
            }
        }
    }
}

void RemoteControlClient::receiveUDP(MessageUDP &msg)
{
    receivedBuffer.push(msg);

    if (msg.getId() == RC_REQUEST)
    {
        // MessageUDP::serialPrintMessageUDP(msg);
    }
}

void RemoteControlClient::handleNodeInitialDataState()
{
    currentState = STATE_NODE_DETAILED_DATA;
}

void RemoteControlClient::handleNodeDetailedDataState()
{
    currentState = STATE_SLAVE_KEEP_ALIVE;
}

void RemoteControlClient::handleKeepAliveState()
{
}

void RemoteControlClient::sendInitialDataResponse()
{
    MessageUDP initialDataResponse(RESPONSE_NODE_INITIAL_DATA, NETWORK_BROADCAST, 9001);

    NodeInitialData initialData = {
        .macAddress = localNodeMACAddress,
        .nodeHash = std::any_cast<uint16_t>(DataContainer::getSignalValue(SIG_RUNTIME_NODE_HASH)),
        .numberOfDevices = 0};

    try
    {

        std::vector<DeviceDescription> deviceDescriptionVector = std::any_cast<std::vector<DeviceDescription>>(DataContainer::getSignalValue(SIG_DEVICE_COLLECTION));
        /* Paste number of Slave devices */
        initialData.numberOfDevices = deviceDescriptionVector.size();
    }
    catch (const std::bad_any_cast &e)
    {
    }

    // Logger::log("InitialData hash content: " + String((int)initialData.nodeHash));

    initialDataResponse.pushData((byte *)&initialData, sizeof(NodeInitialData)); // wkleja do payload

    Logger::log("<-Remote Control Client - ! Wysyłam Initial Data!");
    // MessageUDP::serialPrintMessageUDP(initialDataResponse);

    /* TX transmission will be handled in the available time from cyclic() context */
    pendingTxQueue.push(initialDataResponse);
}

void RemoteControlClient::sendDetailedDataResponse(UdpFrames_RCS udpHeaderValue)
{
    std::any deviceCollection = DataContainer::getSignalValue(SIG_DEVICE_COLLECTION);

    try
    {
        std::vector<DeviceDescription> deviceDescriptionVector = std::any_cast<std::vector<DeviceDescription>>(deviceCollection);

        uint8_t *bufferForDeviceDescription = nullptr;
        for (DeviceDescription &deviceDescription : deviceDescriptionVector)
        {
            deviceDescription.macAddress = localNodeMACAddress;
            MessageUDP detailedDataResponse(udpHeaderValue, NETWORK_BROADCAST, 9001);

            /* serialize device description to byte array */
            if (bufferForDeviceDescription == nullptr)
            {
                /* allocate buffer only once for the first DD */
                bufferForDeviceDescription = (uint8_t *)malloc(deviceDescription.getSize());
            }
            /* Serialize to byte array */
            if (deviceDescription.toByteArray(bufferForDeviceDescription, deviceDescription.getSize()))
            {
                detailedDataResponse.pushData((byte *)bufferForDeviceDescription, deviceDescription.getSize());

                /* TX transmission will be handled in the available time from cyclic() context */
                pendingTxQueue.push(detailedDataResponse);
            }
            else
            {
                Logger::log("RemoteControlClient:// Error during DeviceDescription serialization.");
            }
        }
        /* release resources if needed */
        if (bufferForDeviceDescription != nullptr)
        {
            free(bufferForDeviceDescription);
        }
    }
    catch (const std::bad_any_cast &e)
    {
    }
}

void RemoteControlClient::sendKeepAlive()
{
    KeepAliveData keepAlive;
    keepAlive.mac = localNodeMACAddress;

    /* pobranie wartości Hash informujacej czy cos na ESP sie nie zmienilo*/
    keepAlive.nodeHash = std::any_cast<uint16_t>(DataContainer::getSignalValue(SIG_RUNTIME_NODE_HASH));
    // Logger::log(" Node Hash :" + String(keepAlive.nodeHash));
    // Logger::log("keepAlive.nodeHash : " + String((int)keepAlive.nodeHash ));

    MessageUDP keepAliveResponse(RESPONSE_KEEP_ALIVE, NETWORK_BROADCAST, 9001);
    keepAliveResponse.pushData((byte *)&keepAlive, sizeof(keepAlive));

    /* TX transmission will be handled in the available time from cyclic() context */
    pendingTxQueue.push(keepAliveResponse);
}

bool RemoteControlClient::registerRequestReceiver(RequestType request, std::function<bool(RcRequest &)> receiverCallback)
{
    if (request >= REQ_FIRST && request < UNKNOWN_REQ)
    {
        if (requestReceivers.at(request))
        {
            // receiver allready registered
            return false;
        }
        else
        {
            requestReceivers.at(request) = receiverCallback;
            return true;
        }
    }
    return false;
}

bool RemoteControlClient::sendResponse(RcResponse &response)
{
    if(currentTransaction.request.getRequestId() != response.getResponseId()){
        Logger::log("RemoteControlClient:// Warning! Sending response with mismatched request ID!");
    }else {
        currentTransaction.response = response;
    }

    vecResponseMessage.push(std::move(response));

    return true;
}
// checking if the vector containing the response to the request has an entry
bool RemoteControlClient::processResponse()
{
    if (!vecResponseMessage.empty())
    {
        // Logger::log("!!! RemoteControlClient - processResponse - ");
        RcResponse &remoteControlResponse = vecResponseMessage.front();
        uint8_t *serializedResponse = (uint8_t *)malloc(remoteControlResponse.getSize());

        if (serializedResponse != nullptr)
        {
            // Logger::log("!!! RemoteControlClient - processResponse - serializedResponse != nullptr - ");
            remoteControlResponse.toByteArray(serializedResponse, remoteControlResponse.getSize());

            MessageUDP msg(RC_RESPONSE, NETWORK_BROADCAST, 9001);
            // Logger::log("!!! RemoteControlClient - processResponse - msg - ");
            // Logger::log("Size of serializedResponse: " + String(remoteControlResponse.getSize()));
            msg.pushData((byte *)serializedResponse, remoteControlResponse.getSize());
            // Logger::log("!!! RemoteControlClient - processResponse - msg.pushData - ");

            /* TX transmission will be handled in the available time from cyclic() context */
            pendingTxQueue.push(std::move(msg));

            // Logger::log("!!! RemoteControlClient - processResponse - pendingTxQueue.push - ");

            free(serializedResponse);
            // Logger::log("!!! RemoteControlClient - processResponse - free(serializedResponse) - ");

            vecResponseMessage.pop();
            return true;
        }
        else
        {
            vecResponseMessage.pop();
            return false;
        }
    }
    else
    {

        return false;
    }
    return false;
}
