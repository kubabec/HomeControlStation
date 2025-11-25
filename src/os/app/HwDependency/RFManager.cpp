#include "os/HwDependency/RFManager.hpp"
#include <functional>
#include <RCSwitch.h>

#define NVM_DATA_VALID_FLAG 179

RFManagerState RFManager::internalState{e_RF_IDLE};
std::set<RFButtonInfo> RFManager::connectedButtons;
RFReceptionEvent RFManager::currentReceptionEvent;
unsigned long RFManager::learningStateStartTime{0};
unsigned long RFManager::lastEventCompletionTime{0};

RCSwitch mySwitch = RCSwitch();
const uint8_t RX_PIN = 4;

void RFManager::init()
{
    RFButtonsControlAPI api;
    api.startLearningMode = RFManager::startLearning;
    api.resetConnectedButtons = RFManager::resetConnectedButtons;
    api.deleteConnectedButton = RFManager::deleteConnectedButton;

    DataContainer::setSignalValue(
        SIG_RF_BUTTONS_CTRL_API,
        static_cast<RFButtonsControlAPI>(api));

#ifdef HAS_RF_RECEIVER

    /* Read NVM data for HttpServer application */
    uint16_t sizeOfNvm = (e_BLOCK_RFMANAGER_2 - e_BLOCK_RFMANAGER_1 + 1) * PERSISTENT_DATABLOCK_SIZE;
    /* Allocate memory for NVM data */
    uint8_t *nvmData = (uint8_t *)malloc(sizeOfNvm);
    uint8_t offset = 0;
    for (uint8_t blockID = e_BLOCK_RFMANAGER_1; blockID <= e_BLOCK_RFMANAGER_2; blockID++)
    {
        /* call GET_NVM_DATABLOCK for current datablock to read NVM data */
        std::any_cast<std::function<bool(PersistentDatablockID, uint8_t *)>>(
            DataContainer::getSignalValue(CBK_GET_NVM_DATABLOCK))(
            (PersistentDatablockID)blockID, // Datablock ID
            (uint8_t *)&nvmData[offset]     // local memory buffer for datablock data
        );

        /* Shift the offset, that next datablock will be written next to previous in 'nvmData' */
        offset += PERSISTENT_DATABLOCK_SIZE;
    }

    restoreConnectedButtons(nvmData, sizeOfNvm);

    /* release heap buffer */
    free(nvmData);

    mySwitch.enableReceive(digitalPinToInterrupt(RX_PIN));

    startLearning();
#endif
}

void RFManager::handleLearningState()
{
    UserInterfaceNotification notif;
    if ((millis() - learningStateStartTime) > TIMEOUT_RF_BUTTON_LEARNING_MS)
    {
        internalState = e_RF_IDLE;
        lastEventCompletionTime = millis();
        notif.title = "RF Button learning expired";
        notif.body = "Learning mode for RF buttons has expired due to timeout, no buttons were learned.";
        notif.type = UserInterfaceNotification::WARNING;
        std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);
    }

    RFButtonInfo receivedButton = receiveButtonSignal();
    if (receivedButton.buttonUniqueNumber != 0)
    {
        Logger::log("RFManager:// Received button signal during LEARNING: " + String((unsigned long)receivedButton.buttonUniqueNumber));

        if (connectedButtons.find(receivedButton) == connectedButtons.end())
        {
            if (connectedButtons.size() < MAX_RF_BUTTONS_STORED)
            {
                connectedButtons.insert(receivedButton);
                Logger::log("RFManager:// New RF button learned and stored. Total buttons stored: " + String((int)connectedButtons.size()));
                notif.title = "RF Button learned";
                notif.body = "New RF button (" + String((uint64_t)receivedButton.buttonUniqueNumber) + ") has been successfully learned and stored.";
                notif.type = UserInterfaceNotification::INFO;
                std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);

                internalState = e_RF_IDLE;
                lastEventCompletionTime = millis();
            }
            else
            {
                Logger::log("RFManager:// Cannot store new RF button, memory full.");
            }
        }
        else
        {
            Logger::log("RFManager:// Button already known, not storing, exiting learning mode.");
            notif.title = "RF Button already known";
            notif.body = "This button is already known to the system, exiting learning mode.";
            notif.type = UserInterfaceNotification::WARNING;
            std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);

            internalState = e_RF_IDLE;
            lastEventCompletionTime = millis();
        }
    }
}

