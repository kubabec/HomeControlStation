#include <os/tools/WifiAdapter.hpp>
#include <os/datacontainer/DataContainer.hpp>
#include <ESPmDNS.h>
#include "os/Logger.hpp"

bool WiFiAdapter::isConnectedFlag = false;
bool WiFiAdapter::connectionInitialized = false;
String WiFiAdapter::mSsid{"none"};
String WiFiAdapter::mPassword{"none"};
NetworkStatus WiFiAdapter::currentStatus = DISCONNECTED_FROM_NETWORK;

const unsigned long reconnectDelayMs = 1000; // starting retry delay (1s)
unsigned long nextReconnectAt = 3000;        // kiedy spróbować ponownie (millis)
const unsigned long statusCheckInterval = 2000;
unsigned long lastStatusCheck = 0;

void WiFiAdapter::waitForConnection500ms()
{
    // Wait max 5x 200ms if connection can be established
    for (uint8_t i = 0; i < 5; i++)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            break;
        }
        delay(500);
    }
}

void WiFiAdapter::updateDataContainerIpAddress()
{
    uint32_t ip{0};
    String ipStr{"192.168.0.1"};
    memcpy((uint8_t *)&ip, &WiFi.localIP()[0], 1);
    memcpy((uint8_t *)&ip + 1, &WiFi.localIP()[1], 1);
    memcpy((uint8_t *)&ip + 2, &WiFi.localIP()[2], 1);
    memcpy((uint8_t *)&ip + 3, &WiFi.localIP()[3], 1);

    if (ip != 0)
    {
        ipStr = WiFi.localIP().toString();
    }

    Logger::log("Updating IP address in DataContainer: " + ipStr);
    DataContainer::setSignalValue(
        SIG_IP_ADDRESS_STRING,
        static_cast<String>(ipStr));

    DataContainer::setSignalValue(SIG_IP_ADDRESS, static_cast<uint32_t>(ip));
}

void WiFiAdapter::WiFiEvent(WiFiEvent_t event)
{
    switch (event)
    {
    case SYSTEM_EVENT_STA_GOT_IP:
    {
        currentStatus = CONNECTED_TO_NETWORK;
        Logger::log("WiFiAdapter://Connected.");
        updateDataContainerIpAddress();

        // Enable mDNS responder if the current device is an RC server
        NodeConfiguration currentConfig =
            std::any_cast<NodeConfiguration>(DataContainer::getSignalValue(SIG_DEVICE_CONFIGURATION));
        if (currentConfig.isRcServer)
        {
            enableMDNSResponder();
        }
        isConnectedFlag = true;
        // resetujemy backoff po pomyślnym połączeniu
        nextReconnectAt = 0;
        break;
    }

    case SYSTEM_EVENT_STA_DISCONNECTED:
    {
        currentStatus = DISCONNECTED_FROM_NETWORK;
        Logger::log("WiFi: DISCONNECTED");
        isConnectedFlag = false;
        nextReconnectAt = millis() + reconnectDelayMs;
        break;
    }

    default:
        break;
    }
}

void WiFiAdapter::init()
{
    isConnectedFlag = false;
    updateDataContainerIpAddress();

    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);

    // WiFi.onEvent(WiFiEvent);
}

void WiFiAdapter::connectToNetwork(const String ssid, const String password, bool activeWait)
{
    mSsid = ssid;
    mPassword = password;
    connectionInitialized = true;
    // Logger::log("Network " + ssid+ " with password " + password + " connection try " + String(retryCount));
    Logger::log("WiFiAdapter://Attempting WiFi connection to network " + ssid);
    if (WiFi.status() != WL_CONNECTED)
    {
        WiFi.begin(ssid, password);
        if (activeWait)
        {
            waitForConnection500ms();
        }
    }

    // Enable mDNS responder if the current device is an RC server
    NodeConfiguration currentConfig =
        std::any_cast<NodeConfiguration>(DataContainer::getSignalValue(SIG_DEVICE_CONFIGURATION));
    if (currentConfig.isRcServer)
    {
        enableMDNSResponder();
    }

    manualStatusCheck();
}

