
#pragma once
#include "os/datacontainer/DataContainer.hpp"

#include "os/app/display/Display.hpp"
/**
 * @class DisplayServer
 * @brief Tracks connected display nodes and keeps the display topology synchronized with the network.
 *
 * The server listens for UDP discovery messages, remembers which displays are active, and periodically
 * refreshes their state so the UI layer can push device descriptions and control frames to the proper targets.
 */

class DisplayServer
{

#define DISPLAY_DISCOVERY_FREQ_MS 15000 // 10s
#define DISPLAY_HASH_REFRESH_FREQ_MS 5000

public:
    /**
     * Initializes the display discovery system and registers the input handlers.
     */
    static void init();

    /**
     * Releases the display server state and clears the known display table.
     */
    static void deinit();

    /**
     * Runs the periodic refresh loop for display discovery and status checks.
     */
    static void cyclic();

    /**
     * Receives and queues a UDP packet for display processing.
     * @param msg Packet produced by a display or control application.
     */
    static void receiveUDP(MessageUDP &msg);
    static const std::vector<Display>& getKnownDisplays();

private:
    /**
     * Displays currently known to the station and still considered reachable.
     */
    static std::vector<Display> knownDisplays;

    /**
     * UDP packets waiting to be processed by the display server.
     */
    static std::queue<MessageUDP> receptionUdpQueue;

    struct PendingAction
    {
        bool active = false;
        MessageUDP request;
        DisplayActionRequest action;
        ServiceParameters_set1 parameters;
        DeviceServicesType service = DEVSERVICE_STATE_SWITCH;
    };

    static PendingAction pendingAction;

    /**
     * Timestamp of the last display-discovery sweep.
     */
    static unsigned long lastDiscoveryTime;

    /**
     * Processes all queued UDP packets and distributes them to the correct display handlers.
     */
    static void processReceptionBuffer();

    /**
     * Registers a newly discovered display and stores its metadata for later updates.
     * @param msg Packet containing the new display announcement.
     */
    static void handleNewDisplay(MessageUDP &msg);
    static void handleMessage(MessageUDP &msg);
    static void processPendingAction();
    static void sendDeviceList(const Display& display);
    static void sendRoomNames(const Display& display);
    static void sendActionResponse(const MessageUDP& request, uint16_t sequence, uint8_t status, uint8_t deviceId);
    static void broadcastDeviceState(uint8_t deviceId);

    /**
     * Sends discovery requests to discover currently reachable displays on the LAN.
     */
    static void discoverDisplays();
};