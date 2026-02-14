#include <ESP32SPISlave.h>                                   //for SPI slave
						                                     
ESP32SPISlave slave;                                         //創建一個 ESP32SPISlave 物件

static constexpr uint32_t BUFFER_SIZE {32};
uint8_t spi_slave_tx_buf[BUFFER_SIZE];                       //儲存傳輸資料的緩衝區
uint8_t spi_slave_rx_buf[BUFFER_SIZE];                       //儲存接收資料的緩衝區

void setup() {
    Serial.begin(115200);                                    //以 baud rate = 115200 開始串列通訊
    delay(2000);                                             
    pinMode(LED_BUILTIN, OUTPUT);                            //將 LED 設定為輸出模式
    slave.setDataMode(SPI_MODE0);                            //設定 SPI 資料模式為 MODE 0
    slave.begin();                                           //啟動 SPI slave

    //初始化緩衝區
    memset(spi_slave_tx_buf, 0, BUFFER_SIZE);
    memset(spi_slave_rx_buf, 0, BUFFER_SIZE);
}

void loop() {
    
    slave.queue(NULL, spi_slave_rx_buf, BUFFER_SIZE);        //設定 slave 的接收緩衝區     
    slave.queue(spi_slave_tx_buf, NULL, BUFFER_SIZE);        //設定 slave 的發送緩衝區（此實驗主要練習由 master 發送、slave 接收並控制 LED，並無實際由 slave 傳送資料予 master）

    //等待從 master 接收資料
    const std::vector<size_t> received_bytes = slave.wait(); 
    
    char data;
    
    Serial.print("Command Received: ");
    Serial.println(spi_slave_rx_buf[0]);
    data = spi_slave_rx_buf[0];
    
	  //根據接收到的第一個 byte 控制 LED
    if(data == 1){                                           //如果是 1，點亮 LED
        Serial.println("Setting LED active HIGH ");          
        digitalWrite(LED_BUILTIN, HIGH);                     
    }                                                        
    else if(data == 0){                                      //如果是 0，熄滅 LED
        Serial.println("Setting LED active LOW ");
        digitalWrite(LED_BUILTIN, LOW);
    }
    Serial.println("");
}
