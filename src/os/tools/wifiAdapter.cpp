#include <os/tools/WifiAdapter.hpp>
#include <os/datacontainer/DataContainer.hpp>


bool WiFiAdapter::isConnectedFlag = false;
bool WiFiAdapter::wasConnectionRequestedFlag = false;
bool WiFiAdapter::connectionLostNotificationSent = false;
String WiFiAdapter::lastConnectedSSID = "";
String WiFiAdapter::lastConnectedPassword = "";

void WiFiAdapter::waitForConnection500ms()
{
    // Wait max 5x 200ms if connection can be established
    for(uint8_t i = 0; i < 5; i++)
    {
        if(WiFi.status() == WL_CONNECTED)
        {
            break;
        }
        delay(1000);
    }
}

void WiFiAdapter::connectToNetwork(const String ssid, const String password)
{
    static uint8_t retryCount = 0;
    Serial.println("Network " + ssid+ " with password " + password + " connection try " + String(retryCount));
    retryCount ++;

    wasConnectionRequestedFlag = true;

    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    waitForConnection500ms();

    // Connection is established
    if(WiFi.status() == WL_CONNECTED)
    {
        isConnectedFlag = true;
        lastConnectedSSID = ssid;
        lastConnectedPassword = password;
        connectionLostNotificationSent = false;
    }
    else
    {
        //Retry connection 5 times
        for(uint8_t retryCount = 0; retryCount < 5; retryCount++)
        {
            WiFi.disconnect(true);
            WiFi.mode(WIFI_STA);
            WiFi.begin(ssid, password);

            waitForConnection500ms();

            if(WiFi.status() == WL_CONNECTED)
            {
                isConnectedFlag = true;
                lastConnectedSSID = ssid;
                lastConnectedPassword = password;
                connectionLostNotificationSent = false;
                break;
            }
        }
    }
    Serial.println("======== IP Adres =========");
    Serial.println(WiFi.localIP());
    DataContainer::setSignalValue(
        SIG_IP_ADDRESS_STRING,
        static_cast<String>(WiFi.localIP().toString())
    );

    uint32_t ip;
    memcpy((uint8_t*)&ip, &WiFi.localIP()[0], 1);
    memcpy((uint8_t*)&ip+1, &WiFi.localIP()[1], 1);
    memcpy((uint8_t*)&ip+2, &WiFi.localIP()[2], 1);
    memcpy((uint8_t*)&ip+3, &WiFi.localIP()[3], 1);

    DataContainer::setSignalValue(SIG_IP_ADDRESS, static_cast<uint32_t>(ip));

}

void WiFiAdapter::disconnect()
{
    wasConnectionRequestedFlag = false;

    WiFi.disconnect(true);
    if(WiFi.status() != WL_CONNECTED)
    {
        isConnectedFlag = false;
        lastConnectedSSID = "";
        lastConnectedPassword = "";
    }
}

bool WiFiAdapter::isConnected()
{
    return isConnectedFlag;
}

bool WiFiAdapter::wasConnectionRequested()
{
    return wasConnectionRequestedFlag;
}


void WiFiAdapter::task()
{
    // Serial.print((int)WiFi.status());
    // Serial.print(" - ");
    // Monitor if network connection lost if the connection was requested before
    if(wasConnectionRequested())
    {
        if(WiFi.status() != WL_CONNECTED)
        {
            isConnectedFlag = false;
        }
    }
}

String WiFiAdapter::getLastConnectedSSID()
{
    return lastConnectedSSID;
}

String WiFiAdapter::getLastConnectedPassword()
{
    return lastConnectedPassword;
}

void WiFiAdapter::createAccessPoint()
{
    String ssid = "ESP32_HomeStation";;
    String macAddres = WiFi.macAddress();
    String lastMacFourDigits = macAddres.substring(macAddres.length()-5);
    String ssidWithMac = ssid + "_" + lastMacFourDigits;

    // WiFiAdapter::connectToNetwork( "Xiaomi 12 Lite", "Goldie10");
    // return;



    // const char* accessPointSSID = "ESP32_HomeStation";
    // Serial.println("Access Point initialization ...");
    //WiFi.mode(WIFI_AP);
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(IPAddress(192, 168, 0, 1), IPAddress(192, 168, 0, 1), IPAddress(255,255,255,0));
    WiFi.softAP(ssidWithMac, "HomeStation");
    Serial.println("AP created successfully");


    Serial.println("======== IP Adres =========");
    Serial.println(WiFi.softAPIP());
    DataContainer::setSignalValue(
        SIG_IP_ADDRESS_STRING,
        static_cast<String>(WiFi.softAPIP().toString())
    );

    uint32_t ip;
    memcpy((uint8_t*)&ip, &WiFi.softAPIP()[0], 1);
    memcpy((uint8_t*)&ip+1, &WiFi.softAPIP()[1], 1);
    memcpy((uint8_t*)&ip+2, &WiFi.softAPIP()[2], 1);
    memcpy((uint8_t*)&ip+3, &WiFi.softAPIP()[3], 1);

    DataContainer::setSignalValue(SIG_IP_ADDRESS, static_cast<uint32_t>(ip));
    
    // isConnectedFlag = true;
    // lastConnectedSSID = String("ESP32_HomeStation");
    // lastConnectedPassword = "";
    // connectionLostNotificationSent = false;

    //Serial.println("WiFi AccessPoint created, IP: " + WiFi.softAPIP());
    
    delay(1000);
}
