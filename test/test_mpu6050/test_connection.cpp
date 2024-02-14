#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include "PSEBoard.h"
#include "unity.h"

Adafruit_MPU6050 mpu;

void setUp(void) {
  Wire.setSDA(I2C0_SDA);
  Wire.setSCL(I2C0_SCL);
  Wire.begin();
}

void tearDown(void) {
  // clean stuff up here
}

void test_i2c_mpu6050_connection(void) {
  TEST_ASSERT_TRUE_MESSAGE(mpu.begin(0x68), "Failed to find MPU6050 chip");
}

void setup() {
  delay(2000);
  UNITY_BEGIN();
  RUN_TEST(test_i2c_mpu6050_connection);
  UNITY_END();
}

void loop() {
  // put your main code here, to run repeatedly:
}