# Gate

Controls one or two reversible gate motors through isolated direction-command inputs. Open, Stop, and Close are exposed in Advanced Controls. A 250 ms software interlock separates direction changes; opened/closed endpoints and a travel timeout stop all outputs.

## Hardware and connections

Use certified gate control hardware, safety relays/contactors, limit switches, obstacle detection, emergency stop, and manual release appropriate to the installation. The ESP32 outputs must drive only isolated low-voltage command inputs—never a motor winding or mains contactor coil directly.

- Common Pin: motor 1 OPEN command.
- Motor 1 CLOSE: motor 1 close command.
- Motor 2 OPEN/CLOSE: second leaf commands, ignored in single-motor mode.
- Opened/Closed limit: dry contacts to GND; inputs are active LOW with internal pull-ups. Set `255` to disable an endpoint.

Set the timeout slightly above measured travel time. Hardware interlocking is mandatory even though software prevents simultaneous direction commands. This implementation is an integration example, **not a certified gate safety controller**; leave obstacle edges, photocells, force limits, and emergency stops connected to the certified gate controller.

## Configuration and validation

A newly selected Gate defaults to one motor, disabled (`255`) endpoint inputs, active-LOW outputs, and a 60-second travel timeout. Motor 2 pins are used and claimed only when **Double motor** is selected.

Every used motor output and endpoint must be distinct and must not overlap another active slot. GPIOs must be in `0..48`, excluding ESP32-S3 GPIO `22..32`; optional endpoints may instead be `255`. The timeout range is 1–900 seconds. Invalid configurations are rejected before the stored slots are changed.

Every Open/Close request first switches all motor outputs off. The requested direction starts only after the 250 ms interlock delay. Reaching the relevant endpoint or exceeding the timeout stops all outputs.

Activate with `sh DevicesPredefined/activate-examples.sh Gate`.