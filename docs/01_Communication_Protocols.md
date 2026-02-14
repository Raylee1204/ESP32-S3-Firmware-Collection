# Embedded Communication Protocols: UART, I2C, and SPI

## 1. Overview
本模組實作了嵌入式系統中最常見的三種串列通訊協定：UART、I2C 與 SPI。透過兩塊 ESP32-S3 開發板 (Master/Slave 架構)，驗證了不同協定的時序特性與應用場景。

| Protocol | Type | Architecture | Duplex | Speed | Wiring | Application |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **UART** | Asynchronous | Peer-to-Peer | Full | Low-Med | 2 (TX/RX) | Debug, GSM/GPS Modems |
| **I2C** | Synchronous | Multi-Master | Half | Med | 2 (SDA/SCL) | Sensors, EEPROM |
| **SPI** | Synchronous | Master-Slave | Full | High | 4 (MOSI/MISO...) | Displays, Flash, ADCs |

---

## 2. UART (Universal Asynchronous Receiver Transmitter)

### 2.1 Theoretical Basis
[cite_start]UART 是一種非同步傳輸協定，雙方不共用時脈訊號，因此必須約定好 **鮑率 (Baud Rate)** 以確保取樣正確 [cite: 21, 240]。

* **Frame Structure**:
    * [cite_start]**Idle**: High Voltage (Logic 1)[cite: 239].
    * [cite_start]**Start Bit**: Pull Low (Logic 0) to synchronize[cite: 239].
    * [cite_start]**Data Bits**: 8 bits, transmitted **LSB First**[cite: 55, 237].
    * **Parity**: None (in our lab).
    * [cite_start]**Stop Bit**: Pull High (Logic 1) to end frame.

![UART Timing](assets/uart_timing.png)
[cite_start]*(Ref: UART data transmission format )*

### 2.2 ESP32 Implementation Details
在 `Serial_Bridge` 模組中，我們配置了兩組 UART：
* **UART0 (USB Serial)**: 用於 Debug Log (Baud: 115200).
* **UART1/2 (Inter-chip)**: 用於雙機通訊。

**關鍵設定 (Configuration):**
* **Baud Rate**: 115200 bps. 
    * [cite_start]*Engineering Note:* 根據講義公式 $1/BaudRate$ [cite: 240]，每個 bit 的傳輸時間約為 8.68 µs。若雙方時脈誤差超過 5%，可能導致 Frame Error。
* **Pin Definition**: 
    * [cite_start]TX (GPIO 17) -> RX (GPIO 16).
    * RX (GPIO 16) -> TX (GPIO 17).
    * **GND 共地**：這是新手常犯錯誤，UART 為電壓準位訊號，必須共地才能判讀 Logic 0/1。

---

## 3. I2C (Inter-Integrated Circuit)

### 3.1 Theoretical Basis
[cite_start]I2C 使用 **Open-Drain (開汲極)** 架構，這意味著 Master/Slave 只能將線路「拉低 (Drive Low)」，高電位需靠 **Pull-up Resistor (上拉電阻)** 維持 。

* [cite_start]**Addressing**: 7-bit address + 1-bit R/W flag[cite: 214].
* [cite_start]**Handshaking**: 每一 Byte 傳輸後，接收方必須回傳 **ACK (Low)**，否則發送方視為 **NACK (High)** 並停止傳輸[cite: 207, 218].

![I2C Hardware](assets/i2c_hardware.png)
[cite_start]*(Ref: I2C Open-Drain Structure )*

### 3.2 ESP32 Implementation Details
本實驗實作了 **I2C Scanner** 與 **Master-Slave Command**。

* **Hardware Setup**:
    * [cite_start]SDA (GPIO 8) / SCL (GPIO 9)[cite: 157, 159].
    * **Pull-up**: ESP32 內部雖有弱上拉，但在高速 (400kHz) 下建議外掛 $2.2k\Omega - 4.7k\Omega$ 電阻以改善訊號上升時間 (Rise Time)。
* **Software Logic (Address Scanning)**:
    * Master 發送 `Start Condition` + `Address (0x00 ~ 0x7F)` + `Write bit`.
    * [cite_start]若收到 `ACK` [cite: 216]，代表該位址有設備存在。
    * [cite_start]本實驗掃描到 Slave Address 為 `0x12` [cite: 165]。

---

## 4. SPI (Serial Peripheral Interface)

### 4.1 Theoretical Basis
[cite_start]SPI 是基於 **Shift Register (移位暫存器)** 的環形交換系統 。Master 推出一個 bit 的同時，Slave 也推回一個 bit。

* [cite_start]**4 Modes (CPOL/CPHA)**[cite: 228]:
    * 決定 Clock 的極性 (Idle High/Low) 與取樣邊緣 (Rising/Falling)。
    * [cite_start]本實驗採用 **Mode 0 (CPOL=0, CPHA=0)**，即 Idle Low，上升緣取樣 [cite: 231]。

![SPI Shift Register](assets/spi_shift_register.png)
[cite_start]*(Ref: SPI Master-Slave Shift Register Data Exchange )*

### 4.2 ESP32 Implementation Details
[cite_start]實驗目標為 Master 控制 Slave 的 LED 閃爍 [cite: 175]。

* **Hardware Setup**:
    * [cite_start]MOSI (GPIO 11), MISO (GPIO 13), SCLK (GPIO 12), CS (GPIO 10) [cite: 177-184].
* **Performance Note**:
    * [cite_start]SPI 為 **Full-Duplex (全雙工)** [cite: 225]，傳輸速率可達數十 MHz。在 ESP32 上，我們使用硬體 SPI (HSPI/VSPI) 以達到最大吞吐量，而非軟體模擬 (Bit-banging)。
    * [cite_start]Slave 端使用 `ESP32SPISlave` 函式庫 [cite: 190] 利用中斷處理高速數據流，避免 CPU 阻塞。

---

## 5. Logic Analyzer Analysis (Optional)
(若有邏輯分析儀截圖可放在此處，驗證 Start Bit, ACK, Clock 頻率是否符合預期)
