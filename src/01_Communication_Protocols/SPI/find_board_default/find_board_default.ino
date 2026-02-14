//Find the default SPI pins for your board

void setup() {          //（ps. 只會執行一次）
  Serial.begin(115200); //以 baud rate = 115200 開始串列通訊

  Serial.print("MOSI: ");
  Serial.println(MOSI);
  Serial.print("MISO: ");
  Serial.println(MISO);
  Serial.print("SCK:  ");
  Serial.println(SCK);
  Serial.print("SS:   ");
  Serial.println(SS); 
}

void loop() {
   
}
