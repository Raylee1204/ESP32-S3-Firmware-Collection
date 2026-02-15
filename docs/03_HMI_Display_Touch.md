# Module 03: HMI & Display Drivers (ILI9341, XPT2046)

本模組探討嵌入式圖形介面 (GUI) 的底層實作。使用 **ILI9341** 驅動 IC 的 TFT LCD 螢幕與 **XPT2046** 電阻式觸控控制器，透過 ESP32-S3 的高速 SPI 介面實現即時互動系統。

## 1. Hardware Architecture

### 1.1 Dual-Controller Configuration
本實驗的硬體模組在同一塊 PCB 上整合了兩顆獨立的控制器，並共用同一組 SPI 匯流排：
* **Display Controller**: ILI9341 (320x240 解析度).
* **Touch Controller**: XPT2046 (電阻式觸控 ADC).

**SPI Bus Sharing (匯流排共用):**
由於兩者掛在同一個 SPI Bus，我們利用 **Chip Select (CS)** 腳位來切換控制權：
* 繪圖時：拉低 LCD_CS，拉高 Touch_CS。
* 讀取觸控時：拉低 Touch_CS，拉低 LCD_CS。
* *Engineering Note:* 在高速刷新 (如 40MHz) 情況下，需注意 SPI Transaction 的切換延遲，避免訊號衝突。

<img width="1096" height="508" alt="image" src="https://github.com/user-attachments/assets/e293ff37-e0e9-4d00-84e2-1f0df35437be" />
*(Figure: ILI9341 與 XPT2046 的 SPI 共用架構示意圖)*

### 1.2 Pin Definition (ESP32-S3 VSPI)
* **MOSI (GPIO 23)**: Master Out Slave In (共用)
* **MISO (GPIO 19)**: Master In Slave Out (共用)
* **SCK (GPIO 18)**: Serial Clock (共用)
* **LCD_CS (GPIO 5)**: Display Chip Select
* **LCD_DC (GPIO 2)**: Data/Command Selection (區分指令與像素資料)
* **LCD_RST (GPIO 4)**: Hardware Reset
* **Touch_CS (GPIO 21)**: Touch Chip Select

---

## 2. Graphic Rendering (ILI9341)

### 2.1 Color Depth: RGB565
為了降低記憶體佔用與 SPI 頻寬需求，嵌入式系統通常採用 **16-bit RGB565** 格式，而非電腦常見的 24-bit RGB888。
* **Red**: 5 bits
* **Green**: 6 bits (人眼對綠色最敏感，故分配較多位元)
* **Blue**: 5 bits
* **Bandwidth Calculation**: 
    更新一個 320x240 的畫面需要傳輸 $320 \times 240 \times 16 \text{ bits} \approx 1.2 \text{ Mbits}$。在 40MHz SPI Clock 下，理論最大 FPS 約為 30幀，這對 UI 互動已足夠流暢。

<img width="1330" height="480" alt="image" src="https://github.com/user-attachments/assets/f0d88b60-e11e-457c-9db0-70a616441239" />

### 2.2 Driver Library Optimization (TFT_eSPI)
本專案使用 `TFT_eSPI` 函式庫，相較於 Adafruit_GFX，它針對 ESP32 進行了底層優化：
1.  **DMA (Direct Memory Access)**: 允許在不佔用 CPU 的情況下搬運像素資料，這對於繪製大面積圖片或填色至關重要。
2.  **SPI Frequency**: 將 Display SPI 頻率超頻至 40MHz~80MHz 以提升刷新率。

---

## 3. Touch Input (XPT2046)

### 3.1 Resistive Touch Principle
XPT2046 是一個 12-bit 的 SAR ADC (逐次逼近類比數位轉換器)。
* **機制**: 電阻式觸控面板由兩層導電層組成。當手指按下時，兩層接觸導通，形成分壓電路。
* **讀取流程**: 
    1.  驅動 X 軸電壓，讀取 Y 軸電壓 -> 得到 X 座標。
    2.  驅動 Y 軸電壓，讀取 X 軸電壓 -> 得到 Y 座標。

### 3.2 Calibration (座標校正)
由於物理面板的貼合誤差，ADC 讀到的原始值 (Raw Data, 0~4095) 與螢幕像素座標 (Pixel, 0~320) 並非完美線性對應。
* **Algorithm**: 使用五點校正法 (5-Point Calibration) 或簡單的線性映射 (Linear Mapping) 公式：
    $$X_{pixel} = \frac{X_{raw} - X_{min}}{X_{max} - X_{min}} \times Width$$
    *(需針對每一個面板單獨校正並儲存參數)*

<img width="524" height="514" alt="image" src="https://github.com/user-attachments/assets/26ad3150-ded5-4372-bc45-af4dd0a16a2f" />


---

## 4. Application: HMI System
整合顯示與觸控，我們實作了以下功能：
1.  **Primitive Drawing**: 點、線、圓、矩形的幾何繪圖。
2.  **UI Elements**: 實作簡單的 "Button" 類別，包含 `contains(x, y)` 碰撞檢測邏輯，判斷手指是否按在按鈕範圍內。
3.  **Handwriting Board**: 透過連續讀取觸控座標並繪製線段，實現手寫畫板功能。
