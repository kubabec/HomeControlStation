# Adding a new device type to HomeControlStation

This guide explains how to add a complete device type: its C++ implementation, persistent configuration, published state, services, dashboard widget, and optional advanced-controls popup. It is written for contributors who know C++ but are new to HomeControlStation.

For the wider runtime and network design, see [ARCHITECTURE.md](ARCHITECTURE.md). The authoritative metadata shape is [include/devices/device.schema.json](include/devices/device.schema.json).

## 1. The device package

A normal active device type consists of matching include and source directories:

```text
include/devices/MyDevice/
├── MyDevice.hpp
├── MyDevice.json
└── AdvancedControls.html       # optional

src/device/MyDevice/
└── MyDevice.cpp
```

The responsibilities are deliberately separated:

| File | Responsibility |
|---|---|
| `MyDevice.hpp` | Declares a class derived from `Device` and its hardware/runtime state. |
| `MyDevice.cpp` | Implements initialization, periodic work, state serialization, and services. |
| `MyDevice.json` | Describes the stable type ID, factory, schedule, NVM bytes, state bytes, services, and generated UI. |
| `AdvancedControls.html` | Optionally owns a complex popup and the JavaScript codec for an opaque binary payload. |

PlatformIO scans `include/devices/*/*.json` before every build and normally compiles sources below `src/`. With active descriptions, the generators recreate integration files under `include/generated/`; **never edit generated files**. They are deleted before the next build. With no active descriptions, that directory remains absent and static no-device fallbacks are used.

### Start by selecting predefined devices

A clean checkout intentionally has no concrete device directories in `include/devices`. Ready-made packages are stored inertly under [DevicesPredefined](DevicesPredefined). Copy only the complete packages required by the firmware preset:

```sh
sh DevicesPredefined/activate-examples.sh OnOffDevice
platformio run
```

The copied package is now active: the helper copies both trees and removes the catalog-only `.example` marker at the destination. Its JSON is then discovered, its source is compiled, and its generated type, factory, configuration, state serializer, and UI are produced. The original files left in `DevicesPredefined` are neither discovered nor compiled.

LED devices additionally require the shared animation support package:

```sh
sh DevicesPredefined/activate-examples.sh LedStrip LedWS1228bDeviceType
platformio run
```

See [DevicesPredefined/README.md](DevicesPredefined/README.md) for the package dependency table and removal instructions. Never delete the platform files `device.hpp`, `device.schema.json`, or `AdvancedControls.hpp` while changing the active preset.

When developing a reusable device, create and test it under `include/devices/MyDevice` and `src/device/MyDevice`. Then place copies under `DevicesPredefined/MyDevice/include/devices/MyDevice` and `DevicesPredefined/MyDevice/src/device/MyDevice`, adding `.example` to their catalog filenames, so future users can opt into the complete overlay without making the examples active.

## 2. Plan the contract before writing code

Write down these decisions first:

1. **Type identity:** a permanent, unique numeric ID from `0` through `254`. `255` means unknown and is reserved.
2. **Constructor inputs:** NVM configuration and any runtime callbacks the class needs.
3. **Persistent configuration:** the meaning, width, and offset of each byte in the 20-byte `DeviceConfigSlotType::customBytes` area.
4. **Published state:** the meaning, width, and offset of each value in the 50-byte `DeviceDescription::customBytes` area.
5. **Services:** commands accepted by the device and which of the four parameter shapes each command uses.
6. **Schedule:** every 10 ms manager pass, or a fixed interval.
7. **UI:** which published HTTP fields and currently supported widgets are required.
8. **Advanced controls:** whether a normal button/slider is insufficient and an opaque popup payload is needed.

Configuration bytes and state bytes are different contracts:

- **Configuration bytes** are saved in NVM and passed to the constructor after boot.
- **State bytes** are rebuilt by `getDeviceDescription()`, published approximately once per second, sent between nodes, and converted into dashboard JSON.
- **Advanced-control bytes** are fetched on demand through a service and are not automatically persistent.
- **Extended memory** is a separate, potentially large device buffer assigned by `DeviceManager`.

Keep deployed type IDs and byte layouts stable. Changing their meaning without migration makes existing NVM data and remote nodes interpret bytes incorrectly.

## 3. Implement the C++ class

### 3.1 The required interface

Every runtime device derives from `Device`, declared in [include/devices/device.hpp](include/devices/device.hpp), and implements all pure virtual methods:

```cpp
class MyDevice : public Device
{
public:
    explicit MyDevice(DeviceConfigSlotType config);

    void init() override;
    void cyclic() override;
    DeviceDescription getDeviceDescription() override;
    uint8_t getDeviceIdentifier() override;
    uint8_t getDeviceType() override;
    uint16_t getExtendedMemoryLength() override;

    ServiceRequestErrorCode service(DeviceServicesType serviceType) override;
    ServiceRequestErrorCode service(DeviceServicesType serviceType,
                                    ServiceParameters_set1 parameters) override;
    ServiceRequestErrorCode service(DeviceServicesType serviceType,
                                    ServiceParameters_set2 parameters) override;
    ServiceRequestErrorCode service(DeviceServicesType serviceType,
                                    ServiceParameters_set3 parameters) override;
};
```

Implement unused service overloads too; normally they simply return `SERV_NOT_SUPPORTED`.

### 3.2 Minimal header example

```cpp
#ifndef MY_DEVICE_HPP
#define MY_DEVICE_HPP

#include "devices/device.hpp"

class MyDevice : public Device
{
private:
    uint8_t pinNumber = 255;
    uint8_t deviceId = 255;
    uint8_t roomId = 255;
    String deviceName;

    bool activeHigh = true;
    bool isOn = false;

public:
    explicit MyDevice(DeviceConfigSlotType config);

    void init() override;
    void cyclic() override;
    DeviceDescription getDeviceDescription() override;
    uint8_t getDeviceIdentifier() override;
    uint8_t getDeviceType() override;
    uint16_t getExtendedMemoryLength() override;

    ServiceRequestErrorCode service(DeviceServicesType serviceType) override;
    ServiceRequestErrorCode service(DeviceServicesType serviceType,
                                    ServiceParameters_set1 parameters) override;
    ServiceRequestErrorCode service(DeviceServicesType serviceType,
                                    ServiceParameters_set2 parameters) override;
    ServiceRequestErrorCode service(DeviceServicesType serviceType,
                                    ServiceParameters_set3 parameters) override;
};

#endif
```

