#ifndef WIFIADAPTER_H
#define WIFIADAPTER_H
#include <Arduino.h>
#include <WiFi.h>

class WiFiAdapter{
    static bool isConnectedFlag;
    static bool wasConnectionRequestedFlag;
    static String lastConnectedSSID;
    static String lastConnectedPassword;
    static bool connectionLostNotificationSent;

    static void waitForConnection500ms();

public:
    static void connectToNetwork(const String ssid, const String password);
    static void disconnect();
    static bool isConnected();
    static bool wasConnectionRequested();
    static String getLastConnectedSSID();
    static String getLastConnectedPassword();


    static void task();
};


#endif