#ifndef RENDER_ROOMS_WIDGETS_JS_H
#define RENDER_ROOMS_WIDGETS_JS_H
#include <Arduino.h>

const String deviceWidgetsJS = "\
function generateOnOffWidget(deviceContainer, device) {\
    const btnContainer = document.createElement('div');\
    btnContainer.className = 'button-container';\
    \
    const button = document.createElement('a');\
    button.className = 'button';\
    button.textContent = (device.status == 'on') ? 'OFF' : 'ON';\
\
    var switchValue = 0;\
    if(device.status == 'off'){\
        switchValue = 1;\
    }\
    button.onclick = () => asyncDeviceStateSwitch(device.id, switchValue);\
    button.id = `switchBtn${device.id}`;\
\
    if(device.hasBrightness == 1){\
        const buttonMore = document.createElement('a');\
        buttonMore.className = 'button';\
        buttonMore.textContent = '. . .';\
        buttonMore.onclick = () => getExtendedControlsRequest(device.id, deviceContainer);\
        btnContainer.appendChild(buttonMore);\
    }\
    btnContainer.appendChild(button);\
    deviceContainer.appendChild(btnContainer);\
\
    if(device.hasBrightness == 1){\
\
        const slider = document.createElement('input');\
        slider.type = 'range';\
        slider.min = 0;\
        slider.max = 100;\
        slider.value = device.brightness;\
        slider.onchange = () => onRangeChanged(slider.value, device.id);\
        slider.id = `brightnessSlider${device.id}`;\
        deviceContainer.appendChild(slider);\
    }\
}\
\
function generateLedStripWidget(deviceContainer, device) {\
    const colorPicker = document.createElement('div');\
    colorPicker.className = 'color-picker';\
\
    const colorDisplay = document.createElement('div');\
    colorDisplay.className = `color-display ${device.status}`;\
    colorDisplay.style.backgroundColor = device.avgColor;\
    colorPicker.appendChild(colorDisplay);\
    deviceContainer.appendChild(colorPicker);\
\
    const btnContainer = document.createElement('div');\
    btnContainer.className = 'button-container';\
\
    const button = document.createElement('a');\
    button.className = 'button';\
    button.textContent = (device.status == 'on') ? 'OFF' : 'ON';\
\
    var switchValue = 0;\
    if(device.status == 'off'){\
        switchValue = 1;\
    }\
    button.onclick = () => asyncDeviceStateSwitch(device.id, switchValue);\
    button.id = `switchBtn${device.id}`;\
\
    const buttonMore = document.createElement('a');\
    buttonMore.className = 'button';\
    buttonMore.textContent = '. . .';\
    buttonMore.onclick = () => getExtendedControlsRequest(device.id, deviceContainer);\
    btnContainer.appendChild(buttonMore);\
    btnContainer.appendChild(button);\
    deviceContainer.appendChild(btnContainer);\
}\
\
function generateTempWidget(deviceContainer, device) {\
    const temperatureContainer = document.createElement('div');\
    temperatureContainer.className = `temperature-container`;\
    temperatureContainer.innerHTML = `<div id=\"gauge${device.id}\" class=\"temperature-widget\"><canvas style=\"max-width: 100px;\"></canvas><div class=\"temperature-value\">20°C</div></div><div id=\"humidity${device.id}\" class=\"humidity-widget\"><canvas></canvas><div class=\"value-display humidity-value\">50%</div></div>`;\
    deviceContainer.appendChild(temperatureContainer);\
\
    listOfTempWidgets.push(`gauge${device.id}`);\
    listOfTempValues.push(device.temp);\
    listOfHumidWidgets.push(`humidity${device.id}`);\
    listOfHumidValues.push(device.humid);\
}\
function generateSegLedWidget(deviceContainer, device) {\
    const segCnt = device.segCount;\
    for(let i = 0; i < segCnt; i++) {\
        const colorPicker = document.createElement('div');\
        colorPicker.className = 'color-picker';\
        colorPicker.classList.add('segColDisp');\
        const colorDisplay = document.createElement('div');\
        colorDisplay.className = `color-display on`;\
        colorDisplay.style.backgroundColor = '#1af265';\
        colorPicker.appendChild(colorDisplay);\
        const tog = document.createElement('div');\
        tog.classList.add('switch');\
        tog.classList.add('segLedTog');\
        const tmb = document.createElement('div');\
        tmb.classList.add('segThumb');\
        tog.appendChild(tmb);\
        \
        deviceContainer.appendChild(colorPicker);\
        deviceContainer.appendChild(tog);\
    };\
    const btnContainer = document.createElement('div');\
    btnContainer.className = 'button-container';\
\
    const button = document.createElement('a');\
    button.className = 'button';\
    button.textContent = (device.status == 'on') ? 'OFF' : 'ON';\
\
    var switchValue = 0;\
    if(device.status == 'off'){\
        switchValue = 1;\
    }\
    button.onclick = () => asyncDeviceStateSwitch(device.id, switchValue);\
    button.id = `switchBtn${device.id}`;\
\
    const buttonMore = document.createElement('a');\
    buttonMore.className = 'button';\
    buttonMore.textContent = '. . .';\
    btnContainer.appendChild(buttonMore);\
    btnContainer.appendChild(button);\
    deviceContainer.appendChild(btnContainer);\
}\
\
";
#endif