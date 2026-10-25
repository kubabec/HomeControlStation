#pragma once

#include <algorithm>
#include <cstring>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <ctime>

#if defined(HCS_TARGET_DISPLAY_S3_4INCH)

#include <lvgl.h>
#include "devices/device.hpp"

// Declare fonts from lv_font_montserrat_*.c files
LV_FONT_DECLARE(lv_font_montserrat_20);
LV_FONT_DECLARE(lv_font_montserrat_32);

struct DisplayDeviceView {
    uint8_t deviceType = 255;
    uint64_t macAddress = 0;
    uint8_t deviceId = 0;
    uint8_t roomId = 0;
    uint8_t isEnabled = 0;
    bool isLocal = false;  // hosted by this display terminal (controlled without the master)
    uint8_t customBytes[NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION] = {};
    std::string deviceName;
    

    bool supportOverlay() const{
        switch(deviceType) {
            case type_ONOFFDEVICE:
            case type_LED_STRIP:
            case type_SEGMENTED_LED_STRIP:
                return true;
            default:
                return false;
        }
    }
};

class HomeControlApp {
public:
    using DeviceList = std::vector<DisplayDeviceView>;
    using DeviceActionCallback = std::function<DeviceList(const DeviceList& current, const DisplayDeviceView& changedDevice)>;

    void setCallbacks(DeviceActionCallback toggleCb, DeviceActionCallback longPressCb) {
        toggleCb_ = std::move(toggleCb);
        longPressCb_ = std::move(longPressCb);
    }

    // Initialize UI without creating display (assumes LVGL is already initialized)
    void init();

    void reloadDevices(const DeviceList& newDevices);

    // Shows/hides the top-bar warning about lost communication with the master
    void setMasterConnected(bool connected);

    void setRoomNames(const std::vector<std::pair<uint8_t, std::string>>& names);

    // Process any pending device updates (called from timer)
    void processPendingUpdates();

    // Standby mode management
    void enterStandbyMode();
    void exitStandbyMode();
    void updateStandbyTime();
    void resetInactivityTimer();
    uint32_t getInactivityTime() const;

    // Clock management - the platform (TimeMaster/NTP) owns the reference time
    void updateClock();
    void syncClockWithPlatform();

    // Room states (roomId, 0 = off, 1 = on, 2 = not togglable) published by the master
    void setRoomToggleStates(const std::vector<std::pair<uint8_t, uint8_t>>& states);

private:
    struct TileCtx {
        HomeControlApp* app = nullptr;
        DisplayDeviceView dev;
    };

    #endif

    struct RoomCtx {
        HomeControlApp* app = nullptr;
        uint8_t roomId = 0;
    };

    struct SlotCtx {
        HomeControlApp* app = nullptr;
        uint8_t deviceId = 0;
        uint8_t slot = 0;
        bool isLocal = false;
        bool segmentOn = false;
    };

    /** Rolling readings collected from the device list updates of a temperature sensor. */
    struct TempStats {
        float minTemp = 0.0f;
        float maxTemp = 0.0f;
        double tempSum = 0.0;
        uint8_t minHumidity = 0;
        uint8_t maxHumidity = 0;
        uint32_t samples = 0;
    };

    void buildUi();
    void rebuildContent();
    void createTile(lv_obj_t* parent, const DisplayDeviceView& dev, lv_coord_t height);
    TileCtx* makeTileCtx(const DisplayDeviceView& dev);
    
    void handleAction(const DisplayDeviceView& dev, lv_event_code_t code);
    void toggleAllDevicesInRoom(uint8_t roomId);
    bool isAnyDeviceEnabledInRoom(uint8_t roomId) const;
    
    // Room navigation
    void navigateToRoom(int delta);  // delta: -1 (previous), +1 (next)
    void updateRoomDisplay();
    void rebuildRoomsList();
    
    // Deferred update handling - safe from event callbacks
    void queueDeviceUpdate(const DeviceList& newDevices);
    
    void showOverlayWithLoading(const DisplayDeviceView& dev, const char* reason);
    void showOverlay(const DisplayDeviceView& dev, const char* reason);
    void hideOverlay();

    // Advanced controls rendered inside the device overlay
    void buildAdvancedControls(const DisplayDeviceView& dev);
    void buildBrightnessControl(const DisplayDeviceView& dev);
    void buildLedSlotControls(const DisplayDeviceView& dev);
    void buildSegmentControls(const DisplayDeviceView& dev);
    void buildTemperatureStatistics(const DisplayDeviceView& dev);
    lv_obj_t* createReadout(lv_obj_t* parent, const char* label, const char* value, uint32_t valueColor);
    void recordTemperatureSamples(const DeviceList& newDevices);

