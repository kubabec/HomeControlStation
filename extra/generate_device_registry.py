#!/usr/bin/env python3
"""@file
@brief Generate firmware registries, codecs, and HTTP widgets from device JSON descriptions.

The script validates every registered device description, rejects duplicate or reserved type IDs,
and writes deterministic integration headers before PlatformIO discovers and compiles the firmware.
"""

import json
import re
import shutil
from pathlib import Path

try:
    Import("env")
    PROJECT_DIR = Path(env["PROJECT_DIR"])
except NameError:
    PROJECT_DIR = Path(__file__).resolve().parent.parent

DEVICES_DIR = PROJECT_DIR / "include" / "devices"
GENERATED_INCLUDE_DIR = PROJECT_DIR / "include" / "generated"
RESERVED_TYPE_IDS = {255}
MAX_ADVANCED_CONTROLS_PAYLOAD_SIZE = 391
AREA_BYTE_CAPACITIES = {"configuration": 20, "state": 50}
FACTORY_ARGUMENTS = {
    "config": "config",
    "persistentDataChanged": "context.persistentDataChanged",
    "getRtcTime": "context.getRtcTime",
    "toggleLocalDevice": "context.toggleLocalDevice",
    "fireDigitalEvent": "context.fireDigitalEvent",
    "fireDeviceEvent": (
        "GeneratedDigitalEventTriggers::bind(config.deviceType, config.deviceId, config.deviceName, "
        "context.getNodeMacAddress, context.fireDigitalEventWithSource)"
    ),
}


def load_descriptions():
    descriptions = []
    for path in sorted(DEVICES_DIR.glob("*/*.json")):
        with path.open(encoding="utf-8") as source:
            description = json.load(source)
        description["_path"] = path
        validate_description(description)
        descriptions.append(description)

    registered = [d for d in descriptions if d["implementation"]["manager"]["registered"]]
    ids = {}
    symbols = {}
    for description in registered:
        device_type = description["deviceType"]
        type_id = device_type["enumValue"]
        symbol = device_type["enumSymbol"]
        if type_id in RESERVED_TYPE_IDS:
            raise ValueError(f"{description['_path']}: device type ID {type_id} is reserved")
        if type_id in ids:
            raise ValueError(f"duplicate device type ID {type_id}: {ids[type_id]} and {description['_path']}")
        if not symbol:
            raise ValueError(f"{description['_path']}: registered device requires enumSymbol")
        if symbol in symbols:
            raise ValueError(f"duplicate enum symbol {symbol}: {symbols[symbol]} and {description['_path']}")
        ids[type_id] = description["_path"]
        symbols[symbol] = description["_path"]
    return sorted(registered, key=lambda d: d["deviceType"]["enumValue"])


