# HomeControlStation {#mainpage}

This project implements an embedded home automation controller for ESP-based systems. The firmware coordinates device management, periodic maintenance tasks, configuration persistence, display rendering, and remote-control interfaces.

## Project scope

The documentation covers the project sources under the `include` and `src` directories, with emphasis on:

- Device abstraction layer and device implementations
- System operating loop and scheduler tasks
- Persistent memory and configuration access
- Remote control and network communication
- UI display and status handling
- Hardware dependencies and LED device logic
- Build-time device discovery and generated registries
- Schema-driven HTTP configuration and dashboard widgets

## English documentation standard

All documentation generated for this project is written in English. Descriptions focus on runtime behavior, lifecycle responsibilities, configuration fields, and service payload semantics for each class and method.

## Main modules

- Device layer: data models, service interfaces, device implementations
- Operating system: scheduler, security, reset handling, and NVM logic
- Network and protocol layer: HTTP, UDP, remote control, and RF dependencies
- Display and reporting: status pages, device previews, and user-facing content
- Hardware drivers: board-specific synchronization and peripheral control
- Device code generation: type IDs, factories, configuration codecs, state serializers, and browser widgets

The generated Doxygen pages provide cross-links between classes, methods, and data structures to make the firmware easier to understand and extend.

## Schema-driven device architecture

The operating-system layer does not maintain a handwritten list of concrete device classes. Each device implementation is accompanied by a JSON description under `include/devices/<device>/`. The description conforms to `include/devices/device.schema.json` and is the source of truth for:

- the persistent numeric device type and C++ enum symbol,
- implementation header, class name, build guard, and factory arguments,
- participation in the public device collection,
- configuration fields and their custom-byte layout,
- runtime state fields exposed over HTTP,
- dashboard controls and readouts,
- optional advanced-control payload and renderer registration.

Device type identifiers are part of persisted and network-visible data. Existing identifiers must remain stable, `255` is reserved for an unknown or unconfigured device, and duplicate identifiers cause generation to fail.

## Build-time generation

PlatformIO invokes `extra/generate_device_registry.py` and `extra/generate_device_config_widgets.py` before every compilation. The generators validate the discovered JSON descriptions and produce the integration layer under `include/generated/` and `src/os/app/http/`.

The generated artifacts include:

| Artifact | Responsibility |
|---|---|
| `GeneratedDeviceTypes.hpp` | Stable `DevType` constants and type-name lookup |
| `GeneratedDeviceRegistry.hpp` | Enabled implementations, factory construction, and collection participation |
| `GeneratedDeviceStateSerializer.hpp` | Conversion of `DeviceDescription::customBytes` to HTTP JSON fields |
| `GeneratedDeviceConfiguration.hpp` | Browser-side packing of configuration fields into the common slot representation |
| `GeneratedDeviceWidgets.hpp` | Aggregate dashboard widget bundle for the active build profile |
| `generated/widgets/Generated*Widget.hpp` | One independently generated dashboard widget per device type |
| `GeneratedDeviceConfigWidgets.hpp` | Configuration-page selectors and type-specific field controls |
| `GeneratedAdvancedControls*.inc` | Advanced-control payload-size and renderer dispatch |

Generated files must not be edited directly. Changes belong in a device JSON description or in a generator.

## Runtime device lifecycle

`DeviceManager` owns all generated device instances through `std::unique_ptr<Device>`. During initialization it reads each NVM slot, validates the type through the generated metadata, builds a `GeneratedDeviceRegistry::RuntimeContext`, and asks the generated factory to construct the concrete class.

All registered instances participate in initialization and cyclic processing. The `includedInDeviceCollection` metadata flag determines whether an instance is published to the room dashboard and exposed through the regular device service API. This allows infrastructure devices, such as hardware buttons, to run without appearing as controllable room devices.

## HTTP generation pipeline

The HTTP interface has three generated device-dependent layers:

1. **Configuration page** — configuration field metadata creates selectors and controls for the enabled device set.
2. **Configuration submission** — generated JavaScript packs common values and custom fields into a uniform JSON payload. The server decodes this payload without a per-device switch.
3. **Dashboard** — runtime state metadata serializes device custom bytes into named JSON fields. Separate generated widget files render the controls and readouts described by each device's `ui` section. Button controls are ordered by their optional `displayOrder` value, falling back to JSON declaration order. Only widgets whose build guards are enabled by `SystemDefinition.hpp` are included in the firmware.

Advanced controls retain their specialized renderer implementations, while payload sizing and type-to-renderer dispatch are generated from device metadata.

## Adding a device type

To add a device without modifying the operating-system implementation:

1. Implement a class derived from `Device`.
2. Add a JSON description beside the implementation header.
3. Assign a new, explicit, previously unused type identifier and enum symbol.
4. Describe constructor dependencies with the supported factory argument names.
5. Define configuration-byte and description-state layouts with non-overlapping offsets.
6. Describe dashboard controls and readouts in the `ui` section.
7. Add advanced-control metadata only when the device supports that service.
8. Run a PlatformIO build and inspect generator validation errors.

No concrete device include, factory branch, HTTP state branch, configuration parser, or dashboard type dispatch should be added manually under `include/os` or `src/os`.
