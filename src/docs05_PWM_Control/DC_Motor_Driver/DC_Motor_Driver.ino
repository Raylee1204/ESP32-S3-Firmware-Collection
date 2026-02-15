
#define PIN_IN1  7 
#define PIN_IN2  8 
#define PIN_ENA  14 

#define PIN_IN3  9 
#define PIN_IN4  10 
#define PIN_ENB  15

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  // initialize digital pins as outputs.
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(PIN_IN1, HIGH); // A
  digitalWrite(PIN_IN2, LOW);  // A
  digitalWrite(PIN_IN3, HIGH); // B
  digitalWrite(PIN_IN4, LOW);  // B

  for (int speed = 0; speed <= 255; speed++) {
    analogWrite(PIN_ENA, speed); // speed up
    analogWrite(PIN_ENB, speed); 
    Serial.print("Speed:");
    Serial.print(speed);
    delay(10);
  }

  delay(5000); // rotate at maximum speed 5 seconds in clockwise direction

  // change direction
  digitalWrite(PIN_IN1, LOW);   // A
  digitalWrite(PIN_IN2, HIGH);  // A
  digitalWrite(PIN_IN3, LOW);   // B
  digitalWrite(PIN_IN4, HIGH);  // B

  delay(5000); // rotate at maximum speed for 5 seconds in anti-clockwise direction

  for (int speed = 255; speed >= 0; speed--) {
    analogWrite(PIN_ENA, speed); // speed down
    analogWrite(PIN_ENB, speed); 
    Serial.print("Speed:");
    Serial.print(speed);
    delay(10);
  }

  delay(2000); // stop motor 2 second
}