def validate_description(description):
    path = description["_path"]
    for key in ("schemaVersion", "deviceType", "implementation", "lifecycle", "configuration", "state", "ui"):
        if key not in description:
            raise ValueError(f"{path}: missing {key}")
    implementation = description["implementation"]
    manager = implementation.get("manager", {})
    services = {service.get("name"): service for service in description.get("services", [])}
    action_ids = set()
    for action in description.get("digitalEventActions", []):
        action_id = action.get("id")
        if not isinstance(action_id, int) or isinstance(action_id, bool) or not 1 <= action_id <= 255:
            raise ValueError(f"{path}: digital event action ID must be from 1 to 255")
        if action_id in action_ids:
            raise ValueError(f"{path}: duplicate digital event action ID {action_id}")
        action_ids.add(action_id)
        service = services.get(action.get("service"))
        if not service or service.get("status") != "implemented" or service.get("overload") != "serviceCall_1":
            raise ValueError(
                f"{path}: digital event action {action_id} requires an implemented serviceCall_1 service"
            )
        if action.get("toggleState") and action.get("service") != "DEVSERVICE_STATE_SWITCH":
            raise ValueError(f"{path}: toggleState is only valid for DEVSERVICE_STATE_SWITCH")
        for parameter_name, value in action.get("parameters", {}).items():
            if parameter_name not in "abcde" or not isinstance(value, int) or isinstance(value, bool) or not 0 <= value <= 255:
                raise ValueError(f"{path}: invalid digital event action parameter {parameter_name}")
    trigger_ids = set()
    for trigger in description.get("digitalEventTriggers", []):
        trigger_id = trigger.get("id")
        if not isinstance(trigger_id, int) or isinstance(trigger_id, bool) or not 1 <= trigger_id <= 255:
            raise ValueError(f"{path}: digital event trigger ID must be from 1 to 255")
        if trigger_id in trigger_ids:
            raise ValueError(f"{path}: duplicate digital event trigger ID {trigger_id}")
        if not isinstance(trigger.get("label"), str) or not trigger["label"]:
            raise ValueError(f"{path}: digital event trigger {trigger_id} requires a label")
        trigger_ids.add(trigger_id)
    factory_arguments = implementation.get("factory", {}).get("arguments", []) if implementation.get("factory") else []
    if trigger_ids and "fireDeviceEvent" not in factory_arguments:
        raise ValueError(f"{path}: digitalEventTriggers require the fireDeviceEvent factory argument")
    if "fireDeviceEvent" in factory_arguments and not trigger_ids:
        raise ValueError(f"{path}: fireDeviceEvent requires at least one digitalEventTrigger")
    condition_ids = set()
    for condition in description.get("enablingConditions", []):
        condition_id = condition.get("id")
        if not isinstance(condition_id, int) or isinstance(condition_id, bool) or not 1 <= condition_id <= 255:
            raise ValueError(f"{path}: enabling condition ID must be from 1 to 255")
        if condition_id in condition_ids:
            raise ValueError(f"{path}: duplicate enabling condition ID {condition_id}")
        condition_ids.add(condition_id)
        if not isinstance(condition.get("label"), str) or not condition["label"]:
            raise ValueError(f"{path}: enabling condition {condition_id} requires a label")
        source = condition.get("source")
        source_match = re.fullmatch(r"customBytes\[(\d+)(\.\.(\d+))?\]", source or "")
        if source != "isEnabled" and not source_match:
            raise ValueError(f"{path}: enabling condition {condition_id} has an invalid source")
        value_type = condition.get("type")
        if value_type not in ("boolean", "uint8", "float32"):
            raise ValueError(f"{path}: enabling condition {condition_id} has an invalid type")
        if source == "isEnabled" and value_type != "boolean":
            raise ValueError(f"{path}: isEnabled condition {condition_id} must be boolean")
        if source_match:
            start = int(source_match.group(1))
            end = int(source_match.group(3) or start)
            if start > end or end >= 50:
                raise ValueError(f"{path}: enabling condition {condition_id} source exceeds customBytes")
            if value_type == "float32" and end - start + 1 < 4:
                raise ValueError(f"{path}: float32 enabling condition {condition_id} requires a four-byte span")
        comparator = condition.get("comparator")
        if comparator not in ("equals", "lessThan", "greaterThan", "between"):
            raise ValueError(f"{path}: enabling condition {condition_id} has an invalid comparator")
        if comparator == "between":
            if not isinstance(condition.get("minimum"), (int, float)) or not isinstance(condition.get("maximum"), (int, float)):
                raise ValueError(f"{path}: enabling condition {condition_id} requires minimum and maximum")
        elif not isinstance(condition.get("value"), (int, float, bool)):
            raise ValueError(f"{path}: enabling condition {condition_id} requires a value")
    advanced_controls = description["ui"].get("advancedControls")
    if advanced_controls:
        template_path = advanced_controls.get("template")
        if not isinstance(template_path, str) or not template_path.endswith(".html"):
            raise ValueError(f"{path}: ui.advancedControls.template must reference an HTML file")
        resolved_template = (PROJECT_DIR / template_path).resolve()
        try:
            resolved_template.relative_to(PROJECT_DIR.resolve())
        except ValueError as error:
            raise ValueError(f"{path}: advanced-controls template must be inside the project") from error
        if not resolved_template.is_file():
            raise ValueError(f"{path}: advanced-controls template does not exist: {template_path}")
        template_content = resolved_template.read_text(encoding="utf-8")
        if '<script type="application/x-hcs-advanced-controls">' not in template_content:
            raise ValueError(f"{path}: advanced-controls template requires an HCS controller script")

        payload_size = advanced_controls.get("payloadSize", {})
        if "fixedBytes" in payload_size:
            fixed_bytes = payload_size["fixedBytes"]
            if not isinstance(fixed_bytes, int) or not 1 <= fixed_bytes <= MAX_ADVANCED_CONTROLS_PAYLOAD_SIZE:
                raise ValueError(f"{path}: advanced-controls fixedBytes exceeds transport capacity")
        else:
            required_size_fields = {"countState", "maximumCount", "elementBytes", "overheadBytes"}
            if not required_size_fields.issubset(payload_size):
                raise ValueError(f"{path}: incomplete dynamic advanced-controls payloadSize")
            maximum_size = payload_size["maximumCount"] * payload_size["elementBytes"] + payload_size["overheadBytes"]
            if maximum_size > MAX_ADVANCED_CONTROLS_PAYLOAD_SIZE:
                raise ValueError(f"{path}: dynamic advanced-controls payload exceeds transport capacity")

        for service_name in ("DEVSERVICE_GET_ADVANCED_CONTROLS", "DEVSERVICE_SET_ADVANCED_CONTROLS"):
            service = services.get(service_name)
            if not service or service.get("status") != "implemented" or service.get("overload") != "serviceCall_3":
                raise ValueError(f"{path}: advanced controls require implemented {service_name} with serviceCall_3")
        has_settings_entry = any(
            control.get("widget") == "settings-button"
            and control.get("service") == "DEVSERVICE_GET_ADVANCED_CONTROLS"
            for control in description["ui"].get("controls", [])
        )
        if not has_settings_entry:
            raise ValueError(f"{path}: advanced controls require a settings-button bound to the common read service")

    schedule = description["lifecycle"].get("update", {}).get("schedule", {})
    schedule_mode = schedule.get("mode")
    if schedule_mode == "fixed_interval":
        interval_ms = schedule.get("intervalMs")
        if (
            not isinstance(interval_ms, int)
            or isinstance(interval_ms, bool)
            or not 1 <= interval_ms <= 0x7FFFFFFF
        ):
            raise ValueError(f"{path}: fixed_interval schedule requires intervalMs from 1 to 2147483647")
    elif schedule_mode == "every_manager_cycle":
        if "intervalMs" in schedule:
            raise ValueError(f"{path}: every_manager_cycle schedule must not define intervalMs")
    elif schedule_mode != "not_registered":
        raise ValueError(f"{path}: unsupported lifecycle update schedule mode: {schedule_mode}")

    if manager.get("registered"):
        if schedule_mode == "not_registered":
            raise ValueError(f"{path}: registered device cannot use not_registered schedule mode")
        factory = implementation.get("factory")
        if not factory:
            raise ValueError(f"{path}: registered device requires implementation.factory")
        unknown = set(factory.get("arguments", [])) - set(FACTORY_ARGUMENTS)
        if unknown:
            raise ValueError(f"{path}: unknown factory arguments: {sorted(unknown)}")
        for implementation_file in (implementation["header"], implementation["source"]):
            if not (PROJECT_DIR / implementation_file).is_file():
                raise ValueError(f"{path}: implementation file does not exist: {implementation_file}")

    for area_name in ("configuration", "state"):
        fields = description[area_name].get("customBytes", description[area_name].get("descriptionCustomBytes", []))
        occupied = {}
        for field in fields:
            if "offset" not in field or "size" not in field:
                continue
            if field["offset"] < 0 or field["size"] < 1 or field["offset"] + field["size"] > AREA_BYTE_CAPACITIES[area_name]:
                raise ValueError(
                    f"{path}: {area_name} field {field['name']} exceeds "
                    f"the {AREA_BYTE_CAPACITIES[area_name]}-byte area"
                )
            for offset in range(field["offset"], field["offset"] + field["size"]):
                previous = occupied.get(offset)
                companion = field.get("role", "").startswith("companion-values-for:")
                if previous and not companion:
                    raise ValueError(f"{path}: overlapping {area_name} byte {offset}: {previous} and {field['name']}")
                occupied[offset] = field["name"]


def guard_block(description, body):
    guard = description["implementation"].get("buildGuard")
    return f"#ifdef {guard}\n{body}\n#endif" if guard else body


def write_if_changed(path, content):
    path.parent.mkdir(parents=True, exist_ok=True)
    if not path.exists() or path.read_text(encoding="utf-8") != content:
        path.write_text(content, encoding="utf-8")


