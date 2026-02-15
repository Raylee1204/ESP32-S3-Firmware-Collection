#include <driver/i2s.h>

#define I2S_NUM          I2S_NUM_0  // 使用 I2S0 接口
#define I2S_WS_PIN       11         
#define I2S_SCK_PIN      12         
#define I2S_SD_PIN       10         
#define CLAP_THRESHOLD   1000     
#define CLAP_DELAY       500        

unsigned long last_clap_time = 0;   // 記錄上一次拍手的時間
bool led_state = false;             // LED 狀態

void setup() {
  Serial.begin(115200);

  // 初始化 LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(100);                       // wait for a second

  // 配置 I2S
i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX), // 主模式 + 接收模式
    .sample_rate = 44100,                               // 采樣率 44.1kHz
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,       // 每樣本 16 位
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,        // 只使用左通道
    .communication_format = I2S_COMM_FORMAT_I2S,        // I2S 通訊格式
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,           // 中斷優先級
    .dma_buf_count = 4,                                 // DMA 緩衝區數量
    .dma_buf_len = 512                                  // 每個緩衝區的大小
};

  i2s_pin_config_t pin_config = {
      .bck_io_num = I2S_SCK_PIN,
      .ws_io_num = I2S_WS_PIN,
      .data_out_num = -1, // 不需要輸出數據
      .data_in_num = I2S_SD_PIN
  };

  // 安裝 I2S 驅動
  i2s_driver_install(I2S_NUM, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM, &pin_config);
}

void loop() {
  const int sample_count = 1024;
  int16_t samples[sample_count];
  size_t bytes_read;

  // 從 I2S 讀取數據
  i2s_read(I2S_NUM, samples, sample_count * sizeof(int16_t), &bytes_read, portMAX_DELAY);

  // 計算數據的峰值
  int max_value = 0;
  for (int i = 0; i < sample_count; i++) {
    if (abs(samples[i]) > max_value) {
      max_value = abs(samples[i]);
    }
  }
  Serial.printf("Current Peak Value: %d\n", max_value);
  // 如果聲音峰值超過閾值，判定為拍手聲
  if (max_value > CLAP_THRESHOLD) {
    unsigned long current_time = millis();
    if (current_time - last_clap_time > CLAP_DELAY) {
      last_clap_time = current_time;
      led_state = !led_state; // 切換 LED 狀態
      digitalWrite(LED_BUILTIN, led_state ? HIGH : LOW);
      Serial.printf("Clap detected! LED is now %s\n", led_state ? "ON" : "OFF");
    }
  }
}
