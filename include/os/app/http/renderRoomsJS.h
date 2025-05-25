#ifndef RENDER_ROOMS_JS_H
#define RENDER_ROOMS_JS_H
#include <Arduino.h>
#include "deviceWidgets.h"

const String renderRoomsJS = "\
<script>" + deviceWidgetsJS +
"const onOffType = 43;\
const ledStripType = 44;\
const tempSensorType = 45;\
const segLedStripType = 46;\
const distSensType = 47;\
\
function generateCommonDevInterface(deviceContainer, deviceId, deviceName, favIconNumber, status) {\
    const faviconUrl = 'https://github.com/kubabec/HomeControlStation/blob/faviconsCreation/res/';\
    const loadingOverlay = document.createElement('div');\
    loadingOverlay.className = 'loading-overlay';\
    loadingOverlay.style.display = 'none';\
    const spinner = document.createElement('div');\
    spinner.className = 'spinner';\
    loadingOverlay.appendChild(spinner);\
    deviceContainer.appendChild(loadingOverlay);\
\
    const faviconImageMain = document.createElement('div');\
    faviconImageMain.className = 'device-left';\
    const faviconImg = document.createElement('img');\
    faviconImg.className = 'device-icon';\
    faviconImg.src = faviconUrl + `${favIconNumber}.png?raw=true`;\
    faviconImageMain.appendChild(faviconImg);\
    deviceContainer.appendChild(faviconImageMain);\
\
    const header = document.createElement('div');\
    header.className = 'header';\
    header.textContent = `${deviceName}`;\
    deviceContainer.appendChild(header);\
\
    const statusLight = document.createElement('div');\
    statusLight.className = `status-light ${status}`;\
    statusLight.id = `statusLight${deviceId}`;\
    deviceContainer.appendChild(statusLight);\
}\
function renderRooms(data) {\
    const roomsContainer = document.getElementById('rooms');\\
    roomsContainer.innerHTML = '';\
\
    for (const [roomId, devices] of Object.entries(data)) {\
        var anyOn = false;\
        const roomContainer = document.createElement('div');\
        roomContainer.className = 'room-container';\
\
        const roomHeader = document.createElement('div');\
        roomHeader.className = 'room-header';\
        roomHeader.textContent = `${roomId}`;\
        roomContainer.appendChild(roomHeader);\
        const toglContainer = document.createElement('div');\
        toglContainer.className = 'toggler-container';\
        const roomtoggle = document.createElement('div');\
        roomtoggle.classList.add('switch');\
        const thumb = document.createElement('div');\
        thumb.classList.add('thumb');\
        roomtoggle.appendChild(thumb);\
        roomtoggle.addEventListener('click', () => {\
         roomtoggle.classList.toggle('on');\
         const isOn = roomtoggle.classList.contains('on');\
         asyncRoomStateSwitch(roomId, isOn);\
        });\
        toglContainer.appendChild(roomtoggle);\
        roomContainer.appendChild(toglContainer);\
        devices.forEach(device => {\
            const deviceContainer = document.createElement('div');\
            deviceContainer.className = 'container';\
            deviceContainer.id = `container${device.id}`;\
                \
            if(device.devType == onOffType){\
                generateCommonDevInterface( deviceContainer, device.id, device.name, 1, device.status);\
                generateOnOffWidget(deviceContainer, device);\
            }else if(device.devType == ledStripType){\
                generateCommonDevInterface( deviceContainer, device.id, device.name, 2, device.status);\
                generateLedStripWidget(deviceContainer, device);\
            }else if(device.devType == distSensType){\
                generateCommonDevInterface( deviceContainer, device.id, device.name, 2, device.status);\
                generateDistSensWidget(deviceContainer, device);\             
            }else if(device.devType == tempSensorType){\
                generateCommonDevInterface( deviceContainer, device.id, device.name, 3, device.status);\
                generateTempWidget(deviceContainer, device);\
            } else if(device.devType == segLedStripType){\
                generateCommonDevInterface( deviceContainer, device.id, device.name, 2, device.status);\
                generateSegLedWidget(deviceContainer, device);\       
            }else{\
                const header = document.createElement('div');\
                header.className = 'header';\
                header.textContent = 'UnknownDeviceType';\
                deviceContainer.appendChild(header);\
            }\
\           if(device.status == 'on'){\
                anyOn = true;\
            }\
            roomContainer.appendChild(deviceContainer);\
        });\
\
        if(anyOn){\
            roomtoggle.classList.add('on');\
        }\
        roomsContainer.appendChild(roomContainer);\
    }\
    for (var i = 0, l = listOfTempWidgets.length; i < l; ++i) {\
        createGauge(listOfTempWidgets[i].toString());\
        setTemperature(listOfTempWidgets[i].toString(), listOfTempValues[i]);\
        createHumidGauge(listOfHumidWidgets[i].toString());\
        setHumidity(listOfHumidWidgets[i].toString(), listOfHumidValues[i]);\
    }\
    listOfTempWidgets = [];\
    listOfHumidWidgets = [];\
    listOfHumidValues = [];\
    listOfTempValues = [];\
}\
</script>";

#endif