#ifndef TIMEMASTER_HPP
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

    static NTPClient timeClient;
    static WiFiUDP ntpUDP;
    
    static unsigned long lastUpdateTime;    // Czas ostatniej aktualizacji
    static unsigned long lastNTPTime;     // Ostatni poprawny czas z NTP (epoch)
    static unsigned long lastMillis;      // Czas w ms od startu lokalnego odliczania
    static bool ntpAvailable;         // Flaga dostępności NTP
    static unsigned long updateInterval; // Interwał aktualizacji (1 minuta)
    static unsigned long remainder; // Reszta czasu do dodania do lokalnego odliczania

    static void sendTimeToDataContainer(); // Wysłanie czasu do DataContainer
    
};

#endif // MASTERTIMER_HPP