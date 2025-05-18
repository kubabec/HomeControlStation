#include <os/tools/MessageUDP.hpp>

MessageUDP::MessageUDP()
{
    /* Initialize as empty message with unknown type */
    id = -1;
    updateTotalSize();
}

MessageUDP::MessageUDP(int myId, MessageUDP::IPAddr addr, int port)
{
    /* Initialize message with requested ID*/
    id = myId;
    ipAddress = addr;
    udpPort = port;

    updateTotalSize();
}

MessageUDP::MessageUDP(int myId, MessageUDP::IPAddr addr, int port, byte* buffer, uint8_t bufferSize)
{
    /* Initialize message with requested ID*/
    id = myId;
    ipAddress = addr;
    udpPort = port;

    /* Copy received data to the buffer */
    if(buffer != nullptr && bufferSize > 0){
        for(int i = 0 ; i < bufferSize; i++)
        {
            dataBuffer.push_back(buffer[i]);
        }
    }
    updateTotalSize();
}

void MessageUDP::setIpAddress(MessageUDP::IPAddr addr)
{
    ipAddress = addr;
}

void MessageUDP::setPort(int port){
    udpPort = port;
}

void MessageUDP::updateTotalSize()
{
    /* MessageId + totalSizeChecksum + 4* IP addr byte + port INT + buffer size  + control byte */
    totalSize = sizeof(id) + sizeof(uint16_t) + sizeof(ipAddress) + sizeof(udpPort) + (dataBuffer.size() * sizeof(byte)) + sizeof(lastByteCheckValue);
}

uint8_t MessageUDP::getMinimumSize()
{
    return sizeof(id) + sizeof(uint16_t) + sizeof(ipAddress) + sizeof(udpPort) + sizeof(lastByteCheckValue);
}

bool MessageUDP::validateMessageId(int messageId)
{
    if(messageId != -1)
    {
        return true;
    }

    return false;
}

bool MessageUDP::validateTotalSize(uint16_t bufferSize, uint16_t secondByteValue)
{
    if(bufferSize == secondByteValue)
    {
        return true;
    }

    return false;
    
}

bool MessageUDP::validateLastByteValue(uint8_t lastByteValue)
{
    if(lastByteValue == lastByteCheckValue)
    {
        return true;
    }

    return false;
    
}

bool MessageUDP::validateIpAddress(IPAddr& addr)
{
    const IPAddr invalidIp{0, 0, 0, 0};
    if(!(addr == invalidIp))
    {
        // IP Address valid
        return true;
    }

    // IP Address invalid
    return false;
}

int MessageUDP::getId()
{
    return id;
}

bool MessageUDP::pushData(byte data)
{
    /* push single byte */
    dataBuffer.push_back(data);
    /* update total size */
    updateTotalSize();

    return true;
}

bool MessageUDP::pushData(byte* arr, size_t size)
{
    if(arr != nullptr)
    {

        //dataBuffer.reserve(dataBuffer.size() + size);
        dataBuffer.insert(dataBuffer.end(), arr, arr + size);
        // Serial.println("MessageSize : " + String((int)dataBuffer.size()) + " bytes");

        /* Update total size */
        updateTotalSize();
    }
    else{
        Serial.println("Nullptr passed to pushData");
    }

    return true;
}

size_t MessageUDP::getSize()
{
    updateTotalSize();
    return totalSize;
}

bool MessageUDP::isValid()
{
    // ID != -1 and ipAddress != invalid address
    return ((totalSize >= getMinimumSize()) && (id != -1) && (!(ipAddress == (IPAddr){0,0,0,0})));
}

/* Funkcja konwertuje obiekt MessageUDP na ciag bajtow */
bool MessageUDP::toByteArray(byte* bufferPtr, size_t sizeCheck)
{
    updateTotalSize(); /* Update total size */
    if(sizeCheck == totalSize && bufferPtr != nullptr) /* check if received bufferPtr size is expected */
    {
        memcpy(&(bufferPtr[0]), &id, sizeof(id)); /* copy ID */
        // bufferPtr[4] = (byte)totalSize; /* size checksum as a second byte */
        // memcpy(&(bufferPtr[5]), &ipAddress, sizeof(ipAddress)); /* copy IP address */
        // memcpy(&(bufferPtr[9]), &udpPort, sizeof(udpPort));

        memcpy(bufferPtr + 4, &totalSize, sizeof(uint16_t)); // Kopiowanie rozmiaru (2 bajty)
        memcpy(bufferPtr + 6, &ipAddress, sizeof(ipAddress)); // Kopiowanie adresu IP
        memcpy(bufferPtr + 10, &udpPort, sizeof(udpPort)); // Kopiowanie portu

        if(dataBuffer.size() > 0){ /* There are any data in the buffer */
            /* 2 - const number of bytes before carried data buffer */
            for(int i = 14 ; i < sizeCheck-1; i++) /* start copying from fifth byte */
            {
                bufferPtr[i]  = dataBuffer.at(i-14); /* copy all buffer data */
            }
        }
        bufferPtr[sizeCheck-1] = (byte)lastByteCheckValue; /* End of message byte */

        return true;
    }

    return false;
}

