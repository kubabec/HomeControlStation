#include <os/app/timeMaster.hpp>
#include "os/Logger.hpp"

#define NTP_RESYNC_TIME (5 * 60 * 1000) //5 minut
#define HALF_NTP_RESYNC_TIME (NTP_RESYNC_TIME / 2) //2.5 minut

unsigned long TimeMaster::lastUpdateTime = 0;
unsigned long TimeMaster::lastNTPTime = 0;
unsigned long TimeMaster::lastMillis = 0;
bool TimeMaster::ntpAvailable = false;
uint8_t TimeMaster::initialRetryCount = 0; // Licznik prób synchronizacji NTP
bool TimeMaster::wasNtpEverSynced = false;
bool TimeMaster::startupTimeInitialized = false;

WiFiUDP TimeMaster::ntpUDP;
NTPClient TimeMaster::timeClient(ntpUDP, "pool.ntp.org", 3600);


void TimeMaster::init() {
    Logger::log("DeviceProvider init ...");
    RtcTime startupTime;
    DataContainer::setSignalValue(SIG_STARTUP_TIME, static_cast<RtcTime>(startupTime));


    
    timeClient.begin();
    setTimeZone(1); // Ustawienie strefy czasowej na GMT+1

    
    // Próba synchronizacji z NTP
    if (timeClient.update()) {
        updateNtpVariables();
        Logger::log("Synchronized with NTP!");
        DataContainer::setSignalValue(SIG_STARTUP_TIME, static_cast<RtcTime>(getRtcTime()));
        startupTimeInitialized = true; // Czas startowy został zainicjalizowany
    } else {
        // NTP nie działa - przechodzimy na freerunning
        ntpAvailable = false;
        Logger::log("NTP unavailable");
    }    
    DataContainer::setSignalValue(CBK_GET_CURRENT_TIME, static_cast<std::function<RtcTime()>>(TimeMaster::getRtcTime));
     
    Logger::log("... done");
}

void TimeMaster::deinit() {
    
}

// Funkcja do cyklicznego wywoływania aktualizacji czasu
void TimeMaster::cyclic() {

    if(!wasNtpEverSynced && initialRetryCount < 60) {
        if (timeClient.update()) {
            updateNtpVariables();
            initialRetryCount = 0; // Resetujemy licznik prób synchronizacji

            // Do only once after first successful NTP sync
            if(!startupTimeInitialized) {
                DataContainer::setSignalValue(SIG_STARTUP_TIME, static_cast<RtcTime>(getRtcTime()));
                startupTimeInitialized = true; // Czas startowy został zainicjalizowany
            }
        } else {
            initialRetryCount++;
        }
    }



    unsigned long now = millis();

    if (now - lastUpdateTime >= NTP_RESYNC_TIME) {
        Logger::log("NTP resyncing ...");
        if (timeClient.update()) {
            updateNtpVariables();
        } else {
            // Błąd synchronizacji NTP - tryb freerunning
            
            ntpAvailable = false;
            lastUpdateTime = now - HALF_NTP_RESYNC_TIME;
            
        }

        
    }
}


RtcTime TimeMaster::getRtcTime() {
    time_t rawTime = TimeMaster::getEpochTime(); // Pobranie czasu Unix
    struct tm *timeInfo = localtime(&rawTime);    // Konwersja na czas UTC

    RtcTime rtcTime;
    rtcTime.sec = timeInfo->tm_sec;
    rtcTime.min = timeInfo->tm_min;
    rtcTime.hour = timeInfo->tm_hour;
    rtcTime.mday = timeInfo->tm_mday;
    rtcTime.mon = timeInfo->tm_mon + 1; // Miesiące są indeksowane od 0
    rtcTime.year = timeInfo->tm_year + 1900; // Rok od 1900
    rtcTime.wday = timeInfo->tm_wday;
    rtcTime.yday = timeInfo->tm_yday;
    rtcTime.isdst = timeInfo->tm_isdst;

    return rtcTime; // Zwracamy strukturę RtcTime
}


// Ustawienie przesunięcia strefy czasowej (w godzinach)
void TimeMaster::setTimeZone(int timeZoneOffset) {
    TimeMaster::timeClient.setTimeOffset(timeZoneOffset * 7200);
}

unsigned long TimeMaster::getEpochTime() {
    unsigned long elapsedMillis = millis() - lastMillis;
    return lastNTPTime + (elapsedMillis / 1000);
}

void TimeMaster::updateNtpVariables() {
    // Sukces synchronizacji NTP
    lastNTPTime = timeClient.getEpochTime();
    lastMillis = millis();
    
    ntpAvailable = true;
    lastUpdateTime = millis();
    wasNtpEverSynced = true; // NTP był kiedykolwiek zsynchronizowany 
    Logger::log("NTP time updated: " + String(lastNTPTime));         
}
    
 