def generate_types(descriptions):
    enum_rows = ",\n    ".join(
        f"{d['deviceType']['enumSymbol']} = {d['deviceType']['enumValue']}" for d in descriptions
    )
    info_rows = ",\n    ".join(
        f"{{{d['deviceType']['enumValue']}, {json.dumps(d['deviceType']['name'])}}}" for d in descriptions
    )
    content = f'''// Auto-generated by extra/generate_device_registry.py. Do not edit.
/**
 * @file GeneratedDeviceTypes.hpp
 * @brief Schema-derived stable device identifiers and human-readable type metadata.
 */
#ifndef GENERATED_DEVICE_TYPES_HPP
#define GENERATED_DEVICE_TYPES_HPP

#include <cstddef>
#include <cstdint>

enum DevType : uint8_t
{{
    {enum_rows}
}};

namespace GeneratedDeviceTypes
{{
/** @brief Immutable metadata for a known device type identifier. */
struct DeviceTypeInfo
{{
    uint8_t id;
    const char* name;
}};

inline constexpr DeviceTypeInfo kKnownTypes[] = {{
    {info_rows}
}};

/** @brief Finds known type metadata by persistent numeric identifier. */
inline const DeviceTypeInfo* find(uint8_t id)
{{
    for (const auto& type : kKnownTypes)
    {{
        if (type.id == id) return &type;
    }}
    return nullptr;
}}

/** @brief Reports whether an identifier is declared by a registered device schema. */
inline bool isKnown(uint8_t id) {{ return find(id) != nullptr; }}
/** @brief Returns the schema name or UNKNOWN for an unrecognized identifier. */
inline const char* nameOf(uint8_t id)
{{
    const DeviceTypeInfo* type = find(id);
    return type ? type->name : "UNKNOWN";
}}
}}

#endif
'''
    write_if_changed(GENERATED_INCLUDE_DIR / "GeneratedDeviceTypes.hpp", content)


def generate_registry(descriptions):
    includes = []
    entries = []
    cases = []
    validation_cases = []
    for description in descriptions:
        impl = description["implementation"]
        manager = impl["manager"]
        device_type = description["deviceType"]
        schedule = description["lifecycle"]["update"]["schedule"]
        cycle_interval_ms = schedule["intervalMs"] if schedule["mode"] == "fixed_interval" else 0
        include = f'#include "{impl["header"].removeprefix("include/")}"'
        includes.append(guard_block(description, include))
        entry = (
            f'{{{device_type["enumValue"]}, {json.dumps(device_type["name"])}, '
            f'{str(manager.get("includedInDeviceCollection", True)).lower()}, {cycle_interval_ms}u}}'
        )
        entries.append(guard_block(description, entry + ","))
        args = ", ".join(FACTORY_ARGUMENTS[arg] for arg in impl["factory"]["arguments"])
        case = (
            f"case {device_type['enumSymbol']}:\n"
            f"        return std::unique_ptr<Device>(new {device_type['className']}({args}));"
        )
        cases.append(guard_block(description, case))
        pin_checks = ["if (!claimPin(config.pinNumber, false, candidatePins, claimedPinCount)) return false;"]
        primary_pin = next(
            (field for field in description["configuration"].get("commonFields", []) if field.get("source") == "pinNumber"),
            {},
        )
        if primary_pin.get("hardwareRole") == "adc":
            pin_checks.append("if (!isWifiSafeAdcPin(config.pinNumber)) return false;")
        for field in description["configuration"].get("customBytes", []):
            offset = int(field["offset"])
            size = int(field["size"])
            if size == 1:
                value = f"config.customBytes[{offset}]"
            elif size == 2:
                value = f"readU16(config.customBytes, {offset})"
            else:
                value = None
            if value is not None and ("minimum" in field or "maximum" in field):
                minimum = int(field.get("minimum", 0))
                maximum = int(field.get("maximum", (1 << (size * 8)) - 1))
                pin_checks.append(f"if ({value} < {minimum} || {value} > {maximum}) return false;")
            options = field.get("options", [])
            if value is not None and options:
                allowed = " && ".join(f"{value} != {int(option['value'])}" for option in options)
                pin_checks.append(f"if ({allowed}) return false;")
            is_gpio = (
                field.get("hardwareRole") == "gpio"
                or field.get("htmlType") == "gpio-select"
                or "gpio" in field.get("label", "").lower()
            )
            if not is_gpio:
                continue
            if field.get("size") != 1:
                raise ValueError(f"{description['_path']}: GPIO configuration field {field['name']} must occupy one byte")
            optional = str(bool(field.get("optional", field.get("maximum") == 255))).lower()
            claim = f"!claimPin(config.customBytes[{offset}], {optional}, candidatePins, claimedPinCount)"
            claim_when = field.get("claimWhen")
            if claim_when:
                claim = f"config.customBytes[{int(claim_when['offset'])}] == {int(claim_when['equals'])} && {claim}"
            pin_checks.append(f"if ({claim}) return false;")
        validation_case = (
            f"case {device_type['enumSymbol']}:\n"
            + "        " + "\n        ".join(pin_checks)
            + "\n        break;"
        )
        validation_cases.append(guard_block(description, validation_case))

    content = f'''// Auto-generated by extra/generate_device_registry.py. Do not edit.
/**
 * @file GeneratedDeviceRegistry.hpp
 * @brief Build-profile-specific device factories and runtime participation metadata.
 */
#ifndef GENERATED_DEVICE_REGISTRY_HPP
#define GENERATED_DEVICE_REGISTRY_HPP

#include <functional>
#include <memory>
#include "SystemDefinition.hpp"
#include "devices/device.hpp"
#include "os/datacontainer/NvmConfigSlotDefinition.hpp"
#include "generated/GeneratedDigitalEventTriggers.hpp"
{chr(10).join(includes)}

namespace GeneratedDeviceRegistry
{{
/** @brief Platform callbacks made available to generated device factory calls. */
struct RuntimeContext
{{
    std::function<void()> persistentDataChanged;
    std::function<RtcTime()> getRtcTime;
    std::function<void(uint16_t)> toggleLocalDevice;
    std::function<void(uint64_t)> fireDigitalEvent;
    std::function<uint64_t()> getNodeMacAddress;
    std::function<void(uint64_t, const String&)> fireDigitalEventWithSource;
}};

/** @brief Runtime behavior associated with one enabled device implementation. */
struct Registration
{{
    uint8_t typeId;
    const char* name;
    bool includedInDeviceCollection;
    /** Zero runs every manager cycle; positive values are fixed millisecond intervals. */
    uint32_t cycleIntervalMs;
}};

inline constexpr Registration kEnabledTypes[] = {{
{chr(10).join('    ' + row.replace(chr(10), chr(10) + '    ') for row in entries)}
}};

/** @brief Finds an implementation enabled in the current firmware build. */
inline const Registration* find(uint8_t typeId)
{{
    for (const auto& registration : kEnabledTypes)
    {{
        if (registration.typeId == typeId) return &registration;
    }}
    return nullptr;
}}

/** @brief Reports whether a GPIO can be assigned to a configurable device on this board. */
inline bool isConfigurableGpio(uint8_t pin)
{{
    // GPIO 0 is the station reset input; 22-32 are absent or used by flash/PSRAM.
    return pin > 0 && pin < 49 && !(pin >= 22 && pin <= 32);
}}

/** @brief Claims one configured GPIO and rejects invalid or duplicate assignments. */
inline bool claimPin(uint8_t pin, bool optional, bool* claimedPins, size_t claimedPinCount)
{{
    if (optional && pin == 255) return true;
    if (claimedPins == nullptr || pin >= claimedPinCount || !isConfigurableGpio(pin) || claimedPins[pin]) return false;
    claimedPins[pin] = true;
    return true;
}}

/** @brief ADC1 pins remain available while the station's Wi-Fi stack is active. */
inline bool isWifiSafeAdcPin(uint8_t pin)
{{
    return pin >= 1 && pin <= 10;
}}

/** @brief Reads a little-endian uint16 value from configuration custom bytes. */
inline uint16_t readU16(const uint8_t* bytes, size_t offset)
{{
    return static_cast<uint16_t>(bytes[offset]) |
           (static_cast<uint16_t>(bytes[offset + 1]) << 8);
}}

/** @brief Validates all GPIO assignments declared by one active device schema. */
inline bool validateConfiguration(const DeviceConfigSlotType& config, bool* claimedPins, size_t claimedPinCount)
{{
    if (!config.isActive) return true;
    if (claimedPins == nullptr || claimedPinCount > 49) return false;
    bool candidatePins[49] = {{}};
    memcpy(candidatePins, claimedPins, claimedPinCount * sizeof(bool));
    switch (config.deviceType)
    {{
{chr(10).join('    ' + row.replace(chr(10), chr(10) + '    ') for row in validation_cases)}
    default:
        return false;
    }}
    memcpy(claimedPins, candidatePins, claimedPinCount * sizeof(bool));
    return true;
}}

/** @brief Constructs a concrete device through schema-declared factory arguments. */
inline std::unique_ptr<Device> create(
    uint8_t typeId,
    DeviceConfigSlotType config,
    const RuntimeContext& context)
{{
    switch (typeId)
    {{
{chr(10).join('    ' + row.replace(chr(10), chr(10) + '    ') for row in cases)}
    default:
        return nullptr;
    }}
}}
}}

#endif
'''
    write_if_changed(GENERATED_INCLUDE_DIR / "GeneratedDeviceRegistry.hpp", content)


