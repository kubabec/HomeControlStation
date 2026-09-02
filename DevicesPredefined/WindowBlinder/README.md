# WindowBlinder

Controls a reversible roller-blind, shutter, or curtain motor without blocking the HomeControlStation scheduler. It provides Open, Stop, and Close actions, independent travel timeouts, endpoint inputs, an estimated position, and timeout/limit-conflict faults.

## Required hardware

- A motor with a suitable **interlocked** two-direction relay, contactor, or H-bridge. Never connect a motor to an ESP32 GPIO.
- Optional upper and lower dry-contact limit switches.
- A power supply, fusing, isolation, and enclosure appropriate for the motor voltage.

## Connections

| Signal | Configuration | Connection |
|---|---|---|
| Up command | Common Pin field | Driver UP input |
| Down command | Motor DOWN GPIO | Driver DOWN input |
| Upper endpoint | Upper limit GPIO | Switch between GPIO and GND for active-LOW operation |
| Lower endpoint | Lower limit GPIO | Switch between GPIO and GND for active-LOW operation |

Set an unused limit GPIO to `255`. Inputs use the ESP32 internal pull-up; active-LOW normally gives the safest wiring. Configure timeouts slightly longer than measured full travel. The firmware always deactivates one direction before activating the other, but hardware interlocking is still required because boot, wiring, or transistor faults are outside software control.

## Configuration and validation

New slots default both optional endpoints to `255`, both travel timeouts to 30 seconds, and output/limit polarity to active LOW. Required UP and DOWN outputs must be distinct. Any enabled endpoint must also use its own GPIO.

Configured GPIOs must be in `0..48`, excluding ESP32-S3 GPIO `22..32`, and cannot overlap GPIOs claimed by another active slot. Timeout values must be 1–600 seconds. The server rejects the whole update before persistence if these rules are violated.

Position remains **Not calibrated** after boot until an endpoint establishes a known 0% or 100% position. A travel timeout stops both outputs and reports a fault; requesting a new direction first deactivates both outputs.

Activate with `sh DevicesPredefined/activate-examples.sh WindowBlinder`.