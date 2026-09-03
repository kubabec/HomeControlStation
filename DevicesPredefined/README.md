# Predefined device catalog and activation guide

This directory contains optional, self-contained include/source overlay packages. It is intentionally **not scanned or compiled**. A clean checkout therefore builds the HomeControlStation platform without concrete devices.

Every device description, header, source, and HTML file ends in `.example`. They must stay examples here. Only a user-created copy under the active `include/` and `src/` trees may have that final extension removed.

The active catalog is [../include/devices](../include/devices). It always contains the shared `Device` interface, metadata schema, and advanced-control structures. Only concrete package directories copied there are discovered by the generators.

## Available packages

| Package | Role | Main behavior | Extra dependency or hardware |
|---|---|---|---|
| `OnOffDevice` | Room device | Relay/output switching and optional brightness support | Isolated output driver |
| `HwButton` | Local input/helper | Maps hardware button actions to device toggles or digital events | None; review its package limitations |
| `TempSensorDHT11DeviceType` | Room sensor | Temperature/humidity publication and seven-day persistent trends | Bundled DHT library, matching build guard, and 352 bytes of shared extended memory |
| `LedStrip` | Support library | Shared LED-strip animation/runtime implementation | Required by `LedWS1228bDeviceType`; not a selectable device by itself |
| `LedWS1228bDeviceType` | Room device | Addressable LED strip, segments, colors, animations, extended memory | `LedStrip` and a correctly powered WS2812-compatible strip |
| `WindowBlinder` | Room device | Interlocked Open/Stop/Close, optional endpoints, estimated position, travel timeouts | Interlocked reversible motor driver |
| `WindowDoorSensor` | Room sensor | Debounced dry-contact open/closed state | Reed switch, microswitch, or isolated contact |
| `Gate` | Room device | One/two-motor Open/Stop/Close, endpoint inputs, direction interlock, travel timeout | Certified isolated gate controller |
| `AquariumController` | Room device | NTC temperature, heater thermostat, low-water cutoff, light and filter | 10 kΩ B3950 divider and isolated load drivers |

## Activate packages

Run the activation helper from the project root. For example:

```sh
sh DevicesPredefined/activate-examples.sh OnOffDevice
platformio run
```

The helper:

1. validates each package name;
2. copies its `include/` and `src/` overlays into the matching project paths;
3. removes the final `.example` suffix only from destination filenames;
4. overwrites destination files belonging to the selected package;
5. leaves the catalog unchanged.

Activation is **additive**. It does not remove previously active device directories and it does not create links. Later edits in `DevicesPredefined` are therefore not reflected in active files until the package is activated again. Likewise, edits made only to active files do not update the catalog.

Activate all packages needed by that firmware image in one command, or invoke the helper repeatedly. Rebuild after every active-catalog change; generation first deletes stale integration files under `include/generated/`.

For the currently documented home-automation set:

```sh
sh DevicesPredefined/activate-examples.sh OnOffDevice WindowBlinder WindowDoorSensor Gate AquariumController
platformio run
```

## Dependency examples

| Device package | Additional package required |
|---|---|
| `OnOffDevice` | none |
| `HwButton` | none |
| `TempSensorDHT11DeviceType` | none; requires its build guard, bundled DHT library, and 352 bytes of shared extended memory |
| `LedWS1228bDeviceType` | `LedStrip` |
| `WindowBlinder` | none; requires an interlocked two-direction motor driver |
| `WindowDoorSensor` | none; requires a dry contact/reed switch |
| `Gate` | none; requires certified, isolated gate motor controls |
| `AquariumController` | none; requires a 10k B3950 NTC divider and isolated load drivers |

For an LED-strip preset:

```sh
sh DevicesPredefined/activate-examples.sh LedStrip LedWS1228bDeviceType
platformio run
```

`LedStrip` is a support package without a JSON device description. PlatformIO compiles its animation sources from the active `src/device/LedStrip` directory.

## Configure activated devices

After uploading the rebuilt firmware, open **Settings → Devices Management** (`/localDevices`). Select a type, choose distinct GPIOs, complete its generated fields, enable the slot, and save. The station accepts exactly six slots and restarts after a successful save.

Newly selected types display schema defaults, such as a 50 ms WindowDoorSensor debounce and disabled optional limit inputs. These defaults become persistent only after Save. Existing slots retain their stored values.

The server rejects the complete update without replacing the current configuration when any enabled slot has:

- an unknown type or malformed value;
- a value outside its schema bounds or select options;
- GPIO `22..32`, which is unavailable/reserved in the current ESP32-S3 target;
- a required GPIO outside `0..48`;
- a GPIO already claimed by another active field or slot;
- an AquariumController temperature input outside Wi-Fi-safe ADC1 GPIO `1..10`.

Value `255` disables only fields explicitly declared optional, such as endpoint or low-water inputs. It is invalid for required GPIOs. A rejected save does not schedule a restart; correct the fields and submit again.

## Home-automation package notes

Each package has its own `README.md` with wiring, configuration, limitations, and safety notes:

- [WindowBlinder](WindowBlinder/README.md) — reversible blind/shutter motor, optional endpoints, position estimate, and travel timeouts.
- [WindowDoorSensor](WindowDoorSensor/README.md) — debounced open/closed magnetic or dry contact.
- [Gate](Gate/README.md) — one- or two-motor gate commands, endpoints, interlock delay, and travel timeout.
- [AquariumController](AquariumController/README.md) — NTC temperature sensing, heater thermostat, low-water cutoff, light, and filter.

For example, activate the contact sensor and blind controller together:

```sh
sh DevicesPredefined/activate-examples.sh WindowDoorSensor WindowBlinder
platformio run
```

Motor and mains-powered examples provide control logic only. Use isolated, correctly rated, hardware-interlocked and independently protected equipment. The ESP32 and HomeControlStation firmware are not certified safety controllers.

## Remove active packages

Delete both the active concrete package and any support package no longer used, then rebuild. For example, to return an OnOff-only preset after previously selecting an LED strip, remove:

```text
include/devices/LedWS1228bDeviceType/
include/devices/LedStrip/
src/device/LedWS1228bDeviceType/
src/device/LedStrip/
```

Do **not** delete these platform files:

```text
include/devices/device.hpp
include/devices/device.schema.json
include/devices/AdvancedControls.hpp
```

A selected package is a working copy. Improvements intended for all users must be applied to both the active implementation used for testing and the matching `.example` files under `DevicesPredefined`. Re-activating a package is a convenient way to copy catalog changes into the active tree, but it overwrites active package files.

There is no automatic dependency removal. Remove a support package such as `LedStrip` only when no remaining active device needs it.

For implementation details and new package authoring, see [../ADDING_A_NEW_DEVICE_TYPE.md](../ADDING_A_NEW_DEVICE_TYPE.md).
