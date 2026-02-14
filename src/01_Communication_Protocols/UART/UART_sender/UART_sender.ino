#include "freertos/FreeRTOS.h"                                                                         //for task management
#include "freertos/task.h"                                                                             //task functionalities
#include "esp_system.h"                                                                                //system APIs for ESP
#include "esp_log.h"                                                                                   //logging functionalities
#include "driver/uart.h"                                                                               //UART driver
#include "string.h"                                                                                    //string handling functionalities
#include "driver/gpio.h"                                                                               //GPIO driver
									                                                                   
static const int RX_BUF_SIZE = 1024;                                                                   
static const int TX_BUF_SIZE = 1024;                                                                   
									                                                                   
#define TXD_PIN 17                                                                                     //定義第 17 腳位為傳輸腳位
#define RXD_PIN 16                                                                                     //定義第 16 腳位為接收腳位

String str;
char Buf[1024] = "UART1 TEST\n";

void setup() {
  Serial.begin(115200);                                                                                //以 baud rate = 115200 開始串列通訊       
  init();                                                                                              //初始化 function
																						                  
  //將 rx_task 與 tx_task 加入 FreeRTOS 系統排程器中，即可燒錄至 esp32 中                               
  xTaskCreate(rx_task, "uart_rx_task", 1024*2, NULL, 5, NULL);                                            
  xTaskCreate(tx_task, "uart_tx_task", 1024*2, NULL, 4, NULL);                                            
}                                                                                                         
																						                  
void loop() {                                                                                             
  if(Serial.available()){                                                                                 
    str = Serial.readStringUntil('\0');                                                                //將在 Serial 輸入的資料組合成 String
    str.toCharArray(Buf, 50);                                                                          //再將 string 轉成 char array，最後經由 sendData 與 tx_task 送出
  }
}

//初始化 UART 設定
void init(void) {
    const uart_config_t uart_config = {
        .baud_rate = 115200,                                                                           //Baud rate 設為 115200，每 1/115200 秒傳送一個 bit
        .data_bits = UART_DATA_8_BITS,                                                                 //資料位元個數
        .parity = UART_PARITY_DISABLE,                                                                 //奇偶校驗位元，檢查資料是否完整
        .stop_bits = UART_STOP_BITS_1,                                                                 //結束位元
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,                                                         
        .source_clk = UART_SCLK_APB,                                                                   
    };                                                                                                 
																						               
																						               
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);                                   //設置 UART 驅動，包含 TX、RX 發送/接收時會用到的 Buffer 大小
    uart_param_config(UART_NUM_1, &uart_config);                                                       //配置到 UART 控制器中
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);                //設置 UART 腳位
}                                                                                                      
																						               
int sendData(const char* logName, const char* data)                                                    
{                                                                                                      
    const int len = strlen(data);                                                                      //資料長度
    const int txBytes = uart_write_bytes(UART_NUM_1, data, len);                                       //uart_write_bytes function 設定 UART 控制器、要送出的資料與資料長度，再經由 tx_task function 送出資料
    ESP_LOGI(logName, "Wrote %d bytes", txBytes);                                        
    return txBytes;
}

//TX 送出資料
static void tx_task(void *arg)
{
  static const char *TX_TASK_TAG = "TX_TASK";
  esp_log_level_set(TX_TASK_TAG, ESP_LOG_INFO);
  
  while(1){
    sendData(TX_TASK_TAG, Buf);                                                                        //透過 sendData function 設定 UART controller、要送出的資料與資料長度
    Serial.printf("Send msg: %s", Buf);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}


//RX 接收資料
static void rx_task(void *arg)
{
    static const char *RX_TASK_TAG = "RX_TASK";
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE+1);
    while (1) {
        const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 1000 / portTICK_PERIOD_MS); //透過 uart_read_bytes function 接收 UART_NUM_1 上的資料，並返回長度 assign 給 rxBytes
        if (rxBytes > 0) {                                                                             //若 RX buffer 中的資料長度 > 0，將 rxBytes 的資料印出
            data[rxBytes] = 0;                                                                         //字尾為結束位元
            ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%s'", rxBytes, data);
            ESP_LOG_BUFFER_HEXDUMP(RX_TASK_TAG, data, rxBytes, ESP_LOG_INFO);
            Serial.printf("Read UART1 %d bytes: '%s'",rxBytes, data);
        }
    }
    free(data);
}
