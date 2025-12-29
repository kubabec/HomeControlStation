#include "os/Logger.hpp"
#include <WiFi.h>
#include <WiFiUdp.h>

WiFiUDP udp;

std::queue<String> Logger::offlineLogQueue;
long Logger::logCounter{0};

void Logger::processMessage(String &message)
{
    static uint32_t lastSend = 0;
    const uint32_t MIN_SEND_INTERVAL_MS = 3; // 3ms = ~333 logs/sec max per device

    // Throttle if sending too fast
    if (millis() - lastSend < MIN_SEND_INTERVAL_MS) {
        delay(MIN_SEND_INTERVAL_MS - (millis() - lastSend));
    }
    lastSend = millis();

    message.reserve(message.length() + 30);
    message = "[IP:" + WiFi.localIP().toString() + "] ["+logCounter+"]" + message;

    uint8_t transmissionStatus = 0;

    while(!transmissionStatus){
        transmissionStatus = udp.beginPacket(WiFi.broadcastIP(), 9604);
    }
     // adres IP i port odiornika
    udp.write((uint8_t *)message.c_str(), message.length());
    transmissionStatus = udp.endPacket();
    while(transmissionStatus != 1) {
        udp.beginPacket(WiFi.broadcastIP(), 9604); // adres IP i port odiornika
        udp.write((uint8_t *)message.c_str(), message.length());
        transmissionStatus = udp.endPacket();
        delay(5); // Small delay to avoid tight loop
    }
    logCounter++;
}

void Logger::log(String message)
{
    // if (WiFi.status() == WL_CONNECTED)
    // {
    //     // Process all queued messages first
    //     while(!offlineLogQueue.empty())
    //     {
    //         String queuedMessage = offlineLogQueue.front();
    //         offlineLogQueue.pop();
    //         processMessage(queuedMessage);
    //         delay(15);
    //     }

    //     // Process current message
    //     processMessage(message);
    // }
    // else
    // {
    //     // Queue the message for later processing
    //     offlineLogQueue.push(message);
    // }

    // Serial.println(message);
}