If the JSON uses a build guard, include `SystemDefinition.hpp` and wrap the class declaration and implementation with the same `#ifdef`. A predefined guarded example is [DevicesPredefined/TempSensorDHT11DeviceType/include/devices/TempSensorDHT11DeviceType/TempSensorDHT11DeviceType.hpp.example](DevicesPredefined/TempSensorDHT11DeviceType/include/devices/TempSensorDHT11DeviceType/TempSensorDHT11DeviceType.hpp.example).

### 3.3 Constructor and NVM configuration

The generated factory passes arguments in exactly the order listed by `implementation.factory.arguments`. For the common `"arguments": ["config"]` case:

```cpp
MyDevice::MyDevice(DeviceConfigSlotType config)
{
    pinNumber = config.pinNumber;
    deviceId = config.deviceId;
    roomId = config.roomId;
    deviceName = String(config.deviceName);

    // Must match configuration.customBytes in MyDevice.json.
    activeHigh = config.customBytes[0] != 0;
}
```

For multi-byte values, avoid casts to potentially unaligned addresses. Use `memcpy` or decode bytes explicitly:

```cpp
uint16_t samplePeriodMs = 0;
memcpy(&samplePeriodMs, &config.customBytes[1], sizeof(samplePeriodMs));
```

The generated browser encoder writes multi-byte numeric configuration values least-significant byte first. ESP32 is little-endian, but explicit encoding/decoding is preferable when a contract may later cross architectures.

### 3.4 `init()` and `cyclic()`

Use `init()` for hardware setup after all devices have been constructed and extended memory has been assigned:

```cpp
void MyDevice::init()
{
    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber, activeHigh ? LOW : HIGH);
}
```

Use `cyclic()` for short, non-blocking work:

```cpp
void MyDevice::cyclic()
{
    // Poll a state machine or sensor. Do not delay() here.
}
```

`DeviceManager` itself runs about every 10 ms. JSON scheduling determines whether `cyclic()` runs on every manager pass or only after a fixed interval. Do not add a second timer inside `cyclic()` unless the class genuinely has multiple internal cadences.

### 3.5 Identity and extended memory

```cpp
uint8_t MyDevice::getDeviceIdentifier()
{
    return deviceId;
}

uint8_t MyDevice::getDeviceType()
{
    return type_MY_DEVICE; // Generated from deviceType.enumSymbol.
}

uint16_t MyDevice::getExtendedMemoryLength()
{
    return 0;
}
```

The device identifier is the configured instance ID; the device type is the generated stable type enum.

### 3.6 Publish state with `getDeviceDescription()`

```cpp
DeviceDescription MyDevice::getDeviceDescription()
{
    DeviceDescription description;
    description.deviceType = getDeviceType();
    description.deviceId = getDeviceIdentifier();
    description.roomId = roomId;
    description.isEnabled = isOn ? 1 : 0;
    description.deviceName = deviceName;

    // The constructor already clears all 50 bytes, but being explicit is harmless.
    memset(description.customBytes, 0, NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION);
    description.customBytes[0] = activeHigh ? 1 : 0;
    description.customBytes[1] = isOn ? 1 : 0;

    float measuredValue = 12.5f;
    memcpy(&description.customBytes[2], &measuredValue, sizeof(measuredValue));
    return description;
}
```

The offsets and sizes must match `state.descriptionCustomBytes` and `state.httpFields` in JSON. Use `memcpy` for floats and multi-byte objects. Initialize every field in the returned description; do not expose uninitialized stack bytes.

`deviceName` must contain fewer than 25 characters to serialize successfully. The current configuration UI limits normal names to 24 characters.

Choose `isEnabled` semantics deliberately:

- switchable devices normally use `0` or `1`;
- read-only devices may use a special always-on convention already understood by the project;
- document unusual behavior in `state.enabledSemantics` and verify room-level power behavior.

### 3.7 Implement services

The available result codes are:

| Result | Meaning |
|---|---|
| `SERV_SUCCESS` | Completed successfully. |
| `SERV_EXECUTION_FAILURE` | Valid service, but invalid data or execution failed. |
| `SERV_GENERAL_FAILURE` | General failure. |
| `SERV_BUSY` | Device cannot currently execute it. |
| `SERV_PENDING` | Asynchronous/remote completion is pending. |
| `SERV_NOT_SUPPORTED` | This overload or service is not supported. |

A simple switch service uses `ServiceParameters_set1`:

```cpp
ServiceRequestErrorCode MyDevice::service(
    DeviceServicesType serviceType,
    ServiceParameters_set1 parameters)
{
    switch (serviceType)
    {
    case DEVSERVICE_STATE_SWITCH:
        if (parameters.a > 1)
        {
            return SERV_EXECUTION_FAILURE;
        }
        isOn = parameters.a != 0;
        digitalWrite(pinNumber, isOn == activeHigh ? HIGH : LOW);
        return SERV_SUCCESS;

    default:
        return SERV_NOT_SUPPORTED;
    }
}
```

Unused overloads:

```cpp
ServiceRequestErrorCode MyDevice::service(DeviceServicesType)
{
    return SERV_NOT_SUPPORTED;
}

ServiceRequestErrorCode MyDevice::service(DeviceServicesType,
                                          ServiceParameters_set2)
{
    return SERV_NOT_SUPPORTED;
}

ServiceRequestErrorCode MyDevice::service(DeviceServicesType,
                                          ServiceParameters_set3)
{
    return SERV_NOT_SUPPORTED;
}
```

Validate every incoming value, pointer, size, direction, and index before using it. A service may arrive over UDP from another node, so UI validation alone is not sufficient.

## 4. Service names and parameter shapes

### 4.1 Existing service names

A device may implement these shared `DeviceServicesType` values from [include/devices/device.hpp](include/devices/device.hpp):

- `DEVSERVICE_ENABLE`
- `DEVSERVICE_DISABLE`
- `DEVSERVICE_SET_DATA`
- `DEVSERVICE_GET_DATA`
- `DEVSERVICE_STATE_SWITCH`
- `DEVSERVICE_LED_STRIP_SAVE_CONTENT`
- `DEVSERVICE_LED_STRIP_SWITCH_CONTENT`
- `DEVSERVICE_BRIGHTNESS_CHANGE`
- `DEVSERVICE_SET_EXT_MEMORY_PTR`
- `DEVSERVICE_GET_ADVANCED_CONTROLS`
- `DEVSERVICE_GET_DETAILED_COLORS`
- `DEVSERVICE_SET_DETAILED_COLORS`
- `DEVSERVICE_ROOM_STATE_CHANGE`
- `DEVSERVICE_SEGMENT_STATE_SWITCH`
- `DEVSERVICE_LIVE_ANIMATION`
- `DEVSERVICE_SET_ADVANCED_CONTROLS`

