#include <devices/OnOffDevice.hpp>


Adafruit_NeoPixel testMemoryConsumption;

OnOffDevice::OnOffDevice(int pin, String devName, uint8_t a_deviceId, uint8_t a_roomId) {
    pinNumber = pin;
    deviceName = devName;
    deviceId = a_deviceId;
    roomId = a_roomId;
    //brightnessLevel = 50;
}

void OnOffDevice::on() {
    isOn = true;
    digitalWrite(pinNumber,HIGH);
    //Serial.println("Enabling device " + deviceName +" on pin " + String(pinNumber));
}

void OnOffDevice::off() {
    isOn = false;
    digitalWrite(pinNumber,HIGH);
    //Serial.println("Disabling device " + deviceName +" on pin " + String(pinNumber));
}

bool OnOffDevice::getState() {
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
            Serial.print("Zmiana jasności : ");
            if(brightnessLevel < brightnessLevelTarget) {
                brightnessLevel ++;
                
            } 
            
            if(brightnessLevel > brightnessLevelTarget) {
                brightnessLevel --;
            } 
            Serial.println(brightnessLevel);
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
    brightnessLevelTarget = requestedBrightness;
    float brightnessDelta = abs(brightnessLevelTarget - brightnessLevel);
    brightnessStepDurationMS = brightnessChangeTime / brightnessDelta;    
    //Serial.println("Brightness Step Duration : " + String(brightnessStepDurationMS));
    timePrevious1 = millis();
}


void OnOffDevice::init() {
    
}

void OnOffDevice::cyclic() {
    brightnessChangeHandler();
    timerHandler();
    
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