def generate_digital_event_actions(descriptions):
    rows = []
    for description in descriptions:
        type_id = description["deviceType"]["enumValue"]
        for action in description.get("digitalEventActions", []):
            parameters = action.get("parameters", {})
            values = ", ".join(str(parameters.get(name, 255)) for name in "abcde")
            row = (
                f'{{{type_id}, {action["id"]}, {json.dumps(action["label"])}, {action["service"]}, '
                f'{{{values}}}, {str(action.get("toggleState", False)).lower()}}},'
            )
            rows.append(guard_block(description, row))

    content = f'''// Auto-generated by extra/generate_device_registry.py. Do not edit.
/**
 * @file GeneratedDigitalEventActions.hpp
 * @brief Device-owned actions that can be persisted in compact digital-event mappings.
 */
#ifndef GENERATED_DIGITAL_EVENT_ACTIONS_HPP
#define GENERATED_DIGITAL_EVENT_ACTIONS_HPP

#include <cstdint>
#include "SystemDefinition.hpp"
#include "devices/device.hpp"

namespace GeneratedDigitalEventActions
{{
struct Action
{{
    uint8_t deviceType;
    uint8_t id;
    const char* label;
    DeviceServicesType service;
    ServiceParameters_set1 parameters;
    bool toggleState;
}};

inline constexpr Action kActions[] = {{
{chr(10).join('    ' + row.replace(chr(10), chr(10) + '    ') for row in rows)}
}};

inline const Action* find(uint8_t deviceType, uint8_t actionId)
{{
    for (const auto& action : kActions)
    {{
        if (action.deviceType == deviceType && action.id == actionId) return &action;
    }}
    return nullptr;
}}
}}

#endif
'''
    write_if_changed(GENERATED_INCLUDE_DIR / "GeneratedDigitalEventActions.hpp", content)


def generate_digital_event_triggers(descriptions):
    rows = []
    for description in descriptions:
        type_id = description["deviceType"]["enumValue"]
        for trigger in description.get("digitalEventTriggers", []):
            row = f'{{{type_id}, {trigger["id"]}, {json.dumps(trigger["label"])} }},'
            rows.append(guard_block(description, row))

    content = f'''// Auto-generated by extra/generate_device_registry.py. Do not edit.
/**
 * @file GeneratedDigitalEventTriggers.hpp
 * @brief Device-owned event triggers and stable network-wide event ID generation.
 */
#ifndef GENERATED_DIGITAL_EVENT_TRIGGERS_HPP
#define GENERATED_DIGITAL_EVENT_TRIGGERS_HPP

#include <cstdint>
#include <functional>
#include "Arduino.h"

namespace GeneratedDigitalEventTriggers
{{
struct Trigger
{{
    uint8_t deviceType;
    uint8_t id;
    const char* label;
}};

inline constexpr Trigger kTriggers[] = {{
{chr(10).join('    ' + row.replace(chr(10), chr(10) + '    ') for row in rows)}
}};

inline const Trigger* find(uint8_t deviceType, uint8_t triggerId)
{{
    for (const auto& trigger : kTriggers)
    {{
        if (trigger.deviceType == deviceType && trigger.id == triggerId) return &trigger;
    }}
    return nullptr;
}}

inline uint64_t makeEventId(uint64_t nodeMac, uint8_t deviceType, uint8_t deviceId, uint8_t triggerId)
{{
    uint64_t hash = 14695981039346656037ULL;
    for (uint8_t shift = 0; shift < 64; shift += 8)
    {{
        hash ^= static_cast<uint8_t>(nodeMac >> shift);
        hash *= 1099511628211ULL;
    }}
    const uint8_t identity[] = {{deviceType, deviceId, triggerId}};
    for (uint8_t value : identity)
    {{
        hash ^= value;
        hash *= 1099511628211ULL;
    }}
    return hash == 0 ? 1 : hash;
}}

inline std::function<void(uint8_t)> bind(
    uint8_t deviceType,
    uint8_t deviceId,
    const String& deviceName,
    const std::function<uint64_t()>& getNodeMac,
    const std::function<void(uint64_t, const String&)>& fireEvent)
{{
    return [deviceType, deviceId, deviceName, getNodeMac, fireEvent](uint8_t triggerId)
    {{
        if (!fireEvent || !getNodeMac || find(deviceType, triggerId) == nullptr) return;
        const uint64_t nodeMac = getNodeMac();
        if (nodeMac == 0) return;
        String source = deviceName.length() ? deviceName : "Device " + String((int)deviceId);
        fireEvent(makeEventId(nodeMac, deviceType, deviceId, triggerId), source);
    }};
}}
}}

#endif
'''
    write_if_changed(GENERATED_INCLUDE_DIR / "GeneratedDigitalEventTriggers.hpp", content)


