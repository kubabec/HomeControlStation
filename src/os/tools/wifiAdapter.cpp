#include <os/tools/WifiAdapter.hpp>
#include <os/datacontainer/DataContainer.hpp>
#include <ESPmDNS.h>


bool WiFiAdapter::isConnectedFlag = false;

void WiFiAdapter::waitForConnection500ms()
{
    // Wait max 5x 200ms if connection can be established
    for(uint8_t i = 0; i < 5; i++)
    {
        if(WiFi.status() == WL_CONNECTED)
        {
            break;
        }
        delay(500);
    }
}

void WiFiAdapter::connectToNetwork(const String ssid, const String password, bool activeWait)
{
    // Serial.println("Network " + ssid+ " with password " + password + " connection try " + String(retryCount));
    Serial.println("WiFiAdapter://Attempting WiFi connection to network " + ssid);
    if(WiFi.status() != WL_CONNECTED){
        WiFi.disconnect(true);
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);

        if(activeWait){
            waitForConnection500ms();
        }
    }

    // Connection is established
    if(WiFi.status() == WL_CONNECTED)
    {
        isConnectedFlag = true;
        Serial.println("WiFiAdapter://Connected.");
        Serial.println("======== IP Adres =========");
        Serial.println(WiFi.localIP());
        DataContainer::setSignalValue(
            SIG_IP_ADDRESS_STRING,
            static_cast<String>(WiFi.localIP().toString()
        ));

        uint32_t ip;
        memcpy((uint8_t*)&ip, &WiFi.localIP()[0], 1);
        memcpy((uint8_t*)&ip+1, &WiFi.localIP()[1], 1);
        memcpy((uint8_t*)&ip+2, &WiFi.localIP()[2], 1);
        memcpy((uint8_t*)&ip+3, &WiFi.localIP()[3], 1);

        DataContainer::setSignalValue(SIG_IP_ADDRESS, static_cast<uint32_t>(ip));

        // Enable mDNS responder if the current device is an RC server
        NodeConfiguration currentConfig = 
        std::any_cast<NodeConfiguration>(DataContainer::getSignalValue(SIG_DEVICE_CONFIGURATION));
        if(currentConfig.isRcServer)
        {
            enableMDNSResponder();
        }
        
    }
    else
    {
        DataContainer::setSignalValue(
            SIG_IP_ADDRESS_STRING,
            static_cast<String>("null")
        );
    
        DataContainer::setSignalValue(SIG_IP_ADDRESS, static_cast<uint32_t>(0));
    }
}

void WiFiAdapter::disconnect()
{
    WiFi.disconnect(true);
    if(WiFi.status() != WL_CONNECTED)
    {
        isConnectedFlag = false;
    }
}

bool WiFiAdapter::isConnected()
{
    return isConnectedFlag;
}

void WiFiAdapter::task()
{
    if(WIFI_STA == WiFi.getMode()){
        if(WiFi.status() != WL_CONNECTED)
        {
            isConnectedFlag = false;
        }else {
            if(isConnectedFlag == false){ /*transition from disconnected to connected */
                Serial.println("======== IP Adres =========");
                Serial.println(WiFi.localIP());
                DataContainer::setSignalValue(
                    SIG_IP_ADDRESS_STRING,
                    static_cast<String>(WiFi.localIP().toString()
                ));

                uint32_t ip;
                memcpy((uint8_t*)&ip, &WiFi.localIP()[0], 1);
                memcpy((uint8_t*)&ip+1, &WiFi.localIP()[1], 1);
                memcpy((uint8_t*)&ip+2, &WiFi.localIP()[2], 1);
                memcpy((uint8_t*)&ip+3, &WiFi.localIP()[3], 1);

                DataContainer::setSignalValue(SIG_IP_ADDRESS, static_cast<uint32_t>(ip));

                // TODO: Should we enable MDNS responder here as well??
                // Enable mDNS responder if the current device is an RC server
                NodeConfiguration currentConfig = 
                std::any_cast<NodeConfiguration>(DataContainer::getSignalValue(SIG_DEVICE_CONFIGURATION));
                if(currentConfig.isRcServer)
                {
                    enableMDNSResponder();
                }
            }

            isConnectedFlag = true;
        }
    }else {
        isConnectedFlag = true;
    }
}

String WiFiAdapter::getIpString()
{
    return WiFi.localIP().toString();
}

void WiFiAdapter::createAccessPoint()
{
    String ssid = "ESP32_HomeStation";;
    String macAddres = WiFi.macAddress();
    String lastMacFourDigits = macAddres.substring(macAddres.length()-5);
    String ssidWithMac = ssid + "_" + lastMacFourDigits;

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

    enableMDNSResponder();
    
    delay(1000);
}

void WiFiAdapter::enableMDNSResponder(){
    String hostName = "home";
    if (!MDNS.begin(hostName.c_str())) {
        Serial.println("Error setting up MDNS responder!");
    } else {
        DataContainer::setSignalValue(
            SIG_IP_ADDRESS_STRING,
            static_cast<String>(hostName + String(".local"))
        );
        Serial.println("mDNS responder started with hostname: " + hostName + ".local");
    }
}
