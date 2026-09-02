#include "TestHarness.hpp"
#include "Arduino.h"

#include "devices/device.hpp"
#include "os/app/display/DeviceDescriptionPacker.hpp"
#include "os/app/remoteControl/rcRequest.hpp"
#include "os/app/remoteControl/rcResponse.hpp"
#include "os/datacontainer/datacontainer.hpp"
#include "os/tools/ExtendedDataAllocator.hpp"
#include "os/tools/messageUDP.hpp"

#include <array>
#include <cstring>

TEST_CASE("DeviceDescription round-trips all fields")
{
    DeviceDescription source;
    source.deviceType = type_GATE; source.macAddress = 0x0102030405060708ULL;
    source.deviceId = 7; source.roomId = 3; source.isEnabled = 1; source.deviceName = "Garage";
    for (uint8_t index = 0; index < NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION; ++index) source.customBytes[index] = index;
    std::vector<uint8_t> bytes(source.getSize());
    EXPECT_TRUE(source.toByteArray(bytes.data(), bytes.size()));

    DeviceDescription decoded;
    EXPECT_TRUE(decoded.fromByteArray(bytes.data(), bytes.size()));
    EXPECT_EQ(decoded.deviceType, source.deviceType); EXPECT_EQ(decoded.macAddress, source.macAddress);
    EXPECT_EQ(decoded.deviceId, source.deviceId); EXPECT_EQ(decoded.roomId, source.roomId);
    EXPECT_EQ(decoded.isEnabled, source.isEnabled); EXPECT_EQ(decoded.deviceName, source.deviceName);
    EXPECT_TRUE(std::memcmp(decoded.customBytes, source.customBytes, sizeof(source.customBytes)) == 0);
}

TEST_CASE("DeviceDescription rejects invalid buffers and oversized names")
{
    DeviceDescription description;
    std::vector<uint8_t> bytes(description.getSize());
    EXPECT_FALSE(description.toByteArray(nullptr, bytes.size()));
    EXPECT_FALSE(description.toByteArray(bytes.data(), bytes.size() - 1));
    description.deviceName = "1234567890123456789012345";
    EXPECT_FALSE(description.toByteArray(bytes.data(), bytes.size()));
}

TEST_CASE("MessageUDP round-trips metadata and payload")
{
    MessageUDP source(17, {192, 168, 1, 20}, 9001);
    uint8_t payload[] = {1, 2, 3, 4};
    EXPECT_TRUE(source.pushData(payload, sizeof(payload)));
    std::vector<uint8_t> bytes(source.getSize());
    EXPECT_TRUE(source.toByteArray(bytes.data(), bytes.size()));
    MessageUDP decoded = MessageUDP::fromUint8Vector(bytes);
    EXPECT_TRUE(decoded.isValid()); EXPECT_EQ(decoded.getId(), 17); EXPECT_EQ(decoded.getPort(), 9001);
    EXPECT_EQ(decoded.getIPAddress().toString(), String("192.168.1.20"));
    EXPECT_EQ(decoded.getPayload().size(), size_t(4)); EXPECT_EQ(decoded.getPayload()[3], uint8_t(4));
}

TEST_CASE("MessageUDP rejects corrupted framing")
{
    MessageUDP source(1, {10, 0, 0, 1}, 42);
    std::vector<uint8_t> bytes(source.getSize());
    EXPECT_TRUE(source.toByteArray(bytes.data(), bytes.size()));
    bytes.back() = 0;
    EXPECT_FALSE(MessageUDP::fromUint8Vector(bytes).isValid());
    bytes.pop_back();
    EXPECT_FALSE(MessageUDP::fromUint8Vector(bytes).isValid());
}

TEST_CASE("RcRequest serializes payload and rejects undersized input")
{
    RcRequest source(9, 0x1122334455667788ULL, SERVICE_CALL_REQ);
    source.setID(4); source.setRequestSendCount(2); source.pushData(0xAA); source.pushData(0xBB);
    source.calculateCrc();
    std::vector<uint8_t> bytes(source.getSize());
    EXPECT_TRUE(source.toByteArray(bytes.data(), bytes.size()));
    RcRequest decoded;
    EXPECT_TRUE(decoded.fromByteArray(bytes.data(), bytes.size()));
    EXPECT_EQ(decoded.getRequestId(), uint8_t(4)); EXPECT_EQ(decoded.getRequestNodeMAC(), 0x1122334455667788ULL);
    EXPECT_EQ(decoded.getRequestDeviceId(), uint8_t(9)); EXPECT_EQ(decoded.getRequestSendCount(), uint8_t(2));
    EXPECT_EQ(decoded.getData().size(), size_t(2)); EXPECT_FALSE(decoded.fromByteArray(bytes.data(), RC_REQUEST_MIN_SIZE - 1));
}