def generate_enabling_conditions(descriptions):
    rows = []
    type_names = {"boolean": "BOOLEAN", "uint8": "UINT8", "float32": "FLOAT32"}
    comparator_names = {"equals": "EQUALS", "lessThan": "LESS_THAN", "greaterThan": "GREATER_THAN", "between": "BETWEEN"}
    for description in descriptions:
        type_id = description["deviceType"]["enumValue"]
        for condition in description.get("enablingConditions", []):
            source = condition["source"]
            offset = 255 if source == "isEnabled" else int(re.search(r"\d+", source).group())
            value_a = condition.get("minimum", condition.get("value", 0))
            value_b = condition.get("maximum", value_a)
            row = (
                f'{{{type_id}, {condition["id"]}, {json.dumps(condition["label"])}, '
                f'ValueType::{type_names[condition["type"]]}, Comparator::{comparator_names[condition["comparator"]]}, '
                f'{offset}, {float(value_a)}f, {float(value_b)}f }},'
            )
            rows.append(guard_block(description, row))

    content = f'''// Auto-generated by extra/generate_device_registry.py. Do not edit.
/**
 * @file GeneratedEnablingConditions.hpp
 * @brief Device-owned boolean predicates used to gate digital-event mappings.
 */
#ifndef GENERATED_ENABLING_CONDITIONS_HPP
#define GENERATED_ENABLING_CONDITIONS_HPP

#include <cstdint>
#include <cstring>
#include "SystemDefinition.hpp"
#include "devices/device.hpp"

namespace GeneratedEnablingConditions
{{
enum class ValueType : uint8_t {{ BOOLEAN, UINT8, FLOAT32 }};
enum class Comparator : uint8_t {{ EQUALS, LESS_THAN, GREATER_THAN, BETWEEN }};

struct Predicate
{{
    uint8_t deviceType;
    uint8_t id;
    const char* label;
    ValueType valueType;
    Comparator comparator;
    uint8_t offset;
    float valueA;
    float valueB;
}};

inline constexpr Predicate kPredicates[] = {{
{chr(10).join('    ' + row.replace(chr(10), chr(10) + '    ') for row in rows)}
}};

inline const Predicate* find(uint8_t deviceType, uint8_t predicateId)
{{
    for (const auto& predicate : kPredicates)
    {{
        if (predicate.deviceType == deviceType && predicate.id == predicateId) return &predicate;
    }}
    return nullptr;
}}

inline bool evaluate(const DeviceDescription& device, uint8_t predicateId)
{{
    const Predicate* predicate = find(device.deviceType, predicateId);
    if (predicate == nullptr) return false;

    float actual = 0.0f;
    if (predicate->offset == 255)
    {{
        actual = device.isEnabled ? 1.0f : 0.0f;
    }}
    else if (predicate->valueType == ValueType::FLOAT32)
    {{
        if (predicate->offset + sizeof(float) > NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION) return false;
        std::memcpy(&actual, &device.customBytes[predicate->offset], sizeof(actual));
    }}
    else
    {{
        if (predicate->offset >= NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION) return false;
        actual = static_cast<float>(device.customBytes[predicate->offset]);
    }}

    switch (predicate->comparator)
    {{
    case Comparator::EQUALS: return actual == predicate->valueA;
    case Comparator::LESS_THAN: return actual < predicate->valueA;
    case Comparator::GREATER_THAN: return actual > predicate->valueA;
    case Comparator::BETWEEN: return actual >= predicate->valueA && actual <= predicate->valueB;
    }}
    return false;
}}

inline ServiceRequestErrorCode evaluateService(const DeviceDescription& device, ServiceParameters_set3 parameters)
{{
    if (parameters.buff == nullptr || parameters.size != 1 || parameters.direction != e_OUT_from_DEVICE)
        return SERV_EXECUTION_FAILURE;
    if (parameters.additionalParam > 255) return SERV_NOT_SUPPORTED;
    if (find(device.deviceType, static_cast<uint8_t>(parameters.additionalParam)) == nullptr)
        return SERV_NOT_SUPPORTED;
    parameters.buff[0] = evaluate(device, static_cast<uint8_t>(parameters.additionalParam)) ? 1 : 0;
    return SERV_SUCCESS;
}}
}}

#endif
'''
    write_if_changed(GENERATED_INCLUDE_DIR / "GeneratedEnablingConditions.hpp", content)


def parse_source(source):
    single = re.fullmatch(r"customBytes\[(\d+)\]", source)
    if single:
        return int(single.group(1)), int(single.group(1))
    span = re.fullmatch(r"customBytes\[(\d+)\.\.(\d+)\]", source)
    if span:
        return int(span.group(1)), int(span.group(2))
    raise ValueError(f"unsupported state source expression: {source}")


def state_field_code(field, index):
    name = field["name"]
    field_type = field["type"]
    start, end = parse_source(field["source"])
    prefix = f'result += ",\\\"{name}\\\":";'
    if field_type in ("uint8", "boolean"):
        return f'{prefix}\n        result += String((int)description.customBytes[{start}]);'
    if field_type == "float32":
        return f'''{prefix}
        float value{index} = 0.0f;
        memcpy(&value{index}, &description.customBytes[{start}], sizeof(value{index}));
        result += String(value{index});'''
    if field_type == "hex-color":
        return f'''{prefix}
        result += "\\\"#" + rgbHex(description.customBytes[{start}], description.customBytes[{start + 1}], description.customBytes[{start + 2}]) + "\\\"";'''
    array_match = re.fullmatch(r"(boolean|uint8|hex-color)\[(\d+)\]", field_type)
    if array_match:
        element_type, count_text = array_match.groups()
        count = int(count_text)
        stride = 3 if element_type == "hex-color" else 1
        value = (
            f'"\\\"#" + rgbHex(description.customBytes[{start} + item * 3], '
            f'description.customBytes[{start + 1} + item * 3], description.customBytes[{start + 2} + item * 3]) + "\\\""'
            if element_type == "hex-color"
            else f'String((int)description.customBytes[{start} + item])'
        )
        return f'''{prefix}
        result += "[";
        for (uint8_t item = 0; item < {count}; ++item)
        {{
            if (item) result += ",";
            result += {value};
        }}
        result += "]";'''
    raise ValueError(f"unsupported HTTP state type: {field_type}")


