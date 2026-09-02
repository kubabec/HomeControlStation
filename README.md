<img src="https://github.com/kubabec/HomeControlStation/blob/main/demo/img1.jpeg" width=50% height=50%>


<img src="https://github.com/kubabec/HomeControlStation/blob/main/demo/img2.jpeg" width=50% height=50%>

***Project under development . . . *
**
### Description

> **New to the project?** Start with the [architecture and end-to-end flow guide](ARCHITECTURE.md) for the Platform/Devices boundary, build-time generation, runtime lifecycle, and ESP32 UDP network diagrams.

> **Adding hardware support?** Follow [Adding a new device type](ADDING_A_NEW_DEVICE_TYPE.md) for the C++ interface, JSON metadata, services, generated UI, byte layouts, and optional advanced controls.

> **Building a device preset?** A clean checkout contains no active concrete devices. Overlay both the `include/` and `src/` trees of only the required packages from [DevicesPredefined](DevicesPredefined) before running `platformio run`.

**HomeControlStation** is an open source project developed in order to simplify usage of cheap ESP32 board to control popular home automation usecases, e.g.:
- Light switching
- Color LED strip with customization
- Temperature check
- RC via customizable IrDA controler

![System overview](https://github.com/kubabec/HomeControlStation/blob/main/ArchitectureOverview.png "System overview")

Future plans:
- Integration with Zigbee controls
- Integration with 3rd party temperature sensors
- Integration with MQTT server
- Creation of touch panel communicating with the system

## Goal

**HomeControlStation** enables user to connect multiple ESP32 chips within local WiFi network with all the periperals needed. Each device provides user friendly UI to control its previously configured GPIOs, but additionally there is one node acting as a master in the communication, which collects all the neccessary data from slaves and presents all the devices together in one common user interface divided into room categories. 


## Demo


![Main page](https://github.com/kubabec/HomeControlStation/blob/main/demo/Zrzut%20ekranu%202025-05-31%20o%2017.06.34.png?raw=false "Main page")
![Main page](https://github.com/kubabec/HomeControlStation/blob/main/demo/Zrzut%20ekranu%202025-05-31%20o%2017.07.55.png?raw=false "Pinout configuration")
![Main page](https://github.com/kubabec/HomeControlStation/blob/main/demo/Zrzut%20ekranu%202025-05-31%20o%2017.08.20.png?raw=false "Multi-node inspection")


## Usage

GPIO pin must be connected either to PWM controlled external switch module (e.g. to control cheap LED chain) or directly to relay switch pin to control On/Off device with possible brigthness adjustment via PWM.  

GPIO can also be connected to control line of WS28B LED strip to control the leds via user interface. 



```

###End
