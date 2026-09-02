# Predefined device catalog

This directory contains optional, self-contained include/source overlay packages. It is intentionally **not scanned or compiled**. A clean checkout therefore builds the HomeControlStation platform without concrete devices.

Every device description, header, source, and HTML file ends in `.example`. They must stay examples here. Only a user-created copy under the active `include/` and `src/` trees may have that final extension removed.

The active catalog is [../include/devices](../include/devices). It always contains the shared `Device` interface, metadata schema, and advanced-control structures. Only concrete package directories copied there are discovered by the generators.

## Select a preset

For an OnOff-only station, copy both halves and remove `.example` from the copied filenames by running the explicit activation helper from the project root:

```sh
sh DevicesPredefined/activate-examples.sh OnOffDevice
platformio run
```

The helper never modifies `DevicesPredefined`. Each catalog entry mirrors the project paths: its `include/` tree contains JSON, headers, and optional HTML; its `src/` tree contains implementation sources. The helper copies both trees and removes `.example` only at the destination.

To activate more types, copy each required directory in the same way. Rebuild after every catalog change; generation first deletes stale integration files under `include/generated/`.

## Dependencies

| Device package | Additional package required |
|---|---|
| `OnOffDevice` | none |
| `HwButton` | none |
| `TempSensorDHT11DeviceType` | none; requires its build guard and bundled DHT library |
| `LedWS1228bDeviceType` | `LedStrip` |

For an LED-strip preset:

```sh
sh DevicesPredefined/activate-examples.sh LedStrip LedWS1228bDeviceType
platformio run
```

`LedStrip` is a support package without a JSON device description. PlatformIO compiles its animation sources from the active `src/device/LedStrip` directory.

## Remove a selected type

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

A selected package is a working copy. Improvements intended for all users should be applied to the corresponding package under `DevicesPredefined` and then recopied for testing.

For implementation details and new package authoring, see [../ADDING_A_NEW_DEVICE_TYPE.md](../ADDING_A_NEW_DEVICE_TYPE.md).
