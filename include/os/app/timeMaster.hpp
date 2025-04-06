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

    //Testowanie NTP
    // static unsigned long simulationStartTime;
    // static bool simulateNtpFailure;

    static void init();   // Inicjalizacja klienta NTP
    static void deinit();  // Deinicjalizacja klienta NTP    
    static void cyclic();  // Cykliczne wywoływanie funkcji update()

    
    static unsigned long getEpochTime();    // Pobranie czasu w formacie Unix timestamp
    static void setTimeZone(int timeZoneOffset); // Ustawienie przesunięcia strefy 
    static RtcTime getRtcTime(); // Pobranie czasu RTC w formacie RtcTime
    

private:

    static NTPClient timeClient;
    static WiFiUDP ntpUDP;
    static uint8_t initialRetryCount; // Licznik prób synchronizacji NTP
    static bool wasNtpEverSynced; // Flaga, czy NTP był kiedykolwiek zsynchronizowany
    
    static unsigned long lastUpdateTime;    // Czas ostatniej aktualizacji
    static unsigned long lastNTPTime;     // Ostatni poprawny czas z NTP (epoch)
    static unsigned long lastMillis;      // Czas w ms od startu lokalnego odliczania
    static bool ntpAvailable;         // Flaga dostępności NTP
    static unsigned long updateInterval; // Interwał aktualizacji (1 minuta)
    static void updateNtpVariables(); // Funkcja do aktualizacji zmiennych NTP

    
    
};

#endif // MASTERTIMER_HPP