#include "MPU9250.h"

MPU9250 mpu;

int I2C_SDA=20;
int I2C_SCL=21;

void setup() {
    Serial.begin(115200);
    Wire.begin(I2C_SDA, I2C_SCL);
    delay(2000);

    if (!mpu.setup(0x68)) {  // change to your own address
        while (1) {
            Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
            delay(5000);
        }
    }
}

void loop() {
    if (mpu.update()) {
        static uint32_t prev_ms = millis();
        if (millis() > prev_ms + 100) {
            
            // // exercise one
            // print_Acc();
            // print_Gyro();
            // print_Mag();

            // exercise two
           print_roll_pitch_yaw();
            
            Serial.println();
            prev_ms = millis();
        }
    }
}

void print_Gyro() {
  Serial.print("Gyroscope-X:    ");
  Serial.print(mpu.getGyroX(), 2);
  Serial.print("\t Gyroscope-Y:    ");
  Serial.print(mpu.getGyroY(), 2);
  Serial.print("\t Gyroscope-Z:    ");
  Serial.print(mpu.getGyroZ(), 2);
  Serial.println();
}

void print_Acc() {
  Serial.print("Accelerator-X:  ");
  Serial.print(mpu.getAccX(), 2);
  Serial.print("\t Accelerator-Y:  ");
  Serial.print(mpu.getAccY(), 2);
  Serial.print("\t Accelerator-Z:  ");
  Serial.print(mpu.getAccZ(), 2);
  Serial.println();
}

void print_Mag() {
  Serial.print("Magnetometer-X: ");
  Serial.print(mpu.getMagX(), 2);
  Serial.print("\t Magnetometer-Y: ");
  Serial.print(mpu.getMagY(), 2);
  Serial.print("\t Magnetometer-Z: ");
  Serial.print(mpu.getMagZ(), 2);
  Serial.println();
}

void print_roll_pitch_yaw() {
    Serial.print("X : ");
    Serial.print(mpu.getRoll());
    Serial.print("\tY : ");
    Serial.print(mpu.getPitch());
    Serial.print("\tZ : ");
    Serial.print(mpu.getYaw());
}