void WiFiAdapter::disconnect()
{
    WiFi.disconnect(true);
    if (WiFi.status() != WL_CONNECTED)
    {
        isConnectedFlag = false;
    }
}

bool WiFiAdapter::isConnected()
{
    return isConnectedFlag;
}

void WiFiAdapter::manualStatusCheck()
{
    unsigned long now = millis();
    lastStatusCheck = now;

    bool isNowConnected = (WiFi.isConnected() && WiFi.status() == WL_CONNECTED);

    if (isNowConnected && !isConnectedFlag)
    {
        // event nie został złapany (może wersja core), więc synchronizujemy
        Logger::log("WiFiAdapter://Connected (Manual check).");
        updateDataContainerIpAddress();

        isConnectedFlag = true;
        currentStatus = CONNECTED_TO_NETWORK;
        nextReconnectAt = 0;
    }
    else if (!isNowConnected && isConnectedFlag)
    {
        // utrata połączenia wykryta przez polling
        Logger::log("WiFi: Connection lost");
        isConnectedFlag = false;
        currentStatus = DISCONNECTED_FROM_NETWORK;
        nextReconnectAt = now + reconnectDelayMs;
    }
}

void WiFiAdapter::handleReconnection()
{
    // If not connected, and connection process was initialized, try to reconnect if the backoff time passed
    if (!isConnectedFlag)
    {
        if (millis() >= nextReconnectAt)
        {
            Logger::log("WiFiAdapter://Reconnecting to WiFi network...");
            // WiFi.reconnect();
            WiFi.disconnect();
            WiFi.begin(mSsid.c_str(), mPassword.c_str());
            nextReconnectAt = millis() + reconnectDelayMs;
        }
    }
}

void WiFiAdapter::task()
{
    if (connectionInitialized)
    {
        unsigned long now = millis();
        if (now - lastStatusCheck >= statusCheckInterval)
        {
            manualStatusCheck();
        }
        handleReconnection();
    }
}

NetworkStatus WiFiAdapter::getNetworkStatus()
{
    return currentStatus;
}

String WiFiAdapter::getIpString()
{
    return WiFi.localIP().toString();
}

void WiFiAdapter::createAccessPoint()
{
    currentStatus = ACCESS_POINT_MODE;
    String ssid = "ESP32_HomeStation";
    String macAddres = WiFi.macAddress();
    String lastMacFourDigits = macAddres.substring(macAddres.length() - 5);
    String ssidWithMac = ssid + "_" + lastMacFourDigits;

    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(IPAddress(192, 168, 0, 1), IPAddress(192, 168, 0, 1), IPAddress(255, 255, 255, 0));
    WiFi.softAP(ssidWithMac, "HomeStation");
    Logger::log("AP created successfully");

    Logger::log("======== IP Adres =========");
    Logger::log(WiFi.softAPIP().toString());
    DataContainer::setSignalValue(
        SIG_IP_ADDRESS_STRING,
        static_cast<String>(WiFi.softAPIP().toString()));

    uint32_t ip;
    memcpy((uint8_t *)&ip, &WiFi.softAPIP()[0], 1);
    memcpy((uint8_t *)&ip + 1, &WiFi.softAPIP()[1], 1);
    memcpy((uint8_t *)&ip + 2, &WiFi.softAPIP()[2], 1);
    memcpy((uint8_t *)&ip + 3, &WiFi.softAPIP()[3], 1);

    DataContainer::setSignalValue(SIG_IP_ADDRESS, static_cast<uint32_t>(ip));

    enableMDNSResponder();

    delay(500);
}

void WiFiAdapter::enableMDNSResponder()
{
    String hostName = "home";
    if (!MDNS.begin(hostName.c_str()))
    {
        Logger::log("Error setting up MDNS responder!");
    }
    else
    {
        DataContainer::setSignalValue(
            SIG_IP_ADDRESS_STRING,
            static_cast<String>(hostName + String(".local")));
        Logger::log("mDNS responder started with hostname: " + hostName + ".local");
    }
}
