# ESP32-S3 Firmware Architecture: Drivers, IoT & Edge AI

## 📖 Project Overview (專案簡介)

本專案為 **ESP32-S3 嵌入式系統設計** 課程的完整實作紀錄與技術延伸。
This project serves as a comprehensive implementation archive for the **ESP32-S3 Embedded System Design** curriculum.

**專案核心目標 (Core Objectives):**
不僅止於完成課堂作業，本專案致力於 **「知其然，亦知其所以然」**。我統整了課程中的所有實作範例，並針對課堂上因時間限制而無法深入探討的 **底層原理 (Low-level Principles)** 與 **通訊協定細節 (Protocol Specifications)** 進行了額外的研究與補強。

Beyond standard coursework, this repository aims to bridge the gap between "making it work" and "understanding how it works." I have consolidated all practical labs and supplemented them with in-depth engineering notes, covering theoretical concepts and hardware behaviors that were beyond the scope of the original lectures.

## 🚀 Key Technical Highlights (技術亮點)
* **Comprehensive Implementation**: 涵蓋 GPIO、PWM、UART/I2C/SPI、Wi-Fi/BLE、TinyML 等全方位實作。
* **Theoretical Supplement**: 針對每個模組撰寫深度筆記 (`docs/`)，分析時序圖與暫存器操作。
* **Verification**: 使用邏輯分析儀與示波器圖表驗證軟體邏輯的正確性。

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
