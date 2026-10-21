#include "os/drivers/softwareupdater.hpp"

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <Update.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <mbedtls/sha256.h>

#include "build_info.h"
#include "os/Logger.hpp"
#include "os/drivers/update_server_ca.hpp"

#ifndef HCS_UPDATE_SERVER
#define HCS_UPDATE_SERVER "https://www.homecontrolstation.pl"
#endif

namespace
{
enum class UpdateState
{
    Idle,
    Downloading,
    Ready,
    Restarting,
    Failed
};

UpdateState state = UpdateState::Idle;
HTTPClient firmwareHttp;
WiFiClientSecure firmwareClient;
mbedtls_sha256_context sha256Context;
String selectedVersion;
String expectedSha256;
String statusMessage = "No update running.";
size_t expectedSize = 0;
size_t downloadedSize = 0;
unsigned long lastDataTime = 0;
unsigned long restartAt = 0;
bool sha256Started = false;

void releaseChecksum()
{
    if (sha256Started)
    {
        mbedtls_sha256_free(&sha256Context);
        sha256Started = false;
    }
}

String baseUrl()
{
    String url = HCS_UPDATE_SERVER;
    while (url.endsWith("/"))
        url.remove(url.length() - 1);
    return url;
}

String manifestUrl()
{
    return baseUrl() + "/software/" + SoftwareUpdater::target() + "/manifest.json";
}

void setFailure(const String &message)
{
    if (state == UpdateState::Downloading)
        Update.abort();
    releaseChecksum();
    firmwareHttp.end();
    state = UpdateState::Failed;
    statusMessage = message;
    Logger::log("Software update failed: " + message);
}

bool loadManifest(JsonDocument &document)
{
    if (WiFi.status() != WL_CONNECTED)
    {
        statusMessage = "Wi-Fi is not connected.";
        return false;
    }

    WiFiClientSecure client;
    client.setCACert(HCS_UPDATE_SERVER_CA);
    HTTPClient http;
    if (!http.begin(client, manifestUrl()))
    {
        statusMessage = "Could not connect to update server.";
        return false;
    }

    http.setTimeout(15000);
    const int responseCode = http.GET();
    if (responseCode != HTTP_CODE_OK)
    {
        statusMessage = "Update manifest request failed (" + String(responseCode) + ").";
        http.end();
        return false;
    }

    const DeserializationError error = deserializeJson(document, http.getString());
    http.end();
    if (error)
    {
        statusMessage = "Update manifest is invalid.";
        return false;
    }
    return true;
}

String sha256Hex(const uint8_t *digest)
{
    String result;
    result.reserve(64);
    for (uint8_t index = 0; index < 32; ++index)
    {
        if (digest[index] < 16)
            result += '0';
        result += String(digest[index], HEX);
    }
    result.toLowerCase();
    return result;
}

void finishDownload()
{
    uint8_t digest[32];
    if (mbedtls_sha256_finish_ret(&sha256Context, digest) != 0)
    {
        releaseChecksum();
        setFailure("Could not calculate firmware checksum.");
        return;
    }
    releaseChecksum();

    if (sha256Hex(digest) != expectedSha256)
    {
        setFailure("Firmware checksum does not match.");
        return;
    }
    if (!Update.end(true))
    {
        setFailure("Firmware could not be finalized.");
        return;
    }

    firmwareHttp.end();
    state = UpdateState::Ready;
    statusMessage = "Firmware installed. Confirm restart to boot it.";
    Logger::log("Software update ready: " + selectedVersion);
}
} // namespace

const char *SoftwareUpdater::target()
{
#if defined(HCS_TARGET_DISPLAY_S3_4INCH)
    return "s3-display4inch";
#elif defined(SUPERMINI)
    return "s3-supermini";
#else
    return "s3-c1";
#endif
}

void SoftwareUpdater::init()
{
    state = UpdateState::Idle;
    statusMessage = "No update running.";
}

bool SoftwareUpdater::isBusy()
{
    return state == UpdateState::Downloading || state == UpdateState::Restarting;
}

String SoftwareUpdater::getVersionsJson()
{
    JsonDocument response;
    response["target"] = target();
    response["currentVersion"] = BUILD_VERSION;

    JsonDocument manifest;
    if (!loadManifest(manifest))
    {
        response["ok"] = false;
        response["error"] = statusMessage;
    }
    else
    {
        response["ok"] = true;
        JsonArray releases = response["releases"].to<JsonArray>();
        for (JsonObject release : manifest["releases"].as<JsonArray>())
        {
            JsonObject item = releases.add<JsonObject>();
            item["version"] = release["version"];
            item["size"] = release["size"];
            item["releaseNotes"] = release["releaseNotes"] | "";
        }
    }

    String output;
    serializeJson(response, output);
    return output;
}

