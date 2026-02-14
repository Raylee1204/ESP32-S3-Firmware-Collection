HardwareSerial uart1(1);                             //透過 HardwareSerial 物件控制 esp32 上的 UART 控制器，esp32 有 3 個 uart controller(0~2)，參數代表要控制哪個 controller
						                             
#define TXD_PIN 17                                   //定義第 17 腳位為傳輸腳位
#define RXD_PIN 16                                   //定義第 16 腳位為接收腳位

void setup() {
  Serial.begin(115200);                              //以 baud rate = 115200 開始串列通訊
  uart1.begin(115200, SERIAL_8N1, RXD_PIN, TXD_PIN); //在 uart1 上以指定的 baud rate、資料格式（8 bits data、無校驗位元、1 結束位元）以及 RX 和 TX 的腳位開始 UART 通訊
}

void loop() {
  if(uart1.available() > 0){                         //檢查 uart1 上是否有資料可讀
    String mag = uart1.readString();               //讀取傳入的 String
    Serial.printf("Received msg: %s", mag);
  }
  delay(10);                                         //10 ms 延遲以維持迴圈的穩定性
}  
