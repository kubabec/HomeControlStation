/**
 * @file src/os/app/timeMaster.cpp
 * @brief Implementation of the Home Control Station runtime logic.
 */

#include <os/app/timeMaster.hpp>
#include <ArduinoJson.h>
#include <algorithm>
#include <cerrno>
#include <nvs.h>
#include "os/Logger.hpp"

#define NTP_RESYNC_TIME (5 * 60 * 1000)            // 5 minut
#define HALF_NTP_RESYNC_TIME (NTP_RESYNC_TIME / 2) // 2.5 minut

unsigned long TimeMaster::lastUpdateTime = 0;
unsigned long TimeMaster::lastNTPTime = 0;
unsigned long TimeMaster::lastMillis = 0;
bool TimeMaster::ntpAvailable = false;
uint8_t TimeMaster::initialRetryCount = 0; // Licznik prób synchronizacji NTP
bool TimeMaster::wasNtpEverSynced = false;
bool TimeMaster::startupTimeInitialized = false;
uint8_t TimeMaster::timeSyncFailureCount = 0;
std::vector<RtcEvent::Schedule> TimeMaster::rtcEvents;
uint32_t TimeMaster::lastEvaluatedMinute = UINT32_MAX;

namespace
{
constexpr char RTC_NVM_NAMESPACE[] = "rtc_events";
constexpr char RTC_NVM_KEY[] = "schedules";
constexpr uint32_t RTC_NVM_MAGIC = 0x52544345;
constexpr uint8_t RTC_NVM_VERSION = 1;

struct __attribute__((packed)) RtcEventBlob
{
    uint32_t magic = RTC_NVM_MAGIC;
    uint8_t version = RTC_NVM_VERSION;
    uint8_t count = 0;
    RtcEvent::Schedule schedules[RtcEvent::MAX_EVENTS]{};
    uint32_t checksum = 0;
};

static_assert(sizeof(RtcEventBlob) == 170, "RTC event NVS blob layout changed");

uint32_t calculateRtcBlobChecksum(const RtcEventBlob& blob)
{
    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&blob);
    uint32_t checksum = 2166136261UL;
    for (size_t index = 0; index < offsetof(RtcEventBlob, checksum); ++index)
    {
        checksum ^= bytes[index];
        checksum *= 16777619UL;
    }
    return checksum;
}

bool isValidSchedule(const RtcEvent::Schedule& schedule)
{
    if (schedule.eventId == 0 || schedule.anchorEpoch == 0 ||
        schedule.recurrence > RtcEvent::Recurrence::WEEKLY)
        return false;
    if (schedule.recurrence == RtcEvent::Recurrence::INTERVAL_HOURS)
        return schedule.intervalHours >= 1 && schedule.intervalHours <= 168;
    if (schedule.recurrence == RtcEvent::Recurrence::WEEKLY)
        return schedule.weekdayMask != 0 && (schedule.weekdayMask & 0x80) == 0;
    return true;
}
}

WiFiUDP TimeMaster::ntpUDP;
NTPClient TimeMaster::timeClient(ntpUDP, "pool.ntp.org", 3600);

void TimeMaster::init()
{
    Logger::log("TimeMaster init ...");
    rtcEvents.clear();
    lastEvaluatedMinute = UINT32_MAX;
    restoreRtcEvents();
    RtcTime startupTime;
    DataContainer::setSignalValue(SIG_STARTUP_TIME, static_cast<RtcTime>(startupTime));

    timeClient.begin();
    setTimeZone(1); // Ustawienie strefy czasowej na GMT+1
    DataContainer::setSignalValue(CBK_GET_CURRENT_TIME, static_cast<std::function<RtcTime()>>(TimeMaster::getRtcTime));

    // Próba synchronizacji z NTP
    if (timeClient.update())
    {
        updateNtpVariables();
        Logger::log("Synchronized with NTP!");
        DataContainer::setSignalValue(SIG_STARTUP_TIME, static_cast<RtcTime>(getRtcTime()));
        startupTimeInitialized = true; // Czas startowy został zainicjalizowany

        UserInterfaceNotification notif;
        notif.title = "SYSTEM RUNNING";
        notif.body = "Welcome to Home Control Station!";
        notif.type = UserInterfaceNotification::INFO;
        std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);
    }
    else
    {
        // NTP nie działa - przechodzimy na freerunning
        ntpAvailable = false;
        Logger::log("NTP unavailable");
    }

    Logger::log("... done");
}

void TimeMaster::deinit()
{
}

