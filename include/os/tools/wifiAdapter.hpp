#ifndef WIFIADAPTER_H
#define WIFIADAPTER_H
#include <Arduino.h>
#include <WiFi.h>

class WiFiAdapter{
    static bool isConnectedFlag;
    static String mSsid;
    static String mPassword;
    static bool connectionInitialized;
    static void waitForConnection500ms();


    static void WiFiEvent(WiFiEvent_t event);
    static void handleReconnection();
    static void manualStatusCheck();
    static void updateDataContainerIpAddress();
public:
    static void connectToNetwork(const String ssid, const String password, bool activeWait);
    static void disconnect();
    static bool isConnected();

    static void createAccessPoint();
    static String getIpString();

    static void enableMDNSResponder();

    static void init();
    static void task();
};


#endif