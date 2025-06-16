#ifndef RENDER_ROOMS_WIDGETS_JS_H
#define RENDER_ROOMS_WIDGETS_JS_H
#include <Arduino.h>

const String deviceWidgetsJS = "\
function generateOnOffWidget(deviceContainer, device) {\
    const btnContainer = document.createElement('div');\
    btnContainer.className = 'button-container';\
    \
    const button = document.createElement('a');\
    if(device.status == 'on') {\
        button.className ='icon-btn off';\ 
    } else {\
        button.className ='icon-btn on';\ 
    }\
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
        buttonMore.className ='icon-btn settings';\
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
    if(device.status == 'on') {\
        button.className ='icon-btn off';\ 
    } else {\
        button.className ='icon-btn on';\ 
    }\
    \
    var switchValue = 0;\
    if(device.status == 'off'){\
        switchValue = 1;\
    }\
    button.onclick = () => asyncDeviceStateSwitch(device.id, switchValue);\
    button.id = `switchBtn${device.id}`;\
\
    const buttonMore = document.createElement('a');\
    buttonMore.className ='icon-btn settings';\
    buttonMore.onclick = () => getExtendedControlsRequest(device.id, deviceContainer);\
    const buttonAnim = document.createElement('a');\
    buttonAnim.className ='icon-btn play';\
    buttonAnim.onclick = () => asyncLiveAnimation(device.id, deviceContainer);\
    btnContainer.appendChild(buttonMore);\
    btnContainer.appendChild(buttonAnim);\
    btnContainer.appendChild(button);\
    deviceContainer.appendChild(btnContainer);\
}\
\
function generateDistSensWidget(deviceContainer, device) {\
    const distData = document.createElement('div');\    
    distData.textContent = `Distance: ${device.dist} cm`; \
    deviceContainer.appendChild(distData);\
\    
}\
\
function generateTempWidget(deviceContainer, device) {\
    const temperatureContainer = document.createElement('div');\
    if(device.Err == 0){\
    temperatureContainer.className = `temperature-container`;\
    temperatureContainer.innerHTML = `<div id=\"gauge${device.id}\" class=\"temperature-widget\"><canvas style=\"max-width: 100px;\"></canvas><div class=\"temperature-value\">20°C</div></div><div id=\"humidity${device.id}\" class=\"humidity-widget\"><canvas></canvas><div class=\"value-display humidity-value\">50%</div></div>`;\
\
    listOfTempWidgets.push(`gauge${device.id}`);\
    listOfTempValues.push(device.temp);\
    listOfHumidWidgets.push(`humidity${device.id}`);\
    listOfHumidValues.push(device.humid);\
    }else {\
        temperatureContainer.className = 'sensor-error';\
        temperatureContainer.innerHTML = 'SENSOR ERROR';\
    }\
    deviceContainer.appendChild(temperatureContainer);\
}\
function generateSegLedWidget(deviceContainer, device) {\
    const segCnt = device.segCount;\
    for(let i = 0; i < segCnt; i++) {\
        const colorPicker = document.createElement('div');\
        colorPicker.className = 'color-picker';\
        colorPicker.classList.add('segColDisp');\
        const colorDisplay = document.createElement('div');\
        colorDisplay.className = `color-display`;\
        colorDisplay.style.backgroundColor = device.colors[i];\
        const tog = document.createElement('div');\
        tog.classList.add('switch');\
        if(device.segments[i] == 1){\
            tog.classList.add('on');\
            colorDisplay.classList.add('on');\
        }\
        tog.classList.add('segLedTog');\
        tog.addEventListener('click', () => {\
         tog.classList.toggle('on');\
         const isOn = tog.classList.contains('on');\
         asyncSegSwitch(device.id, i, isOn);\
        });\
        const tmb = document.createElement('div');\
        tmb.classList.add('segThumb');\
        tog.appendChild(tmb);\
        \
        colorPicker.appendChild(colorDisplay);\
        deviceContainer.appendChild(colorPicker);\
        deviceContainer.appendChild(tog);\
    };\
    const btnContainer = document.createElement('div');\
    btnContainer.className = 'button-container';\
\
    const button = document.createElement('a');\
    if(device.status == 'on') {\
        button.className ='icon-btn off';\ 
    } else {\
        button.className ='icon-btn on';\ 
    }\
\
    var switchValue = 0;\
    if(device.status == 'off'){\
        switchValue = 1;\
    }\
    button.onclick = () => asyncDeviceStateSwitch(device.id, switchValue);\
    button.id = `switchBtn${device.id}`;\
\
    const buttonMore = document.createElement('a');\
    buttonMore.className ='icon-btn settings';\
    btnContainer.appendChild(buttonMore);\
    btnContainer.appendChild(button);\
    deviceContainer.appendChild(btnContainer);\
}\
\
";
#endif