Do not invent a service name only in JSON. Add a new shared enum and all required platform/transport/UI handling first if none of these semantics fits.

### 4.2 The four overloads

| JSON `overload` | C++ parameter | Capacity and use |
|---|---|---|
| `serviceCall_NoParams` | no parameter object | Command only. |
| `serviceCall_1` | `ServiceParameters_set1` | Five `uint8_t` values: `a` through `e`. |
| `serviceCall_2` | `ServiceParameters_set2` | Five `float` values: `a` through `e`. |
| `serviceCall_3` | `ServiceParameters_set3` | `uint8_t* buff`, `uint16_t size`, `uint16_t additionalParam`, and direction. |

For `serviceCall_3`, direction is one of `e_OUT_from_DEVICE`, `e_IN_to_DEVICE`, or unknown. Advanced-control GET requests are output-from-device; SET requests are input-to-device.

The JSON `parameters` array documents how named values map to the C++ payload. For example:

```json
{
  "name": "DEVSERVICE_STATE_SWITCH",
  "overload": "serviceCall_1",
  "status": "implemented",
  "parameters": [
    {
      "name": "state",
      "type": "boolean",
      "source": "ServiceParameters_set1.a",
      "minimum": 0,
      "maximum": 1
    }
  ]
}
```

`status` is descriptive:

- `implemented`: production behavior exists;
- `placeholder`: a branch exists but behavior is incomplete;
- `disabled`: intentionally unavailable.

Only advertise `implemented` services to a generated control. The generator does not prove that the corresponding C++ `switch` branch exists.

### 4.3 Event-triggered actions

To expose a service in the `/digBtn` DigitalEvent action popup, add `digitalEventActions` at the JSON root. Every entry is a complete invocation preset with a stable ID that is unique within the device type:

```json
"digitalEventActions": [
  {
    "id": 21,
    "label": "Turn on",
    "service": "DEVSERVICE_STATE_SWITCH",
    "parameters": { "a": 1 }
  },
  {
    "id": 23,
    "label": "Toggle",
    "service": "DEVSERVICE_STATE_SWITCH",
    "toggleState": true
  }
]
```

Action IDs are stored as one byte in NVM. Preserve an ID's meaning after release; mappings refer to the number, not the label. IDs `11` and `12` are reserved for legacy NVM migration. IDs `21`, `22`, and `23` retain the former ON, OFF, and TOGGLE meanings where those actions are supported.

Only an implemented `serviceCall_1` service may be used. Fixed `parameters.a` through `parameters.e` are optional byte values and default to `255`. `toggleState` is valid only with `DEVSERVICE_STATE_SWITCH` and computes parameter `a` from the current state. For a richer operation such as changing an LED strip color, expose compact device presets, for example separate actions that invoke `DEVSERVICE_LED_STRIP_SWITCH_CONTENT` with saved slot values `1`, `2`, and `3`. Do not bind pointer, read, or variable-payload services directly.

The generated `GeneratedDigitalEventActions.hpp` lookup is used by both the UI and runtime dispatcher, so the popup and executable service call stay derived from the same declaration.

### 4.4 Device-generated events

A device type may optionally declare events it can produce. IDs are stable one-byte identifiers within that type:

```json
"implementation": {
  "factory": { "arguments": ["config", "fireDeviceEvent"] }
},
"digitalEventTriggers": [
  { "id": 1, "label": "Opened" },
  { "id": 2, "label": "Closed" }
]
```

The constructor receives `std::function<void(uint8_t)>`. Invoke it only when the corresponding state transition is confirmed, for example `fireDeviceEvent(1)`. The generator rejects duplicate IDs, missing labels, triggers without the factory callback, and callbacks without declared triggers.

The callback converts the trigger ID into a stable 64-bit event ID using FNV-1a over the node MAC address, device type, configured device ID, and trigger ID. This makes two sensor instances on one node, and equivalent sensors on different nodes, produce different IDs. The configured device name is transmitted as the human-readable Source. Keep the device type, device ID, and trigger ID stable to preserve existing mappings.

Do not construct the 64-bit ID in device code and do not call networking directly. The predefined `WindowDoorSensor` package is the reference implementation.

### 4.5 Enabling conditions

A device may expose boolean predicates that users can instantiate once and reuse across DigitalEvent mappings:

```json
"enablingConditions": [
  { "id": 1, "label": "Enabled", "source": "isEnabled", "type": "boolean", "comparator": "equals", "value": true },
  { "id": 2, "label": "Warm", "source": "customBytes[3..6]", "type": "float32", "comparator": "greaterThan", "value": 24 }
]
```

Predicate IDs are stable one-byte identifiers within a device type. Preserve their meaning after release because persisted condition instances refer to the number. Sources may be `isEnabled`, `customBytes[N]`, or a custom-byte span for `float32`. Supported comparators are `equals`, `lessThan`, `greaterThan`, and inclusive `between`, which uses `minimum` and `maximum`.

The generated `GeneratedEnablingConditions.hpp` helper evaluates these predicates and writes the boolean service result. A device that declares conditions must handle `DEVSERVICE_CHECK_ENABLING_CONDITION` in its `serviceCall_3` overload, capture current state, and call `GeneratedEnablingConditions::evaluateService(...)`. Sensors should take a fresh hardware reading there when required. The DigitalEvent receiver requests this service through the normal local/remote device API and waits asynchronously for its result. Keep `state.descriptionCustomBytes`, the C++ `getDeviceDescription()` implementation, and predicate offsets synchronized. Settings permits 20 reusable instances, and each mapping may select up to three; all selected conditions must be true.

## 5. Write the JSON description

### 5.1 Complete starter description

Choose an unused `enumValue`; `49` below is only an example and must be rechecked before use.

