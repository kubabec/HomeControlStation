#if defined(HCS_TARGET_DISPLAY_S3_4INCH)

#include "os/app/display/DisplayRuntime.hpp"

#include "display/HomeControlScreen.h"
#include "os/app/display/DisplayClient.hpp"
#include "os/drivers/display/DisplayHardware.hpp"

namespace
{
HomeControlApp *application = nullptr;

HomeControlApp::DeviceList keepCurrent(
    const HomeControlApp::DeviceList &current,
    const DisplayDeviceView &device)
{
    DisplayClient::requestToggle(device.deviceId);
    return current;
}

HomeControlApp::DeviceList keepCurrentOnLongPress(
    const HomeControlApp::DeviceList &current,
    const DisplayDeviceView &)
{
    return current;
}
}

bool DisplayRuntime::init()
{
    if (!DisplayHardware::init())
    {
        return false;
    }

    application = new HomeControlApp();
    application->setCallbacks(keepCurrent, keepCurrentOnLongPress);
    application->init();
    DisplayHardware::setTouchCallback([] {
        if (application != nullptr)
        {
            application->resetInactivityTimer();
        }
    });
    DisplayClient::init(application);
    return true;
}

void DisplayRuntime::cyclic()
{
    DisplayHardware::cyclic();
}

void DisplayRuntime::deinit()
{
    DisplayClient::deinit();
    DisplayHardware::setTouchCallback({});
    delete application;
    application = nullptr;
    DisplayHardware::deinit();
}

#endif
