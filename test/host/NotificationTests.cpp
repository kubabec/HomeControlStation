#include "TestHarness.hpp"
#include "Arduino.h"

#include "os/app/NotificationHandler.hpp"

#include <any>
#include <cstdint>
#include <limits>

namespace
{
UserInterfaceNotification makeNotification(const char* title)
{
    UserInterfaceNotification notification;
    notification.title = title;
    return notification;
}

void resetNotifications()
{
    ArduinoFake::reset();
    NotificationHandler::deinit();
}
}

TEST_CASE("NotificationHandler expires notifications after the default timeout")
{
    resetNotifications();
    UserInterfaceNotification notification = makeNotification("default");
    EXPECT_TRUE(NotificationHandler::createNotification(notification));

    ArduinoFake::advanceMillis(NotificationHandler::DEFAULT_NOTIFICATION_TIMEOUT_MS - 1);
    EXPECT_EQ(NotificationHandler::getActiveNotificationsCount(), uint8_t(1));
    ArduinoFake::advanceMillis(1);
    EXPECT_EQ(NotificationHandler::getActiveNotificationsCount(), uint8_t(0));
    EXPECT_EQ(NotificationHandler::getOldestNotification().title, String(""));
}

TEST_CASE("NotificationHandler removes expired entries regardless of queue position")
{
    resetNotifications();
    UserInterfaceNotification longLived = makeNotification("long");
    UserInterfaceNotification shortLived = makeNotification("short");
    EXPECT_TRUE(NotificationHandler::createNotification(longLived, 1000));
    EXPECT_TRUE(NotificationHandler::createNotification(shortLived, 100));

    ArduinoFake::advanceMillis(100);
    EXPECT_EQ(NotificationHandler::getActiveNotificationsCount(), uint8_t(1));
    EXPECT_EQ(NotificationHandler::getOldestNotification().title, String("long"));
}

TEST_CASE("NotificationHandler exposes custom timeout creation through its control API")
{
    resetNotifications();
    NotificationHandler::init();
    auto api = std::any_cast<UINotificationsControlAPI>(
        DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL));
    UserInterfaceNotification notification = makeNotification("api");
    EXPECT_TRUE(api.createNotificationWithTimeout(notification, 25));

    ArduinoFake::advanceMillis(25);
    EXPECT_EQ(api.getActiveNotificationsCount(), uint8_t(0));
}

TEST_CASE("NotificationHandler expiry handles millis rollover")
{
    resetNotifications();
    ArduinoFake::setMillis(std::numeric_limits<uint32_t>::max() - 50U);
    UserInterfaceNotification notification = makeNotification("rollover");
    EXPECT_TRUE(NotificationHandler::createNotification(notification, 100));

    ArduinoFake::advanceMillis(99);
    EXPECT_EQ(NotificationHandler::getActiveNotificationsCount(), uint8_t(1));
    ArduinoFake::advanceMillis(1);
    EXPECT_EQ(NotificationHandler::getActiveNotificationsCount(), uint8_t(0));
}