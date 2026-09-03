#ifndef DIGITAL_EVENT_DEFINITIONS_HPP
#define DIGITAL_EVENT_DEFINITIONS_HPP

#define DIGITAL_EVENT_FIRED_MSG_ID 100
#define DIGITAL_EVENT_CONFIRMED_MSG_ID 101
#define DIGITAL_EVENT_FIRED_WITH_SOURCE_MSG_ID 102
#define DIGITAL_EVENT_SOURCE_MAX_LENGTH 31

#include <Arduino.h>

/**
 * @file include/os/app/DigitalEvent/DigitalEventDefinitions.hpp
 * @brief Digital event definitions and handlers for the Home Control Station.
 */

/** @brief One source-aware event occurrence used by runtime queues and diagnostics. */
struct DigitalEventOccurrence
{
	uint64_t eventId = 0;
	String source = "Legacy";
};

#endif
