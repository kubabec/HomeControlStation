#ifndef WIFIADAPTER_H
#define WIFIADAPTER_H
#include <Arduino.h>
#include <WiFi.h>

class WiFiAdapter{
    static bool isConnectedFlag;
    static void waitForConnection500ms();

public:
    static void connectToNetwork(const String ssid, const String password, bool activeWait);
    static void disconnect();
    static bool isConnected();

    static void createAccessPoint();
    static String getIpString();

    static void enableMDNSResponder();

    static void task();
};


#endif