void RFManager::evaluateButtonEvent()
{
    uint64_t systemDigitalEventId = currentReceptionEvent.buttonUniqueNumber;

    // Check if button was clicked or long pressed
    if ((currentReceptionEvent.endTime - currentReceptionEvent.startTime) < 250)
    {
        // Single click event
        systemDigitalEventId += 1; // offset for single click
        Logger::log("RFManager:// Single click event detected for button: " + String((unsigned long)currentReceptionEvent.buttonUniqueNumber));
    }
    else
    {
        // Long press event
        systemDigitalEventId += 2; // offset for long press
        Logger::log("RFManager:// Long press event detected for button: " + String((unsigned long)currentReceptionEvent.buttonUniqueNumber));
    }

    std::any_cast<std::function<void(uint64_t)>>(DataContainer::getSignalValue(CBK_FIRE_DIGITAL_EVENT))(systemDigitalEventId);
}

void RFManager::handleIdleState()
{
    static bool lastReceptionState = false;
    static uint8_t completionCounter = 0;

    if ((millis() - lastEventCompletionTime) < TIME_BETWEEN_EVENTS_MS)
    {
        // Prevent handling new events too quickly after last one
        return;
    }

    RFButtonInfo receivedButton = receiveButtonSignal();
    if (receivedButton.buttonUniqueNumber != 0)
    {

        // Is one of known buttons?
        if (connectedButtons.find(receivedButton) != connectedButtons.end())
        {
            // First click detected
            if (currentReceptionEvent.buttonUniqueNumber == 0)
            {
                currentReceptionEvent.buttonUniqueNumber = receivedButton.buttonUniqueNumber;
                currentReceptionEvent.startTime = millis();
            }
            else if (currentReceptionEvent.buttonUniqueNumber != receivedButton.buttonUniqueNumber)
            {
                // Different button received, reset current event
                resetReceptionEvent();
                currentReceptionEvent.buttonUniqueNumber = receivedButton.buttonUniqueNumber;
                currentReceptionEvent.startTime = millis();
            }

            lastReceptionState = true;
        }
    }
    else
    {

        // Signal stopped, increase amount of clicks if within time window
        if (currentReceptionEvent.buttonUniqueNumber != 0)
        {
            if (lastReceptionState)
            {
                currentReceptionEvent.endTime = millis();
                lastReceptionState = false;
            }
            else
            {
                completionCounter++;
            }

            if (completionCounter >= 5)
            {
                // calculate duration of the press
                long pressDuration = currentReceptionEvent.endTime - currentReceptionEvent.startTime;
                Logger::log("RFManager:// Button event ended. Duration: " + String((long)pressDuration) + " ms");

                uint64_t systemDigitalEventId = currentReceptionEvent.buttonUniqueNumber;
                if (pressDuration < TIME_FOR_LONG_PRESS_MS)
                {
                    // Short press, wait for possible next press
                    systemDigitalEventId += 1;
                }
                else
                {
                    // Long press, evaluate immediately
                    systemDigitalEventId += 2;
                }

                resetReceptionEvent();
                completionCounter = 0;
                lastEventCompletionTime = millis();
                std::any_cast<std::function<void(uint64_t)>>(DataContainer::getSignalValue(CBK_FIRE_DIGITAL_EVENT))(systemDigitalEventId);
            }
        }
    }
}

void RFManager::cyclic()
{
#ifdef HAS_RF_RECEIVER
    switch (internalState)
    {
    case e_RF_BUTTON_LEARNING:
        handleLearningState();
        break;
    case e_RF_IDLE:
        handleIdleState();
        break;
    }
#endif
}

