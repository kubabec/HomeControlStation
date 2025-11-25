#include "SystemDefinition.hpp"
#include "os/datacontainer/datacontainertypes.hpp"
#include "os/datacontainer/DataContainer.hpp"

#ifndef RFManager_H
#define RFManager_H

#include <Arduino.h>
#include <set>

struct RFButtonInfo
{
    uint32_t buttonUniqueNumber{0};
    uint8_t bits{0};
    uint8_t protocol{0};

    bool operator<(const RFButtonInfo& other) const {
        return buttonUniqueNumber < other.buttonUniqueNumber;
    }

    bool operator==(const RFButtonInfo& other) const {
        return (buttonUniqueNumber == other.buttonUniqueNumber) && (bits == other.bits) && (protocol == other.protocol);
    }
};

enum RFManagerState{
    e_RF_BUTTON_LEARNING,
    e_RF_IDLE
};

struct RFReceptionEvent{
    uint32_t buttonUniqueNumber{0};
    long startTime{0};
    long endTime{0};
};

#define MAX_RF_BUTTONS_STORED 15
#define RF_BUTTONS_CLICK_WHILE_LEARNING 3
#define TIMEOUT_RF_BUTTON_LEARNING_MS 30000
#define TIME_BETWEEN_EVENTS_MS 2000
#define TIME_FOR_LONG_PRESS_MS 600
#define TIME_TO_COMPLETE_EVENT 200

class RFManager
{
    static RFManagerState internalState;
    static unsigned long learningStateStartTime;
    static unsigned long lastEventCompletionTime;
    static std::set<RFButtonInfo> connectedButtons;
    static RFReceptionEvent currentReceptionEvent;

    static void restoreConnectedButtons(uint8_t* nvmPtr, uint16_t nvmSize);
    static void saveConnectedButtons(uint8_t* nvmPtr, uint16_t nvmSize);

    static RFButtonInfo receiveButtonSignal();

    static void handleLearningState();
    static void handleIdleState();
    static void resetReceptionEvent();
    static void evaluateButtonEvent();

public:
    static void init();
    static void cyclic();
    static void deinit();

    static void startLearning();
    static void resetConnectedButtons();
    static void deleteConnectedButton(uint32_t buttonUniqueNumber);
};

#endif RFManager_H