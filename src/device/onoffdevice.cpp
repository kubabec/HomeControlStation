#include <devices/OnOffDevice.hpp>

OnOffDevice::OnOffDevice(int pin, String devName, uint8_t a_deviceId, uint8_t a_roomId) {
    pinNumber = pin;
    deviceName = devName;
    deviceId = a_deviceId;
    roomId = a_roomId;

    //brightnessLevel = 50;
}


OnOffDevice::OnOffDevice(DeviceDescription& description, uint8_t pin){
    deviceName = description.deviceName;
    deviceId = description.deviceId;
    roomId = description.deviceId;

    pinNumber = pin;
}

void OnOffDevice::on() {
    isOn = true;
    digitalWrite(pinNumber,LOW);
    //Serial.println("Enabling device " + deviceName +" on pin " + String(pinNumber));
}

void OnOffDevice::off() {
    isOn = false;
    digitalWrite(pinNumber,HIGH);
    //Serial.println("Disabling device " + deviceName +" on pin " + String(pinNumber));
}

bool OnOffDevice::getState() {
    Serial.println("@@@@@@@@@@@@@@@@@@@@ onOffDevice - get state : " + String(isOn));
    return isOn;
       
}

void OnOffDevice::setBrightnessLevelSupport(bool p_brightnessLevelSupport) {
    brightnessLevelSupport = p_brightnessLevelSupport;
}

int OnOffDevice::getBrighnessStep() {
    return brighnessStep;
}
int OnOffDevice::getBrightnessStepDuration() {
    return brightnessStepDurationMS;
}

void OnOffDevice::brightnessChangeHandler() {
    
    if(brightnessLevel != brightnessLevelTarget) {        
        if ((millis() - timePrevious1) >= brightnessStepDurationMS) {
            //Serial.print("Zmiana jasności : ");
            if(brightnessLevel < brightnessLevelTarget) {
                brightnessLevel ++;
                
            } 
            
            if(brightnessLevel > brightnessLevelTarget) {
                brightnessLevel --;
            } 
            //Serial.println(brightnessLevel);
            timePrevious1 = millis(); 
        }        
    }  
    
}

void OnOffDevice::timerHandler() {
    if(lightDurationTimerMS > 0) {
        
        if ((millis() - timePrevious2) > lightDurationTimerMS) {
            
                //Serial.println("!!!!!!!!!!!!!!!Light OFF !!!!!!!!!!!!! ");
                timePrevious2 = millis();
                off();
                setLightDurationTimerMS(0);
                
            }        
             
        }     
    }  

void OnOffDevice::changeBrightness(int requestedBrightness) {
    Serial.println("Requested: " + String(requestedBrightness));
    brightnessLevelTarget = requestedBrightness;
    float brightnessDelta = abs(brightnessLevelTarget - brightnessLevel);
    brightnessStepDurationMS = brightnessChangeTime / brightnessDelta;    
    //Serial.println("Brightness Step Duration : " + String(brightnessStepDurationMS));
    timePrevious1 = millis();
}


void OnOffDevice::init() {
    pinMode(pinNumber,OUTPUT);
    off();


    controls.switchAnimationTime = 200 + deviceId;
    //Serial.println("Device " + deviceName + " initialized on pin " + String(pinNumber));
    
}

void OnOffDevice::cyclic() {
    brightnessChangeHandler();
    timerHandler();
    
}

uint16_t OnOffDevice::getExtendedMemoryLength(){
    return 0;
}

int OnOffDevice::getBrightnessLevel() {
    return brightnessLevel;
}
    
int OnOffDevice::getBrightnessLevelTarget() {
    return brightnessLevelTarget;
}

void OnOffDevice::setLightDurationTimerMS(int requestedLightDuration) {
    lightDurationTimerMS = requestedLightDuration * 1000;
    //Serial.println();
    //Serial.print("========Timer : ");
    //Serial.println(lightDurationTimerMS);
}

int OnOffDevice::getLightDurationTimerMS() {
    return lightDurationTimerMS;
}

String OnOffDevice::getName() {
    return deviceName;
}

uint8_t OnOffDevice::getDeviceId() {
    return deviceId;
}

uint8_t OnOffDevice::getRoomId() {
    return roomId;
}

bool OnOffDevice::getBrightnessIsAdjustable() {
    return brightnessLevelSupport;
}

uint8_t OnOffDevice::getDeviceIdentifier(){

    return deviceId;
}
uint8_t OnOffDevice::getDeviceType(){
    return 5;
}

ServiceRequestErrorCode OnOffDevice::service(DeviceServicesType serviceType){
    switch(serviceType){
        

        default: 
            Serial.println("Device_"+String((int)deviceId)+":Service {"+ String((int)serviceType) + "} is not supported (noParam)");
            return SERV_NOT_SUPPORTED;
    };
}
ServiceRequestErrorCode OnOffDevice::service(DeviceServicesType serviceType, ServiceParameters_set1 param){
    switch(serviceType){
        case DEVSERVICE_STATE_SWITCH:
            if(param.a ==1) {
                on();
                Serial.println("Setting state: ON");
            }
            else {
                off();
                Serial.println("Setting state: OFF");
            }
            return SERV_SUCCESS;

        case DEVSERVICE_BRIGHTNESS_CHANGE:
            changeBrightness(param.a);
            Serial.println("Changing brightness to "+String((int)param.a));
            return SERV_SUCCESS;
        
        default: 
            Serial.println("Device_"+String((int)deviceId)+":Service {"+ String((int)serviceType) + "} is not supported (param1)");
            return SERV_NOT_SUPPORTED;
    };
}
ServiceRequestErrorCode OnOffDevice::service(DeviceServicesType serviceType, ServiceParameters_set2 param){
    switch(serviceType){
        default: 
            Serial.println("Device_"+String((int)deviceId)+":Service {"+ String((int)serviceType) + "} is not supported (param2)");
            return SERV_NOT_SUPPORTED;
    };
}
ServiceRequestErrorCode OnOffDevice::service(DeviceServicesType serviceType, ServiceParameters_set3 param){
    switch(serviceType){
        case DEVSERVICE_GET_ADVANCED_CONTROLS:
            Serial.println("DEVSERVICE_GET_ADVANCED_CONTROLS");
            if(param.size == sizeof(AdvancedControlsOnOff)){
                memcpy(param.buff, &controls, sizeof(AdvancedControlsOnOff));
            }

            return SERV_SUCCESS;
        break;

        default: 
            Serial.println("Device_"+String((int)deviceId)+":Service {"+ String((int)serviceType) + "} is not supported (param3)");
            return SERV_NOT_SUPPORTED;
    };
}

DeviceDescription OnOffDevice::getDeviceDescription(){
    DeviceDescription desc;
    desc.deviceType = type_ONOFFDEVICE;
    desc.deviceId = deviceId;
    desc.roomId = roomId;
    desc.isEnabled = isOn;
    desc.deviceName = deviceName;
    memset(desc.customBytes, 0x00, NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION);
    desc.customBytes[0] = brightnessLevelSupport;
    desc.customBytes[1] = brightnessLevelTarget;
    desc.customBytes[2] = brightnessLevel;

    return desc;
}
