# Module 04: IoT Connectivity (Wi-Fi & BLE)

本模組實作了 AIoT 裝置最核心的無線連接功能。基於 ESP32-S3 的雙模 (Dual-mode) 無線通訊能力，我們實作了 **Wi-Fi 802.11 b/g/n** 用於雲端資料傳輸 (LINE Notify)，以及 **Bluetooth Low Energy (BLE 5.0)** 用於近端低功耗通訊。

## 1. Wi-Fi Architecture (802.11 b/g/n)

### 1.1 Operating Modes
ESP32 支援三種運作模式，本實驗主要探討 Station 模式：
* **Station (STA) Mode**: ESP32 作為客戶端 (Client) 連接到無線基地台 (AP)。這是 IoT 裝置上網最常用的模式。
* **SoftAP Mode**: ESP32 自己當作基地台 (Hotspot)，供其他裝置連接。
* **Station + SoftAP**: 混合模式。

### 1.2 Network Stack (LwIP)
ESP32 的 Wi-Fi 功能是建立在 **LwIP (Lightweight IP)** 協議堆疊之上。
* **Physical Layer**: 2.4GHz 射頻。
* **Application Layer**: 在 `WiFi_LineNotify` 實驗中，我們實作了 **HTTP Client**。為了確保資料安全，使用了 **TLS/SSL (Secure Sockets Layer)** 進行加密連線 (HTTPS)，這對於傳輸 Token 等敏感資料至關重要。

---

## 2. Bluetooth Low Energy (BLE)

### 2.1 BLE vs Classic Bluetooth
不同於傳統藍牙 (Classic) 專注於音訊串流，BLE 專為低功耗、小數據封包傳輸設計。
* **Connectionless**: 透過廣播 (Advertising) 快速交換資料。
* **State Machine**: 裝置在 Advertising (廣播) 與 Connected (連線) 狀態間切換。

### 2.2 GATT Architecture (Generic Attribute Profile)
本實驗實作了一個 **BLE Server**，其資料結構遵循 GATT 規範層級：
1.  **Profile**: 裝置功能的集合 (如 Heart Rate Profile)。
2.  **Service**: 功能的分類 (由 16-bit 或 128-bit UUID 定義)。
3.  **Characteristic**: 實際的資料點 (如「溫度值」、「開關狀態」)。每個特徵值都有屬性 (Read, Write, Notify)。

* **Implementation Detail**: 我們定義了一個客製化的 Service UUID，並透過 `notify()` 機制主動推播數據給手機端 (Client)，而非讓 Client 不斷輪詢 (Polling)，大幅降低功耗。

---

## 3. IoT Application: LINE Notify System

### 3.1 RESTful API Implementation
為了將感測器數據推送到使用者手機，我們整合了 LINE Notify API。
* **Protocol**: HTTPS (Port 443).
* **Method**: `POST` request.
* **Authentication**: 使用 **Bearer Token** 進行身分驗證。
* **Header Construction**:
    ```text
    POST /api/notify HTTP/1.1
    Host: notify-api.line.me
    Authorization: Bearer [YOUR_TOKEN]
    Content-Type: application/x-www-form-urlencoded
    ```

### 3.2 Engineering Challenges
* **Blocking vs Non-blocking**: 在連網過程中，若網路不穩，標準的 `client.connect()` 可能會阻塞 CPU。在進階實作中，應考慮使用 FreeRTOS Task 或 Async TCP 庫來避免 Watchdog Timer (WDT) Reset。
