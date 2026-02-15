int delaytime = 10;

void setup() {
  Serial.begin(115200);
  pinMode(9, OUTPUT); 

}

void loop() {
  for(int a=0;a<=255;a++) // 讓LED燈慢慢亮起來
  {
    analogWrite(9,a); 
    Serial.print("Analog number a:");
    Serial.println(a);
    delay(delaytime);
  }
  
  delay(1000);
  
  for(int a=255;a>=0;a--)// 讓LED燈慢慢熄滅
  {
    analogWrite(9,a); 
    Serial.print("Analog number a:");
    Serial.println(a);
    delay(delaytime);
  }
}
