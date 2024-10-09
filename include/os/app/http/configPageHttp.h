#ifndef CONFIGPAGEHTTP_H
#define CONFIGPAGEHTTP_H

const char* configPageContent1 = "\
    <div class=\"container\">\
    <form onsubmit=\"return false;\">\
        <label>isHTTPServer:\
            <select name=\"isHTTPServer\">";

const char* configPageContent2 = "</select>\
        </label>\
        <label>isRCServer:\
            <select name=\"isRCServer\">";
const char* configPageContent3 = "</select>\
        </label>\
        <label>SSID:\
            <input value=\"";
            
const char* configPageContent4 = "\" type=\"text\" name=\"SSID\">\
        </label>\
        <label>Password:\
            <input value=\"";
const char* configPageContent5  = "\" type=\"text\" name=\"Password\">\
        </label>\
        <div class=\"button-link\" onclick=\"showPopup('Sure you wanna change Node settings? Device will be restarted afterwards.', applySettings)\">Apply</div>\
        <div class=\"button-link\" onclick=\"goToDevicesManagement()\">Devices management</div>\
    </form><br><button onclick=\"showPopup('Do you really wanna clear all node settings? WiFi configuration will also be cleared. Device will reboot in AccessPoint mode.', massErase)\" class=\"error-button\">Restore default</button></div><hr>";

const char* popupContent = "<div class=\"popup-overlay hidden-popup\" id=\"popup-overlay\">\
        <div class=\"popup-content\" id=\"popup-content\">\
            <div class=\"popup-header\">Notification</div>\
            <div class=\"popup-message\" id=\"popup-message\">Message goes here...</div>\
            <button class=\"popup-button\" id=\"popup-button\">Proceed</button>\
            <div class=\"popup-close\" id=\"popup-close\">&times;</div>\
        </div>\
    </div>";


#endif