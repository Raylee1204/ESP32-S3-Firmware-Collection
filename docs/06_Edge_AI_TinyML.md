# Module 06: Edge AI & TinyML (TensorFlow Lite for Microcontrollers)

本模組展示如何在資源受限的 ESP32-S3 上部署深度學習模型。利用 **TensorFlow Lite for Microcontrollers (TFLM)** 框架，實作了從模型訓練、量化壓縮到晶片上推論 (On-device Inference) 的完整流程。

## 1. The TinyML Workflow

將 AI 模型部署到 MCU 包含四個關鍵步驟，這與傳統軟體開發截然不同：

1.  **Model Training (Python/Keras)**: 在 Colab 或 PC 上使用 TensorFlow 建立並訓練模型 (如 Dense Neural Network)。
2.  **Conversion (TFLite Converter)**: 將 `.h5` 模型轉換為 `.tflite` 格式 (FlatBuffers)。
3.  **Quantization (Optimization)**: 將 32-bit Floating Point 權重轉換為 **8-bit Integer (INT8)**，以減少 Flash 佔用並加速運算。
4.  **Deployment (C++ Array)**: 使用 `xxd` 工具將 `.tflite` 轉為 C 語言的 `unsigned char` 陣列，存入 MCU 的 Flash 記憶體。


---

## 2. Implementation: Sine Wave Prediction

### 2.1 Model Architecture
為了預測正弦波 ($y = \sin(x)$)，我們設計了一個輕量級的全連接神經網路 (Fully Connected Network)：
* **Input**: 1 neuron ($x$ value)
* **Hidden Layers**: 2 layers (16 neurons, ReLU activation)
* **Output**: 1 neuron ($y$ value)
* **Total Parameters**: < 1000 parameters (適合 KB 等級的 SRAM)

### 2.2 Quantization Strategy (關鍵技術)
嵌入式系統對記憶體極其敏感。我們使用了 **Post-training Quantization (訓練後量化)**：
* **Size Reduction**: 模型大小縮減約 **4倍** (Float32 -> Int8)。
* **Accuracy Trade-off**: 雖然精度有微小損失，但在訊號預測應用中可忽略不計。


---

## 3. Firmware Integration (TFLM C++ Library)

### 3.1 Tensor Arena (Memory Management)
TFLM 不使用動態記憶體配置 (malloc/free) 以避免碎片化。我們必須預先定義一塊連續記憶體 **Tensor Arena**：
```cpp
// 預留 2KB 記憶體供模型運算 (Input/Output/Intermediate Tensors)
constexpr int kTensorArenaSize = 2 * 1024;
uint8_t tensor_arena[kTensorArenaSize];