def generate_state_serializer(descriptions):
    cases = []
    for description in descriptions:
        fields = description["state"].get("httpFields", [])
        body = "\n        ".join(state_field_code(field, index) for index, field in enumerate(fields))
        case = f'''case {description['deviceType']['enumSymbol']}:
    {{
        {body}
        break;
    }}'''
        cases.append(guard_block(description, case))
    content = f'''// Auto-generated by extra/generate_device_registry.py. Do not edit.
/**
 * @file GeneratedDeviceStateSerializer.hpp
 * @brief Converts schema-described device state bytes into dashboard JSON fields.
 */
#ifndef GENERATED_DEVICE_STATE_SERIALIZER_HPP
#define GENERATED_DEVICE_STATE_SERIALIZER_HPP

#include <cstring>
#include "SystemDefinition.hpp"
#include "devices/device.hpp"

namespace GeneratedDeviceStateSerializer
{{
inline char hexDigit(uint8_t value) {{ return value < 10 ? char('0' + value) : char('A' + value - 10); }}
inline String rgbHex(uint8_t red, uint8_t green, uint8_t blue)
{{
    char value[7] = {{hexDigit(red >> 4), hexDigit(red & 0x0F), hexDigit(green >> 4),
                     hexDigit(green & 0x0F), hexDigit(blue >> 4), hexDigit(blue & 0x0F), '\\0'}};
    return String(value);
}}

/** @brief Serializes device-specific HTTP fields without platform type branches. */
inline String serialize(const DeviceDescription& description)
{{
    String result;
    switch (description.deviceType)
    {{
{chr(10).join('    ' + row.replace(chr(10), chr(10) + '    ') for row in cases)}
    default:
        break;
    }}
    return result;
}}
}}

#endif
'''
    write_if_changed(GENERATED_INCLUDE_DIR / "GeneratedDeviceStateSerializer.hpp", content)


def cpp_identifier(value):
    return re.sub(r"[^A-Za-z0-9_]", "_", value)


def generate_widget_javascript(description):
    ui = description["ui"]
    function_name = ui.get("roomWidget")
    if not function_name:
        return None

    controls = ui.get("controls", [])
    readouts = ui.get("readouts", [])
    rows = [f"function {function_name}(deviceContainer,device){{"]
    simple_readouts = [item for item in readouts if item["widget"] in ("numeric-label", "status-label")]
    if simple_readouts:
        rows.append("const readoutGrid=document.createElement('div');readoutGrid.className='device-readouts';")

    color_readout = next((item for item in readouts if item["widget"] == "color-swatch"), None)
    if color_readout:
        source = color_readout.get("source", "avgColor")
        rows.append(
            "const colorPicker=document.createElement('div');colorPicker.className='color-picker';"
            "const colorDisplay=document.createElement('div');"
            f"colorDisplay.className=`color-display ${{device.status}}`;colorDisplay.style.backgroundColor=device.{source};"
            "colorPicker.appendChild(colorDisplay);deviceContainer.appendChild(colorPicker);"
        )

    if any(item["widget"] in ("temperature-gauge", "humidity-gauge") for item in readouts):
        error = next((item for item in readouts if item["widget"] == "error-message"), None)
        error_source = error.get("source", "Err") if error else "Err"
        temperature = next((item for item in readouts if item["widget"] == "temperature-gauge"), {})
        humidity = next((item for item in readouts if item["widget"] == "humidity-gauge"), {})
        temp_source = temperature.get("source", "temp")
        humidity_source = humidity.get("source", "humid")
        rows.append(
            "const readings=document.createElement('div');"
            f"if(device.{error_source}==0){{readings.className='temperature-container';"
            "readings.innerHTML=`<div id=\"gauge${device.id}\" class=\"temperature-widget\"><canvas style=\"max-width:100px;\"></canvas><div class=\"temperature-value\">20°C</div></div><div id=\"humidity${device.id}\" class=\"humidity-widget\"><canvas></canvas><div class=\"value-display humidity-value\">50%</div></div>`;"
            f"listOfTempWidgets.push(`gauge${{device.id}}`);listOfTempValues.push(device.{temp_source});"
            f"listOfHumidWidgets.push(`humidity${{device.id}}`);listOfHumidValues.push(device.{humidity_source});"
            "}else{readings.className='sensor-error';readings.innerHTML='SENSOR ERROR';}deviceContainer.appendChild(readings);"
        )

    for readout_index, readout in enumerate(readouts):
        if readout["widget"] not in ("numeric-label", "status-label"):
            continue
        variable = f"readout{readout_index}"
        label_variable = f"readoutLabel{readout_index}"
        value_variable = f"readoutValue{readout_index}"
        source = readout.get("source", readout["name"])
        label = readout.get("label", readout["name"].replace("_", " ").title())
        rows.append(
            f"const {variable}=document.createElement('div');{variable}.className='device-readout';"
            f"const {label_variable}=document.createElement('span');{label_variable}.className='device-readout-label';{label_variable}.textContent={json.dumps(label)};"
            f"const {value_variable}=document.createElement('span');{value_variable}.className='device-readout-value';"
        )
        if readout["widget"] == "status-label":
            true_label = json.dumps(readout.get("trueLabel", "Active"))
            false_label = json.dumps(readout.get("falseLabel", "Inactive"))
            true_tone = json.dumps(readout.get("trueTone", "accent"))
            false_tone = json.dumps(readout.get("falseTone", "neutral"))
            rows.append(
                f"{value_variable}.textContent=device.{source}?{true_label}:{false_label};"
                f"{value_variable}.classList.add('tone-'+(device.{source}?{true_tone}:{false_tone}));"
            )
        else:
            unit = readout.get("unit", "")
            value_labels = readout.get("valueLabels", {})
            unknown_value = readout.get("unknownValue")
            unknown_label = readout.get("unknownLabel", "Unknown")
            if value_labels:
                mapping = json.dumps({str(key): value for key, value in value_labels.items()}, separators=(",", ":"))
                rows.append(
                    f"const readoutMap{readout_index}={mapping};const rawReadout{readout_index}=device.{source};"
                    f"{value_variable}.textContent=Object.prototype.hasOwnProperty.call(readoutMap{readout_index},String(rawReadout{readout_index}))?readoutMap{readout_index}[String(rawReadout{readout_index})]:String(rawReadout{readout_index}){('+' + json.dumps(' ' + unit)) if unit else ''};"
                )
            elif unknown_value is not None:
                rows.append(
                    f"{value_variable}.textContent=Number(device.{source})==={json.dumps(unknown_value)}?{json.dumps(unknown_label)}:`${{device.{source}}}{(' ' + unit) if unit else ''}`;"
                )
            else:
                rows.append(f"{value_variable}.textContent=`${{device.{source}}}{(' ' + unit) if unit else ''}`;")
        rows.append(
            f"{variable}.appendChild({label_variable});{variable}.appendChild({value_variable});readoutGrid.appendChild({variable});"
        )

    if simple_readouts:
        rows.append("deviceContainer.appendChild(readoutGrid);")

    button_controls = sorted(
        (item for item in controls if item["widget"] in ("toggle-button", "settings-button", "play-stop-button")),
        key=lambda item: item.get("displayOrder", controls.index(item)),
    )
    if button_controls:
        rows.append("const buttons=document.createElement('div');buttons.className='button-container';")
        for control in button_controls:
            widget = control["widget"]
            if widget == "toggle-button":
                rows.append(
                    "const power=document.createElement('a');power.className=device.status=='on'?'icon-btn off':'icon-btn on';"
                    "power.onclick=()=>asyncDeviceStateSwitch(device.id,device.status=='off'?1:0);"
                    "power.id=`switchBtn${device.id}`;buttons.appendChild(power);"
                )
            elif widget == "settings-button":
                rows.append(
                    "const settings=document.createElement('a');settings.className='icon-btn settings';"
                    "settings.onclick=()=>getExtendedControlsRequest(device.id,deviceContainer);buttons.appendChild(settings);"
                )
            elif widget == "play-stop-button":
                rows.append(
                    "const animation=document.createElement('a');animation.className=device.liveStatus==0?'icon-btn play':'icon-btn stop';"
                    "animation.onclick=()=>asyncLiveAnimation(device.id,device.liveStatus==0?1:0);buttons.appendChild(animation);"
                )
        rows.append("deviceContainer.appendChild(buttons);")

    for control_index, control in enumerate(controls):
        if control["widget"] != "range":
            continue
        variable = f"range{control_index}"
        source = control.get("source", control["name"])
        visible_source = None
        visible_when = control.get("visibleWhen", "")
        visible_match = re.match(r"([A-Za-z_][A-Za-z0-9_]*)\s*==\s*true", visible_when)
        if visible_match:
            visible_source = visible_match.group(1)
        condition_start = f"if(device.{visible_source}){{" if visible_source else ""
        condition_end = "}" if visible_source else ""
        rows.append(
            condition_start
            + f"const {variable}=document.createElement('input');{variable}.type='range';"
            + f"{variable}.min={int(control.get('minimum', 0))};{variable}.max={int(control.get('maximum', 100))};{variable}.value=device.{source};"
            + f"{variable}.onchange=()=>onRangeChanged({variable}.value,device.id);{variable}.id=`brightnessSlider${{device.id}}`;deviceContainer.appendChild({variable});"
            + condition_end
        )

    rows.append("}")
    return "".join(rows)


