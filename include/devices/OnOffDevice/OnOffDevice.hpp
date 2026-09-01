/**
 * @file OnOffDevice.hpp
 * @brief Driver for basic on/off devices and their brightness controls.
 */
#ifndef ONOFF_DEVICE_H
#define ONOFF_DEVICE_H

#include "Arduino.h"
#include "devices/device.hpp"

/**
 * @class OnOffDevice
 * @brief Represents a relay, LED, or basic switchable output controlled by the station.
 */
class OnOffDevice : public Device
{
private:
    /** Animation and timing configuration for switching operations. */
    AdvancedControlsOnOff controls;

    /** Current logical state of the device. */
    bool isOn = false;
    /** GPIO pin connected to the load control output. */
    int pinNumber;
    /** Unique device ID within the system. */
    uint8_t deviceId;
    /** Friendly name displayed by the UI. */
    String deviceName;
    /** Room identifier for grouping and zoning. */
    uint8_t roomId;
    /** True when the hardware interprets logic low as active. */
    bool activeLow = true;
    /** True when brightness modulation is supported. */
    bool brightnessLevelSupport = true;
    /** Current brightness percentage in the 0-100 range. */
    int brightnessLevel = 0;
    /** Target brightness requested by the user. */
    int brightnessLevelTarget = 0;
    /** Saved brightness used when the device is re-enabled. */
    int brightnessLevelBackupWhenOff = 100;
    /** Duration of brightness-change transitions in milliseconds. */
    const int brightnessChangeTime = 3000;
    /** Automatic timer value used for timed lighting. */
    int lightDurationTimerMS = 8000;

    /** Minimum PWM duty cycle. */
    uint8_t minPwmValue = 0;
    /** Maximum PWM duty cycle. */
    uint8_t maxPwmValue = 255;

    /** Current brightness step-sized delta. */
    unsigned long brightnessStep ;
    /** Duration of each brightness step. */
    unsigned long brightnessStepDurationMS ;
    /** Timestamp of the previous brightness update. */
    unsigned long timePrevious1 ;
    /** Timestamp of the previous timer evaluation. */
    unsigned long timePrevious2 ; 
    
public:
    /**
     * @brief Creates a device backed by a raw GPIO pin.
     * @param pin Hardware pin number.
     * @param devName Display name used by the system.
     * @param a_deviceId Unique runtime identifier.
     * @param a_roomId Zone association.
     */
    OnOffDevice(int pin, String devName, uint8_t a_deviceId, uint8_t a_roomId);
    /**
     * @brief Rebuilds the device from a persisted device description.
     * @param description Persistent metadata snapshot.
     * @param pin GPIO pin number.
     */
    OnOffDevice(DeviceDescription& description, uint8_t pin);
    /**
     * @brief Restores the device from a stored NVM configuration slot.
     * @param nvmData Configuration payload saved in non-volatile memory.
     */
    OnOffDevice(DeviceConfigSlotType nvmData);

    /**
     * @brief Switches the device to the on-state.
     */
    void on();
    /**
     * @brief Switches the device to the off-state.
     */
    void off();
    /**
     * @brief Reads the current logical state of the device.
     * @return True when the device is currently active.
     */
    bool getState();
    
    /**
     * @brief Enables or disables brightness support for the device.
     * @param p_brightnessLevelSupport True when brightness controls should be used.
     */
    void setBrightnessLevelSupport(bool p_brightnessLevelSupport);
    /**
     * @brief Updates the dimming state according to the target brightness level.
     */
    void brightnessChangeHandler();
    /**
     * @brief Reads the current brightness level.
     * @return Brightness value in the system range.
     */
    int getBrightnessLevel();
    /**
     * @brief Returns the current brightness step size.
     * @return Step increment in the configured unit.
     */
    int getBrightnessStep();
    /**
        * @brief Returns the duration calculated during the last brightness request.
        * @return Calculated step duration in milliseconds; uninitialized until `changeBrightness()` runs.
     */
    int getBrightnessStepDuration();
    /**
     * @brief Updates the target brightness value and begins a fade sequence.
     * @param requestedBrightness Desired brightness percentage.
     */
    void changeBrightness(int requestedBrightness);
    /**
     * @brief Reads the desired end brightness target.
     * @return Target brightness value.
     */
    int getBrightnessLevelTarget();
    /**
        * @brief Sets the stored timed-light duration.
        * @param requestedLightDuration Duration in seconds, converted to milliseconds internally.
     */
    void setLightDurationTimerMS(int requestedLightDuration);
    /**
        * @brief Reserved handler for timed deactivation; currently performs no action.
     */
    void timerHandler();
    /**
     * @brief Reads the current timed-light duration.
     * @return Remaining duration in milliseconds.
     */
    int getLightDurationTimerMS();
    /**
     * @brief Returns the device name.
     * @return Human-readable device name.
     */
    String getName();
    /**
     * @brief Returns the device identifier.
     * @return System identifier of the device.
     */
    uint8_t getDeviceId();
    /**
     * @brief Returns the room index assigned to the device.
     * @return Zone identifier.
     */
    uint8_t getRoomId();
    /**
     * @brief Indicates whether brightness adjustment is enabled.
     * @return True when dimming is supported.
     */
    bool getBrightnessIsAdjustable();

    /**
     * @brief Maps a normalized brightness value into the hardware PWM range.
     * @param brightness Brightness percentage or raw value.
     * @return Equivalent value for the target hardware output.
     */
    int mapBrightness(int brightness);

    /**
     * @brief Initializes the output pin and internal device state.
     */
    virtual void init();
    /**
     * @brief Performs the recurring state update for the device.
     */
    virtual void cyclic();
    /**
     * @brief Returns a runtime identifier used by the device registry.
     * @return Device identifier code.
     */
    virtual uint8_t getDeviceIdentifier();
    /**
     * @brief Returns the device type enumeration.
     * @return Device category identifier.
     */
    virtual uint8_t getDeviceType();
    /**
     * @brief Returns the metadata description used by the runtime.
     * @return DeviceDescription payload.
     */
    virtual DeviceDescription getDeviceDescription();
    /**
     * @brief Reports the size of the custom memory block required by the device.
     * @return Width in bytes of the persistent state area.
     */
    virtual uint16_t getExtendedMemoryLength();

    /**
     * @brief Executes a general service request without a payload.
     * @param serviceType Requested command.
     * @return Execution result code.
     */
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType);
    /**
     * @brief Executes a service request with a single numeric payload.
     * @param serviceType Requested command.
     * @param param Parameter payload.
     * @return Execution result code.
     */
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set1 param);
    /**
     * @brief Executes a service request with floating-point payload data.
     * @param serviceType Requested command.
     * @param param Parameter payload.
     * @return Execution result code.
     */
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set2 param);
    /**
     * @brief Executes a service request with raw memory payload data.
     * @param serviceType Requested command.
     * @param param Parameter payload.
     * @return Execution result code.
     */
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set3 param);
};

#endif
