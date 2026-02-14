#include <SPI.h>                                                  //for SPI
                                                 
static const int spiClk = 1000000;                                //設置 SPI 通訊的 clock rate 為 1 MHz
                                                 
void setup(){                                                    
  SPI.begin();                                                    //啟動 SPI 通訊
                                                 
  pinMode(SS, OUTPUT);                                            //設定 SS 腳位為輸出模式，用於 master 控制 slave
}

void loop(){                                                      
  send_command(); 
  delay(100);                                                     //短暫延遲，避免過度佔用 CPU                                               
}                                                                 
                                                        
void send_command(){                                              //每一秒傳送 0 或 1
                                                        
  byte data_on = 1;                                               //定義資料 0x01 為開啟 slave LED 的指令
  byte data_off = 0;                                              //定義資料 0x00 為開啟 slave LED 的指令
  
  SPI.beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0)); //開始 SPI 傳輸（設定 clock rate、資料順序和模式）
  digitalWrite(SS, LOW);                                          //拉低 SS 腳位，開始通訊   
  SPI.transfer(data_on);                                          //傳送資料 0x01，開啟 LED
  digitalWrite(SS, HIGH);                                         //拉高 SS 腳位，結束通訊
  SPI.endTransaction();                                           //結束 SPI 傳輸

  delay(1000);                                                    //1 s
  
  SPI.beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(SS, LOW);
  SPI.transfer(data_off);                                         //傳送資料 0x00，關閉 LED 
  digitalWrite(SS, HIGH);
  SPI.endTransaction();
  delay(1000);
}
