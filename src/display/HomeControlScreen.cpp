#if defined(HCS_TARGET_DISPLAY_S3_4INCH)

#include "display/HomeControlScreen.h"
#include <lvgl.h>
#include <cstring>
#include <cstdio>
#include <set>
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "generated/GeneratedDeviceTypes.hpp"
#include "os/datacontainer/DataContainer.hpp"
#include "os/app/display/DisplayClient.hpp"

extern "C" const lv_img_dsc_t logo_hcs_footer;

namespace
{
    float readFloat(const uint8_t *bytes, size_t offset)
    {
        float value = 0.0f;
        memcpy(&value, bytes + offset, sizeof(value));
        return value;
    }

    // Palette mirrors the "control-surface" theme served by the built-in HTTP server.
    constexpr uint32_t kSurface0 = 0x0A111A;
    constexpr uint32_t kSurface1 = 0x101B29;
    constexpr uint32_t kSurface2 = 0x16263A;
    constexpr uint32_t kSurface3 = 0x213650;
    constexpr uint32_t kSteel = 0x3C5878;
    constexpr uint32_t kSteelSoft = 0x243A55;
    constexpr uint32_t kAccent = 0x3B7FF0;
    constexpr uint32_t kAccentStrong = 0x8DCBFF;
    constexpr uint32_t kTextStrong = 0xF2F7FF;
    constexpr uint32_t kTextNormal = 0xC9D6E6;
    constexpr uint32_t kTextMuted = 0x9DACBF;
    constexpr uint32_t kSuccess = 0x72D8B1;
    constexpr uint32_t kSuccessDeep = 0x1F5CA8;
    constexpr uint32_t kDanger = 0xFF8A8F;

#define FONT_TITLE (&lv_font_montserrat_20)
#define FONT_BODY (&lv_font_montserrat_14)

    constexpr lv_coord_t kAppBarHeight = 52;
    constexpr lv_coord_t kFooterHeight = 40;
    constexpr lv_coord_t kNavButtonWidth = 34;
    constexpr lv_coord_t kRoomCardPad = 8;
    constexpr lv_coord_t kGridGap = 6;
    constexpr lv_coord_t kToggleRowHeight = 40;
    constexpr uint8_t kTilesPerRow = 3;

    constexpr unsigned long clockStartupSyncTimeout = 10000; // 10 sec
    bool startupClockSynced = false;

    uint32_t freeze_until = 0;

    // Height left inside the room card for the device grid once the room switch is placed.
    constexpr lv_coord_t kGridAreaHeight =
        480 - kAppBarHeight - kFooterHeight - (2 * kRoomCardPad) - kToggleRowHeight - kGridGap;

    lv_coord_t tileHeightForCount(size_t deviceCount)
    {
        const size_t rows = (deviceCount + kTilesPerRow - 1) / kTilesPerRow;
        if (rows == 0)
        {
            return 0;
        }
        const lv_coord_t usable = kGridAreaHeight - static_cast<lv_coord_t>((rows - 1) * kGridGap);
        lv_coord_t height = usable / static_cast<lv_coord_t>(rows);
        if (height > 150)
            height = 150;
        if (height < 76)
            height = 76;
        return height;
    }
}

// Helper function implementations
std::string HomeControlApp::roomLabel(uint8_t roomId, const std::unordered_map<uint8_t, std::string> &roomNames)
{
    const auto it = roomNames.find(roomId);
    if (it != roomNames.end() && !it->second.empty())
    {
        std::string label = it->second;
        std::transform(label.begin(), label.end(), label.begin(), ::toupper);
        return label;
    }
    return "ROOM " + std::to_string(roomId);
}

std::string HomeControlApp::deviceLabel(const DisplayDeviceView &d)
{
    return d.deviceName.empty() ? ("Device " + std::to_string(d.deviceId)) : d.deviceName;
}

bool HomeControlApp::supportsToggle(uint8_t deviceType)
{
    return deviceType == type_ONOFFDEVICE || deviceType == type_LED_STRIP ||
           deviceType == type_GARDEN_IRRIGATION;
}

std::string HomeControlApp::deviceDetails(const DisplayDeviceView &device)
{
    char values[160] = {};
    switch (device.deviceType)
    {
    case type_TEMP_SENSOR:
        if (device.customBytes[0] != 0)
            return "Sensor status: error";
        snprintf(values, sizeof(values), "Temperature: %.1f C\nHumidity: %u%%",
                 readFloat(device.customBytes, 3), device.customBytes[2]);
        return values;
    case type_ONOFFDEVICE:
        snprintf(values, sizeof(values), "Power: %s\nBrightness: %u%%",
                 device.isEnabled ? "ON" : "OFF", device.customBytes[2]);
        return values;
    case type_LED_STRIP:
        snprintf(values, sizeof(values), "Power: %s\nColor: #%02X%02X%02X\nAnimation: %s",
                 device.isEnabled ? "ON" : "OFF", device.customBytes[2], device.customBytes[3],
                 device.customBytes[4], device.customBytes[15] ? "running" : "stopped");
        return values;
    case type_WINDOW_DOOR_SENSOR:
        return device.customBytes[0] ? "Contact: CLOSED" : "Contact: OPEN";
    case type_GARDEN_IRRIGATION:
        if (device.customBytes[0] != 0)
            return "Sensor status: error";
        snprintf(values, sizeof(values),
                 "Temperature: %.1f C\nHumidity: %.1f%%\nNozzle 1: %s\nNozzle 2: %s",
                 readFloat(device.customBytes, 1), readFloat(device.customBytes, 5),
                 device.customBytes[9] ? "ON" : "OFF", device.customBytes[10] ? "ON" : "OFF");
        return values;
    default:
        snprintf(values, sizeof(values), "Type: %s\nState: %s",
                 GeneratedDeviceTypes::nameOf(device.deviceType), device.isEnabled ? "ON" : "OFF");
        return values;
    }
}

void HomeControlApp::init()
{
    // LVGL and display are already initialized in main.cpp
    // Just build the UI on the default screen
    buildUi();

    // Initialize inactivity timer
    lastActivityTime_ = millis();

    // Initialize clock timer - update every 1 second
    // Local clock ticks every second and is re-anchored to the platform RTC every 10 minutes
    clockUpdateTimer_ = lv_timer_create(&HomeControlApp::updateClockTimer, 1000, this);
    clockSyncTimer_ = lv_timer_create(&HomeControlApp::syncClockTimer, CLOCK_SYNC_PERIOD_MS, this);
    // syncClockWithPlatform();

    reloadDevices(devices_);
}

