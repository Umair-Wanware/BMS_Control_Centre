# BMS Control Centre

> ESP32-based Embedded Control Hub for a Battery Management System

## Overview

BMS Control Centre is an ESP32-C6 based embedded gateway designed to communicate with a custom STM32 Battery Management System over SPI.

The Control Centre serves as the central interface between the embedded BMS firmware and the user by providing real-time monitoring, firmware updates, diagnostics, and a web-based dashboard.

This project is part of a complete Battery Management System ecosystem consisting of:

- STM32 Application Firmware
- STM32 Custom Bootloader
- ESP32 Control Centre
- Web Dashboard

---

## Architecture

```
                    Web Browser
                         │
                 HTTP / WebSocket
                         │
                 ESP32 Control Centre
        ┌────────────────┴────────────────┐
        │                                 │
 Firmware Update                    Live Telemetry
        │                                 │
        └────────────── SPI ──────────────┘
                         │
               STM32 Battery Management
                         │
     ADC • Sensors • Protection • OLED Display
```

---

## Features

### Communication

- SPI Master Communication
- Custom Packet Protocol
- CRC16 Packet Validation
- CRC32 Firmware Validation
- ACK/NACK Packet Handling
- Sequence Number Management

### Firmware Update

- STM32 Firmware Upload
- Packet-Based Image Transfer
- Image Verification
- Retry Mechanism
- Progress Monitoring

### Telemetry

- Real-Time Voltage Monitoring
- Current Monitoring
- Temperature Monitoring
- Battery State of Charge (SOC)
- Fault Monitoring
- Device Status

### Dashboard

- Embedded HTTP Server
- WebSocket Support
- Live Sensor Dashboard
- Firmware Upload Interface
- Device Information
- Configuration Panel

### Storage

- LittleFS
- Configuration Storage
- Firmware Image Storage
- Persistent Settings

### Networking

- Wi-Fi Station Mode
- Access Point Mode
- OTA Updates
- Automatic Reconnection

---

## Project Structure

```
BMS_Control_Centre/

├── src/
│   └── main.cpp
│
├── include/
│
├── lib/
│   ├── application/
│   ├── communication/
│   ├── dashboard/
│   ├── drivers/
│   │   ├── spi/
│   │   ├── wifi/
│   │   └── filesystem/
│   ├── storage/
│   ├── telemetry/
│   ├── updater/
│   └── utils/
│
├── web/
│
├── data/
│
├── partitions.csv
├── platformio.ini
└── README.md
```

---

## Communication Protocol

```
+----------+----------+-----------+---------+---------+----------+
| Header   | Command  | Sequence  | Length  | Payload | CRC32    |
+----------+----------+-----------+---------+---------+----------+
```

### Supported Commands

- Heartbeat
- Telemetry
- Firmware Update
- Configuration
- Diagnostics
- ACK
- NACK
- Reset

---

## Technology Stack

### Hardware

- ESP32-S3
- STM32F103RB

### Framework

- ESP-IDF
- FreeRTOS

### Language

- Modern C++17

### Build System

- PlatformIO

### File System

- LittleFS

---

## Components

### Application

Initializes the entire Control Hub.

Responsibilities

- Hardware Initialization
- Driver Initialization
- Task Startup
- System Supervision

---

### Communication Manager

Handles all communication between the ESP32 and STM32.

Responsibilities

- Packet Encoding
- Packet Decoding
- CRC Validation
- Retries
- ACK/NACK Handling

---

### Firmware Updater

Responsible for updating the STM32 firmware.

Features

- Packet Transfer
- Image Verification
- Retry Logic
- Progress Reporting

---

### Telemetry Manager

Processes sensor data received from the STM32.

Monitored Parameters

- Battery Voltage
- Battery Current
- Temperature
- State of Charge
- Fault Codes

---

### Dashboard Manager

Hosts the embedded web interface.

Features

- REST API
- WebSocket Streaming
- Live Dashboard
- Firmware Upload
- Device Information

---

### Storage Manager

Manages persistent storage.

Features

- Settings
- Firmware Files
- Configuration
- Logs

---

## Development Status

| Module | Status |
|---------|--------|
| ESP-IDF Setup | ✅ |
| Communication Protocol | 🚧 |
| SPI Driver | 🚧 |
| Firmware Updater | 🚧 |
| Telemetry Manager | 🚧 |
| Dashboard | 🚧 |
| Storage | 🚧 |
| OTA | 📋 Planned |

---

## Build

Clone the repository

```bash
git clone https://github.com/Umair-Wanware/BMS_Control_Centre.git
```

Open the project

```bash
cd BMS_Control_Centre
```

Build

```bash
pio run
```

Upload

```bash
pio run --target upload
```

Monitor

```bash
pio device monitor
```

---

## Future Improvements

- Secure Firmware Updates
- TLS Support
- MQTT Integration
- Cloud Dashboard
- Data Logging
- Remote Diagnostics
- Battery History Graphs
- User Authentication
- CAN Bus Support

---

## Related Projects

- STM32 Battery Management Firmware
- STM32 Bootloader
- BMS Control Centre
- Web Dashboard

---

## License

This project is released under the MIT License.

---

## Author

**Umair Wanware**

Embedded Software Developer

- Modern C++
- Embedded Software
- Firmware Development
- STM32
- ESP32
- FreeRTOS
- Bootloaders
- Embedded Linux

---
