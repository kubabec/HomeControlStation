#include "os/Logger.hpp"
#include <WiFi.h>
#include <WiFiUdp.h>

WiFiUDP udp;

std::queue<String> Logger::offlineLogQueue;
long Logger::logCounter{0};

void Logger::processMessage(String &message)
{
    message = "[IP:" + WiFi.localIP().toString() + "] ["+logCounter+"]" + message;
    udp.beginPacket(WiFi.broadcastIP(), 9604); // adres IP i port odiornika
    udp.write((uint8_t *)message.c_str(), message.length());
    udp.endPacket();
    logCounter++;
}

void Logger::log(String message)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        // Process all queued messages first
        while(!offlineLogQueue.empty())
        {
            String queuedMessage = offlineLogQueue.front();
            offlineLogQueue.pop();
            processMessage(queuedMessage);
            delay(10);
        }

        // Process current message
        processMessage(message);
    }
    else
    {
        // Queue the message for later processing
        offlineLogQueue.push(message);
    }

    Serial.println(message);
}