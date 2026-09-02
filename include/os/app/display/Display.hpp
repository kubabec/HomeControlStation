
#pragma once
#include "os/tools/messageUDP.hpp"
#include "os/app/display/DisplayDatatypes.hpp"
/**
 * @class Display
 * @brief Represents a display endpoint discovered by the station and tracked for periodic refresh.
 */

class Display
{
public:
    /**
     * Creates a display descriptor from the discovered display metadata.
     * @param info Network and display information describing the endpoint.
     */
    Display(DisplayInfo info);

    /**
     * Compares two display entries by their network identity.
     * @param other Other display to compare with.
     * @return true when both displays refer to the same display endpoint.
     */
    bool operator==(const Display &other) const;

    /**
     * Returns the timestamp of the most recent data refresh received from this display.
     * @return Last data update time in milliseconds.
     */
    unsigned long getLastDataUpdateTime();

    /**
     * Returns the hash of the latest display data snapshot.
     * @return Hash used to detect changes in the display payload.
     */
    uint16_t getDisplayDataHash();

private:
    /**
     * Time of the most recent payload update received from the display.
     */
    unsigned long lastDataUpdateTime = 0;

    /**
     * Hash of the last known display-data payload used to detect changes.
     */
    uint16_t displayDataHash = 0;

    /**
     * Metadata describing the display address, size, and network location.
     */
    DisplayInfo info;
};