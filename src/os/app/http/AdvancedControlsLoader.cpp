#include <os/app/http/AdvancedControlsLoader.hpp>
#include <SystemDefinition.hpp>

#define ONOFF_ADV_CONTROLS_SIZE 100
#define TEMPSENSOR_ADV_CONTROLS_SIZE 50

uint8_t* AdvancedControlsLoader::currentAdvancedControls = nullptr;
DeviceDescription AdvancedControlsLoader::currentlyRequestedDeviceDescription = {};
String AdvancedControlsLoader::currentRequestJS = "";

uint16_t AdvancedControlsLoader::getControlsSizeBasedOnDevType(uint8_t deviceType){
    switch(deviceType){
        case type_ONOFFDEVICE:
            return sizeof(AdvancedControlsOnOff);
        break;

        case type_LED_STRIP:
            return (currentlyRequestedDeviceDescription.customBytes[0] * sizeof(LedColor)) + sizeof(LedStripAnimationProperties);
        break;

        case type_TEMP_SENSOR:
            return TEMPSENSOR_ADV_CONTROLS_SIZE;
        break;

        default:
            return 0;
    }

}

uint8_t* AdvancedControlsLoader::allocateMemoryForControlsBasedOnDeviceType(uint8_t deviceType){
    /* release previously loaded controls */
    if(currentAdvancedControls != nullptr){
        Serial.println("AdvancedControlsLoader://Releasing old memory");;
        free(currentAdvancedControls);
    }

    currentAdvancedControls = nullptr;
    Serial.println("AdvancedControlsLoader://Allocating memory for device type : " + String((int)deviceType));
    if(deviceType >= type_ONOFFDEVICE && deviceType <= type_DEVICE_TYPE_LAST){
        Serial.println("AdvancedControlsLoader://Allocating memory for type {"+String((int)deviceType)+"} with size : " + String((int)getControlsSizeBasedOnDevType(deviceType)));
        currentAdvancedControls = (uint8_t*)malloc(getControlsSizeBasedOnDevType(deviceType));
    }else {
        Serial.println("AdvancedControlsLoader://Type {"+String((int)deviceType)+"} is out of range");
    }

    return currentAdvancedControls;
}

