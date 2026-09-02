#include <os/app/NotificationHandler.hpp>
#include "os/Logger.hpp"

/**
 * @file src/os/app/NotificationHandler.cpp
 * @brief Notification dispatch: stores and exposes UI notifications for display and retrieval.
 */


std::deque<NotificationHandler::PendingNotification> NotificationHandler::notifications;

void NotificationHandler::init()
{
    Logger::log("NotificationHandler init ...");

    DataContainer::setSignalValue(SIG_UI_NOTIFICATIONS_CONTROL, static_cast<UINotificationsControlAPI>(UINotificationsControlAPI{
                                                                    .createNotification = [](UserInterfaceNotification& notification) {
                                                                        return createNotification(notification);
                                                                    },
                                                                    .createNotificationWithTimeout = [](UserInterfaceNotification& notification, uint32_t timeoutMs) {
                                                                        return createNotification(notification, timeoutMs);
                                                                    },
                                                                    .getActiveNotificationsCount = getActiveNotificationsCount,
                                                                    .getOldestNotification = getOldestNotification}));

    Logger::log("... done");
}

void NotificationHandler::cyclic()
{
    removeExpiredNotifications();
}

void NotificationHandler::deinit()
{
    notifications.clear();
}

bool NotificationHandler::createNotification(UserInterfaceNotification &newNotification)
{
    return createNotification(newNotification, DEFAULT_NOTIFICATION_TIMEOUT_MS);
}

bool NotificationHandler::createNotification(UserInterfaceNotification &newNotification, uint32_t timeoutMs)
{
#ifndef SUPPORT_NOTIFICATION
    return true;
#else
    removeExpiredNotifications();
    bool isSuccessfullyCreated = false;
    if (notifications.size() < MAX_NUMBER_OF_NOTIFICATIONS)
    {

        std::any localAny = DataContainer::getSignalValue(CBK_GET_CURRENT_TIME);
        if (auto p = std::any_cast<std::function<RtcTime()>>(&localAny))
        {
            auto timeCallback = *p;

            String dateTime = timeCallback().toString(); // Get the current time as a string
            newNotification.time = dateTime;
        }
        else
        {
            newNotification.time = "1970-01-01 00:00:00";
        }

        notifications.push_back({newNotification, static_cast<uint32_t>(millis()), timeoutMs});
        Logger::log("NotificationHandler:// New notification: " + newNotification.toJson());
        isSuccessfullyCreated = true;
    }
    return isSuccessfullyCreated;

#endif
}

uint8_t NotificationHandler::getActiveNotificationsCount()
{
    removeExpiredNotifications();
    return static_cast<uint8_t>(notifications.size());
}

UserInterfaceNotification NotificationHandler::getOldestNotification()
{
    UserInterfaceNotification retVal;
    if (getActiveNotificationsCount() > 0)
    {
        retVal = notifications.front().notification;
        notifications.pop_front();
    }
    return retVal;
}

void NotificationHandler::removeExpiredNotifications()
{
    const uint32_t nowMs = static_cast<uint32_t>(millis());
    for (auto notification = notifications.begin(); notification != notifications.end();)
    {
        if (static_cast<uint32_t>(nowMs - notification->createdAtMs) >= notification->timeoutMs)
        {
            notification = notifications.erase(notification);
        }
        else
        {
            ++notification;
        }
    }
}