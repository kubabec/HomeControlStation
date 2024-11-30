#ifndef CONFIGPAGEHTTP_H
#define CONFIGPAGEHTTP_H

const char* configPageContent1 = "\
    <div class=\"container\">\
    <form onsubmit=\"return false;\">\
        <label>HTTP Server:\
            <select name=\"isHTTPServer\">";

const char* configPageContent2 = "</select>\
        </label>\
        <label>RC Mode:\
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
        <label>Node ID:\
            <input value=\"";
const char* configPageContent6 = "\" type=\"text\" name=\"nodeid\">\
        </label>\
        <label>Type:\
            <input value=\"";
const char* configPageContent7 = "\" type=\"text\" name=\"nodetype\">\
        </label>\
        <div class=\"error-button\" onclick=\"showPopup('Sure you wanna change Node settings? Device will be restarted afterwards.', applySettings)\">Apply</div>\
        <div class=\"button-link\" onclick=\"goToDevicesManagement()\">Devices management</div>\
        <div class=\"button-link\" onclick=\"goToRoomSettings()\">Room settings</div>\
    </form><br><button onclick=\"showPopup('Do you really wanna clear all node settings? WiFi configuration will also be cleared. Device will not restart automatically, you must reset device on your own when this option is selected!', massErase)\" class=\"error-button\">Restore default</button></div><hr>";

const char* popupContent = "<div class=\"popup-overlay hidden-popup\" id=\"popup-overlay\">\
        <div class=\"popup-content\" id=\"popup-content\">\
            <div class=\"popup-header\">Notification</div>\
            <div class=\"popup-message\" id=\"popup-message\">Message goes here...</div>\
            <button class=\"popup-button\" id=\"popup-button\">Proceed</button>\
            <div class=\"popup-close\" id=\"popup-close\">&times;</div>\
        </div>\
    </div>";


#endif