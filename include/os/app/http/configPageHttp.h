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
        <div class=\"button-link\" onclick=\"applySettings()\">Apply</div>\
        <div class=\"button-link\" onclick=\"goToDevicesManagement()\">Devices management</div>\
    </form></div><hr>";


#endif