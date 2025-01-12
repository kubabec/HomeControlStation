***Project under development . . . *
**
### Description

**HomeControlStation** is an open source project developed in order to simplify usage of cheap ESP32 board to control popular home automation usecases, e.g.:
- Light switching
- Color LED strip with customization
- Temperature check
- RC via customizable IrDA controler

Future plans:
- Integration with Zigbee controls
- Integration with 3rd party temperature sensors
- Integration with MQTT server
- Creation of touch panel communicating with the system

## Goal

**HomeControlStation** enables user to connect multiple ESP32 chips within local WiFi network with all the periperals needed. Each device provides user friendly UI to control its previously configured GPIOs, but additionally there is one node acting as a master in the communication, which collects all the neccessary data from slaves and presents all the devices together in one common user interface divided into room categories. 


## Demo



## Usage

GPIO pin must be connected either to PWM controlled external switch module (e.g. to control cheap LED chain) or directly to relay switch pin to control On/Off device with possible brigthness adjustment via PWM.  

GPIO can also be connected to control line of WS28B LED strip to control the leds via user interface. 



```

###End
