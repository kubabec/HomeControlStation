#ifndef DEVICE_CONFIG_WIDGETS_FALLBACK_HPP
#define DEVICE_CONFIG_WIDGETS_FALLBACK_HPP

#include <WiFi.h>
#include <Arduino.h>
#include "os/datacontainer/NvmConfigSlotDefinition.hpp"

namespace GeneratedDeviceConfigWidgets
{
inline void emitTypeSelector(uint8_t slotNumber, uint8_t, WiFiClient& client)
{
    client.println("<label>Type: <select onchange=\"showExtraFields(this, 'device-" + String((int)slotNumber) + "');\" id=\"type" + String((int)slotNumber) + "\">");
    client.println("<option value=\"255\" selected>UNKNOWN</option>");
    client.println("</select></label>");
}

inline void emitGeneratedCustomFields(uint8_t, uint8_t, const DeviceConfigSlotType&, WiFiClient&)
{
}
}

#endif
