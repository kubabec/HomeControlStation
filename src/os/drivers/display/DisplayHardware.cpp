#if defined(HCS_TARGET_DISPLAY_S3_4INCH)

#include "os/drivers/display/DisplayHardware.hpp"

#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <TAMC_GT911.h>
#include <esp_heap_caps.h>
#include <lvgl.h>
#include <utility>

#include "display/HomeControlScreen.h"
#include "os/drivers/display/Guition_ESP32_4848S040.h"

namespace
{
Arduino_DataBus *serialBus = new Arduino_SWSPI(
    GFX_NOT_DEFINED, TFT_CS, TFT_SCK, TFT_SDA, GFX_NOT_DEFINED);

Arduino_ESP32RGBPanel *rgbPanel = new Arduino_ESP32RGBPanel(
    TFT_DE, TFT_VS, TFT_HS, TFT_PCLK,
    TFT_R0_DB13, TFT_R1_DB14, TFT_R2_DB15, TFT_R3_DB16, TFT_R4_DB17,
    TFT_G0_DB6, TFT_G1_DB7, TFT_G2_DB8, TFT_G3_DB9, TFT_G4_DB10, TFT_G5_DB11,
    TFT_B0_DB1, TFT_B1_DB2, TFT_B2_DB3, TFT_B3_DB4, TFT_B4_DB5,
    TFT_HSYNC_POLARITY, TFT_HSYNC_FRONT_PORCH, TFT_HSYNC_PULSE_WIDTH,
    TFT_HSYNC_BACK_PORCH, TFT_VSYNC_POLARITY, TFT_VSYNC_FRONT_PORCH,
    TFT_VSYNC_PULSE_WIDTH, TFT_VSYNC_BACK_PORCH, TFT_PCLK_ACTIVE_NEG,
    TFT_DATA_SPEED, TFT_USE_BIG_ENDIAN);

Arduino_RGB_Display *panel = new Arduino_RGB_Display(
    TFT_WIDTH, TFT_HEIGHT, rgbPanel, ROTATION, TFT_AUTO_FLUSH, serialBus,
    GFX_NOT_DEFINED, st7701_type9_init_operations,
    sizeof(st7701_type9_init_operations));

TAMC_GT911 touch(TP_SDA, TP_SCL, TP_INT, TP_RST, TFT_WIDTH, TFT_HEIGHT);
lv_disp_draw_buf_t drawBuffer;
lv_color_t *frameBuffer = nullptr;
lv_disp_drv_t displayDriver;
bool ready = false;
std::function<void()> touchCallback;

void flush(lv_disp_drv_t *display, const lv_area_t *area, lv_color_t *colors)
{
    const uint32_t width = area->x2 - area->x1 + 1;
    const uint32_t height = area->y2 - area->y1 + 1;
    panel->draw16bitRGBBitmap(area->x1, area->y1, &colors->full, width, height);
    panel->flush();
    lv_disp_flush_ready(display);
}

void readTouch(lv_indev_drv_t *, lv_indev_data_t *data)
{
    touch.read();
    if (touch.isTouched)
    {
        if (touchCallback)
        {
            touchCallback();
        }
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = touch.points[0].x;
        data->point.y = touch.points[0].y;
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}
}

void setBacklightBrightness(uint8_t brightness)
{
    digitalWrite(TFT_BL, brightness == 0 ? LOW : HIGH);
}

bool DisplayHardware::init()
{
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    panel->begin();
    panel->fillScreen(0);
    panel->flush();
    *serialBus = Arduino_SWSPI(
        GFX_NOT_DEFINED, GFX_NOT_DEFINED, GFX_NOT_DEFINED,
        GFX_NOT_DEFINED, GFX_NOT_DEFINED);

    touch.begin();
    touch.setRotation(TAMC_GT911_ROTATION);
    touch.setResolution(TFT_WIDTH, TFT_HEIGHT);

    lv_init();
    frameBuffer = static_cast<lv_color_t *>(heap_caps_malloc(
        sizeof(lv_color_t) * TFT_WIDTH * TFT_HEIGHT,
        MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
    if (frameBuffer == nullptr)
    {
        Serial.println("DisplayHardware: unable to allocate LVGL frame buffer");
        return false;
    }

    lv_disp_draw_buf_init(&drawBuffer, frameBuffer, nullptr, TFT_WIDTH * TFT_HEIGHT);
    lv_disp_drv_init(&displayDriver);
    displayDriver.hor_res = TFT_WIDTH;
    displayDriver.ver_res = TFT_HEIGHT;
    displayDriver.flush_cb = flush;
    displayDriver.draw_buf = &drawBuffer;
    displayDriver.sw_rotate = 1;
    displayDriver.rotated = LV_DISP_ROT_270;
    lv_disp_drv_register(&displayDriver);

    static lv_indev_drv_t inputDriver;
    lv_indev_drv_init(&inputDriver);
    inputDriver.type = LV_INDEV_TYPE_POINTER;
    inputDriver.read_cb = readTouch;
    lv_indev_drv_register(&inputDriver);

    ready = true;
    return true;
}

void DisplayHardware::cyclic()
{
    if (ready)
    {
        lv_timer_handler();
    }
}

void DisplayHardware::deinit()
{
    ready = false;
    if (frameBuffer != nullptr)
    {
        heap_caps_free(frameBuffer);
        frameBuffer = nullptr;
    }
    digitalWrite(TFT_BL, LOW);
}

bool DisplayHardware::isReady()
{
    return ready;
}

void DisplayHardware::setTouchCallback(std::function<void()> callback)
{
    touchCallback = std::move(callback);
}

#endif