def generate_device_widgets(descriptions):
    widget_dir = GENERATED_INCLUDE_DIR / "widgets"
    widget_dir.mkdir(parents=True, exist_ok=True)
    expected = set()
    includes = []
    concatenations = []
    dispatch_cases = []

    for description in descriptions:
        manager = description["implementation"]["manager"]
        javascript = generate_widget_javascript(description)
        if not manager.get("includedInDeviceCollection") or not javascript:
            continue
        device_name = cpp_identifier(description["deviceType"]["name"])
        symbol = f"generated{device_name}WidgetJs"
        file_name = f"Generated{device_name}Widget.hpp"
        expected.add(file_name)
        widget_content = f'''// Auto-generated by extra/generate_device_registry.py. Do not edit.
    /**
     * @file {file_name}
     * @brief Generated dashboard widget JavaScript for the {description["deviceType"]["name"]} device type.
     */
#ifndef GENERATED_{device_name.upper()}_WIDGET_HPP
#define GENERATED_{device_name.upper()}_WIDGET_HPP
inline const char* {symbol} = R"DEVICE_WIDGET({javascript})DEVICE_WIDGET";
#endif
'''
        write_if_changed(widget_dir / file_name, widget_content)
        includes.append(guard_block(description, f'#include "generated/widgets/{file_name}"'))
        concatenations.append(guard_block(description, f"widgets += {symbol};"))
        dispatch_cases.append(
            guard_block(
                description,
                f"case {description['deviceType']['enumValue']}: {description['ui']['roomWidget']}(deviceContainer,device); return true;",
            )
        )

    for old_file in widget_dir.glob("Generated*Widget.hpp"):
        if old_file.name not in expected:
            old_file.unlink()

    dispatch = "function renderGeneratedDeviceWidget(deviceContainer,device){switch(Number(device.devType)){" + "".join(
        row for row in dispatch_cases if not row.startswith("#ifdef")
    ) + "default:return false;}}"
    # Build-guarded dispatch cases are appended as independent function fragments below.
    guarded_cases = [row for row in dispatch_cases if row.startswith("#ifdef")]
    if guarded_cases:
        # The generated JavaScript is selected by the C++ preprocessor, so construct its switch in C++.
        dispatch_prefix = "widgets += \"function renderGeneratedDeviceWidget(deviceContainer,device){switch(Number(device.devType)){\";"
        dispatch_rows = []
        for description in descriptions:
            manager = description["implementation"]["manager"]
            if not manager.get("includedInDeviceCollection") or not description["ui"].get("roomWidget"):
                continue
            row = f'widgets += "case {description["deviceType"]["enumValue"]}: {description["ui"]["roomWidget"]}(deviceContainer,device); return true;";'
            dispatch_rows.append(guard_block(description, row))
        dispatch_builder = "\n    ".join([dispatch_prefix] + dispatch_rows + ['widgets += "default:return false;}}";'])
    else:
        dispatch_builder = f"widgets += {json.dumps(dispatch)};"

    aggregate = f'''// Auto-generated by extra/generate_device_registry.py. Do not edit.
/**
 * @file GeneratedDeviceWidgets.hpp
 * @brief Aggregates dashboard widgets enabled in the current firmware build.
 */
#ifndef GENERATED_DEVICE_WIDGETS_HPP
#define GENERATED_DEVICE_WIDGETS_HPP
#include <Arduino.h>
#include "SystemDefinition.hpp"
{chr(10).join(includes)}

/** @brief Builds the JavaScript widget bundle and generated type dispatcher. */
inline String buildGeneratedDeviceWidgetsJs()
{{
    String widgets;
    {chr(10).join(concatenations)}
    {dispatch_builder}
    return widgets;
}}

inline const String generatedDeviceWidgetsJs = buildGeneratedDeviceWidgetsJs();
#endif
'''
    write_if_changed(GENERATED_INCLUDE_DIR / "GeneratedDeviceWidgets.hpp", aggregate)


def state_offset(description, state_name):
    for field in description["state"].get("descriptionCustomBytes", []):
        if field["name"] == state_name:
            return field["offset"]
    raise ValueError(f"{description['_path']}: unknown state field {state_name}")


