#include <Wire.h>                      //for I2C

int val; 

void setup(){
  Serial.begin(9600);                  //以 baud rate = 9600 開始串列通訊
  Wire.begin();                        //初始化 I2C 做為 master，master 只有一個，無需設定地址
}

void loop(){
  if(Serial.available()){              //判斷是否有資料可讀
    val = Serial.read();               //讀取並賦值予 val 變數
    
    if(val == '1'){                    //如果資料內容為 1
      Wire.beginTransmission(3);       //開始傳送並指定給 slave 3 號
      Wire.write('1');                 //寫入訊息 "1"
      Wire.endTransmission();          //結束傳輸
      Serial.println("指令1已經發送");
    }
    
    else if(val == '0'){                    //如果資料內容為 0
      Wire.beginTransmission(3);       //開始傳送並指定給 slave 3 號
      Wire.write('0');                 //寫入訊息 "0"
      Wire.endTransmission();          //結束傳輸
      Serial.println("指令0已經發送");   
      
    }
    else if(val != '\n' && val != '\r') {
      Serial.println("Error message:your val is not 1 or 0!"); 
    }
  }
}
