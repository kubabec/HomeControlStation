#include <os/app/ErrorMonitor.hpp>

std::array<SystemErrorType, ERR_MONT_ERROR_COUNT> ErrorMonitor::errorList;

void ErrorMonitor::deinit() {
    
}

void ErrorMonitor::init()
{
    Serial.println("ErrorMonitor init ...");
    DataContainer::setSignalValue(CBK_ERROR_REPORT,
        "ErrorMonitor",
        static_cast<std::function<void(ERR_MON_ERROR_TYPE errorCode, uint16_t extendedData)>>(ErrorMonitor::errorReport));

    DataContainer::setSignalValue(CBK_ERROR_CLEAR,
        "ErrorMonitor",
        static_cast<std::function<void(ERR_MON_ERROR_TYPE errorCode)>>(ErrorMonitor::errorClear));

    Serial.println("... done");
}

void ErrorMonitor::cyclic()
{
    
}


void ErrorMonitor::errorReport(ERR_MON_ERROR_TYPE errorCode, uint16_t extendedData)
{
    errorCode = (ERR_MON_ERROR_TYPE)(errorCode - 1);
    Serial.println("Reported error: " + String((int)errorCode) + " with extended data " + String((int)extendedData));
    if(errorCode < ERR_MON_LAST_ERROR){
        SystemErrorType& errorRef = errorList.at(errorCode);
        errorRef.occurrenceCount ++;
        errorRef.extendedData = extendedData;
        errorRef.lastOccurrenceTime = millis();

    }else {
        errorReport(ERR_MON_INVALID_ERROR_REPORTED, errorCode);
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
        errorList.at(errorCode).extendedData = 0;
        errorList.at(errorCode).lastOccurrenceTime = 0;

        updateSystemErrorSignal();
    }
}

void ErrorMonitor::updateSystemErrorSignal()
{
    DataContainer::setSignalValue(SIG_SYSTEM_ERROR_LIST,
        "ErrorMonitor",
         errorList
    );
}