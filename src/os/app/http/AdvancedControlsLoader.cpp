#include <os/app/http/AdvancedControlsLoader.hpp>

#define ONOFF_ADV_CONTROLS_SIZE 100
#define LEDSTRIP_ADV_CONTROLS_SIZE 200
#define TEMPSENSOR_ADV_CONTROLS_SIZE 50

uint8_t* AdvancedControlsLoader::currentAdvancedControls = nullptr;
DeviceDescription AdvancedControlsLoader::currentlyRequestedDeviceDescription = {};
String AdvancedControlsLoader::currentRequestJS = "";

uint16_t getControlsSizeBasedOnDevType(uint8_t deviceType){
    switch(deviceType){
        case type_ONOFFDEVICE:
            return sizeof(AdvancedControlsOnOff);
        break;

        case type_LED_STRIP:
            return LEDSTRIP_ADV_CONTROLS_SIZE;
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
        free(currentAdvancedControls);
    }

    currentAdvancedControls = nullptr;
    if(deviceType >= type_ONOFFDEVICE && deviceType <= type_DEVICE_TYPE_LAST){
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

    popupContentJavaScript += "var description = document.createElement('label');";
    popupContentJavaScript += "description.innerText = 'State change animation';";

    popupContentJavaScript += "var animSelector = document.createElement('select');";
    popupContentJavaScript += "animSelector.id = \"animSelect\";";

    popupContentJavaScript += "var option1 = document.createElement(\"option\");";
    popupContentJavaScript += "var option2 = document.createElement(\"option\");";
    popupContentJavaScript += "option1.value = 1;";
    popupContentJavaScript += "option1.text = 'switch';";
    popupContentJavaScript += "animSelector.appendChild(option1);";
    popupContentJavaScript += "option2.value = 2;";
    popupContentJavaScript += "option2.text = 'fade';";
    popupContentJavaScript += "animSelector.appendChild(option2);";
    popupContentJavaScript += "animSelector.onchange = function() {\
        var animSelector = document.getElementById('animSelect');\
        console.log('select change ' + animSelector.value);\
        var animeTimeVal = document.getElementById('animTimeVal');\
        var animeTimeLabel = document.getElementById('animTimeLabel');\
        if(animSelector.value == 1){\
            animeTimeVal.style.display = 'none';\
            animeTimeLabel.style.display = 'none';\
        }\
        else\
        {\
            animeTimeVal.style.display = '';\
            animeTimeLabel.style.display = '';\
        }\
    };";


    popupContentJavaScript += "var animTime = document.createElement('label');";
    popupContentJavaScript += "animTime.innerText = 'Switch animation time:';";
    popupContentJavaScript += "animTime.id = 'animTimeLabel';";

    popupContentJavaScript += "var animTimeVal = document.createElement('input');";
    popupContentJavaScript += "animTimeVal.id = \"animTimeVal\";";
    popupContentJavaScript += "animTimeVal.type = \"text\";";
    popupContentJavaScript += "animTimeVal.value = \""+String((int)controls.switchAnimationTime)+"\";";

    popupContentJavaScript += "popup.appendChild(description);";
    popupContentJavaScript += "popup.appendChild(animSelector);";

    popupContentJavaScript += "popup.appendChild(animTime);";
    popupContentJavaScript += "popup.appendChild(animTimeVal);";

    popupContentJavaScript += "animSelector.onchange();";

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
        Serial.println("AdvancedControlsLoader:// Waiting for the device...");
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
                Serial.println("Service failed with error: " + String((int)serviceErrorCode));
                break;
        }

    }else {
        Serial.println("Unable to allocate resources");
    }
    }
    return retVal;
}