String AdvancedControlsLoader::createJsForOnOff(){
    AdvancedControlsOnOff controls;
    memcpy(&controls, currentAdvancedControls, sizeof(AdvancedControlsOnOff));


    String popupContentJavaScript = "";


    popupContentJavaScript += "var popup = document.getElementById('advanced-ctrl-popup-msg');";
    popupContentJavaScript += "var advCtrlHead = document.getElementById('adv-ctrl-head');";
    popupContentJavaScript += "advCtrlHead.innerHTML = '"+currentlyRequestedDeviceDescription.deviceName+"';";

    /* Enable animation */
    popupContentJavaScript += "var enableAnimDesc = document.createElement('label');";
    popupContentJavaScript += "enableAnimDesc.innerText = 'Enable animation';";

    popupContentJavaScript += "var enableAnimSelector = document.createElement('select');";
    popupContentJavaScript += "enableAnimSelector.id = \"EnAnimSelect\";";

    popupContentJavaScript += "var optEn1 = document.createElement(\"option\");";
    popupContentJavaScript += "var optEn2 = document.createElement(\"option\");";
    popupContentJavaScript += "var optEn3 = document.createElement(\"option\");";
    popupContentJavaScript += "optEn1.value = 0;";
    popupContentJavaScript += "optEn1.text = 'switch';";
    popupContentJavaScript += "optEn2.value = 1;";
    popupContentJavaScript += "optEn2.text = 'fade';";
    popupContentJavaScript += "optEn3.value = 2;";
    popupContentJavaScript += "optEn3.text = 'blink';";

    switch(controls.switchOnAnimation){
        case AdvancedControlsOnOff::OnOffAnimations::ONOFF_SWITCH:
            popupContentJavaScript += "optEn1.setAttribute('selected', true);";
        break;
        case AdvancedControlsOnOff::OnOffAnimations::ONOFF_FADE:
            popupContentJavaScript += "optEn2.setAttribute('selected', true);";
        break;
        case AdvancedControlsOnOff::OnOffAnimations::ONOFF_BLINK:
            popupContentJavaScript += "optEn3.setAttribute('selected', true);";
        break;
    }

    popupContentJavaScript += "enableAnimSelector.appendChild(optEn1);";
    popupContentJavaScript += "enableAnimSelector.appendChild(optEn2);";
    popupContentJavaScript += "enableAnimSelector.appendChild(optEn3);";


    /* Disable animation */
    popupContentJavaScript += "var disableAnimDesc = document.createElement('label');";
    popupContentJavaScript += "disableAnimDesc.innerText = 'Disable animation';";

    popupContentJavaScript += "var disableAnimSelector = document.createElement('select');";
    popupContentJavaScript += "disableAnimSelector.id = \"DisAnimSelect\";";

    popupContentJavaScript += "var optDi1 = document.createElement(\"option\");";
    popupContentJavaScript += "var optDi2 = document.createElement(\"option\");";
    popupContentJavaScript += "var optDi3 = document.createElement(\"option\");";
    popupContentJavaScript += "optDi1.value = 0;";
    popupContentJavaScript += "optDi1.text = 'switch';";
    popupContentJavaScript += "optDi2.value = 1;";
    popupContentJavaScript += "optDi2.text = 'fade';";
    popupContentJavaScript += "optDi3.value = 2;";
    popupContentJavaScript += "optDi3.text = 'blink';";

    switch(controls.switchOffAnimation){
        case AdvancedControlsOnOff::OnOffAnimations::ONOFF_SWITCH:
            popupContentJavaScript += "optDi1.setAttribute('selected', true);";
        break;
        case AdvancedControlsOnOff::OnOffAnimations::ONOFF_FADE:
            popupContentJavaScript += "optDi2.setAttribute('selected', true);";
        break;
        case AdvancedControlsOnOff::OnOffAnimations::ONOFF_BLINK:
            popupContentJavaScript += "optDi3.setAttribute('selected', true);";
        break;
    }

    popupContentJavaScript += "disableAnimSelector.appendChild(optDi1);";
    popupContentJavaScript += "disableAnimSelector.appendChild(optDi2);";
    popupContentJavaScript += "disableAnimSelector.appendChild(optDi3);";



    popupContentJavaScript += "enableAnimSelector.onchange = function() {\
        var enableAnimSelector = document.getElementById('EnAnimSelect');\
        var disableAnimSelector = document.getElementById('DisAnimSelect');\
        var animeTimeVal = document.getElementById('animTimeVal');\
        var animeTimeLabel = document.getElementById('animTimeLabel');\
        if(enableAnimSelector.value == 0 && disableAnimSelector.value == 0){\
            animeTimeVal.style.display = 'none';\
            animeTimeLabel.style.display = 'none';\
        }\
        else\
        {\
            animeTimeVal.style.display = '';\
            animeTimeLabel.style.display = '';\
        }\
    };";

    popupContentJavaScript += "disableAnimSelector.onchange = enableAnimSelector.onchange;";

    
    /* animation time */
    popupContentJavaScript += "var animTime = document.createElement('label');";
    popupContentJavaScript += "animTime.innerText = 'Switch animation time [ms]:';";
    popupContentJavaScript += "animTime.id = 'animTimeLabel';";

    popupContentJavaScript += "var animTimeVal = document.createElement('input');";
    popupContentJavaScript += "animTimeVal.id = \"animTimeVal\";";
    popupContentJavaScript += "animTimeVal.type = \"text\";";
    popupContentJavaScript += "animTimeVal.value = \""+String((int)controls.switchAnimationTime)+"\";";

    popupContentJavaScript += "var saveBtn = document.createElement('button');";
    popupContentJavaScript += "saveBtn.innerHTML = 'Save';";
    popupContentJavaScript += "saveBtn.className = 'popup-button';";
    popupContentJavaScript += "saveBtn.style.marginTop = '10px';";

    popupContentJavaScript += "popup.appendChild(enableAnimDesc);";
    popupContentJavaScript += "popup.appendChild(enableAnimSelector);";

    popupContentJavaScript += "popup.appendChild(disableAnimDesc);";
    popupContentJavaScript += "popup.appendChild(disableAnimSelector);";

    popupContentJavaScript += "popup.appendChild(animTime);";
    popupContentJavaScript += "popup.appendChild(animTimeVal);";

    popupContentJavaScript += "popup.appendChild(document.createElement('br'));";
    popupContentJavaScript += "popup.appendChild(saveBtn);";
    
    popupContentJavaScript += "enableAnimSelector.onchange();";

    return popupContentJavaScript;
}

