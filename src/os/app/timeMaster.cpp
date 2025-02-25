#include <os/app/timeMaster.hpp>

TimeMaster TimeMaster::timeMaster;

// Konstruktor
TimeMaster::TimeMaster()
    : timeClient(ntpUDP, "pool.ntp.org", 3600), updateInterval(60000) {
}


void TimeMaster::init() {
    Serial.println("DeviceProvider init ...");
    timeMaster.timeClient.begin();
    timeMaster.timeClient.setTimeOffset(3600); // Ustawienie strefy czasowej na GMT+1

    // Próba synchronizacji z NTP
    if (timeMaster.timeClient.update()) {
        // Udało się pobrać nowy czas z NTP
        timeMaster.lastNTPTime = timeMaster.timeClient.getEpochTime();
        timeMaster.lastMillis = millis();
        timeMaster.ntpAvailable = true;
        Serial.println("Synchronized with NTP!");

        // Wyślij czas do DataContainer
        timeMaster.sendTimeToDataContainer();
    } else {
        // NTP nie działa - przechodzimy na freerunning
        timeMaster.ntpAvailable = false;
        Serial.println("NTP unavailable, using freerunning mode.");
    }

    Serial.println("... done");
}

void TimeMaster::deinit() {
    
}


void TimeMaster::cyclic() {
    unsigned long now = millis();

    // Co 60 sekund próbujemy zsynchronizować się z NTP
    if (now - timeMaster.lastUpdateTime >= timeMaster.updateInterval) {
        if (timeMaster.timeClient.update()) {
            // Udało się pobrać nowy czas z NTP
            timeMaster.lastNTPTime = timeMaster.timeClient.getEpochTime();
            timeMaster.lastMillis = millis();
            timeMaster.ntpAvailable = true;  // Ustaw flagę na true, bo NTP działa
            timeMaster.updateInterval = 60000; // Przywróć interwał do 60 sekund
            //Serial.println("Synchronized with NTP!");
        } else {
            // NTP nie działa - przechodzimy na freerunning
            if (timeMaster.ntpAvailable) {
                // Tylko jeśli wcześniej NTP było dostępne, wyświetl komunikat
                timeMaster.ntpAvailable = false;
                timeMaster.updateInterval = 10000; // Skróć interwał do 10 sekund
                timeMaster.lastUpdateTime = now;   // Zresetuj czas ostatniej aktualizacji
                Serial.println("NTP unavailable, using freerunning mode.");
            } else {
                // NTP nadal niedostępne - wyświetl komunikat debugowy
                Serial.println("NTP still unavailable, retrying in 10 seconds...");
            }
        }

        // Korekta czasu w trybie freerunning
        if (!timeMaster.ntpAvailable) {
            unsigned long elapsed = millis() - timeMaster.lastMillis;
            timeMaster.lastNTPTime += elapsed / 1000; // Dodaj upływający czas w sekundach
            timeMaster.lastMillis = millis();
        }

        timeMaster.sendTimeToDataContainer(); // Aktualizacja DataContainer
        timeMaster.lastUpdateTime = now;
    }
}



// Pobranie sformatowanego czasu (HH:MM:SS)
String TimeMaster::getFormattedTime() {
    return timeMaster.timeClient.getFormattedTime();
}

// Pobranie sformatowanej daty i czasu (yyyy.mm.dd hh:mm:ss)
String TimeMaster::getFormattedDateTime() {
    time_t rawTime = timeMaster.timeClient.getEpochTime(); // Pobranie czasu Unix
    struct tm *timeInfo = gmtime(&rawTime);    // Konwersja na czas UTC

    char buffer[20]; 
    snprintf(buffer, sizeof(buffer), "%04d.%02d.%02d %02d:%02d:%02d",
             timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, timeInfo->tm_mday,
             timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);

    return String(buffer);
}

// Pobranie czasu w formacie Unix timestamp
unsigned long TimeMaster::getEpochTime() {
    if (timeMaster.ntpAvailable) {
        return timeMaster.timeClient.getEpochTime(); // Jeśli NTP działa, zwracamy aktualny czas
    } else {
        // Jeśli brak NTP, dodajemy różnicę czasu od ostatniej synchronizacji
        return timeMaster.lastNTPTime + (millis() - timeMaster.lastMillis) / 1000;
    }
}

// Ustawienie przesunięcia strefy czasowej (w godzinach)
void TimeMaster::setTimeZone(int timeZoneOffset) {
    timeMaster.timeClient.setTimeOffset(timeZoneOffset * 3600);
}



void TimeMaster::sendTimeToDataContainer() {
    time_t rawTime = getEpochTime(); // Pobranie czasu z NTP lub freerunning
    struct tm *timeInfo = gmtime(&rawTime);

    DataAndTime currentTime;
    currentTime.year = timeInfo->tm_year + 1900;
    currentTime.month = timeInfo->tm_mon + 1;
    currentTime.day = timeInfo->tm_mday;
    currentTime.hour = timeInfo->tm_hour;
    currentTime.minute = timeInfo->tm_min;
    currentTime.second = timeInfo->tm_sec;

    // Serial.printf("Time sent to DataContainer: %04d.%02d.%02d %02d:%02d:%02d\n",
    //               currentTime.year, currentTime.month, currentTime.day,
    //               currentTime.hour, currentTime.minute, currentTime.second);

    DataContainer::setSignalValue(SIG_CURRENT_TIME, currentTime); // Przekazanie czasu do DataContainer
}