```json
{
  "$schema": "../device.schema.json",
  "schemaVersion": 1,
  "deviceType": {
    "name": "MyDevice",
    "className": "MyDevice",
    "enumSymbol": "type_MY_DEVICE",
    "enumValue": 49
  },
  "implementation": {
    "header": "include/devices/MyDevice/MyDevice.hpp",
    "source": "src/device/MyDevice/MyDevice.cpp",
    "buildGuard": null,
    "dependencies": ["Arduino GPIO"],
    "factory": {
      "arguments": ["config"]
    },
    "manager": {
      "registered": true,
      "collection": "DeviceManager::devices",
      "includedInDeviceCollection": true
    }
  },
  "lifecycle": {
    "init": {
      "actions": ["configure the output GPIO"]
    },
    "update": {
      "entryPoint": "cyclic",
      "schedule": {
        "mode": "every_manager_cycle"
      }
    }
  },
  "configuration": {
    "commonFields": [
      { "name": "enabled", "type": "boolean", "source": "isActive", "widget": "checkbox" },
      { "name": "name", "type": "string", "source": "deviceName", "maximumLength": 24, "widget": "text" },
      { "name": "pin", "type": "uint8", "source": "pinNumber", "widget": "gpio-select" },
      { "name": "id", "type": "uint8", "source": "deviceId", "widget": "device-slot" },
      { "name": "room", "type": "uint8", "source": "roomId", "widget": "room-select" }
    ],
    "customBytes": [
      {
        "name": "activeHigh",
        "type": "boolean",
        "label": "Active state",
        "fieldId": "activeHigh",
        "htmlType": "select",
        "offset": 0,
        "size": 1,
        "widget": "select",
        "options": [
          { "value": 0, "label": "Active LOW" },
          { "value": 1, "label": "Active HIGH" }
        ]
      }
    ]
  },
  "services": [
    {
      "name": "DEVSERVICE_STATE_SWITCH",
      "overload": "serviceCall_1",
      "status": "implemented",
      "parameters": [
        { "name": "state", "type": "boolean", "source": "ServiceParameters_set1.a", "minimum": 0, "maximum": 1 }
      ]
    }
  ],
  "state": {
    "enabledSemantics": "isOn",
    "descriptionCustomBytes": [
      { "name": "activeHigh", "type": "boolean", "offset": 0, "size": 1 },
      { "name": "isOn", "type": "boolean", "offset": 1, "size": 1 }
    ],
    "httpFields": [
      { "name": "activeHigh", "type": "boolean", "source": "customBytes[0]" },
      { "name": "power", "type": "boolean", "source": "customBytes[1]" }
    ]
  },
  "ui": {
    "roomWidget": "generateMyDeviceWidget",
    "controls": [
      {
        "name": "power",
        "widget": "toggle-button",
        "service": "DEVSERVICE_STATE_SWITCH",
        "overload": "serviceCall_1",
        "parameter": "state"
      }
    ],
    "readouts": []
  },
  "extendedMemory": {
    "required": false,
    "size": 0
  },
  "limitations": []
}
```

### 5.2 Required top-level sections

The schema requires:

- `schemaVersion`
- `deviceType`
- `implementation`
- `lifecycle`
- `configuration`
- `services`
- `enablingConditions` when the device offers DigitalEvent gating predicates
- `state`
- `ui`
- `extendedMemory`
- `limitations`

`$schema` is strongly recommended for editor completion and validation. `schemaVersion` must currently be `1`.

### 5.3 `deviceType`

| Field | Required | Meaning |
|---|---:|---|
| `name` | yes | Human-readable stable type name. Also the default configuration dropdown label. |
| `className` | yes | Exact C++ class instantiated by the generated factory. |
| `enumSymbol` | yes | Unique C++ enum symbol. It may be `null` only for an unregistered description. |
| `enumValue` | yes | Stable integer. Registered types must use a unique value from `0` to `254`. |
| `httpConfig.displayName` | no | Overrides the label in the device configuration dropdown. |
| `httpConfig.configurable` | no | Set to `false` to omit the type from generated device configuration UI. Default is `true`. |

The schema also accepts extra `httpConfig` metadata such as `group`, `renderer`, and `configWidget`, but the current generators do not consume those fields. Do not rely on them for behavior.

### 5.4 `implementation`

| Field | Required | Meaning |
|---|---:|---|
| `header` | yes | Header path relative to the project root. |
| `source` | yes | Source path relative to the project root. |
| `buildGuard` | yes | C++ feature macro or `null`. Generated includes, factory cases, serializers, and widgets are guarded. |
| `dependencies` | yes | Descriptive list of libraries/hardware dependencies. It does not install libraries. Add real libraries to `platformio.ini` or `lib/`. |
| `manager.registered` | yes | `true` makes the type part of the generated enum, registry, and factory. |
| `manager.collection` | yes | Legacy/descriptive collection name; currently not used by the generated factory. Use a meaningful string or `null`. |
| `manager.includedInDeviceCollection` | yes | Controls publication to the common catalog and eligibility for generated configuration/dashboard UI. Usually `true`. |
| `factory.arguments` | registered types | Ordered constructor arguments for generated `new ClassName(...)`. |

Allowed factory arguments and exact C++ types:

| JSON name | Value passed by the registry | Typical constructor parameter |
|---|---|---|
| `config` | NVM slot | `DeviceConfigSlotType` |
| `persistentDataChanged` | request an NVM save | `std::function<void()>` |
| `getRtcTime` | current RTC value | `std::function<RtcTime()>` |
| `toggleLocalDevice` | toggle a device ID | `std::function<void(uint16_t)>` |
| `fireDigitalEvent` | fire an event ID | `std::function<void(uint64_t)>` |
| `fireDeviceEvent` | fire a declared trigger; the registry derives its ID and source | `std::function<void(uint8_t)>` |

The order in JSON is the constructor order. For example, `["config", "getRtcTime"]` requires a constructor compatible with `MyDevice(DeviceConfigSlotType, std::function<RtcTime()>)`.

PlatformIO compiles active implementation sources under `src`. The generator validates that `implementation.source` exists, which catches a package whose `include/` half was copied without its `src/` half. Shared support packages such as `LedStrip` must also be copied with both trees.

A build guard must be defined for builds that include the device, normally in [include/SystemDefinition.hpp](include/SystemDefinition.hpp). The JSON remains present in all builds; the C++ preprocessor determines whether that device enters a particular firmware image.

### 5.5 `lifecycle`

`init.actions` is contributor-facing documentation. Runtime behavior comes from the C++ `init()` method.

`update.entryPoint` must be `"cyclic"`. Its schedule is one of:

| Mode | Additional field | Behavior |
|---|---|---|
| `every_manager_cycle` | no `intervalMs` | Runs on every manager pass, currently about every 10 ms. |
| `fixed_interval` | `intervalMs` from 1 to 2,147,483,647 | Manager invokes `cyclic()` after that many milliseconds have elapsed. |
| `not_registered` | none | Only for descriptions with `manager.registered: false`. |

