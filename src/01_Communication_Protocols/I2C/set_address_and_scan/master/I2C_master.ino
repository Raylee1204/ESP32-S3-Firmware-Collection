#include <Wire.h>                                      //for I2C
										              
#define SERIAL_BAUD 9600                              
										              
void setup()                                          
{                                                     
  Wire.begin();                                        //初始化 I2C 做為 master，master 只有一個，無需設定地址
  Serial.begin(SERIAL_BAUD);                           //以 baud rate = 9600 開始串列通訊
  Serial.println("I2C Scanner started");
  Serial.println();
}

void loop()
{
  uint8_t error, i2cAddress, devCount, unCount;
  Serial.println("Scanning...");
 
  devCount = 0;
  unCount = 0;
  for(i2cAddress = 1; i2cAddress < 127; i2cAddress++ ) //遍歷所有可能的 I2C 地址
  {
    Wire.beginTransmission(i2cAddress);                //開始傳送，指定給地址為 i2cAddress 的 slave
    error = Wire.endTransmission();                    //結束傳輸並獲取錯誤碼
										               
    if (error == 0)                                    //如果無錯誤，表示找到一個裝置
    {
      Serial.print("I2C device found at 0x");
      if (i2cAddress < 16) Serial.print("0");          //如果地址介於 0 ~ F，為了能有整齊的輸出，在前面補 0
      Serial.println(i2cAddress,HEX);                  //以十六進位格式顯示地址
      devCount++;                                      
    }                                                  
    else if (error == 4)                               //如果錯誤碼為 4，表示位址可能有設備，但發生錯誤
    { 
      Serial.print("Unknow error at 0x");
      if (i2cAddress < 16) Serial.print("0");
      Serial.println(i2cAddress,HEX);
      unCount++;
    }    
  }

  if (devCount + unCount == 0)                         //如果沒有找到任何裝置
    Serial.println("No I2C devices found\n");
  else {
    Serial.println();
    Serial.print(devCount);
    Serial.print(" device(s) found");
    if (unCount > 0) {                         
      Serial.print(", and unknown error in ");
      Serial.print(unCount);
      Serial.print(" address");
    }
    Serial.println();
  }
  delay(5000);                                         //每 5 秒進行一次掃描
}
