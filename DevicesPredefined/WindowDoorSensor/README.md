# WindowDoorSensor

Publishes the debounced state of a door, window, hatch, or cabinet contact. It is intended for magnetic reed switches, microswitches, and isolated alarm-panel outputs.

## Wiring

For the usual active-LOW arrangement, connect one side of a normally-open reed switch to the configured common Pin and the other side to GND. Select **Internal pull-up** and **LOW = closed**. A normally-closed loop can instead be represented by choosing the level that matches its closed electrical state.

Long outdoor cable should use external biasing, surge/ESD protection, and preferably an optocoupler or supervised alarm input. Never apply 5 V, 12 V, or alarm-panel loop voltage directly to an ESP32 GPIO. Typical debounce is 30–100 ms.

## Configuration

When this type is selected for a new slot, the generated form defaults to **LOW = closed**, **Internal pull-up**, and **50 ms** debounce. The accepted debounce range is 10–5000 ms.

Choose a GPIO from `0..48`, excluding the unavailable/reserved ESP32-S3 range `22..32`. The GPIO must not be used by another active device field. Saving rejects an invalid or duplicate assignment without replacing the previous configuration or restarting the station.

Activate with `sh DevicesPredefined/activate-examples.sh WindowDoorSensor`.