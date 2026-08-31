#include "SystemDefinition.hpp"
#include "os/datacontainer/datacontainertypes.hpp"
#include "os/datacontainer/DataContainer.hpp"

#ifndef RFManager_H
#define RFManager_H

#include <Arduino.h>
#include <set>
/**
 * @struct RFButtonInfo
 * @brief Describes a learned RF button code and its protocol parameters.
 */

struct RFButtonInfo
{
    /**
     * Unique value generated from the button transmission.
     */
    uint32_t buttonUniqueNumber{0};

    /**
     * Number of bits used by the telegram.
     */
    uint8_t bits{0};

    /**
     * RF protocol identifier associated with the button.
     */
    uint8_t protocol{0};

    /**
     * Orders button codes by their unique number.
     * @param other Other button descriptor.
     * @return true when this item should sort before the other item.
     */
    bool operator<(const RFButtonInfo& other) const {
        return buttonUniqueNumber < other.buttonUniqueNumber;
    }

    /**
     * Compares two RF button definitions for equality.
     * @param other Other button descriptor.
     * @return true when all signature fields match.
     */
    bool operator==(const RFButtonInfo& other) const {
        return (buttonUniqueNumber == other.buttonUniqueNumber) && (bits == other.bits) && (protocol == other.protocol);
    }
};

enum RFManagerState{
    e_RF_BUTTON_LEARNING,
    e_RF_IDLE
};
/**
 * @struct RFReceptionEvent
 * @brief Captures a complete RF event window from start to end.
 */

struct RFReceptionEvent{
    /**
     * Button identifier that triggered the event.
     */
    uint32_t buttonUniqueNumber{0};

    /**
     * Time when the RF pulse sequence began.
     */
    long startTime{0};

    /**
     * Time when the RF pulse sequence completed.
     */
    long endTime{0};
};

#define MAX_RF_BUTTONS_STORED 15
#define RF_BUTTONS_CLICK_WHILE_LEARNING 3
#define TIMEOUT_RF_BUTTON_LEARNING_MS 30000
#define TIME_BETWEEN_EVENTS_MS 2000
#define TIME_FOR_LONG_PRESS_MS 600
#define TIME_TO_COMPLETE_EVENT 200
/**
 * @class RFManager
 * @brief Learns and recognizes RF event codes used by the Home Control Station.
 *
 * The manager stores known buttons, captures incoming transmissions, and evaluates timing to decide
 * whether the sequence corresponds to a learned button press or a new learning event.
 */

class RFManager
{
    /**
     * Current internal learning state of the RF manager.
     */
    static RFManagerState internalState;

    /**
     * Timestamp indicating when the learning phase started.
     */
    static unsigned long learningStateStartTime;

    /**
     * Timestamp of the last completed RF event.
     */
    static unsigned long lastEventCompletionTime;

    /**
     * Set of RF button signatures learned and kept in the station memory.
     */
    static std::set<RFButtonInfo> connectedButtons;

    /**
     * Current reception event used while decoding a new RF signal.
     */
    static RFReceptionEvent currentReceptionEvent;

    /**
     * Restores learned RF buttons from the persistent NVM payload.
     * @param nvmPtr Buffer containing the stored button data.
     * @param nvmSize Number of bytes in the buffer.
     */
    static void restoreConnectedButtons(uint8_t* nvmPtr, uint16_t nvmSize);

    /**
     * Saves the learned RF buttons into the persistent NVM payload.
     * @param nvmPtr Output buffer receiving the stored data.
     * @param nvmSize Maximum size available in the output buffer.
     */
    static void saveConnectedButtons(uint8_t* nvmPtr, uint16_t nvmSize);

    /**
     * Reads and decodes the next incoming RF signal into a button descriptor.
     * @return Learned button definition extracted from the incoming signal.
     */
    static RFButtonInfo receiveButtonSignal();

    /**
     * Handles the RF learning state and waits for a complete button pattern.
     */
    static void handleLearningState();

    /**
     * Handles the active idle state while waiting for new RF transmissions.
     */
    static void handleIdleState();

    /**
     * Resets the current RF event window before starting a new capture.
     */
    static void resetReceptionEvent();

    /**
     * Evaluates the decoded event and decides whether it is a learned action or a new capture.
     */
    static void evaluateButtonEvent();

public:
    /**
     * Initializes the RF receiver and restores persisted button configuration.
     */
    static void init();

    /**
     * Runs the periodic RF event loop and evaluates incoming transmissions.
     */
    static void cyclic();

    /**
     * Releases runtime state and stops the RF manager.
     */
    static void deinit();

    /**
     * Begins the learning sequence for a new RF button.
     */
    static void startLearning();

    /**
     * Clears all learned RF button definitions from the manager.
     */
    static void resetConnectedButtons();

    /**
     * Removes the learned descriptor for the supplied button identifier.
     * @param buttonUniqueNumber Unique number of the button to remove.
     */
    static void deleteConnectedButton(uint32_t buttonUniqueNumber);
};

#endif RFManager_H