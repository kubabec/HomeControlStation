#ifndef TIMEMASTERR_HPP
#define TIMEMASTER_HPP

#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ctime>  // Do konwersji czasu Unix na datę
#include <os/datacontainer/datacontainertypes.hpp>
#include <os/datacontainer/DataContainer.hpp>

class TimeMaster {
public:
    

    static void init();   // Inicjalizacja klienta NTP
    static void deinit();  // Deinicjalizacja klienta NTP
    
    static void cyclic();  // Cykliczne wywoływanie funkcji update()

    static String getFormattedTime();       // Pobranie sformatowanego czasu HH:MM:SS
    static String getFormattedDateTime();   // Pobranie pełnej daty i czasu yyyy.mm.dd hh:mm:ss
    static unsigned long getEpochTime();    // Pobranie czasu w formacie Unix timestamp
    static void setTimeZone(int timeZoneOffset); // Ustawienie przesunięcia strefy czasowej

private:
    TimeMaster(); // Konstruktor

    WiFiUDP ntpUDP;
    NTPClient timeClient;
    unsigned long lastUpdateTime;    // Czas ostatniej aktualizacji
    unsigned long lastNTPTime = 0;     // Ostatni poprawny czas z NTP (epoch)
    unsigned long lastMillis = 0;      // Czas w ms od startu lokalnego odliczania
    bool ntpAvailable = false;         // Flaga dostępności NTP

    const unsigned long updateInterval = 60000; // Interwał aktualizacji (1 minuta)

    static TimeMaster timeMaster;

    void sendTimeToDataContainer(); // Wysłanie czasu do DataContainer
};

#endif // MASTERTIMER_HPP