Extra keys such as `internalTasks` are documentation only. A registered device cannot use `not_registered`, and `every_manager_cycle` must not contain `intervalMs`.

### 5.6 `configuration.commonFields`

The normal common fields represent `DeviceConfigSlotType` members: activation, name, pin, device ID, and room. Copying the five-field pattern from the starter JSON makes the contract understandable and consistent.

The current HTTP configuration page renders and saves common slot fields separately from this metadata; `commonFields` is primarily descriptive today. Type-specific rendering is driven by `configuration.customBytes`.

### 5.7 `configuration.customBytes`

This array describes the 20 bytes at `DeviceConfigSlotType::customBytes[0..19]`.

Every field needs at least:

- `name`: unique semantic name;
- `type`: descriptive data type;
- `offset`: first byte, from `0` through `19`;
- `size`: number of occupied bytes.

A visible generated field (`widget` is not `null`) also needs:

- `label`: text shown to the user;
- `fieldId`: unique HTML/JavaScript key within that device type;
- `htmlType`: one of the supported renderers below.

The current custom configuration renderers are:

| `htmlType` | Required/optional metadata | Behavior |
|---|---|---|
| `checkbox` | normal size is 1 | Saves `0` or `1`. |
| `select` | `options` | Dropdown. Current storage supports at most two options. |
| `gpio-select` | normal size is 1; optional `minimum`/`maximum`, `default`, `optional`, `claimWhen` | Dropdown containing the inclusive GPIO range. The generated server validator claims the selected GPIO and rejects invalid or duplicate active assignments. |
| `range` | `minimum`, `maximum` | HTML range input. Defaults are `0` and `255`. |
| `number` | `minimum`, `maximum`, optional `displayScale` | HTML number input. Defaults are `0` and `255`. A scale such as `10` displays stored fixed-point value `253` as `25.3` and multiplies it again when saving. |
| `segment-array` | `arrayLength`, bounds, companion field | Specialized segmented-strip editor described below. |

Important details:

- The `widget` string is currently an inclusion marker; `htmlType` chooses the actual renderer. Set `widget` to `null` for a hidden field.
- At most eight visible custom fields are generated for one device type.
- `select` supports at most two `{ "value", "label" }` options.
- Visible custom fields are read into a `uint16_t` by the renderer. Prefer one- or two-byte numeric fields.
- Saved numeric values are encoded little-endian into `size` bytes.
- `minimum`, `maximum`, and `select.options` drive the browser controls and generated server-side validation. C++ must still defend itself because constructors and service calls are separate trust boundaries.
- `default` supplies the generated control value when the slot currently stores a different/unknown device type. It prevents zero-filled fresh slots from submitting invalid values. The value is not persistent until the user saves the form, and an existing slot of the same type continues to display its stored bytes.
- `zeroMeans` remains descriptive metadata; it does not alter rendering or validation.
- `mustEqual` copies bytes from the named source field during browser save. This is useful for a hidden safety copy.
- Gaps are allowed and saved as zero.
- The registry generator rejects configuration fields extending beyond byte 19 and ordinary overlaps. Still keep a byte-layout table because C++ offsets cannot be inferred or verified automatically.

#### Generated hardware validation

For every enabled slot, `DeviceManager` calls the generated configuration validator before replacing the RAM mirror. It runs again while restoring NVM slots, before the factory constructs a device. The validator currently:

- claims the common `pinNumber` GPIO;
- recognizes custom GPIO fields through `hardwareRole: "gpio"`, `htmlType: "gpio-select"`, or a GPIO label;
- permits `255` only for fields marked `optional: true` or having `maximum: 255`;
- rejects duplicate claims within one device and across all six active slots;
- rejects pins outside `0..48` and the unavailable/reserved ESP32-S3 range `22..32`;
- checks one- and two-byte `minimum`/`maximum` values and `select.options`;
- restricts a common pin marked `hardwareRole: "adc"` to Wi-Fi-safe ADC1 GPIO `1..10`.

Use explicit metadata for new descriptions even where inference would currently work:

```json
{ "name": "probePin", "source": "pinNumber", "widget": "gpio-select", "hardwareRole": "adc" }
```

Conditional outputs can be claimed only in one mode. For example, a second motor pin may contain:

```json
"claimWhen": { "offset": 5, "equals": 2 }
```

This means the GPIO is claimed only when custom byte 5 equals 2. Semantic relationships not expressible as individual bounds—such as a cutoff being greater than a setpoint—remain C++ responsibilities.

Hidden safety-copy example:

```json
{
  "name": "limitCopy",
  "type": "uint8",
  "offset": 19,
  "size": 1,
  "widget": null,
  "mustEqual": "limit"
}
```

#### Specialized `segment-array`

This renderer emits one count input and one inversion selector per segment. Use a visible count field followed immediately by an equally sized hidden companion field:

```json
{
  "name": "segmentLedCounts",
  "type": "uint8[5]",
  "label": "Segment counts",
  "fieldId": "segmentLedCounts",
  "htmlType": "segment-array",
  "offset": 5,
  "size": 5,
  "minimum": 1,
  "maximum": 100,
  "widget": "number-array",
  "arrayLength": 5
},
{
  "name": "segmentReverseOrder",
  "type": "boolean[5]",
  "offset": 10,
  "size": 5,
  "widget": null,
  "arrayLength": 5,
  "role": "companion-values-for:segmentLedCounts"
}
```

The visible renderer assumes companion bytes begin at `count.offset + count.arrayLength`; keep that equal to the companion's declared offset.

### 5.8 `services`

Each entry requires `name`, `overload`, `status`, and `parameters`. Additional fields such as `availability`, `result`, and parameter descriptions document conditions and behavior but do not generate C++ service logic.

Keep JSON and C++ synchronized:

- each `implemented` entry needs a matching C++ service branch in the declared overload;
- each UI control needs a service the device actually handles;
- every parameter `source` must describe the correct `a`–`e`, buffer, size, direction, or `additionalParam` member;
- return `SERV_NOT_SUPPORTED` for other service/overload combinations.

### 5.9 `state.descriptionCustomBytes`

This array documents the 50 bytes returned in `DeviceDescription::customBytes[0..49]`. Every entry should have `name`, `type`, `offset`, and `size`.

