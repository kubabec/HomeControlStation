#ifndef DATACONTAINER_H
#define DATACONTAINER_H
#include <array>
#include <vector>
#include <queue>
#include <functional>
#include <string>
#include <any>
#include <chrono>
#include <iostream>
#include <Arduino.h>
#include "DataContainerTypes.hpp"
#include "Signals.hpp"


class DataContainer
{
    // Signals values container
    static std::array<std::any, NUMBER_OF_SIGNALS> dataTable;

    // Signals subscribers container
    static std::array<std::vector<std::pair<std::string, std::function<void(std::any)>>>, NUMBER_OF_SIGNALS> subscribers;
public:
    DataContainer();
    // Register new subscriber (listener) waiting on signal change notification
    static void subscribe(Signal sigName, std::string subscriberName, std::function<void(std::any)> callback);

    // Set signal value to newValue and notify all the signals listeners (subscribers)
    template <typename Type>
    static void setSignalValue(Signal sigName, std::string senderName, Type newValue)
    {
        dataTable.at(sigName) = newValue; //Aktualizuje wartość powiązaną z określonym sigName w tabeli danych (dataTable)
        
 
        //Serial.println("New value - "+ String(newValue) +" - received for signal - "+ 
                //String(signalNames[sigName].c_str()) +" - from sender " + String(senderName.c_str()));
        
        for(const auto& subscriber: subscribers.at(sigName)) // Iteruje po subskrybentach powiązanych z danym sigName.
        {
           // Serial.println("Notifying subscriber: " + String(subscriber.first.c_str()) );
            subscriber.second(newValue);   //Wywołuje funkcję zwrotną powiązaną z subskrybentem, przekazując nową wartość (newValue) jako argument.
            
        }
    }

    // Force ask for signal value
    static std::any getSignalValue(Signal sigName);

};

#endif // DATACONTAINER_H
