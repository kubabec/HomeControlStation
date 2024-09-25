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
    window.onload = function() {\
        document.querySelectorAll('.device-container').forEach(container => {\
            var checkbox = container.querySelector('input[type=\"checkbox\"]');\
            toggleDeviceConfig(checkbox);\
            checkbox.addEventListener('change', function() {\
                toggleDeviceConfig(this);\
            });\
        });\
        document.getElementById('button1').addEventListener('click', function() {\
            window.location.href = 'http://example.com/link1';\
        });\
        document.getElementById('button2').addEventListener('click', function() {\
            window.location.href = 'http://example.com/link2';\
        });\
    };\
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
        var url = '/configUpload';\
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
            var dataId = document.getElementById('identifier' + i).value;\
            var dataName = document.getElementById('name' + i).value;\
            var dataType = document.getElementById('type' + i).value;\
            var dataPin = document.getElementById('pin' + i).value;\
            var dataRoom = document.getElementById('room' + i).value;\
            \
\
            deviceConfigurationString = dataEnable + dataId + dataName.length + dataName + dataType + dataPin + dataRoom;\
            var lengthCount = deviceConfigurationString.length;\
            deviceConfigurationString = lengthCount + deviceConfigurationString;\
            url = url + deviceConfigurationString;\
        }\
        window.location.href = url;\
    };\
</script>";

#endif