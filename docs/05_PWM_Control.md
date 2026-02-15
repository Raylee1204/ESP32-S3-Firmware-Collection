# Module 05: Actuator Control & PWM (LEDC, Servo, Motor)

本模組探討如何利用 ESP32 的 **LEDC (LED Control)** 硬體週邊產生高精度的 PWM (Pulse Width Modulation) 訊號，以驅動 LED 呼吸燈、伺服馬達 (Servo) 以及直流馬達 (DC Motor)。

## 1. PWM Theory & ESP32 LEDC Architecture

### 1.1 Pulse Width Modulation (PWM)
PWM 是一種透過數位訊號模擬類比輸出的技術。透過快速切換訊號的開與關，控制輸出的平均電壓。
* **Duty Cycle (佔空比)**: 高電位時間佔總週期的比例 ($D = \frac{T_{on}}{T_{period}}$)。
* **Frequency (頻率)**: 訊號重複的速度。不同應用需要不同頻率：
    * **LED Dimming**: > 1000 Hz (避免人眼閃爍)。
    * **Servo Motor**: 50 Hz (標準 20ms 週期)。
    * **DC Motor**: 10 kHz - 20 kHz (避免音頻噪音)。

<img width="981" height="474" alt="image" src="https://github.com/user-attachments/assets/9311e59f-0312-4c3f-b303-c6a9464e09da" />
*(Figure: 不同 Duty Cycle 下的平均電壓示意圖)*

### 1.2 ESP32 LEDC Peripheral
不同於傳統 Arduino (AVR) 使用 `analogWrite` 佔用 CPU Timer，ESP32 擁有專用的 **LEDC 硬體控制器**。
* **Hardware Architecture**: 包含 4 個高速計時器 (High Speed Timers) 與 4 個低速計時器 (Low Speed Timers)，可獨立配置頻率與解析度。
* **Resolution (解析度)**: 可配置為 1-bit 到 16-bit。解析度越高，控制越細膩，但最高頻率會受限。
    * Formula: $Max\_Freq = \frac{80MHz}{2^{Resolution}}$

---

## 2. Servo Motor Control (SG90)

### 2.1 Control Logic
伺服馬達採用 **PPM (Pulse Position Modulation)** 控制。標準週期為 **20ms (50Hz)**。
* **0°**: 脈寬 0.5ms ~ 1ms (依型號而定)。
* **90° (Middle)**: 脈寬 1.5ms。
* **180°**: 脈寬 2.0ms ~ 2.5ms。

### 2.2 Implementation Calculation
在 ESP32 上設定解析度為 **16-bit (0-65535)**，頻率 **50Hz**。
* **Period Count**: 65535 ticks per 20ms.
* **1ms Pulse (0°)**: $\frac{1ms}{20ms} \times 65536 \approx 3276$
* **2ms Pulse (180°)**: $\frac{2ms}{20ms} \times 65536 \approx 6553$
* *Engineering Note:* 透過 `ledcWrite` 精準寫入這些數值，可達成比標準 Servo Library 更平滑的轉動效果。

---

## 3. DC Motor Control (H-Bridge)

### 3.1 H-Bridge Circuit (L298N / L9110)
直流馬達無法直接由 MCU 驅動 (電流不足)，需透過 **H-Bridge (H橋)** 驅動電路。
* **Direction Control (方向)**: 控制 H 橋四個開關的導通狀態，改變電流流向 (正轉/反轉)。
* **Speed Control (速度)**: 將 PWM 訊號輸入至 **Enable Pin (EN)** 或直接調變輸入腳位。

<img width="848" height="410" alt="image" src="https://github.com/user-attachments/assets/0c02d9a3-12f7-4cd3-a992-5b975b559e37" />
*(Figure: H-Bridge 電路原理圖，展示如何切換馬達正反轉)*

### 3.2 Firmware Logic
實作了 `setMotor(speed, direction)` 函式：
1.  **Stop**: IN1 = Low, IN2 = Low.
2.  **Forward**: IN1 = High, IN2 = Low, PWM = Speed.
3.  **Reverse**: IN1 = Low, IN2 = High, PWM = Speed.
* **Dead-time Consideration**: 在快速切換正反轉時，軟體需加入微小延遲 (Dead-time)，避免 H 橋同側開關短暫導通造成短路 (Shoot-through)。
