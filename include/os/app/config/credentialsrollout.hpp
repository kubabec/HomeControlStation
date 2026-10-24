#ifndef CREDENTIALS_ROLLOUT_H
#define CREDENTIALS_ROLLOUT_H
#include <Arduino.h>

/**
 * @file include/os/app/config/credentialsrollout.hpp
 * @brief Pushes new WiFi credentials and panel password from the Master to other nodes over their web UI.
 *
 * Nodes are driven exactly like a user would do it in a browser (unlock via /passwordApply, then
 * /newCfgApply&{json}), so nodes running HCS 1.x firmware are supported without any change.
 */
class CredentialsRollout
{
public:
    enum class State : uint8_t
    {
        Idle,
        Running,
        Finished
    };

    /**
     * Validates credentials against limits supported by every firmware generation.
     * @param error Human readable reason when validation fails.
     */
    static bool validate(const String &ssid, const String &wifiPassword, const String &panelPassword, String &error);

    /**
     * Number of slave nodes currently known by the Master.
     */
    static size_t knownSlaveCount();

    /**
     * Starts asynchronous update of all known slave nodes.
     * @return false when rollout cannot be started (error is filled).
     */
    static bool start(const String &ssid, const String &wifiPassword, const String &panelPassword, String &error);

    /** Requests the rollout to stop before processing next node. */
    static void abort();

    /** Drops results of a finished rollout and clears stored credentials. */
    static void reset();

    /**
     * Applies the credentials to the local node (Master). Allowed only after a finished rollout
     * or when there are no slaves. Does not restart the device.
     */
    static bool commitLocal();

    /** Applies given credentials to the local node directly (no slaves in the network). */
    static bool applyLocalDirectly(const String &ssid, const String &wifiPassword, const String &panelPassword, String &error);

    /** JSON status: {"state":"running|finished|idle","nodes":[{"ip","status","reason"}]} */
    static String statusJson();

    static State getState();
};

#endif
