# Module 07: Audio Signal Processing (I2S, MEMS Mic)

本模組探討嵌入式系統中的數位音訊處理。利用 ESP32-S3 的 **I2S (Inter-IC Sound)** 硬體介面，驅動 **INMP441** 全向性 MEMS 麥克風，實現高傳真的音訊採樣與即時波形顯示。

## 1. Hardware: INMP441 MEMS Microphone

### 1.1 Device Overview
INMP441 是一款高性能、低功耗的數位輸出全向性麥克風。
* **MEMS Technology**: 微機電系統結構，相比傳統駐極體麥克風 (Electret)，具有更佳的抗震性與溫度穩定性。
* **Digital Output**: 內部整合了 24-bit $\Sigma-\Delta$ ADC，直接輸出數位訊號，免除了類比訊號在傳輸過程中的雜訊干擾 (Noise Immunity)。

### 1.2 I2S Interface (Inter-IC Sound)
I2S 是專為數位音訊設計的串列匯流排標準，包含三條訊號線：
1.  **SCK (Serial Clock)**: 位元時脈，決定資料傳輸速率。
2.  **WS (Word Select)**: 又稱 LRCLK (Left/Right Clock)，用於切換左/右聲道。
    * WS = 0: 左聲道
    * WS = 1: 右聲道
3.  **SD (Serial Data)**: 實際的音訊二進位數據 (24-bit, 2's Complement)。

---

## 2. Firmware Implementation (ESP32 I2S Driver)

### 2.1 Configuration
ESP32 的 I2S 驅動器支援 **DMA (Direct Memory Access)**，這對於音訊處理至關重要。DMA 可以在不佔用 CPU 的情況下，自動將麥克風數據搬運到 RAM 中的緩衝區 (Buffer)。

* **Mode**: Master Mode, Receiver (RX).
* **Sample Rate**: 設定為 44.1kHz (CD Quality) 或 16kHz (Speech Quality)。
* **Data Format**: 32-bit (雖然 INMP441 是 24-bit，但為了對齊記憶體通常使用 32-bit 容器)。

### 2.2 Pin Definition
* **SCK**: GPIO 14 (Serial Clock)
* **WS**: GPIO 15 (Word Select)
* **SD**: GPIO 32 (Serial Data)
* *Design Note*: INMP441 的 L/R 腳位接地 (GND) 設定為左聲道輸出。

<img width="705" height="354" alt="image" src="https://github.com/user-attachments/assets/909e00a8-90b6-4f9e-add2-d2c3c16bbc8c" />

---

## 3. Signal Processing & Visualization

### 3.1 Raw Data Handling (Lab 1)
從 I2S Buffer 讀出的數據是 **2's Complement (二補數)** 格式的有號整數。
* **Normalization**: 為了在 Serial Plotter 上觀察，我們將 32-bit 整數除以縮放因子，還原為振幅波形。
* **DC Offset Removal**: 數位麥克風通常存在直流偏移，透過簡單的高通濾波器 (High-pass Filter) 或平均值扣除法來歸零。

<img width="1164" height="477" alt="image" src="https://github.com/user-attachments/assets/51972d70-e955-476e-9c06-491d45629d3a" />


### 3.2 Volume Reactive Visualizer (Lab 2)
實作音量反應系統：
1.  **RMS Calculation**: 計算一段時間內訊號的均方根值 (Root Mean Square)，代表「響度 (Loudness)」。
    $$RMS = \sqrt{\frac{1}{N} \sum_{i=0}^{N-1} x[i]^2}$$
2.  **Mapping**: 將 RMS 值映射到 LED 的亮度 (PWM Duty Cycle) 或數量，實現「音樂節奏燈」效果。
