#include <os/app/ErrorMonitor.hpp>

std::array<SystemErrorType, ERR_MONT_ERROR_COUNT> ErrorMonitor::errorList;

/*
typedef enum{
    ERR_MON_UNEXPECTED_RESET = 1,
    ERR_MON_INVALID_NVM_DATA,
    ERR_MON_INVALID_LOCAL_CONFIG,
    ERR_MON_WRONG_CONFIG_STRING_RECEIVED,
    ERR_MON_WRONG_LOCAL_DEVICES_CONFIG_RECEIVED,
    ERR_MON_WRONG_DEVICE_ID_FOR_LOCAL_SERVICE_REQUEST,
    ERR_MON_INVALID_ERROR_REPORTED,
    ERR_MON_LAST_ERROR = ERR_MON_INVALID_ERROR_REPORTED,
    ERR_MONT_ERROR_COUNT = ERR_MON_LAST_ERROR
}ERR_MON_ERROR_TYPE;

*/

String codeToTextMapping[ERR_MONT_ERROR_COUNT]
{
    "Unexpected reset detected",
    "Invalid NVM data detected",
    "Wrong node configuration",
    "Error during configuration creation",
    "Wrong configuration setup created",
    "Service for invalid device requested",
    "Wrong ERROR reported"
};


void ErrorMonitor::deinit() {
    
}

void ErrorMonitor::init()
{
    Serial.println("ErrorMonitor init ...");
    DataContainer::setSignalValue(CBK_ERROR_REPORT,
        static_cast<std::function<void(ERR_MON_ERROR_TYPE, String)>>(ErrorMonitor::errorReport));

    DataContainer::setSignalValue(CBK_ERROR_CLEAR,
        static_cast<std::function<void(ERR_MON_ERROR_TYPE)>>(ErrorMonitor::errorClear));

    Serial.println("... done");
}

void ErrorMonitor::cyclic()
{
    
}



void ErrorMonitor::errorReport(ERR_MON_ERROR_TYPE errorCode, String comment)
{
    errorCode = (ERR_MON_ERROR_TYPE)(errorCode - 1);
    Serial.println("Reported error: " + String((int)errorCode) + " with comment: " + comment);
    if(errorCode < ERR_MON_LAST_ERROR){
        SystemErrorType& errorRef = errorList.at(errorCode);
        errorRef.occurrenceCount ++;

        /* Need to have fixed length comment stored in NVM */
        String commentWithConstLength = comment.length() <= 32 ? comment : comment.substring(0, 31);
        errorRef.comment = commentWithConstLength;

        // Pobierz aktualny czas z DataContainer
        std::any timeValue = DataContainer::getSignalValue(SIG_CURRENT_TIME);
        if (timeValue.has_value()) {
            DataAndTime currentTime = std::any_cast<DataAndTime>(timeValue);
            errorRef.timeOfOccurrence = formatTimeToString(currentTime); // Konwersja na String
        } else {
            errorRef.timeOfOccurrence = "Unknown time"; // Jeśli czas nie jest dostępny
        }

        /* Push error notification */
        UserInterfaceNotification notif{
            .title = codeToTextMapping[errorCode],
            .body = comment,
            .type = UserInterfaceNotification::ERROR
        };

        /* special handling */
        if((errorCode+1) == ERR_MON_UNEXPECTED_RESET)
        {
            notif.body = "Try to avoid unexpected power supply disconnection";
        }

        std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);

        errorRef.lastOccurrenceTime = millis();

    }else {
        errorReport(ERR_MON_INVALID_ERROR_REPORTED, String("Invalid error reported with ID: " + String((int)errorCode)));
    }
    updateSystemErrorSignal();
}



void ErrorMonitor::errorClear(ERR_MON_ERROR_TYPE errorCode)
{
    Serial.println("Clearing error : " + String((int)errorCode));
    errorCode = (ERR_MON_ERROR_TYPE)(errorCode - 1);
    if(errorCode < ERR_MON_LAST_ERROR){
        SystemErrorType& errorRef = errorList.at(errorCode);
        errorList.at(errorCode).occurrenceCount = 0;
        errorList.at(errorCode).comment = "none";
        errorList.at(errorCode).lastOccurrenceTime = 0;

        updateSystemErrorSignal();
    }
}

void ErrorMonitor::updateSystemErrorSignal()
{
    DataContainer::setSignalValue(SIG_SYSTEM_ERROR_LIST,
         errorList
    );
}


String ErrorMonitor::formatTimeToString(const DataAndTime& time) {
    char buffer[20]; // Bufor na sformatowany czas
    snprintf(buffer, sizeof(buffer), "%04d.%02d.%02d %02d:%02d:%02d",
             time.year, time.month, time.day,
             time.hour, time.minute, time.second);
    return String(buffer);
}