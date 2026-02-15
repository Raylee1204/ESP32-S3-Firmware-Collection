# Module 02: Sensor Drivers & Fusion (IMU, ToF)

本模組探討微機電系統 (MEMS) 感測器的驅動與應用。透過 ESP32-S3 整合 **InvenSense MPU-9255** (九軸慣性測量單元) 與 **ST VL53L0X** (飛行時間測距感測器)，實現高精度的姿態估測與距離偵測。

## 1. MPU-9255 (9-Axis IMU)

### 1.1 Architecture & Bypass Mode
MPU-9255 是一個 SiP (System in Package)，內部封裝了兩個晶片：
1.  **MPU-6500**: 包含 3-Axis Gyroscope (陀螺儀) + 3-Axis Accelerometer (加速度計)。
2.  **AK8963**: 3-Axis Magnetometer (磁力計)。

* **Critical Implementation Detail (關鍵實作)**:
    AK8963 磁力計實際上是掛在 MPU-6500 內部的 Auxiliary I2C Bus 上。為了讓 ESP32 直接讀取磁力計數據，必須啟用 **Bypass Mode (Pass-Through Mode)**。
    * *Register Setting*: 設定 `INT_PIN_CFG` (0x37) 的 Bit 1 為 1，使主 I2C lines 直接導通到 Aux I2C lines。

<img width="987" height="687" alt="image" src="https://github.com/user-attachments/assets/6fb2baab-4717-45ae-8eb7-6cae3483da1e" />

*(Figure: MPU-9255 內部架構與 DMP/FIFO 模組)*

### 1.2 Digital Motion Processor (DMP)
不同於傳統作法（在 MCU 上跑卡爾曼濾波），本實作利用 MPU-9255 內建的 **DMP (Digital Motion Processor)** 進行硬體加速。
* **Offloading**: DMP 負責融合 Accel + Gyro 數據，直接輸出 **四元數 (Quaternions)**。
* **Benefit**: 大幅降低 ESP32 的運算負載 (CPU Load)，並減少 I2C 頻寬佔用。
* **FIFO Buffer**: DMP 計算結果會存入 1024-byte FIFO，ESP32 僅需定期讀取 FIFO，避免頻繁中斷。

### 1.3 Signal Processing & Calibration
* **Accelerometer**: 測量重力加速度 ($g$)。
    * *Convert*: $Accel\_g = Raw / 16384$ (若 Scale = $\pm 2g$)。
* **Gyroscope**: 測量角速度 ($dps$)。
    * *Convert*: $Gyro\_dps = Raw / 131$ (若 Scale = $\pm 250^\circ/s$)。
* **Magnetometer Calibration**:
    * 實作 **Figure-8 Calibration (八字繞圈校正)**，找出三軸的最大/最小值，計算 Hard Iron Offset 並從讀值中扣除。

---

## 2. VL53L0X (Time-of-Flight Ranging)

### 2.1 Working Principle (ToF)
VL53L0X 採用 **SPAD (Single Photon Avalanche Diode)** 陣列，發射 940nm 不可見紅外雷射，測量光子反射回來的飛行時間 ($\Delta T$)。
* **Formula**: $Distance = \frac{c \times \Delta T}{2}$
* **Advantage**: 不同於傳統紅外線強度反射 (IR Reflective)，ToF 不受物體顏色與表面材質影響，精確度更高。

<img width="721" height="374" alt="image" src="https://github.com/user-attachments/assets/3ba702e5-bb1d-46f6-ba91-9fe4e27c499d" />

*(Figure: Time-of-Flight 測距原理示意)*

### 2.2 Accuracy Optimization
在實作中，我們針對應用場景調整了 **Timing Budget (時間預算)**：
* **High Accuracy Mode**: 將 Timing Budget 增加至 **200ms**。雖然降低了採樣率 (5Hz)，但透過多次取樣平均，顯著降低了測量雜訊 (Noise)。
* **Long Range Mode**: 調整 VCSEL 脈衝週期 (Pulse Period)，將測量範圍延伸至 2m，但對環境光 (Ambient Light) 較敏感。

### 2.3 Multi-Sensor Implementation (XSHUT)
I2C 裝置通常有固定地址 (VL53L0X 預設為 0x29)。為了同時使用多個感測器，我們利用 **XSHUT Pin** 進行動態地址分配：
1.  MCU 將所有 XSHUT 拉低 (Reset/Standby)。
2.  拉高 Sensor A 的 XSHUT，透過 I2C 寫入新地址 (如 0x30)。
3.  拉高 Sensor B 的 XSHUT，保持預設或寫入另一地址 (如 0x31)。

---

## 3. Visualization (Processing)
為了驗證姿態解算結果，我們使用 **Processing** 接收 ESP32 透過 UART 傳送的 Euler Angles (Roll, Pitch, Yaw)，即時渲染 3D 飛機模型。

<img width="626" height="395" alt="image" src="https://github.com/user-attachments/assets/af72eda7-e548-449c-a46c-f4f4b487d31c" />

*(Figure: 透過 DMP 輸出的四元數進行 3D 姿態同步顯示)*
