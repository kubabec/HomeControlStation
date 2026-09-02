#ifndef TIMEMASTER_HPP
#define TIMEMASTER_HPP

#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ctime>  // Do konwersji czasu Unix na datę
#include <os/datacontainer/datacontainertypes.hpp>
#include <os/datacontainer/DataContainer.hpp>
/**
 * @class TimeMaster
 * @brief Synchronizes the system clock with NTP and exposes time in both epoch and RTC formats.
 *
 * The component keeps track of startup state, successful NTP updates, retry counts, and the last
 * known valid time so the OS can stamp events, schedule tasks, and compute RTC values reliably.
 */

class TimeMaster {
public:
    /**
     * Initializes the NTP client and prepares the time-sync subsystem.
     */
    static void init();

    /**
     * Releases the NTP client and clears the runtime state used by the time service.
     */
    static void deinit();

    /**
     * Runs the periodic time update cycle, refreshing NTP values when the configured interval expires.
     */
    static void cyclic();

    /**
     * Returns the current Unix epoch timestamp.
     * @return Current time in seconds since 1970-01-01 UTC.
     */
    static unsigned long getEpochTime();

    /**
     * Configures the system timezone offset used by the time conversion logic.
     * @param timeZoneOffset Time offset in hours relative to UTC.
     */
    static void setTimeZone(int timeZoneOffset);

    /**
     * Converts the current epoch time into the application-specific RTC structure.
     * @return Current time expressed as RtcTime.
     */
    static RtcTime getRtcTime();

private:
    /**
     * NTP client instance used to query the remote time server.
     */
    static NTPClient timeClient;

    /**
     * true once the startup time initialization path has completed.
     */
    static bool startupTimeInitialized;

    /**
     * UDP socket used for outbound NTP communication.
     */
    static WiFiUDP ntpUDP;

    /**
     * Number of failed NTP synchronization attempts already recorded.
     */
    static uint8_t initialRetryCount;

    /**
     * true when the system has ever received a valid NTP sync.
     */
    static bool wasNtpEverSynced;

    /**
     * Timestamp of the most recent NTP refresh or status update.
     */
    static unsigned long lastUpdateTime;

    /**
     * Last valid epoch time received from the NTP server.
     */
    static unsigned long lastNTPTime;

    /**
     * Milliseconds since the last local timer reset, used for deriving elapsed periods.
     */
    static unsigned long lastMillis;

    /**
     * true when the NTP connection is available and the time is considered usable.
     */
    static bool ntpAvailable;

    /**
     * Interval between NTP refresh attempts.
     */
    static unsigned long updateInterval;

    /**
     * Refreshes internal state variables based on the current NTP status and elapsed time.
     */
    static void updateNtpVariables();

    /**
     * Counter tracking time sync failures so the system can back off or retry deliberately.
     */
    static uint8_t timeSyncFailureCount;
};

#endif // MASTERTIMER_HPP