// Funkcja do cyklicznego wywoływania aktualizacji czasu
void TimeMaster::cyclic()
{

    if (!wasNtpEverSynced && initialRetryCount < 60)
    {
        if (timeClient.update())
        {
            updateNtpVariables();
            initialRetryCount = 0; // Resetujemy licznik prób synchronizacji

            // Do only once after first successful NTP sync
            if (!startupTimeInitialized)
            {
                DataContainer::setSignalValue(SIG_STARTUP_TIME, static_cast<RtcTime>(getRtcTime()));
                startupTimeInitialized = true; // Czas startowy został zainicjalizowany

                UserInterfaceNotification notif;
                notif.title = "SYSTEM RUNNING";
                notif.body = "Welcome to Home Control Station!";
                notif.type = UserInterfaceNotification::INFO;
                std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);
            }
        }
        else
        {
            initialRetryCount++;
        }
    }

    unsigned long now = millis();

    if (now - lastUpdateTime >= NTP_RESYNC_TIME)
    {
        Logger::log("NTP resyncing ...");
        if (timeClient.update())
        {
            updateNtpVariables();
            timeSyncFailureCount = 0;
        }
        else
        {
            // Błąd synchronizacji NTP - tryb freerunning
            timeSyncFailureCount++;

            // Try to reconnect the WiFi when 3x times time sync failed
            if (timeSyncFailureCount == 3)
            {
                timeSyncFailureCount = 0;
                {
                    std::any localAny{DataContainer::getSignalValue(CBK_RECONNECT_WIFI)};
                    if (auto p = std::any_cast<std::function<void()>>(&localAny))
                    {
                        (*p)();
                    }
                    else
                    {
                    }
                }
            }

            ntpAvailable = false;
            lastUpdateTime = now - HALF_NTP_RESYNC_TIME;
        }
    }

    evaluateRtcEvents();
}

const std::vector<RtcEvent::Schedule>& TimeMaster::getRtcEvents()
{
    return rtcEvents;
}

bool TimeMaster::updateRtcEventsViaJson(String& json)
{
    JsonDocument document;
    if (deserializeJson(document, json) != DeserializationError::Ok || !document.is<JsonArray>())
        return false;

    std::vector<RtcEvent::Schedule> updated;
    for (JsonObject object : document.as<JsonArray>())
    {
        if (updated.size() >= RtcEvent::MAX_EVENTS) return false;
        if (!object["anchorEpoch"].is<uint32_t>()) return false;
        const char* eventIdText = object["eventId"] | "";
        char* parseEnd = nullptr;
        errno = 0;
        const uint64_t eventId = strtoull(eventIdText, &parseEnd, 10);
        RtcEvent::Schedule schedule{
            eventId,
            object["anchorEpoch"].as<uint32_t>(),
            object["intervalHours"].as<uint16_t>(),
            static_cast<RtcEvent::Recurrence>(object["recurrence"].as<uint8_t>()),
            object["weekdayMask"].as<uint8_t>()
        };
        const bool duplicate = std::any_of(updated.begin(), updated.end(), [eventId](const auto& existing)
                                           { return existing.eventId == eventId; });
        if (parseEnd == eventIdText || *parseEnd != '\0' || errno == ERANGE || duplicate || !isValidSchedule(schedule))
            return false;
        updated.push_back(schedule);
    }
    rtcEvents = std::move(updated);
    lastEvaluatedMinute = UINT32_MAX;
    return saveRtcEvents();
}

void TimeMaster::restoreRtcEvents()
{
    nvs_handle_t handle;
    if (nvs_open(RTC_NVM_NAMESPACE, NVS_READONLY, &handle) != ESP_OK) return;
    RtcEventBlob blob{};
    size_t blobSize = sizeof(blob);
    const esp_err_t result = nvs_get_blob(handle, RTC_NVM_KEY, &blob, &blobSize);
    nvs_close(handle);
    if (result != ESP_OK || blobSize != sizeof(blob) || blob.magic != RTC_NVM_MAGIC ||
        blob.version != RTC_NVM_VERSION || blob.count > RtcEvent::MAX_EVENTS ||
        blob.checksum != calculateRtcBlobChecksum(blob))
    {
        Logger::log("TimeMaster:// RTC event NVS blob is absent or invalid");
        return;
    }
    for (uint8_t index = 0; index < blob.count; ++index)
    {
        const RtcEvent::Schedule& schedule = blob.schedules[index];
        if (isValidSchedule(schedule)) rtcEvents.push_back(schedule);
    }
    Logger::log("TimeMaster:// Restored " + String((int)rtcEvents.size()) + " RTC events");
}

