#ifndef DEVICE_CONFIGURATION_FALLBACK_HPP
#define DEVICE_CONFIGURATION_FALLBACK_HPP

#include <Arduino.h>

inline const String generatedDeviceConfigurationJs =
    "<script>function createConfigurationStringJson(){const devices=[];"
    "for(let id=1;id<=6;id++){devices.push({typeId:255,id:id,enabled:document.getElementById('enabled'+id).checked,"
    "name:document.getElementById('name'+id).value,pin:Number(document.getElementById('pin'+id).value),"
    "room:Number(document.getElementById('room'+id).value),customBytes:new Array(20).fill(0)});}"
    "const url='/lclSetupJson&'+JSON.stringify({devices:devices});const xhr=new XMLHttpRequest();"
    "xhr.timeout=10000;xhr.open('POST',url,true);xhr.onreadystatechange=function(){if(xhr.readyState===4){window.location.href='/';}};xhr.send();}</script>";

#endif
