#include "os/drivers/ota.hpp"
#include "os/datacontainer/DataContainer.hpp"
#include "os/Logger.hpp"


#include "esp_task_wdt.h"
void OTA::init(const String hostname, const String password)
{
    // Port defaults to 3232
    // ArduinoOTA.setPort(3232);

    // Hostname defaults to esp3232-[MAC]
    // ArduinoOTA.setHostname(hostname.c_str());

    // No authentication by default
    ArduinoOTA.setPassword(password.c_str());

    ArduinoOTA.onStart([]() {
        // Disable watchdog to avoid reset during OTA
        esp_task_wdt_delete(NULL); // disable watchdog during OTA update
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
            type = "sketch";
        else // U_SPIFFS
            type = "filesystem";

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Logger::log("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
        Logger::log("\nEnd");

        std::any_cast<std::function<void()>>(DataContainer::getSignalValue(CBK_RESET_DEVICE))();

    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR)
            Logger::log("Auth Failed");
        else if (error == OTA_BEGIN_ERROR)
            Logger::log("Begin Failed");
        else if (error == OTA_CONNECT_ERROR)
            Logger::log("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR)
            Logger::log("Receive Failed");
        else if (error == OTA_END_ERROR)
            Logger::log("End Failed");
    });
    ArduinoOTA.setMdnsEnabled(false);
    ArduinoOTA.begin();
    Logger::log("OTA Ready");
    Logger::log(WiFi.localIP().toString());
}

void OTA::cyclic()
{
    ArduinoOTA.handle();
}