bool TimeMaster::saveRtcEvents()
{
    RtcEventBlob blob{};
    blob.count = static_cast<uint8_t>(std::min(rtcEvents.size(), static_cast<size_t>(RtcEvent::MAX_EVENTS)));
    for (uint8_t index = 0; index < blob.count; ++index)
    {
        blob.schedules[index] = rtcEvents[index];
    }
    blob.checksum = calculateRtcBlobChecksum(blob);

    nvs_handle_t handle;
    esp_err_t result = nvs_open(RTC_NVM_NAMESPACE, NVS_READWRITE, &handle);
    if (result == ESP_OK)
    {
        result = nvs_set_blob(handle, RTC_NVM_KEY, &blob, sizeof(blob));
        if (result == ESP_OK) result = nvs_commit(handle);
        nvs_close(handle);
    }
    if (result == ESP_OK)
        Logger::log("TimeMaster:// Saved " + String((int)blob.count) + " RTC events to isolated NVS");
    else
        Logger::log("TimeMaster:// RTC event NVS save failed: " + String(esp_err_to_name(result)));
    return result == ESP_OK;
}

void TimeMaster::evaluateRtcEvents()
{
    if (!wasNtpEverSynced) return;
    const uint32_t now = getEpochTime();
    const uint32_t currentMinute = now / 60;
    if (currentMinute == lastEvaluatedMinute) return;
    lastEvaluatedMinute = currentMinute;
    const RtcTime localTime = getRtcTime();
    bool removedOneShot = false;

    for (auto event = rtcEvents.begin(); event != rtcEvents.end();)
    {
        const uint32_t anchorMinute = event->anchorEpoch / 60;
        bool matches = false;
        switch (event->recurrence)
        {
        case RtcEvent::Recurrence::ONCE:
            matches = currentMinute >= anchorMinute;
            break;
        case RtcEvent::Recurrence::INTERVAL_HOURS:
            matches = currentMinute >= anchorMinute &&
                      (currentMinute - anchorMinute) % (event->intervalHours * 60UL) == 0;
            break;
        case RtcEvent::Recurrence::DAILY:
        case RtcEvent::Recurrence::WEEKLY:
        {
            time_t anchor = event->anchorEpoch;
            const struct tm* anchorTime = localtime(&anchor);
            matches = anchorTime != nullptr && localTime.hour == anchorTime->tm_hour &&
                      localTime.min == anchorTime->tm_min;
            if (matches && event->recurrence == RtcEvent::Recurrence::WEEKLY)
                matches = (event->weekdayMask & (1U << localTime.wday)) != 0;
            break;
        }
        }

        if (matches)
        {
            Logger::log("TimeMaster:// Firing RTC DigitalEvent " + String((unsigned long long)event->eventId));
            std::any_cast<std::function<void(uint64_t, const String&)>>(
                DataContainer::getSignalValue(CBK_FIRE_DIGITAL_EVENT_WITH_SOURCE))(event->eventId, "TimeMaster");
        }
        if (matches && event->recurrence == RtcEvent::Recurrence::ONCE)
        {
            event = rtcEvents.erase(event);
            removedOneShot = true;
        }
        else
        {
            ++event;
        }
    }

    if (removedOneShot)
    {
        saveRtcEvents();
    }
}

RtcTime TimeMaster::getRtcTime()
{
    time_t rawTime = TimeMaster::getEpochTime(); // Pobranie czasu Unix
    struct tm *timeInfo = localtime(&rawTime);   // Konwersja na czas UTC

    RtcTime rtcTime;
    rtcTime.sec = timeInfo->tm_sec;
    rtcTime.min = timeInfo->tm_min;
    rtcTime.hour = timeInfo->tm_hour;
    rtcTime.mday = timeInfo->tm_mday;
    rtcTime.mon = timeInfo->tm_mon + 1;      // Miesiące są indeksowane od 0
    rtcTime.year = timeInfo->tm_year + 1900; // Rok od 1900
    rtcTime.wday = timeInfo->tm_wday;
    rtcTime.yday = timeInfo->tm_yday;
    rtcTime.isdst = timeInfo->tm_isdst;

    return rtcTime; // Zwracamy strukturę RtcTime
}

// Ustawienie przesunięcia strefy czasowej (w godzinach)
void TimeMaster::setTimeZone(int timeZoneOffset)
{
    TimeMaster::timeClient.setTimeOffset(timeZoneOffset * 7200);
}

unsigned long TimeMaster::getEpochTime()
{
    unsigned long elapsedMillis = millis() - lastMillis;
    return lastNTPTime + (elapsedMillis / 1000);
}

void TimeMaster::updateNtpVariables()
{
    // Sukces synchronizacji NTP
    lastNTPTime = timeClient.getEpochTime();
    lastMillis = millis();

    ntpAvailable = true;
    lastUpdateTime = millis();
    wasNtpEverSynced = true; // NTP był kiedykolwiek zsynchronizowany
    Logger::log("NTP time updated: " + String(lastNTPTime));
}
