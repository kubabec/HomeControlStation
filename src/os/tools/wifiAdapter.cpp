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
        "WiFiAdapter",
        static_cast<String>(WiFi.localIP().toString())
    );

    uint32_t ip;
    memcpy((uint8_t*)&ip, &WiFi.localIP()[0], 1);
    memcpy((uint8_t*)&ip+1, &WiFi.localIP()[1], 1);
    memcpy((uint8_t*)&ip+2, &WiFi.localIP()[2], 1);
    memcpy((uint8_t*)&ip+3, &WiFi.localIP()[3], 1);

    DataContainer::setSignalValue(SIG_IP_ADDRESS, "WiFiAdapter", static_cast<uint32_t>(ip));

    Serial.println("======== IP Adres =========");
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