void HomeControlApp::buildUi()
{
    screen_ = lv_scr_act();
    lv_obj_clean(screen_);

    lv_obj_set_style_bg_color(screen_, lv_color_hex(kSurface0), 0);
    lv_obj_set_style_bg_opa(screen_, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(screen_, 0, 0);
    lv_obj_set_style_pad_all(screen_, 0, 0);
    lv_obj_set_style_pad_gap(screen_, 0, 0);
    lv_obj_set_flex_flow(screen_, LV_FLEX_FLOW_COLUMN);

    // Room header with name and icon
    appBar_ = lv_obj_create(screen_);
    lv_obj_set_width(appBar_, lv_pct(100));
    lv_obj_set_height(appBar_, kAppBarHeight);

    lv_obj_set_style_bg_color(appBar_, lv_color_hex(kSurface1), 0);
    lv_obj_set_style_bg_opa(appBar_, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(appBar_, 2, 0);
    lv_obj_set_style_border_side(appBar_, LV_BORDER_SIDE_BOTTOM, 0);
    lv_obj_set_style_border_color(appBar_, lv_color_hex(kAccent), 0);
    lv_obj_set_style_border_opa(appBar_, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(appBar_, 0, 0);
    lv_obj_set_style_pad_hor(appBar_, 10, 0);
    lv_obj_set_style_pad_ver(appBar_, 2, 0);
    lv_obj_set_style_shadow_width(appBar_, 0, 0);
    lv_obj_set_flex_flow(appBar_, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(appBar_, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // Room toggle - left side, state is owned by the master (DisplayServer)
    roomToggleButton_ = lv_btn_create(appBar_);
    lv_obj_set_size(roomToggleButton_, 64, 32);
    lv_obj_clear_flag(roomToggleButton_, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(roomToggleButton_, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_border_width(roomToggleButton_, 1, 0);
    lv_obj_set_style_border_opa(roomToggleButton_, LV_OPA_COVER, 0);
    lv_obj_set_style_shadow_width(roomToggleButton_, 0, 0);
    lv_obj_set_style_pad_all(roomToggleButton_, 0, 0);
    lv_obj_add_event_cb(roomToggleButton_, &HomeControlApp::roomToggleEvent, LV_EVENT_CLICKED, this);

    roomToggleThumb_ = lv_obj_create(roomToggleButton_);
    lv_obj_set_size(roomToggleThumb_, 24, 24);
    lv_obj_clear_flag(roomToggleThumb_, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(roomToggleThumb_, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(roomToggleThumb_, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_border_width(roomToggleThumb_, 0, 0);
    lv_obj_set_style_bg_opa(roomToggleThumb_, LV_OPA_COVER, 0);
    updateRoomToggleButton();

    // Room name header - centered
    roomHeaderLabel_ = lv_label_create(appBar_);
    lv_label_set_text(roomHeaderLabel_, "ROOM 0");
    lv_obj_set_flex_grow(roomHeaderLabel_, 1);
    lv_label_set_long_mode(roomHeaderLabel_, LV_LABEL_LONG_DOT);
    lv_obj_set_style_text_align(roomHeaderLabel_, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(roomHeaderLabel_, lv_color_hex(kTextStrong), 0);
    lv_obj_set_style_text_font(roomHeaderLabel_, FONT_TITLE, 0);

    // Clock - read only, driven by the platform time service
    clockLabel_ = lv_label_create(appBar_);
    lv_label_set_text(clockLabel_, "--:--");
    lv_obj_set_width(clockLabel_, 92);
    lv_obj_set_style_text_align(clockLabel_, LV_TEXT_ALIGN_RIGHT, 0);
    lv_obj_set_style_text_color(clockLabel_, lv_color_hex(kAccentStrong), 0);
    lv_obj_set_style_text_font(clockLabel_, FONT_TITLE, 0);

    // The banner covers the top bar only and does not intercept input.
    masterOfflineBanner_ = lv_obj_create(lv_layer_top());
    lv_obj_set_size(masterOfflineBanner_, lv_pct(100), kAppBarHeight);
    lv_obj_align(masterOfflineBanner_, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_clear_flag(masterOfflineBanner_, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(masterOfflineBanner_, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(masterOfflineBanner_, lv_color_hex(kSurface1), 0);
    lv_obj_set_style_bg_opa(masterOfflineBanner_, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(masterOfflineBanner_, 0, 0);
    lv_obj_set_style_radius(masterOfflineBanner_, 0, 0);
    lv_obj_set_style_pad_all(masterOfflineBanner_, 2, 0);
    lv_obj_t *bannerLabel = lv_label_create(masterOfflineBanner_);
    lv_label_set_text(bannerLabel, "No connection to HCS Master");
    lv_obj_set_style_text_align(bannerLabel, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(bannerLabel, lv_color_hex(0xFF3B30), 0);
    lv_obj_center(bannerLabel);
    lv_obj_add_flag(masterOfflineBanner_, LV_OBJ_FLAG_HIDDEN);

    // Main content container with navigation buttons
    contentContainer_ = lv_obj_create(screen_);
    lv_obj_set_width(contentContainer_, lv_pct(100));
    lv_obj_set_flex_grow(contentContainer_, 1);
    lv_obj_set_style_bg_opa(contentContainer_, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(contentContainer_, 0, 0);
    lv_obj_set_style_radius(contentContainer_, 0, 0);
    lv_obj_set_style_pad_all(contentContainer_, 0, 0);
    lv_obj_set_style_pad_gap(contentContainer_, 3, 0);
    lv_obj_clear_flag(contentContainer_, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_flex_flow(contentContainer_, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(contentContainer_, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // Left navigation button
    leftNavButton_ = lv_btn_create(contentContainer_);
    lv_obj_set_width(leftNavButton_, kNavButtonWidth);
    lv_obj_set_height(leftNavButton_, lv_pct(24));
    lv_obj_set_style_radius(leftNavButton_, 6, 0);
    lv_obj_set_style_bg_color(leftNavButton_, lv_color_hex(kSteelSoft), 0);
    lv_obj_set_style_bg_color(leftNavButton_, lv_color_hex(kSteel), LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(leftNavButton_, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(leftNavButton_, 1, 0);
    lv_obj_set_style_border_color(leftNavButton_, lv_color_hex(kSteel), 0);
    lv_obj_set_style_border_opa(leftNavButton_, LV_OPA_COVER, 0);
    lv_obj_set_style_pad_all(leftNavButton_, 0, 0);
    lv_obj_set_style_shadow_width(leftNavButton_, 0, 0);
    lv_obj_add_event_cb(leftNavButton_, &HomeControlApp::navButtonEvent, LV_EVENT_CLICKED, this);

    lv_obj_t *leftArrowLabel = lv_label_create(leftNavButton_);
    lv_label_set_text(leftArrowLabel, LV_SYMBOL_LEFT);
    lv_obj_center(leftArrowLabel);
    lv_obj_set_style_text_color(leftArrowLabel, lv_color_hex(kTextStrong), 0);
    lv_obj_set_style_text_font(leftArrowLabel, FONT_TITLE, 0);

    // Content area - devices sit directly on the common background
    content_ = lv_obj_create(contentContainer_);
    lv_obj_set_height(content_, lv_pct(100));
    lv_obj_set_flex_grow(content_, 1);
    lv_obj_set_style_bg_opa(content_, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(content_, 0, 0);
    lv_obj_set_style_radius(content_, 0, 0);
    lv_obj_set_style_pad_all(content_, kRoomCardPad, 0);
    lv_obj_set_style_pad_gap(content_, kGridGap, 0);
    lv_obj_set_scroll_dir(content_, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(content_, LV_SCROLLBAR_MODE_AUTO);
    lv_obj_set_flex_flow(content_, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(content_, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // Right navigation button
    rightNavButton_ = lv_btn_create(contentContainer_);
    lv_obj_set_width(rightNavButton_, kNavButtonWidth);
    lv_obj_set_height(rightNavButton_, lv_pct(24));
    lv_obj_set_style_radius(rightNavButton_, 6, 0);
    lv_obj_set_style_bg_color(rightNavButton_, lv_color_hex(kSteelSoft), 0);
    lv_obj_set_style_bg_color(rightNavButton_, lv_color_hex(kSteel), LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(rightNavButton_, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(rightNavButton_, 1, 0);
    lv_obj_set_style_border_color(rightNavButton_, lv_color_hex(kSteel), 0);
    lv_obj_set_style_border_opa(rightNavButton_, LV_OPA_COVER, 0);
    lv_obj_set_style_pad_all(rightNavButton_, 0, 0);
    lv_obj_set_style_shadow_width(rightNavButton_, 0, 0);
    lv_obj_add_event_cb(rightNavButton_, &HomeControlApp::navButtonEvent, LV_EVENT_CLICKED, this);

    lv_obj_t *rightArrowLabel = lv_label_create(rightNavButton_);
    lv_label_set_text(rightArrowLabel, LV_SYMBOL_RIGHT);
    lv_obj_center(rightArrowLabel);
    lv_obj_set_style_text_color(rightArrowLabel, lv_color_hex(kTextStrong), 0);
    lv_obj_set_style_text_font(rightArrowLabel, FONT_TITLE, 0);

    // Footer
    footer_ = lv_obj_create(screen_);
    lv_obj_set_width(footer_, lv_pct(100));
    lv_obj_set_height(footer_, kFooterHeight);
    lv_obj_set_style_bg_color(footer_, lv_color_hex(kSurface0), 0);
    lv_obj_set_style_bg_opa(footer_, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(footer_, 1, 0);
    lv_obj_set_style_border_side(footer_, LV_BORDER_SIDE_TOP, 0);
    lv_obj_set_style_border_color(footer_, lv_color_hex(kSteelSoft), 0);
    lv_obj_set_style_border_opa(footer_, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(footer_, 0, 0);
    lv_obj_set_style_pad_all(footer_, 2, 0);
    lv_obj_clear_flag(footer_, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_flex_flow(footer_, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(footer_, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t *footerLogo = lv_img_create(footer_);
    lv_img_set_src(footerLogo, &logo_hcs_footer);

    overlay_ = lv_obj_create(lv_layer_top());
    lv_obj_set_size(overlay_, lv_pct(100), lv_pct(100));
    lv_obj_set_pos(overlay_, 0, 0);
    lv_obj_set_style_bg_color(overlay_, lv_color_hex(0x04080D), 0);
    lv_obj_set_style_bg_opa(overlay_, LV_OPA_80, 0);
    lv_obj_set_style_border_width(overlay_, 0, 0);
    lv_obj_set_style_pad_all(overlay_, 0, 0);
    lv_obj_add_flag(overlay_, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(overlay_, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(overlay_, &HomeControlApp::overlayBackdropEvent, LV_EVENT_CLICKED, this);

    overlayCard_ = lv_obj_create(overlay_);
    lv_obj_set_width(overlayCard_, 380);
    lv_obj_set_height(overlayCard_, LV_SIZE_CONTENT);
    lv_obj_center(overlayCard_);
    lv_obj_set_style_bg_color(overlayCard_, lv_color_hex(kSurface2), 0);
    lv_obj_set_style_bg_opa(overlayCard_, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(overlayCard_, 1, 0);
    lv_obj_set_style_border_color(overlayCard_, lv_color_hex(kSteel), 0);
    lv_obj_set_style_border_opa(overlayCard_, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(overlayCard_, 18, 0);
    lv_obj_set_style_pad_all(overlayCard_, 18, 0);
    lv_obj_set_style_pad_gap(overlayCard_, 12, 0);
    lv_obj_set_style_shadow_width(overlayCard_, 28, 0);
    lv_obj_set_style_shadow_opa(overlayCard_, LV_OPA_30, 0);
    lv_obj_set_style_shadow_ofs_y(overlayCard_, 12, 0);
    lv_obj_set_flex_flow(overlayCard_, LV_FLEX_FLOW_COLUMN);
    lv_obj_clear_flag(overlayCard_, LV_OBJ_FLAG_CLICKABLE);

    overlayTitle_ = lv_label_create(overlayCard_);
    lv_label_set_text(overlayTitle_, "Device Info");
    lv_obj_set_width(overlayTitle_, lv_pct(100));
    lv_obj_set_style_text_align(overlayTitle_, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(overlayTitle_, lv_color_hex(kTextStrong), 0);
    lv_obj_set_style_text_font(overlayTitle_, FONT_TITLE, 0);

    overlayInfo_ = lv_label_create(overlayCard_);
    lv_label_set_text(overlayInfo_, "");
    lv_obj_set_width(overlayInfo_, lv_pct(100));
    lv_label_set_long_mode(overlayInfo_, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_align(overlayInfo_, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(overlayInfo_, lv_color_hex(kTextNormal), 0);
    lv_obj_set_style_text_font(overlayInfo_, FONT_BODY, 0);

    overlayControls_ = lv_obj_create(overlayCard_);
    lv_obj_set_width(overlayControls_, lv_pct(100));
    lv_obj_set_height(overlayControls_, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(overlayControls_, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(overlayControls_, 0, 0);
    lv_obj_set_style_pad_all(overlayControls_, 0, 0);
    lv_obj_set_style_pad_gap(overlayControls_, 10, 0);
    lv_obj_clear_flag(overlayControls_, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_flex_flow(overlayControls_, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(overlayControls_, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    overlayLoading_ = lv_obj_create(overlayCard_);
    lv_obj_set_width(overlayLoading_, lv_pct(100));
    lv_obj_set_height(overlayLoading_, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(overlayLoading_, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(overlayLoading_, 0, 0);
    lv_obj_set_style_pad_all(overlayLoading_, 0, 0);
    lv_obj_set_flex_flow(overlayLoading_, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(overlayLoading_, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    overlayLoadingSpinner_ = lv_spinner_create(overlayLoading_, 1000, 60);
    lv_obj_set_size(overlayLoadingSpinner_, 44, 44);
    lv_obj_set_style_arc_color(overlayLoadingSpinner_, lv_color_hex(kSteelSoft), 0);
    lv_obj_set_style_arc_color(overlayLoadingSpinner_, lv_color_hex(kAccent), LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(overlayLoadingSpinner_, 4, 0);

    overlayLoadingLabel_ = lv_label_create(overlayLoading_);
    lv_label_set_text(overlayLoadingLabel_, "Loading...");
    lv_obj_set_style_text_color(overlayLoadingLabel_, lv_color_hex(kTextNormal), 0);
    lv_obj_set_style_text_font(overlayLoadingLabel_, FONT_BODY, 0);

    lv_obj_add_flag(overlayLoading_, LV_OBJ_FLAG_HIDDEN);

    lv_obj_t *closeOverlayBtn = lv_btn_create(overlayCard_);
    lv_obj_set_width(closeOverlayBtn, lv_pct(100));
    lv_obj_set_height(closeOverlayBtn, 44);
    lv_obj_set_style_radius(closeOverlayBtn, 14, 0);
    lv_obj_set_style_bg_color(closeOverlayBtn, lv_color_hex(kSteelSoft), 0);
    lv_obj_set_style_bg_color(closeOverlayBtn, lv_color_hex(kSteel), LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(closeOverlayBtn, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(closeOverlayBtn, 1, 0);
    lv_obj_set_style_border_color(closeOverlayBtn, lv_color_hex(0x4B6F99), 0);
    lv_obj_set_style_shadow_width(closeOverlayBtn, 0, 0);
    lv_obj_add_event_cb(closeOverlayBtn, &HomeControlApp::closeOverlayEvent, LV_EVENT_CLICKED, this);

    lv_obj_t *closeOverlayLabel = lv_label_create(closeOverlayBtn);
    lv_label_set_text(closeOverlayLabel, "Close");
    lv_obj_center(closeOverlayLabel);
    lv_obj_set_style_text_color(closeOverlayLabel, lv_color_hex(kTextStrong), 0);
    lv_obj_set_style_text_font(closeOverlayLabel, FONT_TITLE, 0);
}

void HomeControlApp::setMasterConnected(bool connected)
{
    masterConnected_ = connected;
    updateRoomToggleButton();
    if (!masterOfflineBanner_)
    {
        return;
    }
    if (connected)
    {
        lv_obj_add_flag(masterOfflineBanner_, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_clear_flag(masterOfflineBanner_, LV_OBJ_FLAG_HIDDEN);
    }
}

void HomeControlApp::setRoomToggleStates(const std::vector<std::pair<uint8_t, uint8_t>> &states)
{
    std::unordered_map<uint8_t, uint8_t> next(states.begin(), states.end());
    if (next == roomToggleStates_)
    {
        return;
    }
    roomToggleStates_ = std::move(next);
    updateRoomToggleButton();
}

void HomeControlApp::roomToggleEvent(lv_event_t *e)
{
    auto *app = static_cast<HomeControlApp *>(lv_event_get_user_data(e));
    if (app == nullptr || app->orderedRooms_.empty() || app->currentRoomIndex_ >= app->orderedRooms_.size())
    {
        return;
    }
    DisplayClient::requestRoomToggle(app->orderedRooms_[app->currentRoomIndex_]);
}

void HomeControlApp::updateRoomToggleButton()
{
    if (!roomToggleButton_ || !roomToggleThumb_)
    {
        return;
    }

    // Unknown rooms, rooms with only non-switchable devices and a lost master leave the switch inactive
    int state = -1;
    if (masterConnected_ && !orderedRooms_.empty() && currentRoomIndex_ < orderedRooms_.size())
    {
        auto it = roomToggleStates_.find(orderedRooms_[currentRoomIndex_]);
        if (it != roomToggleStates_.end() && it->second <= 1)
        {
            state = it->second;
        }
    }
    const bool on = state == 1;

    lv_obj_set_style_bg_opa(roomToggleButton_, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(roomToggleButton_, lv_color_hex(on ? 0x1F5C4A : kSurface2), 0);
    lv_obj_set_style_border_color(roomToggleButton_, lv_color_hex(on ? kSuccess : kSteel), 0);
    lv_obj_set_style_bg_color(roomToggleThumb_, lv_color_hex(on ? 0x6BDFAD : 0x93A4B7), 0);
    lv_obj_align(roomToggleThumb_, on ? LV_ALIGN_RIGHT_MID : LV_ALIGN_LEFT_MID, on ? -3 : 3, 0);

    if (state < 0)
    {
        lv_obj_clear_flag(roomToggleButton_, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_style_opa(roomToggleButton_, LV_OPA_40, 0);
    }
    else
    {
        lv_obj_add_flag(roomToggleButton_, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_style_opa(roomToggleButton_, LV_OPA_COVER, 0);
    }
}

void HomeControlApp::reloadDevices(const DeviceList &newDevices)
{
    recordTemperatureSamples(newDevices);
    if (devices_.size() == newDevices.size())
    {
        bool unchanged = true;
        for (size_t index = 0; index < devices_.size(); ++index)
        {
            const auto &current = devices_[index];
            const auto &incoming = newDevices[index];
            if (current.deviceType != incoming.deviceType ||
                current.macAddress != incoming.macAddress ||
                current.deviceId != incoming.deviceId ||
                current.isLocal != incoming.isLocal ||
                current.roomId != incoming.roomId ||
                current.isEnabled != incoming.isEnabled ||
                current.deviceName != incoming.deviceName ||
                memcmp(current.customBytes, incoming.customBytes, sizeof(current.customBytes)) != 0)
            {
                unchanged = false;
                break;
            }
        }
        if (unchanged)
        {
            return;
        }
    }
    devices_ = newDevices;
    rebuildContent();
}

void HomeControlApp::setRoomNames(const std::vector<std::pair<uint8_t, std::string>> &names)
{
    std::unordered_map<uint8_t, std::string> nextRoomNames;
    for (const auto &pair : names)
    {
        nextRoomNames[pair.first] = pair.second;
    }
    if (nextRoomNames == roomNames_)
    {
        return;
    }
    roomNames_ = std::move(nextRoomNames);
    rebuildContent();
}

void HomeControlApp::rebuildRoomsList()
{
    // Build list of unique room IDs sorted
    std::set<uint8_t> uniqueRooms;
    for (const auto &dev : devices_)
    {
        uniqueRooms.insert(dev.roomId);
    }
    orderedRooms_.clear();
    orderedRooms_.assign(uniqueRooms.begin(), uniqueRooms.end());

    // Reset to first room if current index is out of range
    if (currentRoomIndex_ >= orderedRooms_.size())
    {
        currentRoomIndex_ = 0;
    }
}

void HomeControlApp::rebuildContent()
{
    rebuildRoomsList();
    updateRoomDisplay();
}

void HomeControlApp::updateRoomDisplay()
{
    // Clear only the content area
    clearChildren(content_);
    tileContexts_.clear();
    roomContexts_.clear();

    // If no rooms, show empty state
    if (orderedRooms_.empty())
    {
        lv_label_set_text(roomHeaderLabel_, "NO ROOMS");
        updateRoomToggleButton();
        return;
    }

    // Get current room ID
    uint8_t currentRoomId = orderedRooms_[currentRoomIndex_];

    // Update header with room name
    lv_label_set_text(roomHeaderLabel_, roomLabel(currentRoomId, roomNames_).c_str());
    updateRoomToggleButton();

    // Get devices in current room
    DeviceList roomDevices;
    for (const auto &dev : devices_)
    {
        if (dev.roomId == currentRoomId)
        {
            roomDevices.push_back(dev);
        }
    }

    // Create a container for room header if needed (toggle all button)
    lv_obj_t *roomControlContainer = lv_obj_create(content_);
    lv_obj_set_width(roomControlContainer, lv_pct(100));
    lv_obj_set_height(roomControlContainer, kToggleRowHeight);
    lv_obj_set_style_bg_opa(roomControlContainer, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(roomControlContainer, 0, 0);
    lv_obj_set_style_pad_all(roomControlContainer, 0, 0);
    lv_obj_set_style_pad_gap(roomControlContainer, 0, 0);
    lv_obj_clear_flag(roomControlContainer, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_flex_flow(roomControlContainer, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(roomControlContainer, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // Toggle all button - mirrors the web room switch
    std::unique_ptr<RoomCtx> roomCtx(new RoomCtx());
    roomCtx->app = this;
    roomCtx->roomId = currentRoomId;
    RoomCtx *roomCtxRaw = roomCtx.get();
    roomContexts_.push_back(std::move(roomCtx));

    // lv_obj_t* toggleAllBtn = lv_btn_create(roomControlContainer);
    // lv_obj_set_width(toggleAllBtn, 150);
    // lv_obj_set_height(toggleAllBtn, 34);
    // lv_obj_set_style_radius(toggleAllBtn, 6, 0);

    // bool roomHasEnabledDevices = isAnyDeviceEnabledInRoom(currentRoomId);
    // if (roomHasEnabledDevices) {
    //     lv_obj_set_style_bg_color(toggleAllBtn, lv_color_hex(kSuccessDeep), 0);
    //     lv_obj_set_style_bg_color(toggleAllBtn, lv_color_hex(0x2B7A55), LV_STATE_PRESSED);
    //     lv_obj_set_style_border_color(toggleAllBtn, lv_color_hex(kSuccess), 0);
    // } else {
    //     lv_obj_set_style_bg_color(toggleAllBtn, lv_color_hex(kSurface3), 0);
    //     lv_obj_set_style_bg_color(toggleAllBtn, lv_color_hex(0x37424F), LV_STATE_PRESSED);
    //     lv_obj_set_style_border_color(toggleAllBtn, lv_color_hex(kSteel), 0);
    // }

    // lv_obj_set_style_bg_opa(toggleAllBtn, LV_OPA_COVER, 0);
    // lv_obj_set_style_border_width(toggleAllBtn, 1, 0);
    // lv_obj_set_style_border_opa(toggleAllBtn, LV_OPA_COVER, 0);
    // lv_obj_set_style_shadow_width(toggleAllBtn, 0, 0);
    // lv_obj_set_style_pad_all(toggleAllBtn, 0, 0);
    // lv_obj_add_event_cb(toggleAllBtn, &HomeControlApp::toggleRoomEvent, LV_EVENT_CLICKED, roomCtxRaw);

    // lv_obj_t* toggleAllLabel = lv_label_create(toggleAllBtn);
    // lv_label_set_text(toggleAllLabel, roomHasEnabledDevices ? LV_SYMBOL_POWER "  ALL OFF"
    //                                                         : LV_SYMBOL_POWER "  ALL ON");
    // lv_obj_center(toggleAllLabel);
    // lv_obj_set_style_text_color(toggleAllLabel,
    //                             lv_color_hex(roomHasEnabledDevices ? 0xF3FFF8 : 0xB6C0CD), 0);
    // lv_obj_set_style_text_font(toggleAllLabel, FONT_BODY, 0);

    // Three columns keep up to nine devices on screen without scrolling.
    const lv_coord_t tileHeight = tileHeightForCount(roomDevices.size());
    lv_obj_t *currentRow = nullptr;
    for (size_t i = 0; i < roomDevices.size(); ++i)
    {
        if (i % kTilesPerRow == 0)
        {
            currentRow = lv_obj_create(content_);
            lv_obj_set_width(currentRow, lv_pct(100));
            lv_obj_set_height(currentRow, tileHeight);
            lv_obj_set_style_bg_opa(currentRow, LV_OPA_TRANSP, 0);
            lv_obj_set_style_border_width(currentRow, 0, 0);
            lv_obj_set_style_pad_all(currentRow, 0, 0);
            lv_obj_set_style_pad_gap(currentRow, kGridGap, 0);
            lv_obj_clear_flag(currentRow, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_flex_flow(currentRow, LV_FLEX_FLOW_ROW);
            lv_obj_set_flex_align(currentRow, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        }
        createTile(currentRow, roomDevices[i], tileHeight);
    }
}

void HomeControlApp::createTile(lv_obj_t *parent, const DisplayDeviceView &dev, lv_coord_t height)
{
    const bool toggleable = supportsToggle(dev.deviceType);
    const bool active = toggleable && dev.isEnabled;

    lv_obj_t *tile = lv_btn_create(parent);
    lv_obj_set_width(tile, lv_pct(32));
    lv_obj_set_height(tile, height);
    lv_obj_set_style_radius(tile, 14, 0);
    lv_obj_set_style_border_width(tile, 1, 0);

    if (active)
    {
        lv_obj_set_style_bg_color(tile, lv_color_hex(kSuccessDeep), 0);
        lv_obj_set_style_bg_color(tile, lv_color_hex(0x2B72C8), LV_STATE_PRESSED);
        lv_obj_set_style_bg_grad_color(tile, lv_color_hex(0x143F75), 0);
        lv_obj_set_style_bg_grad_dir(tile, LV_GRAD_DIR_VER, 0);
        lv_obj_set_style_border_color(tile, lv_color_hex(0x6FBAFF), 0);
        lv_obj_set_style_shadow_color(tile, lv_color_hex(0x3B8CFF), 0);
        lv_obj_set_style_shadow_opa(tile, LV_OPA_40, 0);
    }
    else
    {
        lv_obj_set_style_bg_color(tile, lv_color_hex(kSurface2), 0);
        lv_obj_set_style_bg_color(tile, lv_color_hex(kSurface3), LV_STATE_PRESSED);
        lv_obj_set_style_bg_grad_color(tile, lv_color_hex(kSurface1), 0);
        lv_obj_set_style_bg_grad_dir(tile, LV_GRAD_DIR_VER, 0);
        lv_obj_set_style_border_color(tile, lv_color_hex(toggleable ? kSteel : kSteelSoft), 0);
    }

    lv_obj_set_style_bg_opa(tile, LV_OPA_COVER, 0);
    lv_obj_set_style_border_opa(tile, LV_OPA_COVER, 0);
    lv_obj_set_style_shadow_width(tile, 8, 0);
    if (!active)
    {
        lv_obj_set_style_shadow_opa(tile, LV_OPA_20, 0);
        lv_obj_set_style_shadow_color(tile, lv_color_hex(0x000000), 0);
    }
    lv_obj_set_style_shadow_ofs_y(tile, 4, 0);
    lv_obj_set_style_pad_all(tile, 6, 0);
    lv_obj_set_style_pad_gap(tile, 3, 0);

    // Create context once and use for both event callbacks
    TileCtx *ctx = makeTileCtx(dev);
    lv_obj_add_flag(tile, LV_OBJ_FLAG_CLICKABLE);
    if (toggleable)
    {
        lv_obj_add_event_cb(tile, &HomeControlApp::tileEvent, LV_EVENT_CLICKED, ctx);
    }
    lv_obj_add_event_cb(tile, &HomeControlApp::tileEvent, LV_EVENT_LONG_PRESSED, ctx);

    lv_obj_set_flex_flow(tile, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(tile, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    if (toggleable)
    {
        lv_obj_t *statusDot = lv_obj_create(tile);
        lv_obj_add_flag(statusDot, LV_OBJ_FLAG_IGNORE_LAYOUT);
        lv_obj_add_flag(statusDot, LV_OBJ_FLAG_EVENT_BUBBLE);
        lv_obj_clear_flag(statusDot, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(statusDot, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_size(statusDot, 9, 9);
        lv_obj_align(statusDot, LV_ALIGN_TOP_RIGHT, -4, 4);
        lv_obj_set_style_radius(statusDot, LV_RADIUS_CIRCLE, 0);
        lv_obj_set_style_border_width(statusDot, 0, 0);
        lv_obj_set_style_bg_color(statusDot, lv_color_hex(dev.isEnabled ? kSuccess : 0x687382), 0);
        lv_obj_set_style_bg_opa(statusDot, LV_OPA_COVER, 0);
    }

    // Device name
    lv_obj_t *name = lv_label_create(tile);
    lv_label_set_text(name, deviceLabel(dev).c_str());
    lv_obj_add_flag(name, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_set_width(name, lv_pct(100));
    lv_label_set_long_mode(name, LV_LABEL_LONG_DOT);
    lv_obj_set_style_text_align(name, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(name, lv_color_hex(kTextStrong), 0);
    lv_obj_set_style_text_font(name, FONT_BODY, 0);

    // Add device-type-specific content
    switch (dev.deviceType)
    {
    case type_LED_STRIP:
    {
        lv_obj_t *colorBar = lv_obj_create(tile);
        lv_obj_clear_flag(colorBar, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(colorBar, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_flag(colorBar, LV_OBJ_FLAG_EVENT_BUBBLE);
        lv_obj_set_width(colorBar, lv_pct(80));
        lv_obj_set_height(colorBar, 8);
        lv_obj_set_style_radius(colorBar, 4, 0);
        lv_obj_set_style_border_width(colorBar, 0, 0);
        lv_obj_set_style_bg_color(colorBar, lv_color_hex(((uint32_t)dev.customBytes[2] << 16) | ((uint32_t)dev.customBytes[3] << 8) | (uint32_t)dev.customBytes[4]), 0);
        lv_obj_set_style_bg_opa(colorBar, LV_OPA_COVER, 0);
        lv_obj_set_style_shadow_width(colorBar, 0, 0);
        break;
    }
    case type_TEMP_SENSOR:
    {
        if (dev.customBytes[0] != 0)
        {
            lv_obj_t *errorLabel = lv_label_create(tile);
            lv_obj_add_flag(errorLabel, LV_OBJ_FLAG_EVENT_BUBBLE);
            lv_label_set_text(errorLabel, "SENSOR\nERROR");
            lv_obj_set_style_text_align(errorLabel, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_set_style_text_color(errorLabel, lv_color_hex(kDanger), 0);
            lv_obj_set_style_text_font(errorLabel, FONT_BODY, 0);
            break;
        }
        char text[24];
        snprintf(text, sizeof(text), "%.1f°C", readFloat(dev.customBytes, 3));
        lv_obj_t *tempLabel = lv_label_create(tile);
        lv_obj_add_flag(tempLabel, LV_OBJ_FLAG_EVENT_BUBBLE);
        lv_label_set_text(tempLabel, text);
        lv_obj_set_style_text_color(tempLabel, lv_color_hex(kAccentStrong), 0);
        lv_obj_set_style_text_font(tempLabel, FONT_TITLE, 0);

        snprintf(text, sizeof(text), "%u%%", dev.customBytes[2]);
        lv_obj_t *humidityLabel = lv_label_create(tile);
        lv_obj_add_flag(humidityLabel, LV_OBJ_FLAG_EVENT_BUBBLE);
        lv_label_set_text(humidityLabel, text);
        lv_obj_set_style_text_color(humidityLabel, lv_color_hex(kTextMuted), 0);
        lv_obj_set_style_text_font(humidityLabel, FONT_BODY, 0);
        break;
    }
    case type_WINDOW_DOOR_SENSOR:
    {
        lv_obj_t *status = lv_label_create(tile);
        lv_obj_add_flag(status, LV_OBJ_FLAG_EVENT_BUBBLE);
        lv_label_set_text(status, dev.customBytes[0] ? "CLOSED" : "OPEN");
        lv_obj_set_style_text_color(status,
                                    lv_color_hex(dev.customBytes[0] ? kSuccess : kAccentStrong), 0);
        lv_obj_set_style_text_font(status, FONT_BODY, 0);
        break;
    }
    case type_GARDEN_IRRIGATION:
    {
        lv_obj_t *label = lv_label_create(tile);
        lv_obj_add_flag(label, LV_OBJ_FLAG_EVENT_BUBBLE);
        if (dev.customBytes[0])
        {
            lv_label_set_text(label, "SENSOR\nERROR");
            lv_obj_set_style_text_color(label, lv_color_hex(kDanger), 0);
        }
        else
        {
            char readings[32];
            snprintf(readings, sizeof(readings), "%.1f°C\n%.0f%% RH",
                     readFloat(dev.customBytes, 1), readFloat(dev.customBytes, 5));
            lv_label_set_text(label, readings);
            lv_obj_set_style_text_color(label, lv_color_hex(kTextMuted), 0);
        }
        lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_font(label, FONT_BODY, 0);
        break;
    }
    case type_ONOFFDEVICE:
        if (dev.customBytes[0])
        {
            char brightness[16];
            snprintf(brightness, sizeof(brightness), "%u%%", dev.customBytes[2]);
            lv_obj_t *label = lv_label_create(tile);
            lv_obj_add_flag(label, LV_OBJ_FLAG_EVENT_BUBBLE);
            lv_label_set_text(label, brightness);
            lv_obj_set_style_text_color(label, lv_color_hex(kTextMuted), 0);
            lv_obj_set_style_text_font(label, FONT_BODY, 0);
        }
        break;
    default:
        break;
    }
}

HomeControlApp::TileCtx *HomeControlApp::makeTileCtx(const DisplayDeviceView &dev)
{
    std::unique_ptr<TileCtx> ctx(new TileCtx());
    ctx->app = this;
    ctx->dev = dev;
    TileCtx *raw = ctx.get();
    tileContexts_.push_back(std::move(ctx));
    return raw;
}

void HomeControlApp::handleAction(const DisplayDeviceView &dev, lv_event_code_t code)
{
    if (code == LV_EVENT_CLICKED)
    {
        if (toggleCb_)
        {
            DeviceList next = toggleCb_(devices_, dev);
            if (!next.empty())
            {
                queueDeviceUpdate(next);
            }
        }
    }
    else if (code == LV_EVENT_LONG_PRESSED)
    {
        if (dev.supportOverlay())
        {
            showOverlay(dev, "Details");
            if (longPressCb_)
            {
                DeviceList next = longPressCb_(devices_, dev);
                if (!next.empty())
                {
                    queueDeviceUpdate(next);
                }
            }

            freeze_until = lv_tick_get() + 400;
        }
    }
}

void HomeControlApp::toggleAllDevicesInRoom(uint8_t roomId)
{
    std::vector<DisplayDeviceView> next = devices_;
    bool anyEnabled = false;

    for (const auto &d : next)
    {
        if (d.roomId == roomId && d.isEnabled)
        {
            anyEnabled = true;
            break;
        }
    }

    for (auto &d : next)
    {
        if (d.roomId == roomId)
        {
            d.isEnabled = anyEnabled ? 0 : 1;
        }
    }

    queueDeviceUpdate(next);
}

bool HomeControlApp::isAnyDeviceEnabledInRoom(uint8_t roomId) const
{
    for (const auto &d : devices_)
    {
        if (d.roomId == roomId && (d.isEnabled == 1))
        {
            return true;
        }
    }
    return false;
}

void HomeControlApp::queueDeviceUpdate(const DeviceList &newDevices)
{
    // Store the pending devices and mark that we have a pending update
    pendingDevices_ = newDevices;
    hasPendingUpdate_ = true;

    // If there's already a timer running, delete it to reset the timeout
    if (updateTimer_)
    {
        lv_timer_del(updateTimer_);
    }

    // Create a one-shot timer to process the update after the current event is fully handled
    // 5ms delay ensures all event processing is complete before UI rebuild
    updateTimer_ = lv_timer_create(&HomeControlApp::processPendingUpdatesTimer, 5, this);
    lv_timer_set_repeat_count(updateTimer_, 1); // One-shot timer
}

void HomeControlApp::processPendingUpdates()
{
    if (hasPendingUpdate_ && !pendingDevices_.empty())
    {
        // Now it's safe to rebuild the UI - all events from old tiles are processed
        reloadDevices(pendingDevices_);
        hasPendingUpdate_ = false;
        updateTimer_ = nullptr;
    }
}

void HomeControlApp::showOverlayWithLoading(const DisplayDeviceView &dev, const char *reason)
{
    std::string text = "Device: " + deviceLabel(dev) + "\n";
    text += "Room: " + roomLabel(dev.roomId, roomNames_) + "\n";
    text += "State: ";
    text += dev.isEnabled ? "ON" : "OFF";
    text += "\nAction: ";
    text += reason;

    lv_label_set_text(overlayInfo_, text.c_str());
    lv_obj_clear_flag(overlayLoading_, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(overlay_, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(overlay_);

    // Clear any existing timer
    if (loadingTimer_)
    {
        lv_timer_del(loadingTimer_);
        loadingTimer_ = nullptr;
    }
    // No auto-timeout - popup stays until user clicks Close
}

void HomeControlApp::showOverlay(const DisplayDeviceView &dev, const char *reason)
{
    std::string text = "Room: " + roomLabel(dev.roomId, roomNames_) + "\n";
    // text += "Type: ";
    // text += GeneratedDeviceTypes::nameOf(dev.deviceType);
    // text += "  |  State: ";
    // text += dev.isEnabled ? "ON" : "OFF";
    lv_label_set_text(overlayTitle_, deviceLabel(dev).c_str());
    lv_label_set_text(overlayInfo_, text.c_str());
    buildAdvancedControls(dev);
    lv_obj_add_flag(overlayLoading_, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(overlay_, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(overlay_);
}

lv_obj_t *HomeControlApp::createReadout(lv_obj_t *parent, const char *label, const char *value,
                                        uint32_t valueColor)
{
    lv_obj_t *cell = lv_obj_create(parent);
    lv_obj_set_height(cell, LV_SIZE_CONTENT);
    lv_obj_set_flex_grow(cell, 1);
    lv_obj_set_style_bg_color(cell, lv_color_hex(kSurface1), 0);
    lv_obj_set_style_bg_opa(cell, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(cell, 1, 0);
    lv_obj_set_style_border_color(cell, lv_color_hex(kSteelSoft), 0);
    lv_obj_set_style_radius(cell, 10, 0);
    lv_obj_set_style_pad_all(cell, 8, 0);
    lv_obj_set_style_pad_gap(cell, 2, 0);
    lv_obj_clear_flag(cell, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_flex_flow(cell, LV_FLEX_FLOW_COLUMN);

    lv_obj_t *caption = lv_label_create(cell);
    lv_label_set_text(caption, label);
    lv_obj_set_style_text_color(caption, lv_color_hex(kTextMuted), 0);
    lv_obj_set_style_text_font(caption, FONT_BODY, 0);

    lv_obj_t *readout = lv_label_create(cell);
    lv_label_set_text(readout, value);
    lv_obj_set_style_text_color(readout, lv_color_hex(valueColor), 0);
    lv_obj_set_style_text_font(readout, FONT_TITLE, 0);
    return cell;
}

void HomeControlApp::buildAdvancedControls(const DisplayDeviceView &dev)
{
    clearChildren(overlayControls_);
    slotContexts_.clear();
    brightnessValueLabel_ = nullptr;

    switch (dev.deviceType)
    {
    case type_ONOFFDEVICE:
        buildBrightnessControl(dev);
        break;
    case type_LED_STRIP:
        buildLedSlotControls(dev);
        break;
    case type_TEMP_SENSOR:
        // buildTemperatureStatistics(dev);
        break;
    default:
    {
        // lv_obj_t* details = lv_label_create(overlayControls_);
        // lv_label_set_text(details, deviceDetails(dev).c_str());
        // lv_obj_set_width(details, lv_pct(100));
        // lv_label_set_long_mode(details, LV_LABEL_LONG_WRAP);
        // lv_obj_set_style_text_align(details, LV_TEXT_ALIGN_CENTER, 0);
        // lv_obj_set_style_text_color(details, lv_color_hex(kTextNormal), 0);
        // lv_obj_set_style_text_font(details, FONT_BODY, 0);
        break;
    }
    }
}

void HomeControlApp::buildBrightnessControl(const DisplayDeviceView &dev)
{
    // customBytes[0] tells whether the master mapped this output to a dimmable pin.
    if (!dev.customBytes[0])
    {
        lv_obj_t *note = lv_label_create(overlayControls_);
        lv_label_set_text(note, "Brightness control not supported");
        lv_obj_set_style_text_color(note, lv_color_hex(kTextMuted), 0);
        lv_obj_set_style_text_font(note, FONT_BODY, 0);
        return;
    }

    lv_obj_t *headerRow = lv_obj_create(overlayControls_);
    lv_obj_set_width(headerRow, lv_pct(100));
    lv_obj_set_height(headerRow, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(headerRow, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(headerRow, 0, 0);
    lv_obj_set_style_pad_all(headerRow, 0, 0);
    lv_obj_clear_flag(headerRow, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_flex_flow(headerRow, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(headerRow, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t *caption = lv_label_create(headerRow);
    lv_label_set_text(caption, "BRIGHTNESS");
    lv_obj_set_style_text_color(caption, lv_color_hex(kTextMuted), 0);
    lv_obj_set_style_text_font(caption, FONT_BODY, 0);

    char value[8];
    snprintf(value, sizeof(value), "%u%%", dev.customBytes[2]);
    brightnessValueLabel_ = lv_label_create(headerRow);
    lv_label_set_text(brightnessValueLabel_, value);
    lv_obj_set_style_text_color(brightnessValueLabel_, lv_color_hex(kAccentStrong), 0);
    lv_obj_set_style_text_font(brightnessValueLabel_, FONT_TITLE, 0);

    lv_obj_t *slider = lv_slider_create(overlayControls_);
    lv_obj_set_width(slider, lv_pct(100));
    lv_obj_set_height(slider, 14);
    lv_slider_set_range(slider, 0, 100);
    lv_slider_set_value(slider, dev.customBytes[2], LV_ANIM_OFF);
    lv_obj_set_style_bg_color(slider, lv_color_hex(kSteelSoft), LV_PART_MAIN);
    lv_obj_set_style_bg_color(slider, lv_color_hex(kAccent), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(slider, lv_color_hex(0xF2F7FF), LV_PART_KNOB);
    lv_obj_set_style_pad_all(slider, 8, LV_PART_KNOB);

    SlotCtx *ctx = new SlotCtx{this, dev.deviceId, 0, dev.isLocal};
    slotContexts_.emplace_back(ctx);
    lv_obj_add_event_cb(slider, &HomeControlApp::brightnessSliderEvent, LV_EVENT_VALUE_CHANGED, ctx);
    lv_obj_add_event_cb(slider, &HomeControlApp::brightnessSliderEvent, LV_EVENT_RELEASED, ctx);

    lv_obj_t *spacer = lv_obj_create(overlayControls_);

    lv_obj_set_size(spacer, 10, 10);

    lv_obj_set_style_bg_opa(spacer, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(spacer, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(spacer, 0, LV_PART_MAIN);
}

void HomeControlApp::buildLedSlotControls(const DisplayDeviceView &dev)
{
    lv_obj_t *caption = lv_label_create(overlayControls_);
    lv_label_set_text(caption, "COLOR MEMORY");
    lv_obj_set_style_text_color(caption, lv_color_hex(kTextMuted), 0);
    lv_obj_set_style_text_font(caption, FONT_BODY, 0);

    lv_obj_t *row = lv_obj_create(overlayControls_);
    lv_obj_set_width(row, lv_pct(100));
    lv_obj_set_height(row, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(row, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(row, 0, 0);
    lv_obj_set_style_pad_all(row, 0, 0);
    lv_obj_set_style_pad_gap(row, 8, 0);
    lv_obj_clear_flag(row, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // customBytes 5..13 hold the averaged colour of memory slots 1..3.
    for (uint8_t slot = 1; slot <= 3; ++slot)
    {
        const size_t offset = 2 + (slot * 3);
        const uint32_t color = ((uint32_t)dev.customBytes[offset] << 16) |
                               ((uint32_t)dev.customBytes[offset + 1] << 8) |
                               (uint32_t)dev.customBytes[offset + 2];

        lv_obj_t *button = lv_btn_create(row);
        lv_obj_set_flex_grow(button, 1);
        lv_obj_set_height(button, 64);
        lv_obj_set_style_radius(button, 12, 0);
        lv_obj_set_style_bg_color(button, lv_color_hex(kSurface3), 0);
        lv_obj_set_style_bg_color(button, lv_color_hex(kSteel), LV_STATE_PRESSED);
        lv_obj_set_style_bg_opa(button, LV_OPA_COVER, 0);
        lv_obj_set_style_border_width(button, 1, 0);
        lv_obj_set_style_border_color(button, lv_color_hex(kSteel), 0);
        lv_obj_set_style_shadow_width(button, 0, 0);
        lv_obj_set_style_pad_all(button, 6, 0);
        lv_obj_set_style_pad_gap(button, 4, 0);
        lv_obj_set_flex_flow(button, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(button, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

        SlotCtx *ctx = new SlotCtx{this, dev.deviceId, slot, dev.isLocal};
        slotContexts_.emplace_back(ctx);
        lv_obj_add_event_cb(button, &HomeControlApp::ledSlotEvent, LV_EVENT_CLICKED, ctx);

        lv_obj_t *swatch = lv_obj_create(button);
        lv_obj_add_flag(swatch, LV_OBJ_FLAG_EVENT_BUBBLE);
        lv_obj_clear_flag(swatch, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(swatch, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_width(swatch, lv_pct(100));
        lv_obj_set_height(swatch, 26);
        lv_obj_set_style_radius(swatch, 8, 0);
        lv_obj_set_style_bg_color(swatch, lv_color_hex(color), 0);
        lv_obj_set_style_bg_opa(swatch, LV_OPA_COVER, 0);
        lv_obj_set_style_border_width(swatch, 1, 0);
        lv_obj_set_style_border_color(swatch, lv_color_hex(kSteelSoft), 0);

        char text[12];
        snprintf(text, sizeof(text), "SLOT %u", slot);
        lv_obj_t *label = lv_label_create(button);
        lv_obj_add_flag(label, LV_OBJ_FLAG_EVENT_BUBBLE);
        lv_label_set_text(label, text);
        lv_obj_set_style_text_color(label, lv_color_hex(kTextStrong), 0);
        lv_obj_set_style_text_font(label, FONT_BODY, 0);
    }
}

void HomeControlApp::buildTemperatureStatistics(const DisplayDeviceView &dev)
{
    if (dev.customBytes[0] != 0)
    {
        lv_obj_t *note = lv_label_create(overlayControls_);
        lv_label_set_text(note, "Sensor reports an error");
        lv_obj_set_style_text_color(note, lv_color_hex(kDanger), 0);
        lv_obj_set_style_text_font(note, FONT_TITLE, 0);
        return;
    }

    const auto it = temperatureStats_.find(static_cast<uint16_t>((dev.isLocal ? 0x100 : 0) | dev.deviceId));
    if (it == temperatureStats_.end() || it->second.samples == 0)
    {
        lv_obj_t *note = lv_label_create(overlayControls_);
        lv_label_set_text(note, "Collecting statistics...");
        lv_obj_set_style_text_color(note, lv_color_hex(kTextMuted), 0);
        lv_obj_set_style_text_font(note, FONT_BODY, 0);
        return;
    }
    const TempStats &stats = it->second;

    auto makeRow = [this]()
    {
        lv_obj_t *row = lv_obj_create(overlayControls_);
        lv_obj_set_width(row, lv_pct(100));
        lv_obj_set_height(row, LV_SIZE_CONTENT);
        lv_obj_set_style_bg_opa(row, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_width(row, 0, 0);
        lv_obj_set_style_pad_all(row, 0, 0);
        lv_obj_set_style_pad_gap(row, 8, 0);
        lv_obj_clear_flag(row, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
        return row;
    };

    char text[24];
    lv_obj_t *row = makeRow();
    snprintf(text, sizeof(text), "%.1f°C", readFloat(dev.customBytes, 3));
    createReadout(row, "NOW", text, kAccentStrong);
    snprintf(text, sizeof(text), "%.1f°C", static_cast<float>(stats.tempSum / stats.samples));
    createReadout(row, "AVERAGE", text, kTextStrong);

    row = makeRow();
    snprintf(text, sizeof(text), "%.1f°C", stats.minTemp);
    createReadout(row, "MINIMUM", text, kAccent);
    snprintf(text, sizeof(text), "%.1f°C", stats.maxTemp);
    createReadout(row, "MAXIMUM", text, kDanger);

    row = makeRow();
    snprintf(text, sizeof(text), "%u - %u%%", stats.minHumidity, stats.maxHumidity);
    createReadout(row, "HUMIDITY RANGE", text, kTextStrong);
    snprintf(text, sizeof(text), "%lu", static_cast<unsigned long>(stats.samples));
    createReadout(row, "SAMPLES", text, kTextMuted);
}

void HomeControlApp::recordTemperatureSamples(const DeviceList &newDevices)
{
    for (const auto &device : newDevices)
    {
        if (device.deviceType != type_TEMP_SENSOR || device.customBytes[0] != 0)
        {
            continue;
        }
        const float temperature = readFloat(device.customBytes, 3);
        const uint8_t humidity = device.customBytes[2];
        TempStats &stats = temperatureStats_[static_cast<uint16_t>((device.isLocal ? 0x100 : 0) | device.deviceId)];
        if (stats.samples == 0)
        {
            stats.minTemp = stats.maxTemp = temperature;
            stats.minHumidity = stats.maxHumidity = humidity;
        }
        else
        {
            if (temperature < stats.minTemp)
                stats.minTemp = temperature;
            if (temperature > stats.maxTemp)
                stats.maxTemp = temperature;
            if (humidity < stats.minHumidity)
                stats.minHumidity = humidity;
            if (humidity > stats.maxHumidity)
                stats.maxHumidity = humidity;
        }
        stats.tempSum += temperature;
        ++stats.samples;
    }
}

void HomeControlApp::hideOverlay()
{
    // Clean up loading timer if it exists
    if (loadingTimer_)
    {
        lv_timer_del(loadingTimer_);
        loadingTimer_ = nullptr;
    }
    brightnessValueLabel_ = nullptr;
    clearChildren(overlayControls_);
    slotContexts_.clear();
    lv_obj_add_flag(overlay_, LV_OBJ_FLAG_HIDDEN);
}

void HomeControlApp::brightnessSliderEvent(lv_event_t *e)
{
    auto *ctx = static_cast<SlotCtx *>(lv_event_get_user_data(e));
    if (!ctx || !ctx->app)
    {
        return;
    }
    ctx->app->resetInactivityTimer();
    const int32_t value = lv_slider_get_value(lv_event_get_target(e));
    if (ctx->app->brightnessValueLabel_)
    {
        lv_label_set_text_fmt(ctx->app->brightnessValueLabel_, "%d%%", static_cast<int>(value));
    }
    if (lv_event_get_code(e) == LV_EVENT_RELEASED)
    {
        DisplayClient::requestBrightness(ctx->deviceId, static_cast<uint8_t>(value), ctx->isLocal);
    }
}

void HomeControlApp::ledSlotEvent(lv_event_t *e)
{
    auto *ctx = static_cast<SlotCtx *>(lv_event_get_user_data(e));
    if (!ctx || !ctx->app)
    {
        return;
    }
    ctx->app->resetInactivityTimer();
    DisplayClient::requestLedSlot(ctx->deviceId, ctx->slot, ctx->isLocal);
}

// Static callback wrappers
void HomeControlApp::tileEvent(lv_event_t *e)
{
    // exit when events not allowed (exiting standby)
    if (lv_tick_get() < freeze_until)
    {
        return;
    }

    auto *ctx = static_cast<TileCtx *>(lv_event_get_user_data(e));
    if (!ctx || !ctx->app)
    {
        return;
    }
    ctx->app->handleAction(ctx->dev, lv_event_get_code(e));
}

void HomeControlApp::toggleRoomEvent(lv_event_t *e)
{
    auto *ctx = static_cast<RoomCtx *>(lv_event_get_user_data(e));
    if (!ctx || !ctx->app)
    {
        return;
    }
    ctx->app->toggleAllDevicesInRoom(ctx->roomId);
}

void HomeControlApp::overlayBackdropEvent(lv_event_t *e)
{
    auto *app = static_cast<HomeControlApp *>(lv_event_get_user_data(e));
    if (!app)
    {
        return;
    }
    if (lv_event_get_target(e) == lv_event_get_current_target(e))
    {
        app->hideOverlay();
    }
}

void HomeControlApp::closeOverlayEvent(lv_event_t *e)
{
    auto *app = static_cast<HomeControlApp *>(lv_event_get_user_data(e));
    if (!app)
    {
        return;
    }
    app->hideOverlay();
}

void HomeControlApp::navButtonEvent(lv_event_t *e)
{
    auto *app = static_cast<HomeControlApp *>(lv_event_get_user_data(e));
    if (!app)
    {
        return;
    }

    lv_obj_t *btn = lv_event_get_target(e);
    if (btn == app->leftNavButton_)
    {
        app->navigateToRoom(-1); // Previous room
    }
    else if (btn == app->rightNavButton_)
    {
        app->navigateToRoom(1); // Next room
    }
}

void HomeControlApp::hideLoadingTimer(lv_timer_t *t)
{
    void *user_data = t->user_data;
    HomeControlApp *app = static_cast<HomeControlApp *>(user_data);
    if (app && app->overlayLoading_)
    {
        lv_obj_add_flag(app->overlayLoading_, LV_OBJ_FLAG_HIDDEN);
        app->loadingTimer_ = nullptr; // Clear the timer pointer
    }
}

void HomeControlApp::processPendingUpdatesTimer(lv_timer_t *t)
{
    void *user_data = t->user_data;
    HomeControlApp *app = static_cast<HomeControlApp *>(user_data);
    if (app)
    {
        app->processPendingUpdates();
    }
}

void HomeControlApp::clearChildren(lv_obj_t *obj)
{
    if (!obj)
    {
        return;
    }
    while (lv_obj_get_child_cnt(obj) > 0)
    {
        lv_obj_del(lv_obj_get_child(obj, 0));
    }
}

void HomeControlApp::navigateToRoom(int delta)
{
    if (orderedRooms_.empty())
    {
        return;
    }

    // Calculate new index with looping
    int newIndex = static_cast<int>(currentRoomIndex_) + delta;

    // Handle looping (wrap around)
    if (newIndex < 0)
    {
        newIndex = orderedRooms_.size() - 1;
    }
    else if (newIndex >= static_cast<int>(orderedRooms_.size()))
    {
        newIndex = 0;
    }

    currentRoomIndex_ = newIndex;
    updateRoomDisplay();
}

void HomeControlApp::resetInactivityTimer()
{
    lastActivityTime_ = millis();

    // If we're in standby mode, exit it
    if (inStandbyMode_)
    {
        exitStandbyMode();
    }
}

uint32_t HomeControlApp::getInactivityTime() const
{
    uint32_t now = millis();
    uint32_t elapsed = now - lastActivityTime_;
    return elapsed;
}

void HomeControlApp::updateStandbyTime()
{
    if (!startupClockSynced && millis() > clockStartupSyncTimeout)
    {
        syncClockWithPlatform();
        startupClockSynced = true;
    }

    uint32_t inactivityTime = getInactivityTime();

    // Check if we should enter standby mode
    if (!inStandbyMode_ && inactivityTime >= INACTIVITY_TIMEOUT_MS)
    {
        enterStandbyMode();
    }

    // Update standby time label if in standby
    if (inStandbyMode_ && standbyTimeLabel_)
    {
        uint32_t secondsInStandby = inactivityTime / 1000;
        char timeText[64];
        snprintf(timeText, sizeof(timeText), "Screen inactive\nfor %lu seconds\n\nTouch to wake up", secondsInStandby);
        lv_label_set_text(standbyTimeLabel_, timeText);
    }
}

void HomeControlApp::standbyScreenTouchEvent(lv_event_t *e)
{
    auto *app = static_cast<HomeControlApp *>(lv_event_get_user_data(e));
    if (!app)
    {
        return;
    }
    // Any touch on standby screen wakes up the device
    app->exitStandbyMode();
}

void HomeControlApp::enterStandbyMode()
{
    if (inStandbyMode_)
    {
        return; // Already in standby
    }

    inStandbyMode_ = true;

    // Hide the main UI
    if (screen_)
    {
        lv_obj_add_flag(screen_, LV_OBJ_FLAG_HIDDEN);
    }

    // Show standby screen if not already created
    if (!standbyScreen_)
    {
        standbyScreen_ = lv_obj_create(lv_scr_act());
        lv_obj_set_size(standbyScreen_, lv_pct(100), lv_pct(100));
        lv_obj_set_style_bg_color(standbyScreen_, lv_color_hex(kSurface0), 0);
        lv_obj_set_style_bg_opa(standbyScreen_, LV_OPA_COVER, 0);
        lv_obj_set_style_border_width(standbyScreen_, 0, 0);
        lv_obj_set_style_pad_all(standbyScreen_, 0, 0);

        // Make clickable to block touch events from reaching elements below
        lv_obj_add_flag(standbyScreen_, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(standbyScreen_, &HomeControlApp::standbyScreenTouchEvent, LV_EVENT_CLICKED, this);

        standbyTimeLabel_ = lv_label_create(standbyScreen_);
        lv_obj_center(standbyTimeLabel_);
        lv_label_set_text(standbyTimeLabel_, "Screen inactive\nfor 0 seconds\n\nTouch to wake up");
        lv_obj_set_width(standbyTimeLabel_, lv_pct(80));
        lv_label_set_long_mode(standbyTimeLabel_, LV_LABEL_LONG_WRAP);
        lv_obj_set_style_text_align(standbyTimeLabel_, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_color(standbyTimeLabel_, lv_color_hex(kTextMuted), 0);
        lv_obj_set_style_text_font(standbyTimeLabel_, FONT_TITLE, 0);
    }
    else
    {
        lv_obj_clear_flag(standbyScreen_, LV_OBJ_FLAG_HIDDEN);
    }

    // Bring standby screen to front to block all touches
    lv_obj_move_foreground(standbyScreen_);

    // Turn off backlight (0 = off)
    extern void setBacklightBrightness(uint8_t brightness);
    setBacklightBrightness(0);
}

void HomeControlApp::exitStandbyMode()
{
    if (!inStandbyMode_)
    {
        return; // Not in standby
    }

    inStandbyMode_ = false;

    // Turn on backlight (255 = full brightness)
    extern void setBacklightBrightness(uint8_t brightness);
    setBacklightBrightness(255);

    freeze_until = lv_tick_get() + 400; // freeze touch for 200ms

    // Show main UI
    if (screen_)
    {
        lv_obj_clear_flag(screen_, LV_OBJ_FLAG_HIDDEN);
    }

    // Hide standby screen
    if (standbyScreen_)
    {
        lv_obj_add_flag(standbyScreen_, LV_OBJ_FLAG_HIDDEN);
    }
}

void HomeControlApp::syncClockWithPlatform()
{
    std::any callback = DataContainer::getSignalValue(CBK_GET_CURRENT_TIME);
    auto *getTime = std::any_cast<std::function<RtcTime()>>(&callback);
    if (getTime == nullptr || !(*getTime))
    {
        return;
    }
    const RtcTime now = (*getTime)();
    if (now.year < 2020)
    {
        return; // NTP has not delivered a valid timestamp yet
    }
    secondsOfDay_ = static_cast<uint32_t>(now.hour) * 3600u +
                    static_cast<uint32_t>(now.min) * 60u +
                    static_cast<uint32_t>(now.sec);
    lastClockTickMs_ = millis();
    clockSynchronized_ = true;
    updateClock();
}

void HomeControlApp::updateClock()
{
    if (!clockLabel_)
    {
        return;
    }
    if (!clockSynchronized_)
    {
        lv_label_set_text(clockLabel_, "--:--");
        return;
    }

    const uint32_t now = millis();
    const uint32_t elapsedSeconds = (now - lastClockTickMs_) / 1000u;
    if (elapsedSeconds > 0)
    {
        secondsOfDay_ = (secondsOfDay_ + elapsedSeconds) % 86400u;
        lastClockTickMs_ += elapsedSeconds * 1000u;
    }

    char timeText[16];
    snprintf(timeText, sizeof(timeText), "%02u:%02u",
             static_cast<unsigned>(secondsOfDay_ / 3600u),
             static_cast<unsigned>((secondsOfDay_ / 60u) % 60u));
    lv_label_set_text(clockLabel_, timeText);
}

void HomeControlApp::updateClockTimer(lv_timer_t *t)
{
    HomeControlApp *app = static_cast<HomeControlApp *>(t->user_data);
    if (app)
    {
        app->updateClock();
    }
}

void HomeControlApp::syncClockTimer(lv_timer_t *t)
{
    HomeControlApp *app = static_cast<HomeControlApp *>(t->user_data);
    if (app)
    {
        app->syncClockWithPlatform();
    }
}

#endif
