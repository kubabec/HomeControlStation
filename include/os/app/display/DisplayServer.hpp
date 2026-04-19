
#pragma once
#include "os/datacontainer/DataContainer.hpp"

#include "os/app/display/Display.hpp"

class DisplayServer
{

#define DISPLAY_DISCOVERY_FREQ_MS 15000 // 10s
#define DISPLAY_HASH_REFRESH_FREQ_MS 5000


public:
    static void init();
    static void deinit();
    static void cyclic();

    static void receiveUDP(MessageUDP &msg);

private:
    static std::vector<Display> knownDisplays;
    static std::queue<MessageUDP> receptionUdpQueue;

    static unsigned long lastDiscoveryTime;


    static void processReceptionBuffer();
    static void handleNewDisplay(MessageUDP &msg);

    static void discoverDisplays();
};