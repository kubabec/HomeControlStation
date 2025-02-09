#ifndef JAVASCRIPT_H
#define JAVASCRIPT_H


const char* javascript = "\
<script>\
let isNotificationPollingActive = 1;\
    function applySettings() {\
        var isHTTPServer = document.querySelector('select[name=\"isHTTPServer\"]').value;\
        var isRCServer = document.querySelector('select[name=\"isRCServer\"]').value;\
        var isUserAdmin = document.querySelector('select[name=\"isUserAsAdmin\"]').value;\
        var nodeType = document.querySelector('input[name=\"nodetype\"]').value;\
        var SSID = document.querySelector('input[name=\"SSID\"]').value;\
        var Password = document.querySelector('input[name=\"Password\"]').value;\
        var PanelPassword = document.querySelector('input[name=\"UserPassword\"]').value;\
        var url = `/apply?isHTTPServer=${encodeURIComponent(isHTTPServer)}&isRCServer=${encodeURIComponent(isRCServer)}&isUserAdmin=${encodeURIComponent(isUserAdmin)}&SSID=${encodeURIComponent(SSID)}&Password=${encodeURIComponent(Password)}&PanelPassword=${encodeURIComponent(PanelPassword)}&nodeType=${encodeURIComponent(nodeType)}&end`;\
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
    function resetDevice(){\
        var url = `/resetDevice`;\
        window.location.href = url;\
    }\
    function onRangeTimChanged(value) {\
        value = parseInt(value);\
        window.location.assign(\"/?tim\"+value+\"&\");\
    }\
    function onRangeChanged(value, devId) {\
        value = parseInt(value);\
        var url = \"/?bri\"+value+\"DEV\"+devId+\"&\";\
        const xhr = new XMLHttpRequest();\
        xhr.timeout = 10000;\
        const container = document.getElementById('container' + devId);\
        showLoading(container);\
        xhr.open(\"GET\", url, true);\
        xhr.onreadystatechange = function() {\
            if (xhr.readyState === 4) { \
                const statusElement = document.getElementById(\"status\");\
                if (xhr.status === 200) { \
                    const newData = JSON.parse(xhr.responseText);\
                    if (JSON.stringify(newData) !== JSON.stringify(currentData)) {\
                        currentData = newData;\
                        renderRooms(currentData);\
                        console.log(currentData);\
                    }\
                } else { \
                    console.log('Error with AJAX request');\
                }\
            }\
            hideLoading(container);\
        };\
        xhr.send();\
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
            var extraValue = 0;\
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
        function showMessage(message, action) {\
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
            showPopup('popup-overlay', 'popup-content');\
\
            document.getElementById('popup-overlay-close').onclick = function () {\
                hidePopup('popup-overlay', 'popup-content');\
            };\
        }\
        function showPopup(overlay, content) {\
            const popupOverlay = document.getElementById(overlay);\
            const popupContent = document.getElementById(content);\
\
            popupOverlay.classList.remove('hidden-popup');\
            popupOverlay.classList.add('visible-popup');\
            popupContent.classList.add('show');\
        }\
\
        function hidePopup(overlay, content) {\
            const popupOverlay = document.getElementById(overlay);\
            const popupContent = document.getElementById(content);\
\
            popupOverlay.classList.remove('visible-popup');\
            popupOverlay.classList.add('hidden-popup');\
            popupContent.classList.remove('show');\
            isNotificationPollingActive = 1;\
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
                hidePopup('password-popup-overlay', 'password-popup-content');\
            };\
        }\
\
        function submitPassword() {\
            const passwordValue = document.getElementById('password-input').value;\
            alert(`Password entered: ${passwordValue}`);\
            hidePopup(document.getElementById('password-popup-overlay'), document.getElementById('password-popup-content'));\
        }\
        function submitPassword() {\
            const passwordValue = document.getElementById('password-input').value;\
            hidePopup('password-popup-overlay', 'password-popup-content');\
            url = '/passwordApply' + passwordValue;\
            window.location.href = url;\
        }\
        function hexToRgb(hex) {\
            const match = hex.match(/^#([0-9a-f]{2})([0-9a-f]{2})([0-9a-f]{2})$/i);\
            if (match) {\
                return {\
                    r: parseInt(match[1], 16),\
                    g: parseInt(match[2], 16),\
                    b: parseInt(match[3], 16)\
                };\
            }\
            return null;\
        }\
        function sendColor(stripId) {\
            const colorInput = document.getElementById(`colorInput${stripId}`);\
            const color = colorInput.value;\
            const rgbColor = hexToRgb(color);\
            if (rgbColor) {\
                const url = `/ledStripColor?id=${stripId}&r=${rgbColor.r}&g=${rgbColor.g}&b=${rgbColor.b}`;\
                window.location.href = url;\
            }\
        }\
        function openColorPicker(index) {\
            const backdrop = document.querySelector(\".popup-backdrop\");\
            selectedLedIndex = index;\
            colorInput.value = 0xFF00FA;\
            colorPickerPopup.style.display = \"flex\";\
            backdrop.style.display = \"block\";\
        }\
\
        function closePopup() {\
            const backdrop = document.querySelector(\".popup-backdrop\");\
            colorPickerPopup.style.display = \"none\";\
            backdrop.style.display = \"none\";\
        }\
        function openCompositions() {\
            const FavouritesPopup = document.getElementById(\"FavouritesPopup\");\
            const composClose = document.getElementById(\"composClose\");\
            const backdrop = document.querySelector(\".popup-backdrop\");\
            composClose.addEventListener(\"click\", closeCompositions);\
            colorInput.value = 0xFF00FA;\
            FavouritesPopup.style.display = \"flex\";\
            backdrop.style.display = \"block\";\
        }\
\
        function closeCompositions() {\
            const FavouritesPopup = document.getElementById(\"FavouritesPopup\");\
            const backdrop = document.querySelector(\".popup-backdrop\");\
            FavouritesPopup.style.display = \"none\";\
            backdrop.style.display = \"none\";\
        }\
        function openSaveCompositions() {\
            const FavouritesPopup = document.getElementById(\"SaveFavouritesPopup\");\
            const composClose = document.getElementById(\"composSaveClose\");\
            const backdrop = document.querySelector(\".popup-backdrop\");\
            composClose.addEventListener(\"click\", closeSaveCompositions);\
            colorInput.value = 0xFF00FA;\
            FavouritesPopup.style.display = \"flex\";\
            backdrop.style.display = \"block\";\
        }\
\
        function closeSaveCompositions() {\
            const FavouritesPopup = document.getElementById(\"SaveFavouritesPopup\");\
            const backdrop = document.querySelector(\".popup-backdrop\");\
            FavouritesPopup.style.display = \"none\";\
            backdrop.style.display = \"none\";\
        }\
        function rgbToHex(rgb) {\
            return rgb;\
        }\
        function colorClickedAction(index) {\
            const ledStrip = document.getElementById(\"ledStrip\");\
            const colorPickerPopup = document.getElementById(\"colorPickerPopup\");\
            const colorInput = document.getElementById(\"colorInput\");\
            const confirmButton = document.getElementById(\"confirmColorButton\");\
            const cancelButton = document.getElementById(\"cancelColorButton\");\
\
            openColorPicker(index);\
\
            confirmButton.addEventListener(\"click\", () => {\
            const rgbColor = hexToRgb(colorInput.value);\
            closePopup();\
\
            const url = `/ledColor?id=2&led=${selectedLedIndex}&r=${rgbColor.r}&g=${rgbColor.g}&b=${rgbColor.b}`;\
                window.location.href = url;\
            });\
            cancelButton.addEventListener(\"click\", closePopup);\
        }\
\
  function asyncDeviceStateSwitch(device, state){\
    const xhr = new XMLHttpRequest();\
    xhr.timeout = 10000;\
    var url = '/dev' + device.toString() + 'state';\
    const container = document.getElementById('container' + device);\
    showLoading(container);\
    if(state === 1){\
        url = url + '1&';\
    }else {\
        url = url + '0&';\
    }\
    xhr.open(\"GET\", url, true);\
    xhr.onreadystatechange = function() {\
        if (xhr.readyState === 4) { \
            const statusElement = document.getElementById(\"status\");\
            if (xhr.status === 200) { \
                const newData = JSON.parse(xhr.responseText);\
                if (JSON.stringify(newData) !== JSON.stringify(currentData)) {\
                    currentData = newData;\
                    renderRooms(currentData);\
                    console.log(currentData);\
                }\
            } else { \
                console.log('Error with AJAX request');\
            }\
        }\
        hideLoading(container);\
    };\
    xhr.send();\
  }\
  function handleJsonResponse(responseText = \"\"){\
    const response = JSON.parse(responseText);\
    if(response.status === \"succ\"){\
        if(response.type === \"switch\"){\
            var switchBtn = document.getElementById('switchBtn' + response.id);\
            var statusLight = document.getElementById('statusLight' + response.id);\
            if(response.state === \"on\"){\
                switchBtn.innerHTML = \"OFF\";\
                switchBtn.removeAttribute(\"onclick\");\
                switchBtn.onclick = function() { asyncDeviceStateSwitch(response.id, 0); };\
                statusLight.classList.remove('off');\
                statusLight.classList.add('on');\
            }else {\
                switchBtn.innerHTML = \"ON\";\
                switchBtn.removeAttribute(\"onclick\");\
                switchBtn.onclick = function() { asyncDeviceStateSwitch(response.id, 1); };\
                statusLight.classList.remove('on');\
                statusLight.classList.add('off');\
            }\
            console.log(\"Success\")\
        }else if(response.type === \"brightnessChange\"){\
            var slider = document.getElementById('brightnessSlider' + response.id);\
            slider.value = response.level;\
            console.log(\"Success\")\
        }\
    }\
  }\
  function showLoading(container) {\
    container.classList.add(\"loading\");\
    const overlay = container.querySelector(\".loading-overlay\");\
    if (overlay) {\
        overlay.style.display = \"flex\";\
    }\
}\
\
function hideLoading(container) {\
    container.classList.remove(\"loading\");\
    const overlay = container.querySelector(\".loading-overlay\");\
    if (overlay) {\
        overlay.style.display = \"none\";\
    }\
}\
function displayNotifications(data){\
    console.log(data);\
    if (data.count > 0) {\
        isNotificationPollingActive = 0;\
        const notificationsList = document.getElementById('notificationsList');\
\
        showPopup('notifications-popup-overlay', 'notificationsPopup');\
\
        notificationsList.innerHTML = \"\";\
\
        data[\"notifications:\"].forEach(notification => {\
            const notificationDiv = document.createElement('div');\
            notificationDiv.className = 'notification';\
            switch (notification.type){\
            case 0:\
                notificationDiv.classList.add('info');\
                break;\
            case 1:\
                notificationDiv.classList.add('warning');\
                break;\
            case 2:\
                notificationDiv.classList.add('error');\
                break;\
            default:\
            }\
\
            const titleDiv = document.createElement('div');\
            titleDiv.className = 'notification-title';\
            titleDiv.textContent = notification.title;\
\
            const bodyDiv = document.createElement('div');\
            bodyDiv.textContent = notification.body;\
\
            notificationDiv.appendChild(titleDiv);\
            notificationDiv.appendChild(bodyDiv);\
\
            notificationsList.appendChild(notificationDiv);\
        });\
\
        document.getElementById('notification-popup-overlay-close').onclick = function () {\
            hidePopup('notifications-popup-overlay', 'notificationsPopup');\
        };\
    }\
}\
\
        \
function fetchDataxxx() {\
                const xhr = new XMLHttpRequest();\
                var url = '/getPageContent';\
                xhr.open(\"GET\", url, true);\
                xhr.onreadystatechange = function() {\
                    if (xhr.readyState === 4) { \
                        if (xhr.status === 200) { \
                            console.log(xhr.responseText);\
                            const response = JSON.parse(xhr.responseText);\
                            console.log(response);\
                        } else { \
                            console.log('Error with AJAX request');\
                        }\
                    }\
                };\
                xhr.send();\
            }\
\
async function getNotifications() {\
    if (isNotificationPollingActive) {\
        try {\
                console.log(\"Trying to get notifications\");\
                const response = await fetch('/getNotifications');\
                const newData = await response.json();\
    \
                displayNotifications(newData);\
        } catch (error) {\
            console.error('Error fetching notifications data:', error);\
        }\ 
    };\
}\
</script>";

#endif


        

        