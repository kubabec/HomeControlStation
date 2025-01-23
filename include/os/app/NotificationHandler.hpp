#ifndef NOTIFICATION_HANDLER_H
#define NOTIFICATION_HANDLER_H
#include <Arduino.h>
#include <os/datacontainer/DataContainer.hpp>
#include <queue>

#define MAX_NUMBER_OF_NOTIFICATIONS 10

class NotificationHandler {
    static std::queue<UserInterfaceNotification> notifications;

public:
    static void init();
    static void cyclic();
    static void deinit();


    static bool createNotification(UserInterfaceNotification& newNotification);
    static uint8_t getActiveNotificationsCount();
    static UserInterfaceNotification getOldestNotification();

};

#endif