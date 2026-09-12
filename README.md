<div align="center">
<p align="right">
  <img src="https://flagcdn.com/w40/pl.png" width="16" alt="Polska">
</p>
  <!-- PLACEHOLDER: Insert your logo image here -->
  <img src="docs/logo2.png" alt="HomeControlStation Logo" width="200"/>


  **Decentralized, Serverless Smart Home Network Powered Exclusively by ESP32**

  [![PlatformIO](https://img.shields.io/badge/Platform-PlatformIO-orange.svg?style=for-the-badge&logo=platformio)](#)
  [![ESP32 Network](https://img.shields.io/badge/Hardware-ESP32-blue.svg?style=for-the-badge&logo=espressif)](#)
  [![No Central Hub](https://img.shields.io/badge/Architecture-Decentralized-success.svg?style=for-the-badge)](#)
  [![License](https://img.shields.io/badge/license-MIT-green.svg?style=for-the-badge)](#)

  *No Raspberry Pi. No Home Assistant. Just standalone ESP32 microcontrollers communicating directly with each other.*

  [www.homecontrolstation.pl](http://www.homecontrolstation.pl)
</div>

<br><br>

## 🌟 Welcome to True Edge of Home Controls

**HomeControlStation** completely flips the traditional smart home model upside down. Instead of relying on a dedicated server, hub, or central PC, this system operates entirely as a **decentralized, distributed ESP32 node network**. 

Built for **PlatformIO** and **Arduino IDE**, every single ESP32 runs standalone firmware equipped with its own web stack, hardware state manager, and local network control routines.

* ESP32-S3 required for Master node

<br><br>

## ✨ System Architecture & Highlights

* **🚫 Serverless & Decentralized:** No host PC / RPI required
* **👑 Master / Slave Topology:** One node acts as the primary gateway while lightweight slave nodes seamlessly join the HCS network.
* **🌐 Embedded Web Interface:** Configured nodes serve a local, high-speed interface via `home.local` accessible across your local network using web browser on any device.
* **🛠 Dynamic Pin Mapping:** Define relay switches, sensors, or dimmers directly on pinouts via software without rewriting code for every hardware variant.
<br><br>

## ⚡ Quick Setup & User Experience Flow

1. **Flash Firmware:** Flash the **HomeControlStation** firmware onto your ESP32 device using PlatformIO or Arduino IDE.
2. **Wi-Fi & Master Configuration:** Connect to the temporary Access Point network broadcasted by the ESP32, configure the node as **Master**, and enter your home Wi-Fi credentials.
3. **GPIO Pin Assignment:** Navigate to **`http://home.local`** within your home network and configure the GPIO pins to run the end device in your desired hardware type (e.g., relay, sensor, switch).
4. **Local Network Control:** Use the device remotely across your home network directly from the main page.
5. **Add a Slave Device:** Take another ESP32 device, follow the initial configuration steps, and assign it as a **Slave** within your HomeControlStation network.
6. **Slave Hardware Setup:** Configure the Slave's GPIO pins to run another end device in another location or room.
7. **Distributed Automation:** Enjoy using the powerful distributed architecture of HomeControlStation with zero external server dependencies!

<br><br>

## 📸 Media, Demos & Showcase

#### System demo
[![HomeControlStation usage](https://img.youtube.com/vi/fdsndvb5hg0/maxresdefault.jpg)](https://www.youtube.com/watch?v=fdsndvb5hg0)



#### Setup video guide
[![HomeControlStation guide](https://img.youtube.com/vi/TAbCHl1-19U/maxresdefault.jpg)](https://www.youtube.com/watch?v=TAbCHl1-19U)


<br><br>

## Check also

> **New to the project?** Start with the [architecture and end-to-end flow guide](ARCHITECTURE.md) for the Platform/Devices boundary, build-time generation, runtime lifecycle, and ESP32 UDP network diagrams.

> **Adding hardware support?** Follow [Adding a new device type](ADDING_A_NEW_DEVICE_TYPE.md) for the C++ interface, JSON metadata, services, generated UI, byte layouts, and optional advanced controls.

> **Building a device preset?** The concrete devices compiled into a firmware image are the package directories currently present under `include/devices/` and `src/device/`. Use the additive activation helper documented in [DevicesPredefined](DevicesPredefined) to copy complete opt-in packages, then run `platformio run`.


---

<div align="center">
  <b>Built for Builders. Powered by ESP32. Fully Local.</b> <br>
  <i>Found this project helpful? Give it a ⭐ on GitHub!</i>
</div>