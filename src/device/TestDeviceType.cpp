#include "devices/TestDeviceType.hpp"

void TestDeviceType::init(){

}

void TestDeviceType::cyclic(){

}

uint8_t TestDeviceType::getDeviceIdentifier(){
    return 11;
}
uint8_t TestDeviceType::getDeviceType(){
    return 5;
}

ServiceRequestErrorCode TestDeviceType::service(DeviceServicesType serviceType){
    switch(serviceType){
        case DEVSERVICE_ENABLE:
            /* do somethign */
            break;
        case DEVSERVICE_DISABLE:
            /* do something*/
            break;

        default: 
            return SERV_NOT_SUPPORTED;
    };
}
ServiceRequestErrorCode TestDeviceType::service(DeviceServicesType serviceType, ServiceParameters_set1 param){
    switch(serviceType){
        case DEVSERVICE_SET_DATA:
            /* data = param.a ;*/
            break;
        default: 
            return SERV_NOT_SUPPORTED;
    };
}
ServiceRequestErrorCode TestDeviceType::service(DeviceServicesType serviceType, ServiceParameters_set2 param){
    switch(serviceType){
        default: 
            return SERV_NOT_SUPPORTED;
    };
}
ServiceRequestErrorCode TestDeviceType::service(DeviceServicesType serviceType, ServiceParameters_set3 param){
    switch(serviceType){
        default: 
            return SERV_NOT_SUPPORTED;
    };
}