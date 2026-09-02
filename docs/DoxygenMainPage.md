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

The operating-system layer does not maintain a handwritten list of concrete device classes. A clean checkout stores optional include/source overlays under `DevicesPredefined/<device>/`, where they are inert. A firmware author copies both trees of only wanted packages into active `include/devices/<device>/` and `src/device/<device>/` locations. Each active implementation is accompanied by a JSON description that conforms to `include/devices/device.schema.json` and is the source of truth for:

- the persistent numeric device type and C++ enum symbol,
- implementation header, class name, build guard, and factory arguments,
- participation in the public device collection,
- lifecycle scheduling for the device's main `cyclic()` function,
- configuration fields and their custom-byte layout,
- runtime state fields exposed over HTTP,
- dashboard controls and readouts,
- optional advanced-control payload sizing and device-owned HTML pattern.

Device type identifiers are part of persisted and network-visible data. Existing identifiers must remain stable, `255` is reserved for an unknown or unconfigured device, and duplicate identifiers cause generation to fail.

## Build-time generation

PlatformIO invokes `extra/generate_device_registry.py` and `extra/generate_device_config_widgets.py` before every compilation. Before loading any descriptions, the registry generator removes the complete `include/generated/` directory. When active descriptions exist, both generators validate them and recreate device-dependent integration artifacts under `include/generated/`. PlatformIO discovers matching selected sources through the normal `src/` tree. This prevents removed or renamed devices from surviving through stale files and keeps unselected predefined sources out of compilation.

An empty device catalog is valid. If `include/devices/` contains no concrete descriptions and `src/device/` contains no concrete sources, generation leaves `include/generated/` absent. Static no-device handlers under `include/devices/fallback/` keep the platform buildable; persisted or handshaked device identifiers are treated as unknown and no concrete device instance is created.

To create a preset, overlay both package trees from `DevicesPredefined` onto `include/` and `src/` before building. LED device types additionally require both trees from the `LedStrip` support package. Removing both active halves and rebuilding removes a type's generated factory, serializers, widgets, and compiled implementation. Selection details are documented in `DevicesPredefined/README.md`.

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
| `GeneratedAdvancedControlsSize.inc` | Advanced-control payload-size dispatch |
| `GeneratedAdvancedControlTemplates.hpp` | Build-guarded device-owned HTML patterns referenced by JSON |

Generated files must not be edited directly. Changes belong in a device JSON description or in a generator.

## Runtime device lifecycle

`DeviceManager` owns all generated device instances through `std::unique_ptr<Device>`. During initialization it reads each NVM slot, validates the type through the generated metadata, builds a `GeneratedDeviceRegistry::RuntimeContext`, and asks the generated factory to construct the concrete class.

All registered instances participate in initialization and cyclic processing. A lifecycle schedule with `mode: every_manager_cycle` runs `cyclic()` whenever the manager runs. A schedule with `mode: fixed_interval` and a positive `intervalMs` runs it when that interval has elapsed. Timing uses wrap-safe `millis()` subtraction; effective resolution is the 10 ms `DeviceManager` task period. Device implementations using a fixed schedule should perform one processing iteration per `cyclic()` call rather than adding another internal interval for the same work.

The `includedInDeviceCollection` metadata flag determines whether an instance is published to the room dashboard and exposed through the regular device service API. This allows infrastructure devices, such as hardware buttons, to run without appearing as controllable room devices.

## HTTP generation pipeline

The HTTP interface has three generated device-dependent layers:

1. **Configuration page** — configuration field metadata creates selectors and controls for the enabled device set.
2. **Configuration submission** — generated JavaScript packs common values and custom fields into a uniform JSON payload. The server decodes this payload without a per-device switch.
3. **Dashboard** — runtime state metadata serializes device custom bytes into named JSON fields. Separate generated widget files render the controls and readouts described by each device's `ui` section. Button controls are ordered by their optional `displayOrder` value, falling back to JSON declaration order. Only widgets whose build guards are enabled by `SystemDefinition.hpp` are included in the firmware.

`DeviceManager` publishes its latest local descriptions once per second, and the dashboard requests the complete device list once per second. This HTTP cadence is independent of individual device schedules: fast sensors may execute several times between requests, while slow sensors may return the same latest sample across many requests.

## Generic advanced controls

Advanced controls are a generic capability rather than a platform-owned list of device renderers. A capable device provides an HTML pattern beside its implementation and references it through `ui.advancedControls.template`. The same section declares either a fixed byte count or a state-dependent element and overhead byte count. Build-time generation validates the path and embeds the pattern in `GeneratedAdvancedControlTemplates.hpp`.

The platform always reads opaque bytes through `DEVSERVICE_GET_ADVANCED_CONTROLS` using `serviceCall_3` and writes them through `DEVSERVICE_SET_ADVANCED_CONTROLS` using the same overload. Payloads are limited to 391 bytes by the current asynchronous request transport. Fixed and maximum dynamic sizes are validated during generation. `AdvancedControlsLoader` does not include concrete device classes or interpret device payload fields. Its JSON response contains the pattern, payload bytes, and description bytes. The generic browser host mounts the pattern and executes its controller script with a `context` object containing:

- `root`, `deviceId`, `deviceType`, and `deviceName`,
- `payload` and `descriptionBytes` as `Uint8Array` values,
- `save(payload)` for a normal opaque-byte update,
- `action(additionalParam)` for a device-defined action using the common write service,
- `close()`, `showError(message)`, and `onCleanup(callback)` lifecycle helpers.

The pattern controller is placed in a `<script type="application/x-hcs-advanced-controls">` element. It owns payload decoding, DOM event handling, validation, and encoding. The device C++ class owns the corresponding binary contract and service behavior. Pattern scripts are trusted build-time resources and are never accepted from runtime HTTP input.

## Adding a device type

To add a device without modifying the operating-system implementation:

1. Select an existing complete package from `DevicesPredefined`, or implement a new class derived from `Device` in a self-contained active package.
2. Add the JSON description beside the implementation header under `include/devices/<device>/` and the implementation source under `src/device/<device>/`.
3. Assign a new, explicit, previously unused type identifier and enum symbol.
4. Describe constructor dependencies with the supported factory argument names.
5. Define configuration-byte and description-state layouts with non-overlapping offsets.
6. Select `every_manager_cycle`, or select `fixed_interval` and provide `intervalMs`, under `lifecycle.update.schedule`.
7. Describe dashboard controls and readouts in the `ui` section.
8. For advanced controls, implement the common get/set services, add the HTML pattern, and reference its path and payload size in `ui.advancedControls`.
9. Run a PlatformIO build and inspect generator validation errors.
10. Place a reusable finished include/source overlay under `DevicesPredefined/<device>/` while keeping the clean-checkout active catalog empty.

No concrete device include, factory branch, HTTP state branch, configuration parser, or dashboard type dispatch should be added manually under `include/os` or `src/os`.
