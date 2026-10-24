#include <os/app/config/credentialsrollout.hpp>
#include <os/app/config/ConfigProvider.hpp>
#include <os/datacontainer/DataContainer.hpp>
#include <os/datacontainer/datacontainertypes.hpp>
#include <os/Logger.hpp>
#include <WiFi.h>
#include <vector>

/**
 * @file src/os/app/config/credentialsrollout.cpp
 * @brief Master-driven propagation of WiFi credentials and panel password to other nodes.
 */

namespace
{
    constexpr size_t MAX_FIELD_LENGTH = 29; /* NVM fields are 30 bytes including terminator */
    constexpr size_t MAX_CFG_RESPONSE = 16 * 1024;

    enum NodeStatus : uint8_t
    {
        NODE_PENDING,
        NODE_IN_PROGRESS,
        NODE_OK,
        NODE_NOK
    };

    struct NodeResult
    {
        String ip;
        NodeStatus status = NODE_PENDING;
        String reason;
    };

    std::vector<NodeResult> results;
    CredentialsRollout::State state = CredentialsRollout::State::Idle;
    volatile bool abortRequested = false;
    SemaphoreHandle_t dataMutex = nullptr;

    String newSsid;
    String newWifiPassword;
    String newPanelPassword;
    String currentPanelPassword;

    struct Lock
    {
        Lock() { xSemaphoreTake(dataMutex, portMAX_DELAY); }
        ~Lock() { xSemaphoreGive(dataMutex); }
    };

    /* Characters which survive the legacy (1.x) request parser unchanged. */
    bool isSafeChar(char c)
    {
        if (c < 0x20 || c > 0x7E)
            return false;
        return strchr("\"\\%#&+?", c) == nullptr;
    }

    bool isSafeString(const String &s)
    {
        for (size_t i = 0; i < s.length(); i++)
        {
            if (!isSafeChar(s.charAt(i)))
                return false;
        }
        return true;
    }

    struct HttpResult
    {
        bool connected = false;
        bool sawFooter = false;
        String data;
    };

    /* Plain HTTP/1.1 GET with a hand-made request line, as 1.x nodes only understand GET with payload in URL. */
    HttpResult httpGet(const IPAddress &ip, const String &path, uint32_t timeoutMs, size_t maxStoredBytes)
    {
        HttpResult result;
        WiFiClient client;
        if (!client.connect(ip, 80, 3000))
            return result;

        result.connected = true;
        client.print("GET " + path + " HTTP/1.1\r\nHost: " + ip.toString() + "\r\nConnection: close\r\n\r\n");

        const unsigned long start = millis();
        String tail;
        uint8_t buffer[256];
        while ((client.connected() || client.available()) && millis() - start < timeoutMs)
        {
            const int count = client.read(buffer, sizeof(buffer));
            if (count <= 0)
            {
                vTaskDelay(pdMS_TO_TICKS(5));
                continue;
            }
            String chunk;
            chunk.concat(reinterpret_cast<const char *>(buffer), count);

            /* Rejected requests render the full page including footer, accepted config apply closes before it */
            const String joined = tail + chunk;
            if (!result.sawFooter && joined.indexOf("<footer") >= 0)
                result.sawFooter = true;
            tail = joined.length() > 8 ? joined.substring(joined.length() - 8) : joined;

            if (result.data.length() < maxStoredBytes)
                result.data += chunk;
        }
        client.stop();
        return result;
    }

    String extractNodeConfigField(const String &json, const String &key, int from)
    {
        const String marker = "\"" + key + "\":";
        int pos = json.indexOf(marker, from);
        if (pos < 0)
            return "";
        pos += marker.length();
        if (json.charAt(pos) == '"')
        {
            const int end = json.indexOf('"', pos + 1);
            return end < 0 ? String("") : json.substring(pos + 1, end);
        }
        int end = pos;
        while (end < (int)json.length() && json.charAt(end) != ',' && json.charAt(end) != '}')
            end++;
        return json.substring(pos, end);
    }

    String encodeForLegacyUrl(String json)
    {
        json.replace("{", "%7B");
        json.replace("\"", "%22");
        json.replace("}", "%7D");
        json.replace(" ", "%20");
        return json;
    }

    void setResult(size_t index, NodeStatus status, const String &reason)
    {
        Lock lock;
        results[index].status = status;
        results[index].reason = reason;
    }