MessageUDP MessageUDP::fromUint8Vector(std::vector<uint8_t>& vec)
{
    // Serial.println("Constructing new UDP message ...");
    MessageUDP returnedMessage(-1, {0,0,0,0}, 0);

    /* Minimum size is fulfilled */
    if(vec.size() >= getMinimumSize()){
        // Serial.println("Size correct.");

        // Extract id bytes from vector
        uint8_t idExtractBytes[4] = {0};
        for(int i = 0; i < 4; i++) {
            idExtractBytes[i] = vec.at(i);  
        }
        int tmpId = -1;
        memcpy(&tmpId, idExtractBytes, sizeof(tmpId));

        // Copy size byte
        uint16_t tmpSize; //= (uint8_t)vec.at(4);
        memcpy(&tmpSize, vec.data() + 4, sizeof(uint16_t));  // Read 2 bytes

        // Extract IP address bytes from vector
        uint8_t ipAddrBytes[4] = {0};
        for(int i = 0; i < 4; i++) {
            ipAddrBytes[i] = vec.at(i+6);  
        }
        IPAddr tmpAddr{0,0,0,0};
        memcpy(&tmpAddr, ipAddrBytes, sizeof(tmpAddr));


         // Extract port bytes from vector
        uint8_t portBytes[4] = {0};
        for(int i = 0; i < 4; i++) {
            portBytes[i] = vec.at(i+10);  
        }
        int tmpPort = 0;
        memcpy(&tmpPort, portBytes, sizeof(tmpPort));


        uint8_t tmpLastByte = (uint8_t)vec.at(vec.size()-1);
        // Serial.println("tmplastByte " + String(tmpLastByte));

        if( validateMessageId(tmpId) && 
            validateTotalSize(vec.size(), tmpSize) && 
            validateLastByteValue(tmpLastByte) &&
            validateIpAddress(tmpAddr) )
        {
            // Serial.println("Validation completed successfully");
            returnedMessage.id = tmpId;
            returnedMessage.totalSize = tmpSize;
            returnedMessage.ipAddress = tmpAddr;
            returnedMessage.udpPort = tmpPort;
            
            uint16_t dataOnlySize = tmpSize - (getMinimumSize() * sizeof(uint8_t));

            if(dataOnlySize > 0){
                returnedMessage.dataBuffer.reserve(dataOnlySize);

                for(uint16_t i = 14; i < vec.size() - 1; i ++)
                {
                    returnedMessage.dataBuffer.push_back(vec.at(i));
                }
                // Serial.println("Data copying completed with payload size " + String(returnedMessage.dataBuffer.size()) + " bytes");
            }
        }
    }

    // Serial.println("Constructing new message done");
    return returnedMessage;
}

void MessageUDP::serialPrintMessageUDP(MessageUDP& msg)
{
    Serial.println("- - ** UDP Message ** - -");

    Serial.print("ID: " + String(msg.id));
    Serial.print(" TotalSize: " + String(msg.totalSize));
    Serial.print(" IP Address: " + String(msg.ipAddress.octet1) + "." + String(String(msg.ipAddress.octet2)) + "." + String(msg.ipAddress.octet3) + "." + String(msg.ipAddress.octet4));
    Serial.println(" Port: " + String(msg.udpPort));

    Serial.print(" Payload: ");
    for(int i = 0 ; i < msg.dataBuffer.size(); i ++)
    {
        Serial.print(String((uint8_t)msg.dataBuffer.at(i)));
        Serial.print(" ");
    }

    Serial.flush();
    Serial.println();
}

void MessageUDP::resetByteIterationCount()
{
    byteIterationIndex = -1;
}

uint8_t MessageUDP::getCurrentByte()
{
     // Serial.println("Total size: " + String(totalSize));
    // Serial.println("byteIterationIndex = " + String(byteIterationIndex));
    if(byteIterationIndex > 13)
    {
        //return 0;
        if(byteIterationIndex != totalSize - 1)
        {
            // Serial.println("Returning payload byte at index: " + String(byteIterationIndex-2));
            return dataBuffer.at(byteIterationIndex-14); /* Return payload byte */
        }else
        {
            // Serial.println("Returning checkusm byte");
            return lastByteCheckValue;
        }
        
    }else {
        /* Return message ID*/
        if(byteIterationIndex >= 0 && byteIterationIndex <= 3)
        {
            // Serial.println("Returning message ID");
            return *(((uint8_t*)&id) + byteIterationIndex);
            
        } else if (byteIterationIndex == 4) /* Return message size */
        {
            // Serial.println("Returning total size");
            return (uint8_t)totalSize;
        } else if (byteIterationIndex >= 6 && byteIterationIndex <= 9)
        {
            // Return IP Address bytes
            return *(((uint8_t*)&ipAddress) + (byteIterationIndex - 6));

        } else if (byteIterationIndex >= 10 && byteIterationIndex <= 13)
        {
            // Return port bytes
            return *(((uint8_t*)&udpPort) + (byteIterationIndex - 10));
        }
    }

    return -7;
}

bool MessageUDP::switchToNextByte()
{
    // Increment index
    byteIterationIndex ++;

    // If next byte exists
    if(byteIterationIndex < (totalSize))
    {
        return true;
    }

    byteIterationIndex = -1;
    // No more bytes
    return false;
}

std::vector<uint8_t>& MessageUDP::getPayload()
{
    return dataBuffer;
}

int MessageUDP::getPort()
{
    return udpPort;
}

MessageUDP::IPAddr& MessageUDP::getIPAddress()
{
    return ipAddress;
}