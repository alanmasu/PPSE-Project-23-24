// Test con MPU9250

#include <Arduino.h>
#include <Wire.h>
#include <MPU9250.h>
#include "PSEBoard.h"

MPU9250 mpu;

void setup() {
    Wire.setSDA(I2C0_SDA);
    Wire.setSCL(I2C0_SCL);
    Wire.setClock(400000);
    Wire.begin();
    Serial.begin(115200);  
    while(!Serial);
    delay(2000);
    Serial.println("MPU9250 Test program started...");
    mpu.verbose(true);
    MPU9250Setting setting;
    setting.accel_fs_sel = ACCEL_FS_SEL::A16G;
    setting.gyro_fs_sel = GYRO_FS_SEL::G2000DPS;
    setting.mag_output_bits = MAG_OUTPUT_BITS::M16BITS;
    setting.fifo_sample_rate = FIFO_SAMPLE_RATE::SMPL_200HZ;
    setting.gyro_fchoice = 0x03;
    setting.gyro_dlpf_cfg = GYRO_DLPF_CFG::DLPF_41HZ;
    setting.accel_fchoice = 0x01;
    setting.accel_dlpf_cfg = ACCEL_DLPF_CFG::DLPF_45HZ;
    Wire.beginTransmission(0x68);
    bool error = 1;
    error = Wire.endTransmission();
    if (!mpu.setup(0x69, setting)) {
        Serial.println("Failed to find MPU9250 chip");
        while (1) {
        delay(10);
        }
    }
    Serial.println("MPU9250 Found!");
    Serial.println();
    Serial.println("MPU9250 Information:");
}

void loop() {
    mpu.update();
    Serial.println("Compass:");
    Serial.print("Angle in Degree X: ");
    Serial.print(mpu.getMagX());
    Serial.print(" Y: ");
    Serial.print(mpu.getMagY());
    Serial.print(" Z: ");
    Serial.println(mpu.getMagZ());
    Serial.print("Temperature: ");
    Serial.println(mpu.getTemperature());
    Serial.println();
    delay(500);
}