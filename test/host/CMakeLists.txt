cmake_minimum_required(VERSION 3.16)

if(APPLE AND EXISTS "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.sdk")
    set(CMAKE_OSX_SYSROOT "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.sdk" CACHE PATH "macOS SDK" FORCE)
endif()

project(HomeControlStationHostTests LANGUAGES CXX)

option(HCS_ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

get_filename_component(PROJECT_ROOT "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
set(GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
file(MAKE_DIRECTORY "${GENERATED_DIR}/include/devices/HwButton")
file(MAKE_DIRECTORY "${GENERATED_DIR}/include/devices/TempSensorDHT11DeviceType")

configure_file(
    "${PROJECT_ROOT}/DevicesPredefined/HwButton/include/devices/HwButton/HwButton.hpp.example"
    "${GENERATED_DIR}/include/devices/HwButton/HwButton.hpp" COPYONLY)
configure_file(
    "${PROJECT_ROOT}/DevicesPredefined/TempSensorDHT11DeviceType/include/devices/TempSensorDHT11DeviceType/TempSensorDHT11DeviceType.hpp.example"
    "${GENERATED_DIR}/include/devices/TempSensorDHT11DeviceType/TempSensorDHT11DeviceType.hpp" COPYONLY)
configure_file(
    "${PROJECT_ROOT}/DevicesPredefined/HwButton/src/device/HwButton/HwButton.cpp.example"
    "${GENERATED_DIR}/HwButton.cpp" COPYONLY)
configure_file(
    "${PROJECT_ROOT}/DevicesPredefined/TempSensorDHT11DeviceType/src/device/TempSensorDHT11DeviceType/TempSensorDHT11DeviceType.cpp.example"
    "${GENERATED_DIR}/TempSensorDHT11DeviceType.cpp" COPYONLY)

set(LED_PACKAGE "${PROJECT_ROOT}/DevicesPredefined/LedStrip")
file(GLOB_RECURSE LED_HEADERS CONFIGURE_DEPENDS "${LED_PACKAGE}/include/*.hpp.example")
foreach(HEADER IN LISTS LED_HEADERS)
    file(RELATIVE_PATH RELATIVE_HEADER "${LED_PACKAGE}/include" "${HEADER}")
    string(REGEX REPLACE "\\.example$" "" OUTPUT_HEADER "${RELATIVE_HEADER}")
    get_filename_component(OUTPUT_DIRECTORY "${GENERATED_DIR}/include/${OUTPUT_HEADER}" DIRECTORY)
    file(MAKE_DIRECTORY "${OUTPUT_DIRECTORY}")
    configure_file("${HEADER}" "${GENERATED_DIR}/include/${OUTPUT_HEADER}" COPYONLY)
endforeach()

file(MAKE_DIRECTORY "${GENERATED_DIR}/include/devices/LedWS1228bDeviceType")
configure_file(
    "${PROJECT_ROOT}/DevicesPredefined/LedWS1228bDeviceType/include/devices/LedWS1228bDeviceType/LedWS1228bDeviceType.hpp.example"
    "${GENERATED_DIR}/include/devices/LedWS1228bDeviceType/LedWS1228bDeviceType.hpp" COPYONLY)

set(LED_SOURCES)
file(GLOB_RECURSE LED_EXAMPLE_SOURCES CONFIGURE_DEPENDS "${LED_PACKAGE}/src/*.cpp.example")
foreach(SOURCE IN LISTS LED_EXAMPLE_SOURCES)
    get_filename_component(SOURCE_NAME "${SOURCE}" NAME)
    string(REGEX REPLACE "\\.example$" "" OUTPUT_NAME "${SOURCE_NAME}")
    set(OUTPUT_SOURCE "${GENERATED_DIR}/led/${OUTPUT_NAME}")
    file(MAKE_DIRECTORY "${GENERATED_DIR}/led")
    configure_file("${SOURCE}" "${OUTPUT_SOURCE}" COPYONLY)
    list(APPEND LED_SOURCES "${OUTPUT_SOURCE}")
endforeach()
configure_file(
    "${PROJECT_ROOT}/DevicesPredefined/LedWS1228bDeviceType/src/device/LedWS1228bDeviceType/LedWS1228bDeviceType.cpp.example"
    "${GENERATED_DIR}/LedWS1228bDeviceType.cpp" COPYONLY)

add_executable(host_tests
    TestMain.cpp
    Arduino.cpp
    OsTests.cpp
    ActiveDeviceTests.cpp
    PredefinedDeviceTests.cpp
    LedDeviceTests.cpp
    LoggerStub.cpp
    "${PROJECT_ROOT}/src/os/datacontainer/datacontainer.cpp"
    "${PROJECT_ROOT}/src/os/tools/messageUDP.cpp"
    "${PROJECT_ROOT}/src/os/tools/ExtendedDataAllocator.cpp"
    "${PROJECT_ROOT}/src/os/app/remoteControl/rcRequest.cpp"
    "${PROJECT_ROOT}/src/os/app/remoteControl/rcResponse.cpp"
    "${PROJECT_ROOT}/src/os/app/display/DeviceDescriptionPacker.cpp"
    "${PROJECT_ROOT}/src/device/OnOffDevice/OnOffDevice.cpp"
    "${PROJECT_ROOT}/src/device/WindowDoorSensor/WindowDoorSensor.cpp"
    "${PROJECT_ROOT}/src/device/WindowBlinder/WindowBlinder.cpp"
    "${PROJECT_ROOT}/src/device/Gate/Gate.cpp"
    "${PROJECT_ROOT}/src/device/AquariumController/AquariumController.cpp"
    "${GENERATED_DIR}/HwButton.cpp"
    "${GENERATED_DIR}/TempSensorDHT11DeviceType.cpp"
    "${GENERATED_DIR}/LedWS1228bDeviceType.cpp"
    ${LED_SOURCES}
)

target_compile_definitions(host_tests PRIVATE LED_STRIP_SUPPORTED)
target_include_directories(host_tests PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/stubs"
    "${GENERATED_DIR}/include"
    "${PROJECT_ROOT}/include"
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(host_tests PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
    if(HCS_ENABLE_SANITIZERS)
        if(APPLE)
            target_compile_options(host_tests PRIVATE -fsanitize=undefined -fno-omit-frame-pointer)
            target_link_options(host_tests PRIVATE -fsanitize=undefined)
        else()
            target_compile_options(host_tests PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
            target_link_options(host_tests PRIVATE -fsanitize=address,undefined)
        endif()
    endif()
endif()

enable_testing()
add_test(NAME host_tests COMMAND host_tests)
