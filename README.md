# ESP32-S3 Firmware Collection: Peripheral Drivers & IoT Labs

This repository documents my comprehensive study and implementation of embedded systems using the **Espressif ESP32-S3** platform. 

It serves as a technical portfolio demonstrating **Bare-metal concepts**, **Communication Protocols**, **Sensor Fusion**, and **Edge AI (TinyML)** integration.

## 📂 Project Architecture

The repository is organized by technical modules. Each module contains raw firmware code (`src/`) and detailed engineering notes (`docs/`) analyzing the underlying hardware behavior.

| Module | Topic | Tech Stack | Documentation |
| :--- | :--- | :--- | :--- |
| **Lab 1** | **Communication Protocols** | UART, I2C, SPI, Logic Analyzer | [📖 Deep Dive](docs/01_Communication_Protocols.md) |
| **Lab 2** | **Sensor Drivers** | MPU9255 (IMU), VL53L0X (ToF) | *(Coming Soon)* |
| **Lab 3** | **HMI & Display** | TFT LCD, Touch Panel | *(Coming Soon)* |
| **Lab 4** | **IoT Connectivity** | Wi-Fi, BLE, LINE Notify | *(Coming Soon)* |
| **Lab 5** | **Actuator Control** | PWM, Servo, DC Motor | *(Coming Soon)* |
| **Lab 6** | **Edge AI** | TinyML, TensorFlow Lite | *(Coming Soon)* |

## 🛠️ Development Environment
* **MCU**: ESP32-S3 (Xtensa® 32-bit LX7 Dual-core)
* **Framework**: Arduino Core for ESP32
* **Tools**: Logic Analyzer, Oscilloscope

## 📝 Engineering Approach
Unlike standard library usage, this project focuses on understanding the **physical layer**:
* Verified signal integrity using logic analyzers.
* Analyzed **Timing Diagrams** (Start/Stop bits, ACK/NACK).
* Implemented **Open-Drain** and **Shift Register** logic verification.

---
*Author: Ping-Jui, Lee | 2026*