Examples of useful descriptive types are `boolean`, `uint8`, `uint16`, `float32`, `enum`, `rgb888`, and fixed arrays. The generator does not encode these fields for the class; C++ remains responsible for writing the bytes.

Keep every `offset + size <= 50`, with no unintended overlap. The generator rejects fields extending beyond byte 49 and ordinary overlaps; C++ is still responsible for writing exactly the declared representation.

### 5.10 `state.httpFields`

This array generates the conversion from raw state bytes to properties in dashboard JSON. It is operational, not merely documentation.

Supported HTTP field types are:

| `type` | `source` | Output |
|---|---|---|
| `uint8` | `customBytes[N]` | JSON number. |
| `boolean` | `customBytes[N]` | JSON number `0` or `1`. |
| `float32` | `customBytes[N..N+3]` | Native four-byte float copied with `memcpy`. |
| `hex-color` | `customBytes[N..N+2]` | String `"#RRGGBB"`. |
| `uint8[K]` | span covering at least `K` bytes | Fixed JSON number array. |
| `boolean[K]` | span covering at least `K` bytes | Fixed JSON `0`/`1` array. |
| `hex-color[K]` | span covering at least `3*K` bytes | Fixed array of color strings. |

The source syntax must be exactly `customBytes[N]` or `customBytes[N..M]`. Use a unique `name`; room widget `source` properties refer to this name.

The serializer currently treats booleans as numeric `0`/`1`, not JSON `true`/`false`, and uses native float representation. All communicating ESP32 firmware should therefore use the same byte contract.

### 5.11 `ui` and generated room widgets

`roomWidget` is the JavaScript function name generated for this type. Set it to `null` for a type intentionally absent from the room dashboard. If `includedInDeviceCollection` is `false`, no room widget is generated or published regardless of this value.

The current room-widget generator supports these controls:

| `widget` | Relevant fields | Current behavior |
|---|---|---|
| `toggle-button` | `displayOrder` | Calls the common state-switch UI handler. |
| `settings-button` | `displayOrder` | Opens advanced controls; required when `ui.advancedControls` exists. |
| `play-stop-button` | `displayOrder` | Uses HTTP field `liveStatus` and calls the live-animation handler. |
| `range` | `minimum`, `maximum`, `source`, optional `visibleWhen` | Calls the common range/brightness handler. `visibleWhen` currently recognizes only `field == true`. |

Supported readouts:

| `widget` | Relevant fields | Current behavior |
|---|---|---|
| `numeric-label` | `source`, optional `label`, `unit`, `valueLabels`, `unknownValue`, `unknownLabel` | Displays a metric tile. `valueLabels` maps numeric codes to readable text; `unknownValue` replaces one sentinel value with `unknownLabel`. |
| `status-label` | `source`, optional `label`, `trueLabel`, `falseLabel`, `trueTone`, `falseTone` | Maps a boolean/numeric state to two labels. Optional tones are `neutral`, `accent`, `success`, `warning`, or `danger`. |
| `color-swatch` | `source` | Displays one generated hex-color HTTP field. |
| `temperature-gauge` + `humidity-gauge` | each has `source` | The current generator renders these as a pair. |
| `error-message` | `source` | Used by the temperature/humidity pair to show sensor error. |

The schema intentionally permits additional widget strings and existing metadata mentions items such as `color-array`, `toggle-array`, `color-swatch-array`, `serviceRead`, and `serviceWrite`. The current generic generator does not render those items. Extend [extra/generate_device_registry.py](extra/generate_device_registry.py) and the common HTTP handlers before depending on a new widget.

Also note that service fields in `ui.controls` document intent, but the current generated widgets call a small set of hard-coded common JavaScript handlers. They are not yet a fully generic service dispatcher. Reuse the established service for each widget or extend the generator and HTTP layer.

`displayOrder` affects only the three button widgets. Range controls are appended separately.

### 5.12 `extendedMemory`

Use no extended memory for ordinary devices:

```json
"extendedMemory": { "required": false, "size": 0 }
```

For a large runtime/persistent buffer:

```json
"extendedMemory": {
  "required": true,
  "size": "4 * virtualLedCount * sizeof(LedColor)",
  "layout": ["active frame", "saved frame 1", "saved frame 2", "saved frame 3"]
}
```

`size` and `layout` are descriptive; C++ `getExtendedMemoryLength()` is authoritative at runtime. When the length is nonzero, `DeviceManager` obtains or reserves storage and sends its pointer through `DEVSERVICE_SET_EXT_MEMORY_PTR` before `init()`. A newly requested allocation can require one reboot. Validate the pointer and size in that service and do not free manager-owned memory.

### 5.13 `limitations`

List real restrictions, incomplete behavior, hardware assumptions, non-persistent settings, and known incompatibilities. This is contributor-facing documentation and should not be left misleadingly empty for an incomplete driver.

## 6. Optional advanced controls

Advanced controls are appropriate for palettes, animations, grouped settings, or another interaction too complex for the generated room widget. A device owns an HTML fragment plus JavaScript that decodes and encodes an opaque byte payload.

A complete writable implementation needs all of the following:

1. `ui.advancedControls` in JSON;
2. an implemented `DEVSERVICE_GET_ADVANCED_CONTROLS` using `serviceCall_3`;
3. an implemented `DEVSERVICE_SET_ADVANCED_CONTROLS` using `serviceCall_3`;
4. a `settings-button` bound to the GET service;
5. an HTML file containing exactly an HCS controller script tag;
6. C++ and JavaScript that agree on every payload byte.

For a read-only statistics or diagnostics panel, set `"readOnly": true` next to `payloadSize`, implement only `DEVSERVICE_GET_ADVANCED_CONTROLS`, and omit save controls from the HTML controller.

### 6.1 JSON for a fixed payload

```json
"advancedControls": {
  "template": "include/devices/MyDevice/AdvancedControls.html",
  "payloadSize": { "fixedBytes": 4 },
  "readOnly": false
}
```

The size must be from 1 through 391 bytes.

### 6.2 JSON for a dynamic payload

```json
"payloadSize": {
  "countState": "channelCount",
  "maximumCount": 100,
  "elementBytes": 3,
  "overheadBytes": 5
}
```

`countState` must name a field in `state.descriptionCustomBytes`. Runtime size is:

```text
current count * elementBytes + overheadBytes
```

The build checks `maximumCount * elementBytes + overheadBytes <= 391`. If the published count exceeds `maximumCount`, runtime payload size becomes zero and the popup cannot load.

