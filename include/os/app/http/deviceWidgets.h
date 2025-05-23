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
    const colorPicker1 = document.createElement('div');\
    colorPicker1.className = 'color-picker';\
    colorPicker1.classList.add('segColDisp');\
    const colorDisplay1 = document.createElement('div');\
    colorDisplay1.className = `color-display on`;\
    colorDisplay1.style.backgroundColor = '#1af265';\
    colorPicker1.appendChild(colorDisplay1);\
    const tog1 = document.createElement('div');\
    tog1.classList.add('switch');\
    tog1.classList.add('segLedTog');\
    const tmb1 = document.createElement('div');\
    tmb1.classList.add('thumb');\
    tog1.appendChild(tmb1);\
\
    const colorPicker2 = document.createElement('div');\
    colorPicker2.className = 'color-picker';\
    colorPicker2.classList.add('segColDisp');\
    const colorDisplay2 = document.createElement('div');\
    colorDisplay2.className = `color-display off`;\
    colorDisplay2.style.backgroundColor = '#6305a5';\
    colorPicker2.appendChild(colorDisplay2);\
    const tog2 = document.createElement('div');\
    tog2.classList.add('switch');\
    tog2.classList.add('segLedTog');\
    const tmb2 = document.createElement('div');\
    tmb2.classList.add('thumb');\
    tog2.appendChild(tmb2);\
\
    const colorPicker3 = document.createElement('div');\
    colorPicker3.className = 'color-picker';\
    colorPicker3.classList.add('segColDisp');\
    const colorDisplay3 = document.createElement('div');\
    colorDisplay3.className = `color-display off`;\
    colorDisplay3.style.backgroundColor = '#ff0510';\
    colorPicker3.appendChild(colorDisplay3);\
    const tog3 = document.createElement('div');\
    tog3.classList.add('switch');\
    tog3.classList.add('segLedTog');\
    const tmb3 = document.createElement('div');\
    tmb3.classList.add('thumb');\
    tog3.appendChild(tmb3);\
\
    const colorPicker4 = document.createElement('div');\
    colorPicker4.className = 'color-picker';\
    colorPicker4.classList.add('segColDisp');\
    const colorDisplay4 = document.createElement('div');\
    colorDisplay4.className = `color-display on`;\
    colorDisplay4.style.backgroundColor = '#e3daf9';\
    colorPicker4.appendChild(colorDisplay4);\
    const tog4 = document.createElement('div');\
    tog4.classList.add('switch');\
    tog4.classList.add('segLedTog');\
    const tmb4 = document.createElement('div');\
    tmb4.classList.add('thumb');\
    tog4.appendChild(tmb4);\
\
    deviceContainer.appendChild(colorPicker1);\
    deviceContainer.appendChild(tog1);\
    deviceContainer.appendChild(colorPicker2);\
    deviceContainer.appendChild(tog2);\
    deviceContainer.appendChild(colorPicker3);\
    deviceContainer.appendChild(tog3);\
    deviceContainer.appendChild(colorPicker4);\
    deviceContainer.appendChild(tog4);\
}\
\
";
#endif