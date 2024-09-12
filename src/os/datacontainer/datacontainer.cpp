#include <os/datacontainer/DataContainer.hpp>



std::array<std::any, NUMBER_OF_SIGNALS>  DataContainer::dataTable;

std::array<
    std::vector<std::pair<std::string, std::function<void(std::any)>>
            >, NUMBER_OF_SIGNALS> DataContainer::subscribers;

// std::string getCurrentTime() {
//     std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
//     std::string ts = std::ctime(&t);
//     ts = "[" + ts;
//     ts.resize(ts.size()-1);
//     ts = ts + "]";
//     return ts;
// }

DataContainer::DataContainer()
{

}

void DataContainer::subscribe(Signal sigName, std::string subscriberName, std::function<void (std::any)> dupa)
{
    Serial.println(String(subscriberName.c_str()) + " has been subscribed to signal " + String(signalNames[sigName].c_str()));
    subscribers.at(sigName).push_back({subscriberName, dupa});
}

std::any DataContainer::getSignalValue(Signal sigName)
{
    if(dataTable.at(sigName).has_value())
    {
        return dataTable.at(sigName);
    }else
    {
        Serial.println("Data Container - No signal "+ String(signalNames[sigName].c_str()) + " value set");
        return std::any();
    }
}
