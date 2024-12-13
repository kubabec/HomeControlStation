#ifndef JAVASCRIPT_H
#define JAVASCRIPT_H


const char* javascript = "\
<script>\
    function applySettings() {\
        var isHTTPServer = document.querySelector('select[name=\"isHTTPServer\"]').value;\
        var isRCServer = document.querySelector('select[name=\"isRCServer\"]').value;\
        var nodeId = document.querySelector('input[name=\"nodeid\"]').value;\
        var nodeType = document.querySelector('input[name=\"nodetype\"]').value;\
        var SSID = document.querySelector('input[name=\"SSID\"]').value;\
        var Password = document.querySelector('input[name=\"Password\"]').value;\
        var url = `/apply?isHTTPServer=${encodeURIComponent(isHTTPServer)}&isRCServer=${encodeURIComponent(isRCServer)}&SSID=${encodeURIComponent(SSID)}&Password=${encodeURIComponent(Password)}&nodeId=${encodeURIComponent(nodeId)}&nodeType=${encodeURIComponent(nodeType)}`;\
        window.location.href = url;\
    }\
    function goToDevicesManagement() {\
        var url = `/localDevices`;\
        window.location.href = url;\
    }\
    function goToRoomSettings() {\
        var url = `/roomAssignment`;\
        window.location.href = url;\
    }\
    function massErase(){\
        var url = `/masseraseviahttp`;\
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
    function toggleDeviceConfig(checkbox) {\
        var container = checkbox.closest('.device-container');\
        var statusText = container.querySelector('.status-text');\
        if (checkbox.checked) {\
            container.classList.remove('disabled');\
            statusText.textContent = 'Enabled';\
            container.querySelectorAll('input[type=\"text\"]').forEach(input => input.disabled = false);\
        } else {\
            container.classList.add('disabled');\
            statusText.textContent = 'Disabled';\
            container.querySelectorAll('input[type=\"text\"]').forEach(input => input.disabled = true);\
        }\
    }\
    function showExtraFields(select, deviceId) {\
        const deviceContainer = document.getElementById(deviceId);\
        const extraFields = deviceContainer.querySelectorAll('.extra-fields');\
        extraFields.forEach(field => {\
            field.classList.remove('visible');\
        });\
        const selectedValue = select.value;\
        const fieldsToShow = deviceContainer.querySelector(`.extra-${selectedValue}`);\
        if (fieldsToShow) {\
            fieldsToShow.classList.add('visible');\
        }\
    };\
    function createConfigurationString()\
    {\
        var configStr = '';\
        var url = '';\
        var lengthCount = 0;\
        var crc = 0;\
        for (let i = 1; i <= 6; i++) {\
            const container = document.getElementById(\"device-\"+i);\
            var enable = \"enabled\" + i;\
            var enableValue = document.getElementById(enable).checked;\
            \
            var dataEnable = '0';\
            if(enableValue == true){\
                dataEnable = '1';\
            }\
\
            var dataId = i;\
            if(dataId < 10) { dataId = '0' + dataId; }\
            var dataName = document.getElementById('name' + i).value;\
            var dataType = document.getElementById('type' + i).value;\
            var dataPin = document.getElementById('pin' + i).value;\
            if(dataPin < 10) { dataPin = '0' + dataPin; }\
            var dataRoom = document.getElementById('room' + i).value;\
            if(dataRoom < 10) { dataRoom = '0' + dataRoom; }\
            \
\
            var data43 =  document.getElementById('extra-43-' + i).value;\
            var data44 =  document.getElementById('extra-44-' + i).value;\
            var extraValue = '';\
            if(dataType == 43) { extraValue = data43;}\
            if(dataType == 44) { extraValue = data44;}\
            if(extraValue < 10) { extraValue = '0' + extraValue; }\
            var nameLength = dataName.length;\
            if(nameLength < 10) { nameLength = '0' + nameLength; }\
            deviceConfigurationString = dataEnable + dataId + nameLength + dataName + dataType + dataPin + dataRoom + extraValue;\
            stringLength = deviceConfigurationString.length;\
            stringLengthBytes = 1;\
            if(stringLength > 10){\
                stringLengthBytes = 2;\
            }\
\
\
            lengthCount = lengthCount + 1 + stringLengthBytes + deviceConfigurationString.length;\
            url = url + stringLengthBytes + stringLength + deviceConfigurationString;\
        }\
        var lengthCountLength = 2;\
        if(lengthCount > 99){\
            lengthCountLength = 3;\
        }\
        url = lengthCountLength.toString() + lengthCount.toString() + url;\
        for(let i = 0; i < url.length; i++){\
            crc = crc + Number(url.charCodeAt(i));\
        }\
        url = '/localSetup' + url;\
\
        url = url + crc.toString().length + crc;\
        window.location.href = url;\
    };\
    function roomMappingCreateString(count)\
    {\
        var mappingStr = '';\
        var lengthCount = 0;\
        for (let i = 1; i <= count; i++) {\
            var dataId = document.getElementById(\"roomMappingID\" + i).value;\
            var idLength = dataId.length;\
            var dataName = document.getElementById(\"roomMappingName\" + i).value;\
            var nameLength = dataName.length;\
            \
            var idLengthLength = 1;\
            \
            mappingStr = mappingStr + idLength + dataId + nameLength.toString().length + nameLength + dataName;\
            lengthCount = lengthCount + 1  + dataId.length + 1 + nameLength.toString().length + dataName.length;\
        }\
        mappingStr = lengthCount.toString().length.toString() + lengthCount + mappingStr;\
        url = '/roomMappingApply' + mappingStr;\
        window.location.href = url;\
    };\
    window.onload = function() {\
        document.querySelectorAll('.device-container').forEach(container => {\
            var checkbox = container.querySelector('input[type=\"checkbox\"]');\
            toggleDeviceConfig(checkbox);\
            checkbox.addEventListener('change', function() {\
                toggleDeviceConfig(this);\
            });\
        });\
\
        showExtraFields(document.getElementById('type1'), 'device-1');\
        showExtraFields(document.getElementById('type2'), 'device-2');\
        showExtraFields(document.getElementById('type3'), 'device-3');\
        showExtraFields(document.getElementById('type4'), 'device-4');\
        showExtraFields(document.getElementById('type5'), 'device-5');\
        showExtraFields(document.getElementById('type6'), 'device-6');\
    };\
\
\
        function showPopup(message, action) {\
            const popupOverlay = document.getElementById('popup-overlay');\
            const popupContent = document.getElementById('popup-content');\
            const popupMessage = document.getElementById('popup-message');\
            const popupButton = document.getElementById('popup-button');\
\
            popupMessage.textContent = message;\
\
            if (typeof action === 'string') {\
                popupButton.onclick = function () {\
                    window.location.href = action;\
                };\
            } else if (typeof action === 'function') {\
                popupButton.onclick = function () {\
                    action();\
                };\
            }\
\
            popupOverlay.classList.remove('hidden-popup');\
            popupOverlay.classList.add('visible-popup');\
            popupContent.classList.add('show');\
\
            document.getElementById('popup-close').onclick = function () {\
                hidePopup();\
            };\
        }\
\
        function hidePopup() {\
            const popupOverlay = document.getElementById('popup-overlay');\
            const popupContent = document.getElementById('popup-content');\
\
            popupOverlay.classList.remove('visible-popup');\
            popupOverlay.classList.add('hidden-popup');\
            popupContent.classList.remove('show');\
        };\
        function showPasswordPopup() {\
            const passwordPopupOverlay = document.getElementById('password-popup-overlay');\
            const passwordPopupContent = document.getElementById('password-popup-content');\
\
            passwordPopupOverlay.classList.remove('hidden-popup');\
            passwordPopupOverlay.classList.add('visible-popup');\
            passwordPopupContent.classList.add('show');\
\
            document.getElementById('password-popup-close').onclick = function () {\
                hidePopup(passwordPopupOverlay, passwordPopupContent);\
            };\
        }\
        function hidePopup(popupOverlay, popupContent) {\
            popupOverlay.classList.remove('visible-popup');\
            popupOverlay.classList.add('hidden-popup');\
            popupContent.classList.remove('show');\
        }\
\
        function submitPassword() {\
            const passwordValue = document.getElementById('password-input').value;\
            alert(`Password entered: ${passwordValue}`);\
            hidePopup(document.getElementById('password-popup-overlay'), document.getElementById('password-popup-content'));\
        }\
        function submitPassword() {\
            const passwordValue = document.getElementById('password-input').value;\
            hidePopup(document.getElementById('password-popup-overlay'), document.getElementById('password-popup-content'));\
            url = '/passwordApply' + passwordValue;\
            window.location.href = url;\
        }\
</script>";

#endif
            //        deviceConfigurationString = deviceConfigurationString + crc.toString().length + crc + \" \";\

        //window.location.href = url;\


            // crc = crc + Number(enableValue);\
            // crc = crc + Number(dataId);\
            // crc = crc + Number(dataType);\
            // crc = crc + Number(dataPin);\
            // crc = crc + Number(dataRoom);\
            // crc = crc + Number(nameLength);\
            // crc = crc + Number(extraValue);\
            // for(let i = 0; i < dataName.length; i++){\
            //     crc = crc + dataName.charCodeAt(i);\
            // }\
            // crc = crc + Number(stringLength) + Number(stringLengthBytes);\