### 6.3 Service metadata

```json
{
  "name": "DEVSERVICE_GET_ADVANCED_CONTROLS",
  "overload": "serviceCall_3",
  "status": "implemented",
  "parameters": [
    {
      "name": "controls",
      "type": "byte[]",
      "source": "ServiceParameters_set3.buff",
      "direction": "out",
      "size": "4"
    }
  ]
},
{
  "name": "DEVSERVICE_SET_ADVANCED_CONTROLS",
  "overload": "serviceCall_3",
  "status": "implemented",
  "parameters": [
    {
      "name": "controls",
      "type": "byte[]",
      "source": "ServiceParameters_set3.buff",
      "direction": "in",
      "size": "4"
    }
  ]
}
```

### 6.4 C++ service implementation

An explicit byte codec avoids struct padding and endian surprises:

```cpp
ServiceRequestErrorCode MyDevice::service(
    DeviceServicesType serviceType,
    ServiceParameters_set3 parameters)
{
    constexpr uint16_t payloadSize = 4;

    switch (serviceType)
    {
    case DEVSERVICE_GET_ADVANCED_CONTROLS:
        if (parameters.buff == nullptr || parameters.size != payloadSize ||
            parameters.direction != e_OUT_from_DEVICE)
        {
            return SERV_EXECUTION_FAILURE;
        }
        parameters.buff[0] = enableAnimation;
        parameters.buff[1] = disableAnimation;
        parameters.buff[2] = static_cast<uint8_t>(animationTimeMs & 0xFF);
        parameters.buff[3] = static_cast<uint8_t>(animationTimeMs >> 8);
        return SERV_SUCCESS;

    case DEVSERVICE_SET_ADVANCED_CONTROLS:
        if (parameters.buff == nullptr || parameters.size != payloadSize ||
            parameters.direction != e_IN_to_DEVICE)
        {
            return SERV_EXECUTION_FAILURE;
        }
        if (parameters.buff[0] > maximumAnimation ||
            parameters.buff[1] > maximumAnimation)
        {
            return SERV_EXECUTION_FAILURE;
        }
        enableAnimation = parameters.buff[0];
        disableAnimation = parameters.buff[1];
        animationTimeMs = static_cast<uint16_t>(parameters.buff[2]) |
                          (static_cast<uint16_t>(parameters.buff[3]) << 8);
        return SERV_SUCCESS;

    default:
        return SERV_NOT_SUPPORTED;
    }
}
```

If a POD struct is copied directly, add a `static_assert(sizeof(StructName) == fixedBytes)` and make its byte order/padding part of the contract. Prefer explicit byte offsets for persistent or cross-language formats; the temperature sensor history is an example.

Advanced-control changes are runtime-only unless the class explicitly copies them into persistent storage and invokes the `persistentDataChanged` callback.

### 6.5 HTML controller

The HTML is embedded in firmware and mounted inside the generic popup:

```html
<div class="advanced-controls-form" data-role="my-device-controls">
  <label>Enable animation
    <select data-field="enableAnimation">
      <option value="0">None</option>
      <option value="1">Fade</option>
    </select>
  </label>
  <label>Animation time [ms]
    <input data-field="animationTime" type="number" min="0" max="65535">
  </label>
  <button class="popup-button" data-action="save">Save</button>
</div>
<script type="application/x-hcs-advanced-controls">
const payload = context.payload;
if (payload.length !== 4) {
  context.showError('Invalid MyDevice payload.');
  return;
}

const animation = context.root.querySelector('[data-field="enableAnimation"]');
const duration = context.root.querySelector('[data-field="animationTime"]');
animation.value = String(payload[0]);
duration.value = String(payload[2] | (payload[3] << 8));

context.root.querySelector('[data-action="save"]').addEventListener('click', async () => {
  const value = Math.max(0, Math.min(65535, Number(duration.value) || 0));
  const next = new Uint8Array(4);
  next[0] = Number(animation.value);
  next[1] = payload[1];
  next[2] = value & 255;
  next[3] = value >> 8;
  if (await context.save(next)) context.close();
});
</script>
```

The controller receives:

| Context member | Meaning |
|---|---|
| `root` | Popup root element; scope selectors to this element. |
| `deviceId`, `deviceType`, `deviceName` | Current device metadata. |
| `payload` | `Uint8Array` returned by the GET service. |
| `descriptionBytes` | The current 50-byte `DeviceDescription::customBytes` as `Uint8Array`. |
| `save(payload)` | Sends `DEVSERVICE_SET_ADVANCED_CONTROLS` with `additionalParam = 255`; resolves to success/failure. |
| `action(value)` | Sends the SET service with an empty payload and the supplied one-byte action value. |
| `close()` | Closes the popup. |
| `showError(message)` | Replaces popup content with an error. |
| `onCleanup(callback)` | Registers cleanup for document-level listeners or other external resources. |

Use `context.action()` only if C++ intentionally supports action-only SET calls; in that branch, validate `additionalParam` and do not require a nonempty buffer. Use `context.onCleanup()` for listeners attached outside `context.root`.

The template must not contain the reserved raw-string terminator `)HCSADV"`.

## 7. What the build generates and validates

The pre-build scripts in [platformio.ini](platformio.ini) run in this order:

1. `extra/generate_build_info.py` creates build metadata.
2. `extra/generate_device_registry.py` deletes `include/generated/`, reads descriptions, and creates type/factory/state/UI/advanced-control integration.
3. `extra/generate_device_config_widgets.py` creates configuration-page metadata.
4. PlatformIO compiles the active C++ sources under `src`.

The generated integration includes:

- stable type enum and names;
- guarded concrete-class includes and factory cases;
- manager scheduling metadata;
- state-to-HTTP serialization;
- room-widget JavaScript;
- advanced-control templates and payload sizing;
- device configuration fields and browser byte encoding.
- schema-derived defaults and per-slot semantic/GPIO validation.

The generators currently check important items such as:

- duplicate or reserved IDs among registered types;
- duplicate registered enum symbols;
- registered type has a factory and known factory arguments;
- registered header and source files exist;
- schedule mode and fixed interval are valid;
- ordinary byte ranges do not overlap;
- configuration/state fields remain inside their 20-/50-byte areas;
- at most eight visible custom configuration fields;
- at most two select options;
- advanced-controls template, services, settings button, and maximum payload.

