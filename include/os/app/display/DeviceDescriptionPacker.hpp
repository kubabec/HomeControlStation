#pragma once

#include <vector>
#include <cstdint>
#include "devices/device.hpp"

// This class is responsible for split of device description when there is too many (more than 5 devices) in global signal, 
// in orded to do not overload the UDP packet with the data. When DisplayServer updates the DeviceDescription vector on the concrete Display,
// data will be send in chunks of 5 devices, and this class will be responsible for spliting the data into correct chunks and sending them one by one to the DisplayServer.
// On the client side, this class will be responsible for merge of received chunks of DeviceDescription into one vector and update the global signal with the merged data.
// Class provides static methods for both server and client side, so it can be used in both DisplayServer and DisplayClient.

class DeviceDescriptionPacker
{
private:
    static constexpr uint8_t MAX_DEVICES_PER_CHUNK = 5;

public:
    // Server-side method: Split a vector of DeviceDescription into chunks
    // Returns a vector of vectors, where each inner vector contains up to MAX_DEVICES_PER_CHUNK devices
    static std::vector<std::vector<DeviceDescription>> packDeviceDescriptions(
        const std::vector<DeviceDescription>& devices);

    // Client-side method: Merge received chunks of DeviceDescription into one vector
    // Accumulates device descriptions from chunks and returns the complete merged vector
    static std::vector<DeviceDescription> unpackDeviceDescriptions(
        const std::vector<std::vector<DeviceDescription>>& chunks);

    // Client-side helper: Add a single chunk to a growing collection
    // Used when receiving chunks one by one over time
    static void addChunkToCollection(
        std::vector<DeviceDescription>& collection,
        const std::vector<DeviceDescription>& chunk);

    // Get the number of chunks needed for a given number of devices
    static uint8_t getRequiredChunkCount(uint16_t deviceCount);

    // Get the chunk at the specified index
    static std::vector<DeviceDescription> getChunk(
        const std::vector<DeviceDescription>& devices,
        uint8_t chunkIndex);
};