#include <os/app/timeMaster.hpp>


unsigned long TimeMaster::lastUpdateTime = 0;
unsigned long TimeMaster::lastNTPTime = 0;
unsigned long TimeMaster::lastMillis = 0;
bool TimeMaster::ntpAvailable = false;
unsigned long TimeMaster::updateInterval = 60000;
unsigned long TimeMaster::remainder = 0;

WiFiUDP TimeMaster::ntpUDP;
NTPClient TimeMaster::timeClient(ntpUDP, "pool.ntp.org", 3600);


void TimeMaster::init() {
    Serial.println("DeviceProvider init ...");


    
    TimeMaster::timeClient.begin();
    TimeMaster::timeClient.setTimeOffset(3600); // Ustawienie strefy czasowej na GMT+1
    
    // Próba synchronizacji z NTP
    if (TimeMaster::timeClient.update()) {
        // Udało się pobrać nowy czas z NTP
        TimeMaster::lastNTPTime = TimeMaster::timeClient.getEpochTime();
        TimeMaster::lastMillis = millis();
        TimeMaster::ntpAvailable = true;
        Serial.println("Synchronized with NTP!");
        // Wyślij czas do DataContainer
        TimeMaster::sendTimeToDataContainer();
    } else {
        // NTP nie działa - przechodzimy na freerunning
        TimeMaster::ntpAvailable = false;
        Serial.println("NTP unavailable, using freerunning mode.");
    }

    //Rejestracja callbacka dla CBK_GET_CURRENT_TIME
    // DataContainer::setSignalValue(CBK_GET_CURRENT_TIME, std::any_cast {
    //     return TimeMaster::getFormattedDateTime(); // Zwraca sformatowany czas jako String    
    // });

    DataContainer::setSignalValue(CBK_GET_CURRENT_TIME, static_cast<std::function<String()>>(TimeMaster::getFormattedDateTime));

    

     
    Serial.println("... done");
}

void TimeMaster::deinit() {
    
}

// Funkcja do cyklicznego wywoływania aktualizacji czasu
void TimeMaster::cyclic() {
    unsigned long now = millis();

    if (now - lastUpdateTime >= updateInterval) {
        if (timeClient.update()) {
            // Sukces synchronizacji NTP
            lastNTPTime = timeClient.getEpochTime();
            lastMillis = now;
            remainder = 0; // Reset reszty
            ntpAvailable = true;
            updateInterval = 60000; 
            sendTimeToDataContainer();
        } else {
            // Błąd synchronizacji NTP - tryb freerunning
            if (ntpAvailable) {
                ntpAvailable = false;
                updateInterval = 10000; 
                Serial.println("NTP unavailable, using freerunning.");
            }

            // Oblicz upływający czas z uwzględnieniem reszty
            unsigned long elapsed = now - lastMillis;
            unsigned long totalElapsed = elapsed + remainder;
            unsigned long seconds = totalElapsed / 1000;
            remainder = totalElapsed % 1000; // Zapamiętaj resztę

            // Aktualizuj czas i zapobiegaj przekłamaniu
            lastNTPTime += seconds;
            lastMillis = now - remainder; // Korekta o resztę
        }

        lastUpdateTime = now;
    }
}

// Pobranie sformatowanego czasu (HH:MM:SS)
String TimeMaster::getFormattedTime() {
    return TimeMaster::timeClient.getFormattedTime();
}

// Pobranie sformatowanej daty i czasu (yyyy.mm.dd hh:mm:ss)
String TimeMaster::getFormattedDateTime() {

    time_t rawTime = TimeMaster::getEpochTime(); // Pobranie czasu Unix
    struct tm *timeInfo = localtime(&rawTime);    // Konwersja na czas UTC

    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%04d.%02d.%02d %02d:%02d:%02d",
             timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, timeInfo->tm_mday,
             timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);

    return String(buffer); // Zwracamy Arduino String
   
}


// Ustawienie przesunięcia strefy czasowej (w godzinach)
void TimeMaster::setTimeZone(int timeZoneOffset) {
    TimeMaster::timeClient.setTimeOffset(timeZoneOffset * 3600);
}

unsigned long TimeMaster::getEpochTime() {
    unsigned long elapsedMillis = millis() - lastMillis;
    return lastNTPTime + (elapsedMillis / 1000);
}
 

void TimeMaster::sendTimeToDataContainer() {

    time_t rawTime = getEpochTime(); // Pobranie czasu z NTP lub freerunning
    struct tm *timeInfo = localtime(&rawTime);

    DataAndTime currentTime;
    currentTime.year = timeInfo->tm_year + 1900;
    currentTime.month = timeInfo->tm_mon + 1;
    currentTime.day = timeInfo->tm_mday;
    currentTime.hour = timeInfo->tm_hour;
    currentTime.minute = timeInfo->tm_min;
    currentTime.second = timeInfo->tm_sec;

    Serial.printf("Time sent to DataContainer: %04d.%02d.%02d %02d:%02d:%02d\n",
                  currentTime.year, currentTime.month, currentTime.day,
                  currentTime.hour, currentTime.minute, currentTime.second);

    DataContainer::setSignalValue(SIG_CURRENT_TIME, currentTime); // Przekazanie czasu do DataContainer
}