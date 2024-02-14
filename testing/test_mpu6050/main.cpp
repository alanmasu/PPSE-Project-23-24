#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include "PSEBoard.h"

Adafruit_MPU6050 mpu;

void setup() {
  Wire.setSDA(I2C0_SDA);
  Wire.setSCL(I2C0_SCL);
  Wire.begin();
  Serial.begin(115200);  
  while(!Serial);
  delay(2000);
  Serial.println("MPU6050 Test program started...");
  if (!mpu.begin(0x68)) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
}

void loop() {
  // put your main code here, to run repeatedly:
}