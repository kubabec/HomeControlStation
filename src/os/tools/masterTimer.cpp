#include <os/tools/masterTimer.hpp>

// Konstruktor
MasterTimer::MasterTimer()
    : timeClient(ntpUDP, "pool.ntp.org", 3600) {
}

// Inicjalizacja klienta NTP
void MasterTimer::begin() {
    timeClient.begin();
}


void MasterTimer::cyclic() {
    // Sprawdzamy, czy czas od ostatniej aktualizacji jest większy niż interwał
  timeClient.update();
}

// Aktualizacja czasu z serwera NTP




// Pobranie sformatowanego czasu (HH:MM:SS)
String MasterTimer::getFormattedTime() {
    return timeClient.getFormattedTime();
}

// Pobranie sformatowanej daty i czasu (yyyy.mm.dd hh:mm:ss)
String MasterTimer::getFormattedDateTime() {
    //timeClient.update(); // Aktualizacja czasu
    time_t rawTime = timeClient.getEpochTime(); // Pobranie czasu Unix
    struct tm *timeInfo = gmtime(&rawTime);    // Konwersja na czas UTC

    char buffer[20]; 
    snprintf(buffer, sizeof(buffer), "%04d.%02d.%02d %02d:%02d:%02d",
             timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, timeInfo->tm_mday,
             timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);

    return String(buffer);
}

// Pobranie czasu w formacie Unix timestamp
unsigned long MasterTimer::getEpochTime() {
    return timeClient.getEpochTime();
}

// Ustawienie przesunięcia strefy czasowej (w godzinach)
void MasterTimer::setTimeZone(int timeZoneOffset) {
    timeClient.setTimeOffset(timeZoneOffset * 3600);
}