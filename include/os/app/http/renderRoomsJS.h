#ifndef RENDER_ROOMS_JS_H
#define RENDER_ROOMS_JS_H

const char* renderRoomsJS = "\
<script>\
function renderRooms(data) {\
    const roomsContainer = document.getElementById('rooms');\
\
    roomsContainer.innerHTML = '';\
\
    for (const [roomId, devices] of Object.entries(data)) {\
        const roomContainer = document.createElement('div');\
        roomContainer.className = 'room-container';\
\
        const roomHeader = document.createElement('div');\
        roomHeader.className = 'room-header';\
        roomHeader.textContent = `Room: ${roomId}`;\
        roomContainer.appendChild(roomHeader);\
        devices.forEach(device => {\
            const deviceContainer = document.createElement('div');\
            deviceContainer.className = 'container';\
            deviceContainer.id = `container${device.id}`;\
                \
            if(device.devType == 43){\
                const loadingOverlay = document.createElement('div');\
                loadingOverlay.className = 'loading-overlay';\
                loadingOverlay.style.display = 'none';\
                const spinner = document.createElement('div');\
                spinner.className = 'spinner';\
                const loadingText = document.createElement('div');\
                loadingText.className = 'loading-text';\
                loadingText.textContent = 'Loading...';\
                loadingOverlay.appendChild(spinner);\
                loadingOverlay.appendChild(loadingText);\
                deviceContainer.appendChild(loadingOverlay);\
                \
                const header = document.createElement('div');\
                header.className = 'header';\
                header.textContent = device.name;\
                deviceContainer.appendChild(header);\
    \
                const statusLight = document.createElement('div');\
                statusLight.className = `status-light ${device.status}`;\
                statusLight.id = `statusLight${device.id}`;\
                deviceContainer.appendChild(statusLight);\
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
                deviceContainer.appendChild(button);\
    \
                if(device.hasBrightness == 1){\
                    const sliderLabel = document.createElement('div');\
                    sliderLabel.className = 'header2';\
                    sliderLabel.textContent = 'Brightness';\
                    deviceContainer.appendChild(sliderLabel);\
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
            }else if(device.devType == 44){\
                const loadingOverlay = document.createElement('div');\
                loadingOverlay.className = 'loading-overlay';\
                loadingOverlay.style.display = 'none';\
                const spinner = document.createElement('div');\
                spinner.className = 'spinner';\
                const loadingText = document.createElement('div');\
                loadingText.className = 'loading-text';\
                loadingText.textContent = 'Loading...';\
                loadingOverlay.appendChild(spinner);\
                loadingOverlay.appendChild(loadingText);\
                deviceContainer.appendChild(loadingOverlay);\
                \
                const header = document.createElement('div');\
                header.className = 'header';\
                header.textContent = device.name;\
                deviceContainer.appendChild(header);\
    \
                const statusLight = document.createElement('div');\
                statusLight.className = `status-light ${device.status}`;\
                statusLight.id = `statusLight${device.id}`;\
                deviceContainer.appendChild(statusLight);\
\
                const colorPicker = document.createElement('div');\
                colorPicker.className = 'color-picker';\
                loadingText.textContent = 'Loading...';\
\
                const colorDisplay = document.createElement('div');\
                colorDisplay.className = `color-display ${device.status}`;\
                colorDisplay.style.backgroundColor = device.avgColor;\
                console.log(hexToRgb(device.avgColor));\
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
                btnContainer.appendChild(buttonMore);\
                btnContainer.appendChild(button);\
                deviceContainer.appendChild(btnContainer);\
\
                const sliderLabel = document.createElement('div');\
                sliderLabel.className = 'header2';\
                sliderLabel.textContent = 'Brightness';\
                deviceContainer.appendChild(sliderLabel);\
\
                const slider = document.createElement('input');\
                slider.type = 'range';\
                slider.min = 0;\
                slider.max = 100;\
                slider.value = device.brightness;\
                slider.onchange = () => onRangeChanged(slider.value, device.id);\
                slider.id = `brightnessSlider${device.id}`;\
                deviceContainer.appendChild(slider);\
            \
            }else{\
                const header = document.createElement('div');\
                header.className = 'header';\
                header.textContent = 'UnknownDeviceType';\
                deviceContainer.appendChild(header);\
            }\
\
            roomContainer.appendChild(deviceContainer);\
        });\
\
        roomsContainer.appendChild(roomContainer);\
    }\
}\
</script>";

#endif