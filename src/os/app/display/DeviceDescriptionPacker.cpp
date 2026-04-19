#include "os/app/display/DeviceDescriptionPacker.hpp"

std::vector<std::vector<DeviceDescription>> DeviceDescriptionPacker::packDeviceDescriptions(
    const std::vector<DeviceDescription>& devices)
{
    std::vector<std::vector<DeviceDescription>> chunks;

    // If empty, return empty chunks
    if (devices.empty())
    {
        return chunks;
    }

    // Calculate number of chunks needed
    uint8_t totalChunks = getRequiredChunkCount(devices.size());

    // Split devices into chunks
    for (uint8_t chunkIndex = 0; chunkIndex < totalChunks; ++chunkIndex)
    {
        std::vector<DeviceDescription> chunk = getChunk(devices, chunkIndex);
        chunks.push_back(chunk);
    }

    return chunks;
}

std::vector<DeviceDescription> DeviceDescriptionPacker::unpackDeviceDescriptions(
    const std::vector<std::vector<DeviceDescription>>& chunks)
{
    std::vector<DeviceDescription> mergedDevices;

    // Merge all chunks into a single vector
    for (const auto& chunk : chunks)
    {
        addChunkToCollection(mergedDevices, chunk);
    }

    return mergedDevices;
}

void DeviceDescriptionPacker::addChunkToCollection(
    std::vector<DeviceDescription>& collection,
    const std::vector<DeviceDescription>& chunk)
{
    // Add all devices from the chunk to the collection
    for (const auto& device : chunk)
    {
        collection.push_back(device);
    }
}

uint8_t DeviceDescriptionPacker::getRequiredChunkCount(uint16_t deviceCount)
{
    // Calculate chunks needed using ceiling division
    if (deviceCount == 0)
    {
        return 0;
    }

    return (deviceCount + MAX_DEVICES_PER_CHUNK - 1) / MAX_DEVICES_PER_CHUNK;
}

std::vector<DeviceDescription> DeviceDescriptionPacker::getChunk(
    const std::vector<DeviceDescription>& devices,
    uint8_t chunkIndex)
{
    std::vector<DeviceDescription> chunk;

    // Calculate start and end indices for this chunk
    uint16_t startIndex = chunkIndex * MAX_DEVICES_PER_CHUNK;
    uint16_t endIndex = startIndex + MAX_DEVICES_PER_CHUNK;

    // Ensure endIndex doesn't exceed vector size
    if (endIndex > devices.size())
    {
        endIndex = devices.size();
    }

    // Return empty chunk if startIndex is out of bounds
    if (startIndex >= devices.size())
    {
        return chunk;
    }

    // Copy devices from start to end index
    for (uint16_t i = startIndex; i < endIndex; ++i)
    {
        chunk.push_back(devices[i]);
    }

    return chunk;
}