    void updateNode(size_t index)
    {
        setResult(index, NODE_IN_PROGRESS, "");
        const String ipString = results[index].ip;
        IPAddress ip;
        ip.fromString(ipString);

        /* 1. Unlock the node with the panel password owned by this Master */
        HttpResult unlock = httpGet(ip, "/passwordApply" + currentPanelPassword, 4000, 0);
        if (!unlock.connected)
        {
            setResult(index, NODE_NOK, "unreachable");
            return;
        }
        vTaskDelay(pdMS_TO_TICKS(300));

        /* 2. Verify the unlock - a locked node answers with a page instead of the JSON configuration */
        HttpResult config;
        int configPos = -1;
        for (uint8_t attempt = 0; attempt < 2 && configPos < 0; attempt++)
        {
            config = httpGet(ip, "/dwlddevcfg", 6000, MAX_CFG_RESPONSE);
            configPos = config.data.indexOf("\"NodeConfig\":[");
            if (configPos < 0)
                vTaskDelay(pdMS_TO_TICKS(500));
        }
        if (!config.connected)
        {
            setResult(index, NODE_NOK, "unreachable");
            return;
        }
        if (configPos < 0)
        {
            setResult(index, NODE_NOK, "panel password rejected");
            return;
        }

        /* Never touch a Master */
        if (extractNodeConfigField(config.data, "isRcServer", configPos) == "true")
        {
            setResult(index, NODE_NOK, "node is a Master");
            return;
        }

        const String nodeType = extractNodeConfigField(config.data, "nodeType", configPos);
        if (nodeType.length() == 0)
        {
            setResult(index, NODE_NOK, "node type unreadable");
            return;
        }

        if (extractNodeConfigField(config.data, "ssid", configPos) == newSsid &&
            extractNodeConfigField(config.data, "wifiPass", configPos) == newWifiPassword &&
            extractNodeConfigField(config.data, "usrPass", configPos) == newPanelPassword)
        {
            setResult(index, NODE_OK, "already up to date");
            return;
        }

        /* 3. Apply the same payload which the browser sends on the node's own configuration page.
              'type' is required by 1.x firmware and ignored by current one, Master flag is always off. */
        const String payload = "{\"httpActive\":\"yes\",\"rcServerActive\":\"no\",\"type\":\"" + nodeType +
                               "\",\"network\":\"" + newSsid + "\",\"netPwd\":\"" + newWifiPassword +
                               "\",\"cfgPwd\":\"" + newPanelPassword + "\"}";
        HttpResult apply = httpGet(ip, "/newCfgApply&" + encodeForLegacyUrl(payload), 4000, 0);

        if (apply.sawFooter)
        {
            setResult(index, NODE_NOK, "configuration rejected");
        }
        else
        {
            /* No answer or an early close means the node is restarting into the new network */
            setResult(index, NODE_OK, "");
        }
    }

    void rolloutTask(void *)
    {
        for (size_t i = 0; i < results.size(); i++)
        {
            if (abortRequested)
            {
                setResult(i, NODE_NOK, "skipped (aborted)");
                continue;
            }
            updateNode(i);
            vTaskDelay(pdMS_TO_TICKS(300));
        }

        {
            Lock lock;
            state = CredentialsRollout::State::Finished;
        }
        vTaskDelete(nullptr);
    }
} // namespace

bool CredentialsRollout::validate(const String &ssid, const String &wifiPassword, const String &panelPassword, String &error)
{
    if (ssid.length() == 0 || ssid.length() > MAX_FIELD_LENGTH)
    {
        error = "SSID must have 1-29 characters";
        return false;
    }
    if (panelPassword.length() == 0 || panelPassword.length() > MAX_FIELD_LENGTH)
    {
        error = "Panel password must have 1-29 characters";
        return false;
    }
    if (wifiPassword.length() > MAX_FIELD_LENGTH)
    {
        error = "WiFi password must have at most 29 characters";
        return false;
    }
    if (!isSafeString(ssid) || !isSafeString(wifiPassword) || !isSafeString(panelPassword))
    {
        error = "Only printable ASCII characters except \\\" \\\\ % # & + ? are allowed";
        return false;
    }
    return true;
}

