#include <Wire.h>            //for I2C

#define SLAVE_ADDRESS 0x12   //定義 slave 地址為 0x12

void setup() {
  
  Wire.begin(SLAVE_ADDRESS); //初始化 I2C 做為 slave，並設定地址
}

void loop() {
}