String SoftwareUpdater::getStatusJson()
{
    JsonDocument response;
    const char *stateName = "idle";
    if (state == UpdateState::Downloading)
        stateName = "downloading";
    else if (state == UpdateState::Ready)
        stateName = "ready";
    else if (state == UpdateState::Restarting)
        stateName = "restarting";
    else if (state == UpdateState::Failed)
        stateName = "failed";

    response["state"] = stateName;
    response["progress"] = expectedSize == 0 ? 0 : (downloadedSize * 100 / expectedSize);
    response["message"] = statusMessage;
    response["version"] = selectedVersion;
    response["target"] = target();

    String output;
    serializeJson(response, output);
    return output;
}

bool SoftwareUpdater::start(const String &version)
{
    if (isBusy() || version.length() == 0)
        return false;

    JsonDocument manifest;
    if (!loadManifest(manifest))
    {
        state = UpdateState::Failed;
        return false;
    }

    JsonObject selectedRelease;
    for (JsonObject release : manifest["releases"].as<JsonArray>())
    {
        if (String(release["version"] | "") == version)
        {
            selectedRelease = release;
            break;
        }
    }

    const String path = selectedRelease["path"] | "";
    expectedSize = selectedRelease["size"] | 0;
    expectedSha256 = selectedRelease["sha256"] | "";
    if (selectedRelease.isNull() || path.length() == 0 || expectedSize == 0 || expectedSha256.length() != 64)
    {
        state = UpdateState::Failed;
        statusMessage = "Selected version is not available for this device.";
        return false;
    }

    String firmwareUrl = baseUrl() + "/software/" + target() + "/" + path;
    firmwareClient.setCACert(HCS_UPDATE_SERVER_CA);
    if (!firmwareHttp.begin(firmwareClient, firmwareUrl))
    {
        state = UpdateState::Failed;
        statusMessage = "Could not connect to firmware server.";
        return false;
    }
    firmwareHttp.setTimeout(15000);
    firmwareHttp.useHTTP10(true);
    const int responseCode = firmwareHttp.GET();
    if (responseCode != HTTP_CODE_OK || firmwareHttp.getSize() != static_cast<int>(expectedSize))
    {
        firmwareHttp.end();
        state = UpdateState::Failed;
        statusMessage = "Firmware download could not be started.";
        return false;
    }
    if (!Update.begin(expectedSize, U_FLASH))
    {
        firmwareHttp.end();
        state = UpdateState::Failed;
        statusMessage = "Inactive OTA partition is not available.";
        return false;
    }

    mbedtls_sha256_init(&sha256Context);
    if (mbedtls_sha256_starts_ret(&sha256Context, 0) != 0)
    {
        mbedtls_sha256_free(&sha256Context);
        Update.abort();
        firmwareHttp.end();
        state = UpdateState::Failed;
        statusMessage = "Could not initialize checksum.";
        return false;
    }
    sha256Started = true;
    selectedVersion = version;
    downloadedSize = 0;
    lastDataTime = millis();
    state = UpdateState::Downloading;
    statusMessage = "Downloading firmware.";
    return true;
}

void SoftwareUpdater::cyclic()
{
    if (state == UpdateState::Restarting)
    {
        if (millis() >= restartAt)
            ESP.restart();
        return;
    }
    if (state != UpdateState::Downloading)
        return;

    WiFiClient *stream = firmwareHttp.getStreamPtr();
    uint8_t buffer[4096];
    const int available = stream->available();
    if (available > 0)
    {
        const size_t requested = min(static_cast<size_t>(available), sizeof(buffer));
        const size_t read = stream->readBytes(buffer, requested);
        if (read == 0 || Update.write(buffer, read) != read ||
            mbedtls_sha256_update_ret(&sha256Context, buffer, read) != 0)
        {
            setFailure("Firmware download failed.");
            return;
        }
        downloadedSize += read;
        lastDataTime = millis();
    }
    else if (downloadedSize < expectedSize && millis() - lastDataTime > 15000)
    {
        setFailure("Firmware download timed out.");
        return;
    }

    if (downloadedSize == expectedSize)
        finishDownload();
}

bool SoftwareUpdater::requestRestart()
{
    if (state != UpdateState::Ready)
        return false;
    state = UpdateState::Restarting;
    statusMessage = "Restarting device.";
    restartAt = millis() + 1000;
    return true;
}