static std::vector<String> collectSlaveIps()
{
    std::vector<String> ips;
    std::any nodesAny = DataContainer::getSignalValue(SIG_NETWORK_NODES_INFO);
    if (auto nodes = std::any_cast<std::vector<NetworkNodeInfo>>(&nodesAny))
    {
        for (auto &node : *nodes)
        {
            if (node.nodeType != NetworkNodeInfo::NodeType::Slave)
                continue;
            const MessageUDP::IPAddr &a = node.nodeIP;
            if (a.octet1 == 0 && a.octet2 == 0 && a.octet3 == 0 && a.octet4 == 0)
                continue;
            ips.push_back(String((int)a.octet1) + "." + String((int)a.octet2) + "." +
                          String((int)a.octet3) + "." + String((int)a.octet4));
        }
    }
    return ips;
}

size_t CredentialsRollout::knownSlaveCount()
{
    return collectSlaveIps().size();
}

bool CredentialsRollout::start(const String &ssid, const String &wifiPassword, const String &panelPassword, String &error)
{
    if (dataMutex == nullptr)
        dataMutex = xSemaphoreCreateMutex();

    if (!validate(ssid, wifiPassword, panelPassword, error))
        return false;

    if (state == State::Running)
    {
        error = "Update is already in progress";
        return false;
    }

    NodeConfiguration config = std::any_cast<NodeConfiguration>(DataContainer::getSignalValue(SIG_DEVICE_CONFIGURATION));
    if (!isSafeString(config.panelPassword))
    {
        error = "Current panel password contains characters which cannot be sent to other nodes";
        return false;
    }

    std::vector<String> ips = collectSlaveIps();
    if (ips.empty())
    {
        error = "No other nodes found";
        return false;
    }

    {
        Lock lock;
        results.clear();
        for (auto &ip : ips)
        {
            NodeResult node;
            node.ip = ip;
            results.push_back(node);
        }
        newSsid = ssid;
        newWifiPassword = wifiPassword;
        newPanelPassword = panelPassword;
        currentPanelPassword = config.panelPassword;
        abortRequested = false;
        state = State::Running;
    }

    if (xTaskCreate(rolloutTask, "credRollout", 10240, nullptr, 1, nullptr) != pdPASS)
    {
        Lock lock;
        state = State::Idle;
        error = "Not enough memory to start the update";
        return false;
    }
    return true;
}

void CredentialsRollout::abort()
{
    abortRequested = true;
}

void CredentialsRollout::reset()
{
    if (dataMutex == nullptr || state == State::Running)
        return;
    Lock lock;
    results.clear();
    newSsid = newWifiPassword = newPanelPassword = currentPanelPassword = "";
    state = State::Idle;
}

bool CredentialsRollout::applyLocalDirectly(const String &ssid, const String &wifiPassword, const String &panelPassword, String &error)
{
    if (!validate(ssid, wifiPassword, panelPassword, error))
        return false;
    if (!ConfigProvider::setCredentialsOnly(ssid, wifiPassword, panelPassword))
    {
        error = "Credentials cannot be applied, device is locked";
        return false;
    }
    return true;
}

bool CredentialsRollout::commitLocal()
{
    if (dataMutex == nullptr || state != State::Finished)
        return false;
    String ssid, wifiPassword, panelPassword;
    {
        Lock lock;
        ssid = newSsid;
        wifiPassword = newWifiPassword;
        panelPassword = newPanelPassword;
    }
    const bool applied = ConfigProvider::setCredentialsOnly(ssid, wifiPassword, panelPassword);
    if (applied)
        reset();
    return applied;
}

CredentialsRollout::State CredentialsRollout::getState()
{
    return state;
}

String CredentialsRollout::statusJson()
{
    if (dataMutex == nullptr)
        return "{\"state\":\"idle\",\"nodes\":[]}";

    Lock lock;
    const char *stateName = state == State::Running ? "running" : (state == State::Finished ? "finished" : "idle");
    String json = String("{\"state\":\"") + stateName + "\",\"nodes\":[";
    for (size_t i = 0; i < results.size(); i++)
    {
        const char *status = "pending";
        if (results[i].status == NODE_IN_PROGRESS)
            status = "progress";
        else if (results[i].status == NODE_OK)
            status = "ok";
        else if (results[i].status == NODE_NOK)
            status = "nok";
        if (i > 0)
            json += ",";
        json += "{\"ip\":\"" + results[i].ip + "\",\"status\":\"" + status + "\",\"reason\":\"" + results[i].reason + "\"}";
    }
    json += "]}";
    return json;
}
