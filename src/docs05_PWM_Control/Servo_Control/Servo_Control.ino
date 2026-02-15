int servoPin = 18;

void setup()
{
  Serial.begin(115200);
  pinMode(servoPin, OUTPUT);
}
double mapf(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void pwmWrite(int pin, double period, double duty_cycle) //servo、led pin number,pwm period(ms),duty cycle(ms)
{
  int period_us = period * 1000;
  int duty_cycle_us = duty_cycle * 1000;
  digitalWrite(pin, HIGH);
  delayMicroseconds(duty_cycle_us);
  digitalWrite(pin, LOW);
  delayMicroseconds(period_us - duty_cycle_us);
  Serial.println(String(duty_cycle_us));
}
void ServoWrite(int servoPin, double degree, double delay_ms) //degree -90 ~ 90
{
  double value = mapf(degree, -90.0, 90.0, 0.5, 2.5);
  for (int i = 0; i < delay_ms/30; i++)
  {
    pwmWrite(servoPin, 20, value); //sg90's period is 20 ms
  }
}
void loop()
{
  ServoWrite(servoPin, -73 , 2000);
  ServoWrite(servoPin, 0 , 2000);
  ServoWrite(servoPin, 90 , 2000);
  ServoWrite(servoPin, 0 , 1000);
}

