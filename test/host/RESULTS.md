# Host test results

Last checked: 2026-09-02 on macOS, AppleClang 16, Debug configuration.

## Result

- 32 scenarios compiled and executed.
- 32 scenarios passed normally.
- 0 expected failures remain.
- 0 unexpected failures.
- CTest result: passed.
- UndefinedBehaviorSanitizer result: passed. AddressSanitizer is additionally
  enabled by the same option on non-macOS hosts; the installed AppleClang ASan
  runtime aborts during its own initialization on this machine.

## Covered OS behavior

- `DeviceDescription` byte serialization/deserialization, metadata, custom
  bytes, invalid buffers, and name boundary.
- `MessageUDP` framing, payload/metadata round-trip, invalid checksum byte, and
  truncated packet rejection.
- `RcRequest` and `RcResponse` payload round-trip, CRC validation/tampering, and
  minimum input sizes.
- `DeviceDescriptionPacker` empty, boundary, multi-chunk, out-of-range, and
  order-preserving pack/unpack behavior.
- `ExtendedDataAllocator` allocation, identifier lookup, deletion, and missing
  identifier behavior.
- `DataContainer` value storage and multiple-subscriber notification.

## Covered device behavior

- OnOffDevice: active-high/low GPIO, on/off services, PWM mapping, brightness
  progression, and advanced-control payload round-trip/size validation.
- WindowDoorSensor: startup sampling and debounce in both directions.
- WindowBlinder: motor interlock, limits, timeout fault, state payload, and
  malformed advanced-control requests.
- Gate: 250 ms output interlock, one/two-motor operation, endpoint stop,
  timeout fault, state payload, and malformed requests.
- AquariumController: NTC conversion path, hysteresis heating, low-water and
  failed-ADC safety shutdown, auxiliary outputs, and control validation.
- HwButton predefined package: interrupt wiring; single, double, and long press
  recognition; local/remote callbacks; metadata; unsupported services.
- TempSensorDHT predefined package: valid temperature/humidity publication,
  failed reading, jump rejection, metadata, and unsupported services.
- LedWS1228b predefined package: virtual storage size, extended-memory setup,
  saved-content callback, averaged state colors, detailed payload sizes, and
  service dispatch.
- LedStrip support package: FadeIn, FadeOut, RollIn, RollOut, BounceIn,
  SparkleIn, TwinkleIn, WaveIn, and live SingleWave behavior. Every predefined
  animation source is also compiled by the host target.

## Fixed defects

1. `RcRequest::toByteArray()` and `RcResponse::toByteArray()` now reject a null
  or incorrectly sized destination before writing.
2. OnOffDevice now rejects state values other than 0/1, brightness above 100%,
  and brightness requests on devices without dimming support. A no-op
  brightness request no longer divides by zero.
3. WaveInAnimation now combines its wave with bounded global progress, restores
  exact target colors, and completes after one finite cycle.
4. LedWS1228bDeviceType now returns `SERV_BUSY` when an enabling or disabling
  animation prevents a service from being processed. Callers can retry without
  mistaking an untouched output payload for a successful response.

All former `XFAIL` cases are normal passing regression tests.

## Compile-time findings

The strict host build also reports issues worth cleaning up later:

- multiple signed/unsigned and narrowing conversions;
- C-style anonymous typedef structs with C++ default member initializers;
- `Logger.hpp` has extra tokens after `#endif`;
- MessageUDP uses deprecated `sprintf()` and a non-const equality operator;
- several predefined source parameters and fields are unused.

Two pointer-to-`int` logging casts found by host compilation were replaced with
`uintptr_t`-width formatting in production code; this does not change firmware
logic.

Case-mismatched includes discovered by the strict build were corrected so the
tested platform and predefined-device sources also compile on case-sensitive
filesystems.

## Intentionally skipped on the host

- Real ESP32 GPIO electrical behavior, ADC calibration/noise, interrupts under
  concurrency, timers, watchdogs, FreeRTOS scheduling, and PSRAM allocation.
- Wi-Fi, UDP sockets, HTTP server, OTA, ESP NVM, RF hardware, and reset paths.
- Real DHT timing/protocol and WS2812 waveform/current behavior.
- Full HomeStation OS startup orchestration and on-device integration timing.

These require ESP32 hardware-in-the-loop or dedicated adapter abstractions. The
host fakes validate control logic and API contracts, not electrical correctness.