def generate_advanced_controls(descriptions):
    size_cases = []
    template_definitions = []
    template_cases = []
    for description in descriptions:
        advanced = description["ui"].get("advancedControls")
        if not advanced:
            continue
        payload = advanced["payloadSize"]
        if "fixedBytes" in payload:
            expression = str(payload["fixedBytes"])
        else:
            offset = state_offset(description, payload["countState"])
            count = f"currentlyRequestedDeviceDescription.customBytes[{offset}]"
            expression = (
                f"({count} <= {payload['maximumCount']}u) ? "
                f"(({count} * {payload['elementBytes']}u) + {payload['overheadBytes']}u) : 0"
            )
        size_cases.append(guard_block(description, f"case {description['deviceType']['enumSymbol']}: return {expression};"))
        template_name = f"advancedControlsTemplate_{description['deviceType']['enumSymbol']}"
        template_content = (PROJECT_DIR / advanced["template"]).read_text(encoding="utf-8")
        delimiter = "HCSADV"
        if f'){delimiter}\"' in template_content:
            raise ValueError(f"{description['_path']}: advanced-controls template contains reserved delimiter")
        template_definitions.append(
            guard_block(
                description,
                f'inline const char {template_name}[] = R"{delimiter}({template_content}){delimiter}";',
            )
        )
        template_cases.append(
            guard_block(
                description,
                f"case {description['deviceType']['enumSymbol']}: return {template_name};",
            )
        )
    write_if_changed(
        GENERATED_INCLUDE_DIR / "GeneratedAdvancedControlsSize.inc",
        "// Auto-generated by extra/generate_device_registry.py. Do not edit.\n" + "\n".join(size_cases) + "\n",
    )
    template_registry = f'''// Auto-generated by extra/generate_device_registry.py. Do not edit.
/**
 * @file GeneratedAdvancedControlTemplates.hpp
 * @brief Device-owned HTML templates for the generic advanced-controls host.
 */
#ifndef GENERATED_ADVANCED_CONTROL_TEMPLATES_HPP
#define GENERATED_ADVANCED_CONTROL_TEMPLATES_HPP
#include <cstdint>
#include "SystemDefinition.hpp"
#include "generated/GeneratedDeviceTypes.hpp"

namespace GeneratedAdvancedControlTemplates
{{
{chr(10).join(template_definitions)}

/** @brief Returns the device-provided advanced-controls template, or nullptr. */
inline const char* find(uint8_t typeId)
{{
    switch (typeId)
    {{
{chr(10).join('    ' + row.replace(chr(10), chr(10) + '    ') for row in template_cases)}
    default:
        return nullptr;
    }}
}}
}}
#endif
'''
    write_if_changed(GENERATED_INCLUDE_DIR / "GeneratedAdvancedControlTemplates.hpp", template_registry)


def generate_configuration_javascript(descriptions):
    type_cases = []
    for description in descriptions:
        http_config = description["deviceType"].get("httpConfig", {})
        if not http_config.get("configurable", True):
            continue
        fields = description["configuration"].get("customBytes", [])
        by_name = {field["name"]: field for field in fields}
        statements = []
        for field in fields:
            if field.get("widget") is None:
                continue
            offset = int(field["offset"])
            size = int(field["size"])
            if field.get("htmlType") == "segment-array":
                length = int(field.get("arrayLength", 0))
                companion = next(
                    (
                        candidate
                        for candidate in fields
                        if candidate.get("role") == f"companion-values-for:{field['name']}"
                    ),
                    None,
                )
                statements.append(
                    f"for(let item=0;item<{length};item++){{bytes[{offset}+item]=Number(document.getElementById('seg'+(item+1)+'Count-'+id).value)||0;}}"
                )
                if companion:
                    companion_offset = int(companion["offset"])
                    statements.append(
                        f"for(let item=0;item<{length};item++){{bytes[{companion_offset}+item]=Number(document.getElementById('Seg'+(item+1)+'Flip-'+id).value)||0;}}"
                    )
                continue

            field_id = field["fieldId"]
            value_expression = (
                f"(document.getElementById('{field_id}-'+id).checked?1:0)"
                if field.get("htmlType") == "checkbox"
                else f"Math.round((Number(document.getElementById('{field_id}-'+id).value)||0)*{float(field.get('displayScale', 1))})"
            )
            statements.append(f"{{let value={value_expression};")
            for byte in range(size):
                statements.append(f"bytes[{offset + byte}]=(value>>{byte * 8})&255;")
            statements.append("}")

        for field in fields:
            target_name = field.get("mustEqual")
            if not target_name:
                continue
            source = by_name.get(target_name)
            if not source:
                raise ValueError(f"{description['_path']}: mustEqual references unknown field {target_name}")
            for byte in range(min(int(field["size"]), int(source["size"]))):
                statements.append(f"bytes[{int(field['offset']) + byte}]=bytes[{int(source['offset']) + byte}];")

        type_cases.append((
            description,
            f"case {description['deviceType']['enumValue']}:" + "".join(statements) + "break;"
        ))

    prefix = (
        "<script>"
        "function createConfigurationStringJson(){const devices=[];"
        "for(let id=1;id<=6;id++){const typeId=Number(document.getElementById('type'+id).value);"
        "const bytes=new Array(20).fill(0);switch(typeId){"
    )
    suffix = (
        "default:break;}devices.push({typeId:typeId,id:id,enabled:document.getElementById('enabled'+id).checked,"
        "name:document.getElementById('name'+id).value,pin:Number(document.getElementById('pin'+id).value),"
        "room:Number(document.getElementById('room'+id).value),customBytes:bytes});}"
        "const url='/lclSetupJson&'+JSON.stringify({devices:devices});const xhr=new XMLHttpRequest();"
        "xhr.timeout=10000;xhr.open('POST',url,true);xhr.onreadystatechange=function(){if(xhr.readyState===4){window.location.href='/';}};xhr.send();}"
        "</script>"
    )
    case_rows = []
    for description, case in type_cases:
        case_rows.append(guard_block(description, f"script += {json.dumps(case)};"))
    content = f'''// Auto-generated by extra/generate_device_registry.py. Do not edit.
/**
 * @file GeneratedDeviceConfiguration.hpp
 * @brief Browser-side device configuration encoder generated from schema fields.
 */
#ifndef GENERATED_DEVICE_CONFIGURATION_HPP
#define GENERATED_DEVICE_CONFIGURATION_HPP
#include <Arduino.h>
#include "SystemDefinition.hpp"
/** @brief Builds configuration JavaScript for device types enabled at compile time. */
inline String buildGeneratedDeviceConfigurationJs()
{{
    String script = {json.dumps(prefix)};
    {chr(10).join(case_rows)}
    script += {json.dumps(suffix)};
    return script;
}}
inline const String generatedDeviceConfigurationJs = buildGeneratedDeviceConfigurationJs();
#endif
'''
    write_if_changed(GENERATED_INCLUDE_DIR / "GeneratedDeviceConfiguration.hpp", content)


def generate():
    # Never allow stale device integration files to survive an incremental build.
    shutil.rmtree(GENERATED_INCLUDE_DIR, ignore_errors=True)
    descriptions = load_descriptions()
    if not descriptions:
        print("No registered device descriptions; generated device integration directory removed")
        return
    generate_types(descriptions)
    generate_registry(descriptions)
    generate_digital_event_actions(descriptions)
    generate_digital_event_triggers(descriptions)
    generate_enabling_conditions(descriptions)
    generate_state_serializer(descriptions)
    generate_device_widgets(descriptions)
    generate_advanced_controls(descriptions)
    generate_configuration_javascript(descriptions)
    print(f"Generated device registry from {len(descriptions)} registered device descriptions")


generate()
