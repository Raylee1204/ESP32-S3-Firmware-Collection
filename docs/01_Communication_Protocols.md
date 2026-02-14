# Module 01: Communication Protocols (UART, I2C, SPI)

本模組實作了嵌入式系統中最常見的三種串列通訊協定。透過 ESP32-S3 的實作 (Master/Slave 架構)，驗證了不同協定的硬體特性、時序邏輯與應用場景。

## 1. Protocol Comparison (協定比較)

| Feature | UART | I2C | SPI |
| :--- | :--- | :--- | :--- |
| **Full Name** | Universal Asynchronous Receiver Transmitter | Inter-Integrated Circuit | Serial Peripheral Interface |
| **Type** | **Asynchronous (非同步)**<br>無 Clock 線，依賴 Baud Rate | **Synchronous (同步)**<br>依賴 SCL 線同步 | **Synchronous (同步)**<br>依賴 SCLK 線同步 |
| **Architecture** | Peer-to-Peer (點對點) | Multi-Master (多主從)<br>Shared Bus | Master-Slave (主從)<br>Dedicated Select Lines |
| **Duplex** | **Full-Duplex (全雙工)**<br>TX/RX 獨立通道 | **Half-Duplex (半雙工)**<br>SDA 線路分時共用 | **Full-Duplex (全雙工)**<br>MOSI/MISO 獨立通道 |
| **Speed** | Low (< 1 Mbps) | Medium (100kHz - 3.4MHz) | High (> 10 MHz) |
| **Wires** | 2 (TX, RX) | 2 (SDA, SCL) | 4 (MOSI, MISO, SCLK, CS) |

---

## 2. UART (Universal Asynchronous Receiver Transmitter)

### 2.1 Technical Analysis
UART 是一種非同步傳輸協定。由於沒有共用的時脈訊號 (Clock)，通訊雙方必須嚴格約定好 **鮑率 (Baud Rate)** 以確保取樣時機正確。

* **Idle State (閒置狀態)**: 線路預設保持 **High (Logic 1)** 電位，以區別「閒置」與「斷線」。
* **Start Bit (起始位元)**: 傳輸開始時，發送端將線路拉低 **Low (Logic 0)**，喚醒接收端開始計時。
* **Data Bits**: 傳輸 8 bits 資料，遵循 **LSB First (最低有效位先傳)** 的規則。
* **Parity Bit (校驗位)**: (本實驗未啟用) 可選擇奇/偶校驗來檢查位元錯誤。
* **Stop Bit (停止位)**: 傳輸結束後，將線路拉回 **High (Logic 1)**，讓接收端有時間處理資料並回到閒置狀態。

<img width="1386" height="207" alt="image" src="https://github.com/user-attachments/assets/6af8a356-0406-414d-b414-77f63c3e7141" />

*(Figure: UART 訊號波形圖)*

### 2.2 ESP32 Implementation
* **Baud Rate**: 設定為 **115200 bps**。
    * *Calculation*: 每個 Bit 的傳輸時間 $T_{bit} = 1 / 115200 \approx 8.68 \mu s$。
* **Pin Configuration**:
    * **TX**: GPIO 17 (Connect to Peer RX)
    * **RX**: GPIO 16 (Connect to Peer TX)
    * **Ground**: 雙方 GND 必須相連以建立共同參考電位。

---

## 3. I2C (Inter-Integrated Circuit)

### 3.1 Technical Analysis
I2C 採用 **Open-Drain (開汲極)** 輸出架構，這意味著裝置只能主動將線路拉低 (Drive Low)，無法主動推高。高電位必須依靠外部的 **Pull-up Resistor (上拉電阻)** 來維持。

* **Wired-AND Logic**: 若多個裝置同時輸出，只要有一方輸出 Low，整條線路即為 Low。此特性用於實現 **仲裁 (Arbitration)** 與 **Clock Stretching**。
* **Start Condition**: 當 `SCL` 為 High 時，`SDA` 由 High 轉 Low。
* **Stop Condition**: 當 `SCL` 為 High 時，`SDA` 由 Low 轉 High。
* **Addressing**: 採用 7-bit Address 模式，Master 發送的第一個 Byte 包含 `[7-bit Address] + [1-bit R/W]`。
* **ACK/NACK**: 每傳輸 8 bits，接收方需在第 9 個 Clock 週期將 `SDA` 拉低回應 ACK。

<img width="1209" height="378" alt="image" src="https://github.com/user-attachments/assets/d449c046-a9e7-46d8-ba76-140ebd82bf37" />

*(Figure: I2C 架構與上拉電阻示意圖)*

### 3.2 ESP32 Implementation
本實驗實作了 I2C Scanner 與 Master-Slave 通訊。

* **Hardware**:
    * **SDA**: GPIO 8
    * **SCL**: GPIO 9
    * **Pull-up**: 啟用內部 Pull-up 或外接 4.7kΩ 電阻。
* **Logic**:
    * Master 透過迴圈發送寫入請求至 `0x00` - `0x7F`。
    * 若收到 ACK，則判定該位址有 Slave 裝置 (本實驗 Slave Address 為 `0x12`)。

---

## 4. SPI (Serial Peripheral Interface)

### 4.1 Technical Analysis
SPI 是基於 **Shift Register (移位暫存器)** 的環形交換系統。Master 與 Slave 內部各有一個暫存器，隨著 Clock 的脈衝，資料在兩者之間循環移位。

* **Full-Duplex**: 在同一個 Clock 週期內，Master 透過 `MOSI` 送出一位元，同時讀取 `MISO` 傳回的一位元。
* **4 Modes (CPOL/CPHA)**: 決定 Clock 的極性與取樣邊緣。
    * 本實驗採用 **Mode 0 (CPOL=0, CPHA=0)**。
    * **CPOL=0**: Clock Idle 時為 Low。
    * **CPHA=0**: 在 Clock 的第一個邊緣 (Rising Edge) 讀取資料 (Sample)，在第二個邊緣 (Falling Edge) 改變資料 (Setup)。

<img width="591" height="443" alt="image" src="https://github.com/user-attachments/assets/3dd4acd7-92dc-453d-aaff-17316d16d781" />

*(Figure: SPI 主從端資料交換示意圖)*

### 4.2 ESP32 Implementation
實驗目標為透過 Master 控制 Slave 端的 LED 狀態。

* **Pin Configuration**:
    * **MOSI**: GPIO 11 (Master Out Slave In)
    * **MISO**: GPIO 13 (Master In Slave Out)
    * **SCLK**: GPIO 12 (Serial Clock)
    * **CS/SS**: GPIO 10 (Chip Select, Active Low)
* **Driver Note**:
    * Slave 端使用 `ESP32SPISlave` 函式庫，利用中斷 (Interrupt) 處理高速 SPI 傳輸，確保資料不遺失。