String AdvancedControlsLoader::createJsForLedStrip(){
    String popupContentJavaScript = "";
#ifdef LED_STRIP_SUPPORTED
    LedStripAnimationProperties stripAnimationProperties;
    // Copy the properties from the current advanced controls
    memcpy(&stripAnimationProperties, currentAdvancedControls, sizeof(LedStripAnimationProperties));

    // Set pointer to the colors array
    LedColor* ledColors = (LedColor*) ((uint8_t*) currentAdvancedControls + sizeof(LedStripAnimationProperties)); 

    uint8_t diodeCount = currentlyRequestedDeviceDescription.customBytes[0];
    
    popupContentJavaScript += "var ledColors = [";
    for(int i = 0 ; i < diodeCount; i ++){
        // Serial.println("-- LED --");
        // Serial.println("R: " + String((int)ledColors[i].r) + " , G: " + String((int)ledColors[i].g) + " , B: " + String((int)ledColors[i].b));
        popupContentJavaScript += "['"+String((int)ledColors[i].r)+"', '"+String((int)ledColors[i].g)+"', '"+String((int)ledColors[i].b)+"'],";
    }
    popupContentJavaScript.remove(popupContentJavaScript.length()-1);
    popupContentJavaScript += "];";

    popupContentJavaScript += "var popup = document.getElementById('advanced-ctrl-popup-msg');";
    popupContentJavaScript += "var advCtrlHead = document.getElementById('adv-ctrl-head');";
    popupContentJavaScript += "advCtrlHead.innerHTML = '"+currentlyRequestedDeviceDescription.deviceName+"';";

    uint16_t ledsCount = currentlyRequestedDeviceDescription.customBytes[0]; /* virtual diodes count 0 - 100*/

    popupContentJavaScript += "var ledStrip = document.createElement('div');";
    popupContentJavaScript += "ledStrip.className = 'led-strip';";
    popupContentJavaScript += "var ledsTab = [];";
    popupContentJavaScript += "var ledMarkingState = 0;";
    popupContentJavaScript += "var ledMarkingStartId = 0;";
    popupContentJavaScript += "function ledOnMouseDown(){\
        if(this.classList.contains('marked')){\
            ledMarkingState = 2;\
        }else {\
            ledMarkingState = 1;\
        }\
        \
        ledMarkingStartId = parseInt(this.id.replace('ledContainer', ''));\
        ledUpdateMarkedState(this);\
    };";
    popupContentJavaScript += "function ledOnMouseMove(){\
        if(ledMarkingState != 0){\
            let ledId = parseInt(this.id.replace('ledContainer', ''));\
            let startIdx = Math.min(ledMarkingStartId, ledId);\
            let endIdx = Math.max(ledMarkingStartId, ledId);\
            for(let i = startIdx; i <= endIdx; i++){\
                ledUpdateMarkedState(ledsTab[i]);\
            }\
        }\
    };";
    popupContentJavaScript += "function ledOnMouseUp(){\
        ledUpdateMarkedState(this);\
        ledMarkingState = 0;\
    };";
    popupContentJavaScript += "document.addEventListener('mouseup', function(evnt){\
        ledMarkingState = 0;\
    });";
    popupContentJavaScript += "document.addEventListener('touchend', function(evnt){\
        ledMarkingState = 0;\
    });";
    popupContentJavaScript += "document.addEventListener('touchcancel', function(evnt){\
        ledMarkingState = 0;\
    });";
    
    popupContentJavaScript += "function ledUpdateMarkedState(ledContainer){\
        if(ledMarkingState == 2 && ledContainer.classList.contains('marked')){\
            ledContainer.classList.remove('marked');\
        }else if(ledMarkingState == 1 && !ledContainer.classList.contains('marked')){\
            ledContainer.classList.add('marked');\
        }\
    };";


    popupContentJavaScript += "\
    for(let i = 0; i < "+String((int)ledsCount)+"; i++){\
        var ledContainer = document.createElement('div');\
        ledContainer.classList.add('ledContainer');\
        ledContainer.id = 'ledContainer'+i;\
        ledContainer.onmousedown = ledOnMouseDown;\
        ledContainer.onmouseup = ledOnMouseUp;\
        ledContainer.onmousemove = ledOnMouseMove;\
        \
        ledContainer.ontouchstart = function(e){\
            e.preventDefault();\
            ledOnMouseDown.call(this, e);\
        };\
        ledContainer.ontouchend = function(e){\
            e.preventDefault();\
            ledOnMouseUp.call(this, e);\
        };\
        \
        var led = document.createElement('div');\
        led.classList.add('led');\
        led.id = 'led'+i;\
        if(ledColors[i][0] != 0 || ledColors[i][1] != 0 || ledColors[i][2] != 0){\
            led.style.backgroundColor = 'rgb(' + ledColors[i][0] + ', ' + ledColors[i][1] + ', ' + ledColors[i][2] + ')';\
            led.style.boxShadow = '0 0 0px 6px rgba(0, 205, 0, 0.007), 0 0 6px 8px rgba(0, 205, 0, 0.08)';\
        }else {\
            led.style.backgroundColor = 'rgba(' + ledColors[i][0] + ', ' + ledColors[i][1] + ', ' + ledColors[i][2] + ', 0.5)';\
        }\
        led.style.borderRadius = '3px';\
        \
        ledContainer.appendChild(led);\
        ledStrip.appendChild(ledContainer);\
        ledsTab.push(ledContainer);\
    }";

    popupContentJavaScript += "ledStrip.addEventListener('touchmove', \
    function(e){\
        e.preventDefault();\
        let touch = e.touches[0];\
        let targetElem = document.elementFromPoint(touch.clientX, touch.clientY);\
        if(targetElem && targetElem.classList.contains('ledContainer')){\
            ledOnMouseMove.call(targetElem, e);\
        }\
        else if(targetElem && targetElem.classList.contains('led')){\
            ledOnMouseMove.call(targetElem.parentNode, e);\
        }\
    });";

    popupContentJavaScript += "popup.appendChild(ledStrip);";

    popupContentJavaScript += "var colorPickerDiv = document.createElement('div');";
    popupContentJavaScript += "colorPickerDiv.className = 'color-picker';";
    popupContentJavaScript += "var colorPicker = document.createElement('input');";
    popupContentJavaScript += "colorPicker.className = 'color-input';";
    popupContentJavaScript += "colorPicker.type = 'color';";
    popupContentJavaScript += "colorPicker.value = '#0010Af';";
    popupContentJavaScript += "colorPicker.onblur = function (){\
        const r = parseInt(this.value.substr(1,2), 16);\
        const g = parseInt(this.value.substr(3,2), 16);\
        const b = parseInt(this.value.substr(5,2), 16);\
        \
        let isAnyLedMarked = false;\
        for(let i = 0; i < "+String((int)ledsCount)+"; i++){\
            if(ledsTab[i].classList.contains('marked')){\
                isAnyLedMarked = true;\
                break;\
            }\
        }\
        for(let i = 0; i < "+String((int)ledsCount)+"; i++){\
            if(!isAnyLedMarked || ledsTab[i].classList.contains('marked')){\
                if(r != 0 || g != 0 || b != 0){\
                    ledsTab[i].childNodes[0].style.backgroundColor = 'rgb('+r+', '+g+', '+b+')';\
                    ledsTab[i].childNodes[0].style.boxShadow = '0 0 0px 6px rgba(0, 205, 0, 0.007), 0 0 6px 8px rgba(0, 205, 0, 0.04)';\
                }else {\
                    ledsTab[i].childNodes[0].style.backgroundColor = 'rgb(0, 0, 0, 0.1)';\
                    ledsTab[i].childNodes[0].style.boxShadow = 'none';\
                }\
                ledsTab[i].classList.remove('marked');\
            }\
        }\
    };";

    popupContentJavaScript += "popup.appendChild(colorPickerDiv);";
    popupContentJavaScript += "popup.appendChild(colorPicker);";

    /* Enable Animation Section */
    /* Enable animation */
    popupContentJavaScript += "var enableAnimDesc = document.createElement('label');";
    popupContentJavaScript += "enableAnimDesc.innerText = 'Enable animation:';";

    popupContentJavaScript += "var enableAnimSelector = document.createElement('select');";
    popupContentJavaScript += "enableAnimSelector.id = \"EnAnimSelect\";";

    popupContentJavaScript += "var optEn1 = document.createElement(\"option\");";
    popupContentJavaScript += "var optEn2 = document.createElement(\"option\");";
    popupContentJavaScript += "var optEn3 = document.createElement(\"option\");";
    popupContentJavaScript += "var optEn4 = document.createElement(\"option\");";
    popupContentJavaScript += "var optEn5 = document.createElement(\"option\");";
    popupContentJavaScript += "optEn1.value = 0;";
    popupContentJavaScript += "optEn1.text = 'Roll (left)';";
    popupContentJavaScript += "optEn2.value = 1;";
    popupContentJavaScript += "optEn2.text = 'Fade';";
    popupContentJavaScript += "optEn3.value = 2;";
    popupContentJavaScript += "optEn3.text = 'Sparkle';";
    popupContentJavaScript += "optEn4.value = 3;";
    popupContentJavaScript += "optEn4.text = 'Twinkle';";
    popupContentJavaScript += "optEn5.value = 4;";
    popupContentJavaScript += "optEn5.text = 'Bounce';";


    switch(stripAnimationProperties.enableAnimation){
        case 0:
            popupContentJavaScript += "optEn1.setAttribute('selected', true);";
        break;
        case 1:
            popupContentJavaScript += "optEn2.setAttribute('selected', true);";
        break;
        case 2:
            popupContentJavaScript += "optEn3.setAttribute('selected', true);";
        break;
        case 3:
            popupContentJavaScript += "optEn4.setAttribute('selected', true);";
        break;
        case 4:
            popupContentJavaScript += "optEn5.setAttribute('selected', true);";
        break;
    }

    popupContentJavaScript += "enableAnimSelector.appendChild(optEn1);";
    popupContentJavaScript += "enableAnimSelector.appendChild(optEn2);";
    popupContentJavaScript += "enableAnimSelector.appendChild(optEn3);";
    popupContentJavaScript += "enableAnimSelector.appendChild(optEn4);";
    popupContentJavaScript += "enableAnimSelector.appendChild(optEn5);";

    popupContentJavaScript += "enableAnimDesc.appendChild(enableAnimSelector);";
    popupContentJavaScript += "popup.appendChild(enableAnimDesc);";

    /* DISABLE Animation Section */
    popupContentJavaScript += "var disableAnimDesc = document.createElement('label');";
    popupContentJavaScript += "disableAnimDesc.innerText = 'Disable animation:';";

    popupContentJavaScript += "var disableAnimSelector = document.createElement('select');";
    popupContentJavaScript += "disableAnimSelector.id = \"DiAnimSelect\";";

    popupContentJavaScript += "var optDi1 = document.createElement(\"option\");";
    popupContentJavaScript += "var optDi2 = document.createElement(\"option\");";
    popupContentJavaScript += "var optDi3 = document.createElement(\"option\");";
    popupContentJavaScript += "optDi1.value = 0;";
    popupContentJavaScript += "optDi1.text = 'Fade out';";
    popupContentJavaScript += "optDi2.value = 1;";
    popupContentJavaScript += "optDi2.text = 'Roll out (right)';";
    popupContentJavaScript += "optDi3.value = 2;";
    popupContentJavaScript += "optDi3.text = 'Roll out (left)';";

    switch(stripAnimationProperties.disableAnimation){
        case 0:
            popupContentJavaScript += "optDi1.setAttribute('selected', true);";
        break;
        case 1:
            popupContentJavaScript += "optDi2.setAttribute('selected', true);";
        break;
        case 2:
            popupContentJavaScript += "optDi3.setAttribute('selected', true);";
        break;
    }

    popupContentJavaScript += "disableAnimSelector.appendChild(optDi1);";
    popupContentJavaScript += "disableAnimSelector.appendChild(optDi2);";
    popupContentJavaScript += "disableAnimSelector.appendChild(optDi3);";

    popupContentJavaScript += "disableAnimDesc.appendChild(disableAnimSelector);";
    popupContentJavaScript += "popup.appendChild(disableAnimDesc);";

    /* DISABLE Animation Section END */

    /* Enable Animation Section END */

    /* Enable Animation SPEED Section */
    popupContentJavaScript += "var enspeedDesc = document.createElement('label');";
    popupContentJavaScript += "enspeedDesc.innerText = 'Animation speed:';";

    popupContentJavaScript += "var enspeedSelector = document.createElement('select');";
    popupContentJavaScript += "enspeedSelector.id = \"EnSpeSelect\";";

    popupContentJavaScript += "var optES1 = document.createElement(\"option\");";
    popupContentJavaScript += "var optES2 = document.createElement(\"option\");";
    popupContentJavaScript += "var optES3 = document.createElement(\"option\");";
    popupContentJavaScript += "optES1.value = 0;";
    popupContentJavaScript += "optES1.text = 'Normal';";
    popupContentJavaScript += "optES2.value = 1;";
    popupContentJavaScript += "optES2.text = 'Fast';";
    popupContentJavaScript += "optES3.value = 2;";
    popupContentJavaScript += "optES3.text = 'Slow';";

    switch(stripAnimationProperties.animationSpeed){
        case 0:
            popupContentJavaScript += "optES1.setAttribute('selected', true);";
        break;
        case 1:
            popupContentJavaScript += "optES2.setAttribute('selected', true);";
        break;
        case 2:
            popupContentJavaScript += "optES3.setAttribute('selected', true);";
        break;
    }

    popupContentJavaScript += "enspeedSelector.appendChild(optES1);";
    popupContentJavaScript += "enspeedSelector.appendChild(optES2);";
    popupContentJavaScript += "enspeedSelector.appendChild(optES3);";

    popupContentJavaScript += "enspeedDesc.appendChild(enspeedSelector);";
    popupContentJavaScript += "popup.appendChild(enspeedDesc);";

    /* enable Animation SPEED Section END */
    /* LIVE ANIMATION */
    popupContentJavaScript += "var liveDesc = document.createElement('label');";
    popupContentJavaScript += "liveDesc.innerText = 'Live animation';";

    popupContentJavaScript += "var liveSel = document.createElement('select');";
    popupContentJavaScript += "liveSel.id = \"LiveSel\";";

    popupContentJavaScript += "var optLive1 = document.createElement(\"option\");";
    popupContentJavaScript += "var optLive2 = document.createElement(\"option\");";
    popupContentJavaScript += "var optLive3 = document.createElement(\"option\");";
    popupContentJavaScript += "optLive1.value = 0;";
    popupContentJavaScript += "optLive1.text = 'Picking';";
    popupContentJavaScript += "optLive2.value = 1;";
    popupContentJavaScript += "optLive2.text = 'Wave';";
    popupContentJavaScript += "optLive3.value = 2;";
    popupContentJavaScript += "optLive3.text = 'Double wave';";

    switch(stripAnimationProperties.liveAnimation){
        case 0:
            popupContentJavaScript += "optLive1.setAttribute('selected', true);";
        break;
        case 1:
            popupContentJavaScript += "optLive2.setAttribute('selected', true);";
        break;
        case 2:
            popupContentJavaScript += "optLive3.setAttribute('selected', true);";
        break;
    }
    popupContentJavaScript += "liveSel.appendChild(optLive1);";
    popupContentJavaScript += "liveSel.appendChild(optLive2);";
    popupContentJavaScript += "liveSel.appendChild(optLive3);";

    popupContentJavaScript += "liveDesc.appendChild(liveSel);";
    popupContentJavaScript += "popup.appendChild(liveDesc);";

    // ---------
    popupContentJavaScript += "var liveSpdDesc = document.createElement('label');";
    popupContentJavaScript += "liveSpdDesc.innerText = 'Live speed';";

    popupContentJavaScript += "var liveSpdSel = document.createElement('select');";
    popupContentJavaScript += "liveSpdSel.id = \"LiveSpdSel\";";

    popupContentJavaScript += "var optLSpd1 = document.createElement(\"option\");";
    popupContentJavaScript += "var optLSpd2 = document.createElement(\"option\");";
    popupContentJavaScript += "var optLSpd3 = document.createElement(\"option\");";
    popupContentJavaScript += "optLSpd1.value = 0;";
    popupContentJavaScript += "optLSpd1.text = 'Slow';";
    popupContentJavaScript += "optLSpd2.value = 1;";
    popupContentJavaScript += "optLSpd2.text = 'Medium';";
    popupContentJavaScript += "optLSpd3.value = 2;";
    popupContentJavaScript += "optLSpd3.text = 'Fast';";

    switch(stripAnimationProperties.liveAnimationSpeed){
        case 0:
            popupContentJavaScript += "optLSpd1.setAttribute('selected', true);";
        break;
        case 1:
            popupContentJavaScript += "optLSpd2.setAttribute('selected', true);";
        break;
        case 2:
            popupContentJavaScript += "optLSpd3.setAttribute('selected', true);";
        break;
    }

    popupContentJavaScript += "liveSpdSel.appendChild(optLSpd1);";
    popupContentJavaScript += "liveSpdSel.appendChild(optLSpd2);";
    popupContentJavaScript += "liveSpdSel.appendChild(optLSpd3);";

    popupContentJavaScript += "liveSpdDesc.appendChild(liveSpdSel);";
    popupContentJavaScript += "popup.appendChild(liveSpdDesc);";
    
    /* LIVE ANIMATION */

    popupContentJavaScript += "var memoryBtn = document.createElement('button');";
    popupContentJavaScript += "memoryBtn.innerHTML = 'Memory';";
    popupContentJavaScript += "memoryBtn.className = 'button';";
    popupContentJavaScript += "memoryBtn.onclick = openLedStripMemorySlots;";

    popupContentJavaScript += "var saveBtn = document.createElement('button');";
    popupContentJavaScript += "saveBtn.innerHTML = 'Apply';";
    popupContentJavaScript += "saveBtn.className = 'button';";

    popupContentJavaScript += "saveBtn.onclick = function () { \
    let jsonString = JSON.stringify(ledGetStateJson());\
    var url = '/setStripColor&' + jsonString;\
    const xhr = new XMLHttpRequest();\
    xhr.open(\"POST\", url, true);\
    xhr.onreadystatechange = function() {\
        if (xhr.readyState === 4) { \
            if (xhr.status === 200) { \
                const newData = JSON.parse(xhr.responseText);\
                if (JSON.stringify(newData) !== JSON.stringify(currentData)) {\
                    currentData = newData;\
                    renderRooms(currentData);\
                }\
            } else { \
                console.log('Error with AJAX request');\
            }\
        }\
    };\
    xhr.send();\
    hidePopup('advanced-ctrl-overlay', 'advanced-ctrl-popup');\
    };";
    popupContentJavaScript += "var btnContainer = document.createElement('div');";
    popupContentJavaScript += "btnContainer.className = 'button-container';";
    popupContentJavaScript += "btnContainer.appendChild(memoryBtn);";
    popupContentJavaScript += "btnContainer.appendChild(saveBtn);";
    popupContentJavaScript += "popup.appendChild(btnContainer);";

    popupContentJavaScript += "\
    function nameToRgb(name) {\
        let canvas = document.createElement('canvas');\
        let context = canvas.getContext('2d');\
        context.fillStyle = name;\
        context.fillRect(0,0,1,1);\
        let data = context.getImageData(0,0,1,1).data;\
        return [String(data[0]), String(data[1]), String(data[2])];\
    }";

    popupContentJavaScript += "function ledGetStateJson()\
    {\
        let enAnim = document.getElementById('EnAnimSelect').value;\
        let disAnim = document.getElementById('DiAnimSelect').value;\
        let enSpeed = document.getElementById('EnSpeSelect').value;\
        let liveAnim = document.getElementById('LiveSel').value;\
        let liveSpeed = document.getElementById('LiveSpdSel').value;\
        let ledState = { 'devId': "+String((int)currentlyRequestedDeviceDescription.deviceId)+", 'enableAnimation': enAnim, 'disableAnimation': disAnim, 'speed': enSpeed, 'live': liveAnim, 'liveSpd': liveSpeed ,'color': []};\
        for(let i = 0; i < "+String((int)ledsCount)+"; i++){\
            ledState.color.push(nameToRgb(ledsTab[i].childNodes[0].style.backgroundColor));\
        }\
        return ledState;\
    }";

    popupContentJavaScript += "var ledExt1 = document.getElementById('ledStripExt1');";
    popupContentJavaScript += "var ledExt2 = document.getElementById('ledStripExt2');";
    popupContentJavaScript += "var ledExt3 = document.getElementById('ledStripExt3');";
    popupContentJavaScript += "ledExt1.style.backgroundColor = 'rgb("+String((int)currentlyRequestedDeviceDescription.customBytes[5])+", "+String((int)currentlyRequestedDeviceDescription.customBytes[6])+","+String((int)currentlyRequestedDeviceDescription.customBytes[7])+")';";
    popupContentJavaScript += "ledExt2.style.backgroundColor = 'rgb("+String((int)currentlyRequestedDeviceDescription.customBytes[8])+", "+String((int)currentlyRequestedDeviceDescription.customBytes[9])+","+String((int)currentlyRequestedDeviceDescription.customBytes[10])+")';";
    popupContentJavaScript += "ledExt3.style.backgroundColor = 'rgb("+String((int)currentlyRequestedDeviceDescription.customBytes[11])+", "+String((int)currentlyRequestedDeviceDescription.customBytes[12])+","+String((int)currentlyRequestedDeviceDescription.customBytes[13])+")';";
    popupContentJavaScript += "ledStripExtCtrlId = "+String((int)currentlyRequestedDeviceDescription.deviceId)+";";
    
#endif
    return popupContentJavaScript;
}

