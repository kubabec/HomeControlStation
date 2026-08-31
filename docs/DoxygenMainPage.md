# HomeControlStation

This project implements an embedded home automation controller for ESP-based systems. The firmware coordinates device management, periodic maintenance tasks, configuration persistence, display rendering, and remote-control interfaces.

## Project scope

The documentation covers the project sources under the `include` and `src` directories, with emphasis on:

- Device abstraction layer and device implementations
- System operating loop and scheduler tasks
- Persistent memory and configuration access
- Remote control and network communication
- UI display and status handling
- Hardware dependencies and LED device logic

## English documentation standard

All documentation generated for this project is written in English. Descriptions focus on runtime behavior, lifecycle responsibilities, configuration fields, and service payload semantics for each class and method.

## Main modules

- Device layer: data models, service interfaces, device implementations
- Operating system: scheduler, security, reset handling, and NVM logic
- Network and protocol layer: HTTP, UDP, remote control, and RF dependencies
- Display and reporting: status pages, device previews, and user-facing content
- Hardware drivers: board-specific synchronization and peripheral control

The generated Doxygen pages provide cross-links between classes, methods, and data structures to make the firmware easier to understand and extend.