At runtime, configuration submission also checks JSON value types, exact slot/custom-byte counts, known enabled types, numeric/select bounds, board GPIO availability, ADC restrictions, and cross-slot GPIO conflicts. Only a fully valid six-slot set replaces the current RAM mirror and schedules a safe restart. Imported configuration files pass through the same generated slot validation.

However, do not treat the generator as complete semantic proof. Configuration fields are checked against 20 bytes and state fields against 50 bytes, but still manually review:

- service name/overload agreement with C++;
- constructor signature agreement with factory argument order;
- `descriptionCustomBytes` agreement with `httpFields` sources;
- widget support and expected source fields;
- numeric ranges and buffer/direction validation in C++;
- direct struct layout shared with JavaScript.

The `$schema` line enables JSON-aware editors to apply [include/devices/device.schema.json](include/devices/device.schema.json), but the current Python generator performs its own checks rather than running a complete JSON Schema validator. Keep the document schema-compliant even if a generator does not reject every schema mistake.

## 8. Build and test checklist

Run from the project root:

```sh
platformio run
```

Before running it, verify that only the wanted concrete package directories exist under `include/devices`. An empty active catalog is valid and builds platform-only firmware.

Then test on hardware:

- [ ] The build reports no generator or C++ error.
- [ ] The generated type enum contains the selected symbol and value.
- [ ] The type appears in the device configuration dropdown when configurable.
- [ ] Every custom field renders and saves the expected NVM bytes.
- [ ] A newly selected type renders valid, safe `default` values.
- [ ] Duplicate, unavailable, out-of-range, and malformed GPIO configurations are rejected.
- [ ] The device is reconstructed correctly after reboot.
- [ ] `init()` leaves hardware in a safe state.
- [ ] `cyclic()` follows the expected cadence and remains non-blocking.
- [ ] The device appears in the intended room and its state refreshes.
- [ ] Every HTTP field has the expected JSON value.
- [ ] Each widget invokes a C++ service that returns `SERV_SUCCESS`.
- [ ] Invalid service values and malformed buffers are rejected safely.
- [ ] Advanced controls load, validate their payload, save, and clean up listeners.
- [ ] Extended memory, if used, survives the allocation/reboot flow.
- [ ] A master can render and control the device hosted on a slave.
- [ ] Existing configured devices still decode correctly.

The safest multi-node deployment is to build the same device catalog into every node. A slave needs the C++ factory to instantiate the physical type; a master needs the same JSON-derived state and UI metadata to render a remote instance.

## 9. Common mistakes

| Mistake | Result | Fix |
|---|---|---|
| Using type ID `255` | Type remains unknown. | Choose a unique stable ID from `0..254`. |
| Reusing or changing a deployed ID | Old NVM/network data becomes another type. | Never repurpose IDs; add a new ID or migration. |
| Editing `include/generated/` | Change disappears on the next build. | Edit C++, JSON, HTML, or generator source. |
| Expecting `DevicesPredefined` to compile directly | No concrete devices are generated. | Activate each wanted example explicitly with `activate-examples.sh`. |
| Copying only a predefined JSON/header | Source or template validation fails. | Activate the entire example package. |
| Selecting an LED device without `LedStrip` | Required animation headers/sources are missing. | Activate the `LedStrip` support package too. |
| Factory arguments do not match the constructor | C++ compilation fails. | Match names, order, and callback types exactly. |
| Configuration extends beyond byte 19 | NVM corruption or out-of-bounds access. | Draw and review a 20-byte layout. |
| State extends beyond byte 49 | Description corruption. | Draw and review a 50-byte layout. |
| JSON offset differs from C++ | Wrong dashboard/configuration values. | Keep one byte-layout table and test raw bytes. |
| Omitting a valid `default` for a nonzero-constrained field | A fresh type-selection form may submit zero and be rejected. | Declare a safe schema default and also handle corrupted/legacy bytes in C++. |
| Unsupported custom `htmlType` | Field is not rendered. | Use the supported table or extend the generator. |
| Unsupported room widget string | Control/readout is omitted. | Use current widgets or implement generator and HTTP support. |
| Advertising a service only in JSON | UI request returns unsupported. | Implement the matching C++ overload. |
| Blocking inside `cyclic()` | Delays all devices and platform tasks. | Use timestamps and state machines. |
| Copying a padded struct to advanced controls | C++ and JavaScript disagree. | Prefer explicit bytes; otherwise use fixed-width fields and `static_assert`. |
| Trusting browser constraints | Remote/malformed input may be unsafe. | Validate again in C++. |
| Dynamic advanced count exceeds its declared maximum | Popup payload size becomes zero. | Clamp/validate state and choose a correct maximum. |

## 10. Best predefined examples

- [DevicesPredefined/OnOffDevice/include/devices/OnOffDevice/OnOffDevice.json.example](DevicesPredefined/OnOffDevice/include/devices/OnOffDevice/OnOffDevice.json.example) and [DevicesPredefined/OnOffDevice/src/device/OnOffDevice/OnOffDevice.cpp.example](DevicesPredefined/OnOffDevice/src/device/OnOffDevice/OnOffDevice.cpp.example): configuration fields and switch/brightness services without advanced controls.
- [DevicesPredefined/TempSensorDHT11DeviceType/include/devices/TempSensorDHT11DeviceType/TempSensorDHT11DeviceType.json.example](DevicesPredefined/TempSensorDHT11DeviceType/include/devices/TempSensorDHT11DeviceType/TempSensorDHT11DeviceType.json.example): fixed-interval sensor, persistent ExtendedMemory statistics, read-only advanced controls, gauges, callback factory arguments, and a build guard.
- [DevicesPredefined/LedWS1228bDeviceType/include/devices/LedWS1228bDeviceType/LedWS1228bDeviceType.json.example](DevicesPredefined/LedWS1228bDeviceType/include/devices/LedWS1228bDeviceType/LedWS1228bDeviceType.json.example): dynamic advanced payload, colors, persistent callback, extended memory, and shared `LedStrip` support.
- [DevicesPredefined/HwButton/include/devices/HwButton/HwButton.json.example](DevicesPredefined/HwButton/include/devices/HwButton/HwButton.json.example): callbacks and a device intentionally excluded from the common room collection. Read its listed limitations before copying its byte layout.

When examples and this guide differ from actual behavior, the current C++ interfaces and generator source are authoritative: [include/devices/device.hpp](include/devices/device.hpp), [extra/generate_device_registry.py](extra/generate_device_registry.py), and [extra/generate_device_config_widgets.py](extra/generate_device_config_widgets.py).
