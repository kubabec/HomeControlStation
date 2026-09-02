# AquariumController

Combines a water-temperature monitor, hysteretic heater thermostat, light output, filter output, and optional low-water heater interlock. Advanced Controls changes heater mode and switches light/filter at runtime; temperatures and output states are published to the room dashboard.

## Temperature probe

This implementation expects a waterproof **10 kΩ B3950 NTC thermistor** and a 10 kΩ 1% series resistor:

```text
3.3 V --- 10 kΩ series resistor ---+--- configured ADC Pin
                                   |
                              10 kΩ NTC probe
                                   |
                                  GND
```

Use Wi-Fi-safe ESP32-S3 ADC1 GPIO `1..10` and short, protected wiring. Other GPIOs are rejected for the temperature input. The built-in constants must be changed in the source for a different thermistor or divider. Calibration against a trusted thermometer is recommended.

## Outputs and level input

- Heater, light, and filter GPIOs drive properly rated **isolated** relay/SSR modules, never mains loads directly.
- The optional low-water float switch connects between its GPIO and GND and is active LOW with internal pull-up. Set its GPIO to `255` to disable it.
- All outputs share the configured active level. On boot, heater and light are off, filter is on, and heater mode is Automatic.
- Fault bit `1` means temperature sensor failure, `2` low water, and `4` over-temperature. Any fault forces the heater off, including Forced-on mode.

Use an independent mechanical/electronic heater thermostat, RCD/GFCI, drip loops, correct fusing, and an alarm. HomeControlStation is not a life-support or electrical-safety system. Runtime Advanced Controls are intentionally not persisted; fixed setpoint, hysteresis, cutoff, and pin assignments come from the device configuration slot.

## Configuration and validation

New slots default to a 25.0 °C setpoint, 0.5 °C hysteresis, 32.0 °C cutoff, active-LOW outputs, and a disabled (`255`) low-water input. Heater, light, and filter GPIOs are required and must be distinct from the ADC input, an enabled low-water input, and every GPIO used by another active slot.

Non-ADC GPIOs must be in `0..48`, excluding ESP32-S3 GPIO `22..32`. Setpoint accepts 5.0–40.0 °C, hysteresis 0.1–5.0 °C, and cutoff 5.0–60.0 °C. C++ additionally falls back to safe thermostat values if corrupted legacy bytes reach the constructor. Probe failure, low water, or over-temperature always forces the heater off.

Activate with `sh DevicesPredefined/activate-examples.sh AquariumController`.