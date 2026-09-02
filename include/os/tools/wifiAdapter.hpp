#ifndef WIFIADAPTER_H
#define WIFIADAPTER_H
#include <Arduino.h>
#include <WiFi.h>

/**
 * @enum NetworkStatus
 * @brief High-level runtime state of the device Wi-Fi interface.
 */
enum NetworkStatus{
    /**
     * The adapter is exposing its own access point instead of joining a network.
     */
    ACCESS_POINT_MODE = 0,
    /**
     * The device is connected to a Wi-Fi network and can exchange traffic.
     */
    CONNECTED_TO_NETWORK = 1,
    /**
     * The device is not connected to any network and is waiting for a new connection.
     */
    DISCONNECTED_FROM_NETWORK = 2
};
/**
 * @class WiFiAdapter
 * @brief Wraps ESP32 Wi-Fi lifecycle management for the Home Control Station.
 *
 * The adapter handles network connection, reconnection, access-point creation, and state reporting
 * for the rest of the OS. It also updates the shared data container with the current network IP.
 */

class WiFiAdapter{
    /**
     * true when the adapter believes the Wi-Fi interface is currently connected.
     */
    static bool isConnectedFlag;

    /**
     * SSID currently configured for the station connection attempt.
     */
    static String mSsid;

    /**
     * Password currently associated with the station connection attempt.
     */
    static String mPassword;

    /**
     * true once the Wi-Fi service has completed its initial setup and event handlers.
     */
    static bool connectionInitialized;

    /**
     * Current abstraction-level network state tracked by the adapter.
     */
    static NetworkStatus currentStatus;

    /**
     * Pauses execution briefly to let the ESP32 finish establishing a Wi-Fi connection.
     */
    static void waitForConnection500ms();

    /**
     * Handles ESP32 Wi-Fi event notifications and updates the adapter state accordingly.
     * @param event Event type emitted by the ESP-IDF Wi-Fi stack.
     */
    static void WiFiEvent(WiFiEvent_t event);

    /**
     * Performs reconnect logic after a lost connection or authentication failure.
     */
    static void handleReconnection();

    /**
     * Polls the current connection state and updates the adapter if the ESP32 reports drift.
     */
    static void manualStatusCheck();

    /**
     * Publishes the current IP address into the shared data container for other OS components.
     */
    static void updateDataContainerIpAddress();

    
public:
    /**
     * Connects the adapter to the target SSID using the supplied password.
     * @param ssid Network name to join.
     * @param password Network key used for authentication.
     * @param activeWait true when the caller wants blocking wait until the connection is available.
     */
    static void connectToNetwork(const String ssid, const String password, bool activeWait);

    /**
     * Disconnects the adapter from the current station or access-point mode.
     */
    static void disconnect();

    /**
     * Reports whether the ESP32 Wi-Fi interface is connected to a network at the moment.
     * @return true when the interface is connected; false otherwise.
     */
    static bool isConnected();

    /**
     * Reconnects using a new SSID and password pair.
     * @param ssid Replacement network name.
     * @param pwd Replacement password.
     */
    static void reconnect(String& ssid, String& pwd);

    /**
     * Starts the ESP32 access point mode so other devices can connect to the station.
     */
    static void createAccessPoint();

    /**
     * Returns the current IPv4 string of the ESP32 interface.
     * @return IP address formatted as a dotted string, or an empty string when unavailable.
     */
    static String getIpString();

    /**
     * Enables the multicast DNS responder so the node can be discovered by name on the LAN.
     */
    static void enableMDNSResponder();

    /**
     * Gets the high-level status reported by the adapter.
     * @return Current adapter state as a NetworkStatus enum.
     */
    static NetworkStatus getNetworkStatus();

    /**
     * Initializes the Wi-Fi adapter and sets up the event-handling and startup logic.
     */
    static void init();

    /**
     * Runs the task loop for the Wi-Fi adapter. Intended to be used by the scheduler.
     */
    static void task();
};


#endif