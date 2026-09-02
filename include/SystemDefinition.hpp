/**
 * @file SystemDefinition.hpp
 * @brief Compile-time feature selection for the HomeControlStation firmware.
 *
 * @details This file enables or disables optional hardware and device families.
 * It allows the firmware to compile only the modules that are present on the
 * target installation.
 */
#ifndef SYSTEM_DEFINITION_H
#define SYSTEM_DEFINITION_H

/** Enables the radio-frequency receiver capability of the station. */
#define HAS_RF_RECEIVER

/** Enables support for WS2812B-style LED strips. */
#define LED_STRIP_SUPPORTED
/** Enables support for standard on/off devices. */
#define ON_OFF_SUPPORTED
/** Enables support for temperature and humidity sensing devices. */
#define TEMP_SENSOR_SUPPORTED
/** Enables support for distance sensing devices. */
#define DISTANCE_SENSOR_SUPPORTED


#endif