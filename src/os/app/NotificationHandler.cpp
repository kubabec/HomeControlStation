#include <os/app/NotificationHandler.hpp>

std::queue<UserInterfaceNotification> NotificationHandler::notifications;

void NotificationHandler::init(){
    Serial.println("NotificationHandler init ...");

    DataContainer::setSignalValue(SIG_UI_NOTIFICATIONS_CONTROL, static_cast<UINotificationsControlAPI>(UINotificationsControlAPI{
        .createNotification = createNotification,
        .getActiveNotificationsCount = getActiveNotificationsCount,
        .getOldestNotification = getOldestNotification
    }));


    Serial.println("... done");
}

void NotificationHandler::cyclic(){
    
}

void NotificationHandler::deinit(){

}


bool NotificationHandler::createNotification(UserInterfaceNotification& newNotification)
{
    bool isSuccessfullyCreated = false;
    if(notifications.size() < MAX_NUMBER_OF_NOTIFICATIONS)
    {
        notifications.push(newNotification);
        isSuccessfullyCreated = true;      
    }

    return isSuccessfullyCreated;
}



uint8_t NotificationHandler::getActiveNotificationsCount()
{
    return notifications.size();
}

UserInterfaceNotification NotificationHandler::getOldestNotification()
{
    UserInterfaceNotification retVal;
    if(getActiveNotificationsCount() > 0){
        retVal = notifications.front();
        notifications.pop();
    }
    return retVal;
}