void RFManager::deinit()
{
#ifdef HAS_RF_RECEIVER
    uint16_t sizeOfNvm = (e_BLOCK_RFMANAGER_2 - e_BLOCK_RFMANAGER_1 + 1) * PERSISTENT_DATABLOCK_SIZE;
    /* Allocate memory for NVM data */
    uint8_t *nvmData = (uint8_t *)malloc(sizeOfNvm);

    if (nvmData == nullptr)
    {
        Logger::log("RFManager:// Unable to allocate memory for NVM data saving!");
        return;
    }

    saveConnectedButtons(nvmData, sizeOfNvm);

    uint8_t offset = 0;
    for (uint8_t blockID = e_BLOCK_RFMANAGER_1; blockID <= e_BLOCK_RFMANAGER_2; blockID++)
    {
        /* call GET_NVM_DATABLOCK for current datablock to read NVM data */
        std::any_cast<std::function<bool(PersistentDatablockID, uint8_t *)>>(
            DataContainer::getSignalValue(CBK_SET_NVM_DATABLOCK))(
            (PersistentDatablockID)blockID, // Datablock ID
            (uint8_t *)&nvmData[offset]     // local memory buffer for datablock data
        );

        /* Shift the offset, that next datablock will be written next to previous in 'nvmData' */
        offset += PERSISTENT_DATABLOCK_SIZE;
    }

    /* release heap buffer */
    free(nvmData);
#endif
}

void RFManager::restoreConnectedButtons(uint8_t *nvmPtr, uint16_t nvmSize)
{
    // Were NVM data saved with any content?
    if (nvmPtr[0] == NVM_DATA_VALID_FLAG)
    {
        uint8_t howManyButtonsSaved = nvmPtr[1];
        for (int i = 0; i < howManyButtonsSaved; i++)
        {
            RFButtonInfo button;
            memcpy(&button, &nvmPtr[2 + (i * sizeof(RFButtonInfo))], sizeof(RFButtonInfo));

            connectedButtons.insert(button);
        }
    }
}

void RFManager::saveConnectedButtons(uint8_t *nvmPtr, uint16_t nvmSize)
{
    if (connectedButtons.size() > 0)
    {
        uint8_t howManyButtonsSaved = connectedButtons.size();

        nvmPtr[0] = NVM_DATA_VALID_FLAG;
        nvmPtr[1] = howManyButtonsSaved;
        int i = 0;
        for (RFButtonInfo button : connectedButtons)
        {
            memcpy(&nvmPtr[2 + (i * sizeof(RFButtonInfo))], &button, sizeof(RFButtonInfo));
            i++;
        }
    }
}

void RFManager::startLearning()
{
    if (connectedButtons.size() >= MAX_RF_BUTTONS_STORED)
    {
        Logger::log("RFManager:// Cannot start learning mode, memory full.");
        UserInterfaceNotification notif;
        notif.title = "RF buttons memory full";
        notif.body = "You cannot connect any more RF buttons, memory is full.";
        notif.type = UserInterfaceNotification::ERROR;
        std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);
        return;
    }

    UserInterfaceNotification notif;
    notif.title = "Listening for RF buttons";
    notif.body = "New buttons can now be learned, press a button on the remote.";
    notif.type = UserInterfaceNotification::INFO;
    std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);
    internalState = e_RF_BUTTON_LEARNING;
    learningStateStartTime = millis();
}

RFButtonInfo RFManager::receiveButtonSignal()
{
    RFButtonInfo receivedButton;
    if (mySwitch.available())
    {
        receivedButton.buttonUniqueNumber = mySwitch.getReceivedValue();
        receivedButton.bits = mySwitch.getReceivedBitlength();
        receivedButton.protocol = mySwitch.getReceivedProtocol();

        mySwitch.resetAvailable();
    }

    return receivedButton;
}

void RFManager::resetReceptionEvent()
{
    currentReceptionEvent.buttonUniqueNumber = 0;
    currentReceptionEvent.startTime = 0;
    currentReceptionEvent.endTime = 0;
}

void RFManager::resetConnectedButtons()
{
    connectedButtons.clear();
}

void RFManager::deleteConnectedButton(uint32_t buttonUniqueNumber)
{
    RFButtonInfo buttonToDelete;
    buttonToDelete.buttonUniqueNumber = buttonUniqueNumber;

    auto it = connectedButtons.find(buttonToDelete);
    if (it != connectedButtons.end())
    {
        connectedButtons.erase(it);
        Logger::log("RFManager:// Deleted RF button: " + String((unsigned long)buttonUniqueNumber));
    }
    else
    {
        Logger::log("RFManager:// RF button to delete not found: " + String((unsigned long)buttonUniqueNumber));
    }
}