void AdvancedControlsLoader::prepareJsStringWithAdvancedControls()
{
    if(currentlyRequestedDeviceDescription.deviceId != 255 &&
        currentAdvancedControls != nullptr){
            currentRequestJS = "";
        
            switch(currentlyRequestedDeviceDescription.deviceType){
                case type_ONOFFDEVICE:
                    currentRequestJS = createJsForOnOff();
                    break;
                case type_LED_STRIP:
                    currentRequestJS = createJsForLedStrip();
                    break;
                case type_TEMP_SENSOR:

                    break;

                default:
                    break;
            }
    }
}

String AdvancedControlsLoader::getOutpuJavaScript()
{
    String output = "";
    output = currentRequestJS;
    currentlyRequestedDeviceDescription = {};

    return output;
}

ServiceRequestErrorCode AdvancedControlsLoader::loadAdvancedControlsToJavaScript(
    uint16_t deviceIdentifier
){
    ServiceRequestErrorCode retVal = SERV_GENERAL_FAILURE;

    if(deviceIdentifier != 255){
    /* we do call for the first time, so the device loading has not started yet */
    if(currentlyRequestedDeviceDescription.deviceId != deviceIdentifier){

        std::vector<DeviceDescription> devicesCollection = 
            std::any_cast<std::vector<DeviceDescription>>(DataContainer::getSignalValue(SIG_DEVICE_COLLECTION));

        /* Find requested device information in the devices list */
        for(int i = 0 ; i < devicesCollection.size(); i++){
            if(deviceIdentifier == devicesCollection.at(i).deviceId){
                /* save currently requested device */
                currentlyRequestedDeviceDescription = devicesCollection.at(i);


                // Requested device found
                // Allocate buffer for advanced controls for this particular device type
                Serial.println("AdvancedControlsLoader:// Allocating memory for id:" + String((int)deviceIdentifier));
                allocateMemoryForControlsBasedOnDeviceType(currentlyRequestedDeviceDescription.deviceType);


                //return SERV_SUCCESS;
            }

        }
        
        if(currentlyRequestedDeviceDescription.deviceId != deviceIdentifier){
            Serial.println("AdvancedControlsLoader:// Unable to find this device");
            return SERV_GENERAL_FAILURE;
        }
    }

    /* we successfully allocated memory for the device which is requested */
    if(currentAdvancedControls != nullptr)
    {
        /* proceed with advanced controls menu download directly from the device */
        ServiceParameters_set3 parameters;
        parameters.buff = currentAdvancedControls;
        parameters.size = getControlsSizeBasedOnDevType(currentlyRequestedDeviceDescription.deviceType);
        parameters.additionalParam = 0xFF;
        parameters.direction = (uint8_t)e_OUT_from_DEVICE;

        /* call service on the device, to fulfill the controls memory */
        // Serial.println("AdvancedControlsLoader:// Waiting for the device...");
        ServiceRequestErrorCode serviceErrorCode = 
            std::any_cast<DeviceServicesAPI>(DataContainer::getSignalValue(SIG_DEVICE_SERVICES)).serviceCall_set3(
                currentlyRequestedDeviceDescription.deviceId,
                DEVSERVICE_GET_ADVANCED_CONTROLS,
                parameters
            );

        /* check what is the progress of service call execution */
        switch (serviceErrorCode)
        {
            case SERV_SUCCESS:
                /* Advanced controls successfully loaded */
                Serial.println("AdvancedControlsLoader:// Loading successfully completed.");
                prepareJsStringWithAdvancedControls();
                retVal = SERV_SUCCESS;
                break;
            
            case SERV_PENDING:
                /* loading in progress, please wait */
                retVal = SERV_PENDING;
                break;
            
            default:
                /* error with service execution */
                /* GENERAL_FAILURE will be returned */
                currentRequestJS = "hidePopup('advanced-ctrl-overlay', 'advanced-ctrl-popup');";
                /* act like nothing bad happened to the UI */
                retVal = SERV_SUCCESS;
                UserInterfaceNotification notif;
                notif.title = "Request processing failure";
                notif.body = "Could not load advanced controls from " + currentlyRequestedDeviceDescription.deviceName+ ". Please try again.";
                notif.type = UserInterfaceNotification::WARNING;
                std::any_cast<UINotificationsControlAPI>(DataContainer::getSignalValue(SIG_UI_NOTIFICATIONS_CONTROL)).createNotification(notif);
                Serial.println("Service failed with error: " + String((int)serviceErrorCode));
                break;
        }

    }else {
        Serial.println("Unable to allocate resources");
    }
    }
    return retVal;
}