# ESP32-S3 Firmware Collection: Peripheral Drivers & IoT Labs

本儲存庫記錄了我使用 **Espressif ESP32-S3** 平台進行嵌入式系統研究與實作的完整過程。

這是一份技術作品集，展示了 **Bare-metal 概念**、**通訊協定 (Communication Protocols)**、**感測器融合 (Sensor Fusion)** 以及 **邊緣 AI (Edge AI/TinyML)** 的整合應用能力。

## 📂 專案架構 (Project Architecture)

本專案依照技術模組進行分類。每個模組皆包含原始韌體程式碼 (`src/`) 以及詳細的工程筆記 (`docs/`)，深入分析底層硬體行為。

| Module | Topic | Tech Stack | Documentation |
| :--- | :--- | :--- | :--- |
| **Lab 1** | **Communication Protocols** | UART, I2C, SPI, Logic Analyzer | [📖 Deep Dive (深度解析)](docs/01_Communication_Protocols.md) |
| **Lab 2** | **Sensor Drivers** | MPU9255 (IMU), VL53L0X (ToF) | *(Coming Soon)* |
| **Lab 3** | **HMI & Display** | TFT LCD, Touch Panel | *(Coming Soon)* |
| **Lab 4** | **IoT Connectivity** | Wi-Fi, BLE, LINE Notify | *(Coming Soon)* |
| **Lab 5** | **Actuator Control** | PWM, Servo, DC Motor | *(Coming Soon)* |
| **Lab 6** | **Edge AI** | TinyML, TensorFlow Lite | *(Coming Soon)* |

## 🛠️ 開發環境 (Development Environment)
* **MCU**: ESP32-S3 (Xtensa® 32-bit LX7 Dual-core)
* **Framework**: Arduino Core for ESP32
* **Tools**: Logic Analyzer (邏輯分析儀), Oscilloscope (示波器)

## 📝 工程思維 (Engineering Approach)
不同於僅僅呼叫標準函式庫，本專案著重於理解**物理層 (Physical Layer)** 的運作原理：
* 驗證訊號完整性 (Signal Integrity)。
* 分析 **時序圖 (Timing Diagrams)** (Start/Stop bits, ACK/NACK)。
* 實作並驗證 **Open-Drain (開汲極)** 與 **Shift Register (移位暫存器)** 邏輯。

---
*Author: Ping-Jui, Lee | 2026*
