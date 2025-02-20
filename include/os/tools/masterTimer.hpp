#ifndef MASTERTIMER_HPP
#define MASTERTIMER_HPP

#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ctime>  // Do konwersji czasu Unix na datę

class MasterTimer {
public:
    MasterTimer(); // Konstruktor

    void begin();   // Inicjalizacja klienta NTP
    
    void cyclic();  // Cykliczne wywoływanie funkcji update()

    String getFormattedTime();       // Pobranie sformatowanego czasu HH:MM:SS
    String getFormattedDateTime();   // Pobranie pełnej daty i czasu yyyy.mm.dd hh:mm:ss
    unsigned long getEpochTime();    // Pobranie czasu w formacie Unix timestamp
    void setTimeZone(int timeZoneOffset); // Ustawienie przesunięcia strefy czasowej

private:
    WiFiUDP ntpUDP;
    NTPClient timeClient;
    unsigned long lastUpdateTime;    // Czas ostatniej aktualizacji
    const unsigned long updateInterval = 120000; // Interwał aktualizacji (2 minuty
};

#endif // MASTERTIMER_HPP