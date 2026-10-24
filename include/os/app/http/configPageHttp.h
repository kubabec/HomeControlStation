#ifndef CONFIGPAGEHTTP_H
#define CONFIGPAGEHTTP_H

/**
 * @file include/os/app/http/configPageHttp.h
 * @brief Embedded web UI asset used by the Home Control Station HTTP interface.
 */


const char* popupContent = "<div class=\"popup-overlay hidden-popup\" id=\"popup-overlay\">\
        <div class=\"popup-content\" id=\"popup-content\">\
            <div class=\"popup-header\">Notification</div>\
            <div class=\"popup-message\" id=\"popup-message\">Message goes here...</div>\
            <button class=\"popup-button\" id=\"popup-button\">Proceed</button>\
            <div class=\"popup-close\" id=\"popup-overlay-close\">&times;</div>\
        </div>\
    </div>\
\
\
<div class=\"popup-overlay hidden-popup\" id=\"credentials-popup-overlay\">\
    <div class=\"popup-content\" id=\"credentials-popup-content\" style=\"display:flex;flex-direction:column;gap:14px\">\
        <div class=\"popup-header\">Updating other nodes</div>\
        <div class=\"popup-message\" id=\"credentials-popup-message\">Updating other nodes, please wait...</div>\
        <div id=\"credentials-node-list\" style=\"text-align:left;font-family:monospace;font-size:14px;line-height:1.6\"></div>\
        <button class=\"popup-button\" id=\"credentials-continue\" disabled onclick=\"continueCredentialsChange()\">Change Master settings</button>\
        <button class=\"popup-button\" id=\"credentials-abort\" onclick=\"abortCredentialsChange()\">Abort</button>\
    </div>\
</div>\
\
<div class=\"popup-overlay hidden-popup\" id=\"notifications-popup-overlay\">\
    <div class=\"popup-content\" id=\"notificationsPopup\">\
        <div class=\"popup-header\">Notifications</div>\
        <div id=\"notificationsList\">\
        </div>\
            <div class=\"popup-close\" id=\"notification-popup-overlay-close\">&times;</div>\
    </div>\
</div>\
    ";


#endif