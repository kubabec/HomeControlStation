#ifndef JAVASCRIPT_H
#define JAVASCRIPT_H


const char* javascript = "\
<script>\
    function applySettings() {\
        var isHTTPServer = document.querySelector('select[name=\"isHTTPServer\"]').value;\
        var isRCServer = document.querySelector('select[name=\"isRCServer\"]').value;\
        var SSID = document.querySelector('input[name=\"SSID\"]').value;\
        var Password = document.querySelector('input[name=\"Password\"]').value;\
        var url = `/apply?isHTTPServer=${encodeURIComponent(isHTTPServer)}&isRCServer=${encodeURIComponent(isRCServer)}&SSID=${encodeURIComponent(SSID)}&Password=${encodeURIComponent(Password)}`;\
        window.location.href = url;\
    }\
    function goToDevicesManagement() {\
        var url = `/localDevices`;\
        window.location.href = url;\
    }\
    function onRangeTimChanged(value) {\
        value = parseInt(value);\
        window.location.assign(\"/?tim\"+value+\"&\");\
    }\
    function onRangeChanged(value, devId) {\
        value = parseInt(value);\
        window.location.assign(\"/?bri\"+value+\"DEV\"+devId+\"&\");\
    }\
</script>";

#endif