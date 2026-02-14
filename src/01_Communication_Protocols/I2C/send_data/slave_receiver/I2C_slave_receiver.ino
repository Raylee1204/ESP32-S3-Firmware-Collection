#include <Wire.h>                //for I2C

void setup()
{
  Wire.begin(3);                 //初始化 I2C 做為 master，設定地址為 3
  Wire.onReceive(receiveEvent);  //當收到 I2C 訊號時啟動
  Serial.begin(9600);            //以 baud rate = 9600 開始串列通訊  
}

void loop()
{
  delay(100);                    //短暫延遲，避免過度佔用 CPU
}

void receiveEvent(int numBytes)
{  
  while(Wire.available())        //判斷是否有資料可讀
  {         
    char c = Wire.read();        //將傳入資料給 c 變數
    Serial.println(c);           //並透過 Serial 印出內容
  }
}
