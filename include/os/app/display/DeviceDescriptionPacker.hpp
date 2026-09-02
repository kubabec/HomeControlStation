#pragma once

#include <vector>
#include <cstdint>
#include "devices/device.hpp"

/**
 * @file include/os/app/display/DeviceDescriptionPacker.hpp
 * @brief Display rendering and presentation definitions for the Home Control Station.
 */


// This class is responsible for split of device description when there is too many (more than 5 devices) in global signal, 
// in orded to do not overload the UDP packet with the data. When DisplayServer updates the DeviceDescription vector on the concrete Display,
// data will be send in chunks of 5 devices, and this class will be responsible for spliting the data into correct chunks and sending them one by one to the DisplayServer.
// On the client side, this class will be responsible for merge of received chunks of DeviceDescription into one vector and update the global signal with the merged data.
// Class provides static methods for both server and client side, so it can be used in both DisplayServer and DisplayClient.
/**
 * @class DeviceDescriptionPacker
 * @brief Splits and merges device-description vectors across UDP chunk boundaries.
 *
 * The packet size used by the display and remote-control protocols is limited, so the packer splits a
 * large device-description collection into fixed-size chunks and reassembles it on the receiving side.
 */

class DeviceDescriptionPacker
{
private:
    /**
     * Maximum number of device descriptions stored in a single UDP chunk.
     */
    static constexpr uint8_t MAX_DEVICES_PER_CHUNK = 5;

public:
    /**
     * Splits a full device-description list into fixed-size chunks for network transfer.
     * @param devices Full collection of device descriptions to serialize.
     * @return Vector of chunks, each containing at most MAX_DEVICES_PER_CHUNK items.
     */
    static std::vector<std::vector<DeviceDescription>> packDeviceDescriptions(
        const std::vector<DeviceDescription>& devices);

    /**
     * Reassembles a full device-description list from a collection of network chunks.
     * @param chunks Chunks received over the network.
     * @return Combined device-description vector in original order.
     */
    static std::vector<DeviceDescription> unpackDeviceDescriptions(
        const std::vector<std::vector<DeviceDescription>>& chunks);

    /**
     * Adds a single received chunk to an already-growing output collection.
     * @param collection Collection being assembled on the receiving side.
     * @param chunk New chunk to append to the collection.
     */
    static void addChunkToCollection(
        std::vector<DeviceDescription>& collection,
        const std::vector<DeviceDescription>& chunk);

    /**
     * Calculates how many chunks are needed to represent a given device count.
     * @param deviceCount Number of device descriptions to be transferred.
     * @return Required number of chunks.
     */
    static uint8_t getRequiredChunkCount(uint16_t deviceCount);

    /**
     * Returns the content of a specific chunk from a device vector.
     * @param devices Full device list.
     * @param chunkIndex Index of the chunk to return.
     * @return Selected chunk of device descriptions.
     */
    static std::vector<DeviceDescription> getChunk(
        const std::vector<DeviceDescription>& devices,
        uint8_t chunkIndex);
};