TEST_CASE("RcRequest rejects an undersized output buffer")
{
    RcRequest request(1, 2, SERVICE_CALL_REQ); request.pushData(3);
    std::vector<uint8_t> storage(request.getSize());
    EXPECT_FALSE(request.toByteArray(storage.data(), 0));
}

TEST_CASE("RcResponse validates CRC and detects tampering")
{
    RcResponse source(8, 0x1020304050607080ULL, SERVICE_CALL_REQ, POSITIVE_RESP);
    source.pushData(10); source.pushData(20);
    std::vector<uint8_t> bytes(source.getSize());
    EXPECT_TRUE(source.toByteArray(bytes.data(), bytes.size()));
    RcResponse decoded;
    EXPECT_TRUE(decoded.fromByteArray(bytes.data(), bytes.size())); EXPECT_TRUE(decoded.isValid());
    bytes[10] ^= 1;
    EXPECT_TRUE(decoded.fromByteArray(bytes.data(), bytes.size())); EXPECT_FALSE(decoded.isValid());
}

TEST_CASE("RcResponse rejects an undersized output buffer")
{
    RcResponse response(1, 2, SERVICE_CALL_REQ, POSITIVE_RESP); response.pushData(3);
    std::vector<uint8_t> storage(response.getSize());
    EXPECT_FALSE(response.toByteArray(storage.data(), 0));
}

TEST_CASE("DeviceDescriptionPacker handles boundaries and preserves order")
{
    std::vector<DeviceDescription> devices(12);
    for (uint8_t index = 0; index < devices.size(); ++index) devices[index].deviceId = index;
    auto chunks = DeviceDescriptionPacker::packDeviceDescriptions(devices);
    EXPECT_EQ(chunks.size(), size_t(3)); EXPECT_EQ(chunks[0].size(), size_t(5));
    EXPECT_EQ(chunks[2].size(), size_t(2));
    auto unpacked = DeviceDescriptionPacker::unpackDeviceDescriptions(chunks);
    EXPECT_EQ(unpacked.size(), devices.size()); EXPECT_EQ(unpacked[11].deviceId, uint8_t(11));
    EXPECT_TRUE(DeviceDescriptionPacker::packDeviceDescriptions({}).empty());
    EXPECT_TRUE(DeviceDescriptionPacker::getChunk(devices, 9).empty());
}

TEST_CASE("ExtendedDataAllocator allocates, finds, and deletes records")
{
    ExtendedDataAllocator allocator;
    ExtendedData first = allocator.createNewExtendedData(16);
    ExtendedData second = allocator.createNewExtendedData(8);
    EXPECT_EQ(first.identifier, uint8_t(1)); EXPECT_EQ(second.identifier, uint8_t(2));
    EXPECT_EQ(allocator.getExtendedDataById(1).length, uint16_t(16));
    EXPECT_TRUE(allocator.deleteExtendedData(1)); EXPECT_FALSE(allocator.deleteExtendedData(1));
    EXPECT_EQ(allocator.getExtendedDataById(1).identifier, uint8_t(255));
    EXPECT_TRUE(allocator.deleteExtendedData(2));
}

TEST_CASE("DataContainer stores values and notifies all subscribers")
{
    int first = 0; int second = 0;
    DataContainer::subscribe(SIG_CONNECTION_STATUS, [&](std::any value) { first = std::any_cast<int>(value); });
    DataContainer::subscribe(SIG_CONNECTION_STATUS, [&](std::any value) { second = std::any_cast<int>(value) * 2; });
    DataContainer::setSignalValue(SIG_CONNECTION_STATUS, 7);
    EXPECT_EQ(std::any_cast<int>(DataContainer::getSignalValue(SIG_CONNECTION_STATUS)), 7);
    EXPECT_EQ(first, 7); EXPECT_EQ(second, 14);
}
