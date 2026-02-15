# ESP32-S3 Firmware Architecture: Drivers, IoT & Edge AI

![License](https://img.shields.io/badge/license-MIT-blue.svg) ![Platform](https://img.shields.io/badge/platform-ESP32--S3-green) ![Framework](https://img.shields.io/badge/framework-Arduino%20Core-orange)

## 📖 Project Overview
本專案為 **ESP32-S3** 嵌入式系統的完整韌體實作集。不同於僅呼叫高階函式庫 (High-level APIs)，本專案著重於 **硬體抽象層 (HAL)** 的理解與實作，涵蓋 **通訊協定時序分析**、**感測器驅動開發**、**即時訊號處理 (DSP)** 以及 **邊緣運算 (TinyML)**。

This repository demonstrates the implementation of peripheral drivers and system integration on the Espressif ESP32-S3 platform, focusing on physical layer analysis and performance optimization.

## 🚀 Key Technical Highlights
* **Low-Level Protocols**: 深入分析 UART, I2C, SPI, I2S 的時序圖 (Timing Diagrams) 與電氣特性 (Open-Drain, Push-Pull)。
* **Driver Optimization**: 實作 **Direct Memory Access (DMA)** 進行音訊與顯示數據傳輸，大幅降低 CPU 負載。
* **Sensor Fusion**: 利用 **DMP (Digital Motion Processor)** 硬體加速器進行九軸感測器姿態解算 (Quaternion)。
* **Edge AI**: 部署 TensorFlow Lite for Microcontrollers (TFLM) 模型，實現離線手勢辨識與訊號預測。

---

## 📂 Module Architecture

| Module | Technical Focus | Hardware / Driver | Implementation Note (Docs) |
| :--- | :--- | :--- | :--- |
| **01. Protocols** | **Serial Communication**<br>Start/Stop bits, ACK/NACK, Shift Register | UART, I2C, SPI | [📝 Deep Dive: Protocol Analysis](docs/01_Communication_Protocols.md) |
| **02. Sensors** | **IMU & ToF Ranging**<br>Bypass Mode, DMP, XSHUT Addressing | MPU9255, VL53L0X | [📝 Deep Dive: Sensor Fusion](docs/02_Sensor_Drivers.md) |
| **03. HMI** | **Display & Touch**<br>RGB565, SPI Bandwidth, Touch Calibration | ILI9341, XPT2046 | [📝 Deep Dive: Display Drivers](docs/03_HMI_Display_Touch.md) |
| **04. IoT** | **Connectivity**<br>LwIP Stack, TLS/SSL, GATT Profile | Wi-Fi Station, BLE 5.0 | [📝 Deep Dive: Network Stack](docs/04_IoT_Connectivity.md) |
| **05. Actuators** | **Motor Control**<br>PWM Duty Cycle, Dead-time, H-Bridge | LEDC, Servo, DC Motor | [📝 Deep Dive: PWM Control](docs/05_Actuator_Control.md) |
| **06. Edge AI** | **TinyML**<br>Quantization (INT8), Tensor Arena | TensorFlow Lite | [📝 Deep Dive: AI Workflow](docs/06_Edge_AI_TinyML.md) |
| **07. Audio** | **Digital Signal Processing**<br>I2S Interface, DMA, RMS Calculation | INMP441 (MEMS Mic) | [📝 Deep Dive: Audio Processing](docs/07_Audio_Processing.md) |

---

## 🛠️ Hardware & Tools
* **MCU**: Espressif ESP32-S3 (Xtensa® 32-bit LX7 Dual-core, 240MHz)
* **Sensors**: InvenSense MPU-9255 (9-Axis), ST VL53L0X (ToF), INMP441 (I2S Mic)
* **Actuators**: SG90 Servo, L9110/L298N Motor Driver
* **Debugging Tools**:
    * **Logic Analyzer**: Used for verifying I2C/SPI timing integrity.
    * **Serial Plotter**: Used for visualizing real-time sensor data and audio waveforms.

## 💻 Development Environment
* **IDE**: Arduino IDE / VS Code
* **Core**: Arduino Core for ESP32
* **Libraries**: TensorFlowLite_ESP32, TFT_eSPI (Optimized), ESP32-BLE

---
*Author: Ping-Jui, Lee*
*Last Updated: 2026-02*