    // Event callbacks (static)
    static void tileEvent(lv_event_t* e);
    static void toggleRoomEvent(lv_event_t* e);
    static void overlayBackdropEvent(lv_event_t* e);
    static void closeOverlayEvent(lv_event_t* e);
    static void hideLoadingTimer(lv_timer_t* t);
    static void processPendingUpdatesTimer(lv_timer_t* t);
    static void clearChildren(lv_obj_t* obj);
    static void navButtonEvent(lv_event_t* e);
    static void standbyScreenTouchEvent(lv_event_t* e);
    static void updateClockTimer(lv_timer_t* t);
    static void syncClockTimer(lv_timer_t* t);
    static void roomToggleEvent(lv_event_t* e);
    static void brightnessSliderEvent(lv_event_t* e);
    static void ledSlotEvent(lv_event_t* e);
    static void segmentEvent(lv_event_t* e);

    // Helper functions
    static std::string roomLabel(uint8_t roomId, const std::unordered_map<uint8_t, std::string>& roomNames);
    static std::string deviceLabel(const DisplayDeviceView& d);
    static std::string deviceDetails(const DisplayDeviceView& device);
    static bool supportsToggle(uint8_t deviceType);

    // Callbacks
    DeviceActionCallback toggleCb_;
    DeviceActionCallback longPressCb_;

    // Data
    DeviceList devices_;
    std::unordered_map<uint8_t, std::string> roomNames_;

    // UI Objects
    lv_obj_t* screen_ = nullptr;
    lv_obj_t* appBar_ = nullptr;
    lv_obj_t* masterOfflineBanner_ = nullptr;
    lv_obj_t* roomHeaderLabel_ = nullptr;  // Room name in header
    lv_obj_t* titleUnderline_ = nullptr;
    lv_obj_t* contentContainer_ = nullptr;  // Main container with nav buttons
    lv_obj_t* leftNavButton_ = nullptr;
    lv_obj_t* content_ = nullptr;  // Content area for devices (grid with vertical scroll)
    lv_obj_t* rightNavButton_ = nullptr;
    lv_obj_t* footer_ = nullptr;  // Footer with HomeControlStation logo

    lv_obj_t* overlay_ = nullptr;
    lv_obj_t* overlayCard_ = nullptr;
    lv_obj_t* overlayTitle_ = nullptr;
    lv_obj_t* overlayInfo_ = nullptr;
    lv_obj_t* overlayControls_ = nullptr;
    uint8_t overlayDeviceId_ = 0;
    bool overlayDeviceLocal_ = false;
    bool overlayDeviceSegmented_ = false;
    lv_obj_t* brightnessValueLabel_ = nullptr;
    lv_obj_t* overlayLoading_ = nullptr;
    lv_obj_t* overlayLoadingSpinner_ = nullptr;
    lv_obj_t* overlayLoadingLabel_ = nullptr;
    lv_timer_t* loadingTimer_ = nullptr;

    // Clock - free running locally, re-anchored to the platform RTC every 10 minutes
    lv_obj_t* clockLabel_ = nullptr;
    uint32_t secondsOfDay_ = 12 * 3600;
    bool clockSynchronized_ = false;
    uint32_t lastClockTickMs_ = 0;
    lv_timer_t* clockUpdateTimer_ = nullptr;
    lv_timer_t* clockSyncTimer_ = nullptr;
    static const uint32_t CLOCK_SYNC_PERIOD_MS = 10 * 60 * 1000;

    // Room toggle button in the app bar (master driven)
    lv_obj_t* roomToggleButton_ = nullptr;
    lv_obj_t* roomToggleThumb_ = nullptr;
    std::unordered_map<uint8_t, uint8_t> roomToggleStates_;
    bool masterConnected_ = true;
    void updateRoomToggleButton();

    // Deferred update handling
    DeviceList pendingDevices_;
    bool hasPendingUpdate_ = false;
    lv_timer_t* updateTimer_ = nullptr;

    // Context management
    std::vector<std::unique_ptr<TileCtx>> tileContexts_;
    std::vector<std::unique_ptr<RoomCtx>> roomContexts_;
    std::vector<std::unique_ptr<SlotCtx>> slotContexts_;
    std::unordered_map<uint16_t, TempStats> temperatureStats_;

    // Room navigation
    std::vector<uint8_t> orderedRooms_;  // Sorted list of room IDs
    size_t currentRoomIndex_ = 0;

    // Standby mode
    bool inStandbyMode_ = false;
    lv_obj_t* standbyScreen_ = nullptr;
    lv_obj_t* standbyTimeLabel_ = nullptr;
    uint32_t lastActivityTime_ = 0;
    static const uint32_t INACTIVITY_TIMEOUT_MS = 30000; // 30 seconds
};
