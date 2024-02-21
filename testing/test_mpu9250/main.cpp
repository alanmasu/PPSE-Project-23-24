#include "PSEBoard.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

Adafruit_LSM303_Accel_Unified acc = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

struct CalAccStruct {
    float x;
    float y;
    float z;
};

CalAccStruct calibrateAcc(Adafruit_LSM303_Accel_Unified *acc) {
    #ifndef CALIBRATION_SAMPLES
        #define CALIBRATION_SAMPLES 300
    #endif
    sensors_event_t event;
    float calib_x, calib_y, calib_z;
    for(int i = 0; i < CALIBRATION_SAMPLES; i++){
        acc->getEvent(&event);
        calib_x += event.acceleration.x;
        calib_y += event.acceleration.y;
        calib_z += event.acceleration.z;
        delay(10);
    }
    CalAccStruct calib_struct;
    calib_struct.x = calib_x / (float)CALIBRATION_SAMPLES;
    calib_struct.y = calib_y / (float)CALIBRATION_SAMPLES;
    calib_struct.z = calib_z / (float)CALIBRATION_SAMPLES;
    return calib_struct;
}

void setup(void)
{
    Wire.setSDA(I2C0_SDA);
    Wire.setSCL(I2C0_SCL);
    Serial.begin(115200);
    while (!Serial);
    delay(2000);
    Serial.println("Accelerometer and Compass Test"); Serial.println("");

    /* Initialise the sensor */
    if(!acc.begin())
    if(!mag.begin()){
        {
            Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
            while(1);
        }
    }
    
    mag.enableAutoRange(true);

    cal_acc_struct = calibrateAcc();
}

void loop(void)
{
    /* Get a new sensor event */
    sensors_event_t event_mag;
    mag.getEvent(&event_mag);
    sensors_event_t event_acc;
    acc.getEvent(&event_acc);

    float x_mag = event_mag.magnetic.x;
    float y_mag = event_mag.magnetic.y;
    float z_mag = event_mag.magnetic.z;

    double vector_angle = asin(y_mag/sqrt(sq(x_mag) + sq(y_mag))) * 180.0 / PI;

    Serial.print("X: "); Serial.print(event_acc.acceleration.x); Serial.print("  ");
    Serial.print("Y: "); Serial.print(event_acc.acceleration.y); Serial.print("  ");
    Serial.print("Z: "); Serial.print(event_acc.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");
    Serial.print("Angle: "); Serial.print(vector_angle); Serial.println(" Degree");
    Serial.println();
    delay(500);
}

// // Test con MPU9250

// #include <Arduino.h>
// #include <Wire.h>
// #include <MPU9250.h>
// #include "PSEBoard.h"

// MPU9250 mpu;

// void setup() {
//     Wire.setSDA(I2C0_SDA);
//     Wire.setSCL(I2C0_SCL);
//     Wire.begin();
//     Serial.begin(115200);  
//     while(!Serial);
//     delay(2000);
//     Serial.println("MPU9250 Test program started...");
//     mpu.verbose(true);
//     MPU9250Setting setting;
//     setting.accel_fs_sel = ACCEL_FS_SEL::A16G;
//     setting.gyro_fs_sel = GYRO_FS_SEL::G2000DPS;
//     setting.mag_output_bits = MAG_OUTPUT_BITS::M16BITS;
//     setting.fifo_sample_rate = FIFO_SAMPLE_RATE::SMPL_200HZ;
//     setting.gyro_fchoice = 0x03;
//     setting.gyro_dlpf_cfg = GYRO_DLPF_CFG::DLPF_41HZ;
//     setting.accel_fchoice = 0x01;
//     setting.accel_dlpf_cfg = ACCEL_DLPF_CFG::DLPF_45HZ;
//     Wire.beginTransmission(0x68);
//     bool error = 1;
//     error = Wire.endTransmission();
//     if (!mpu.setup(0x68, setting)) {
//         Serial.println("Failed to find MPU9250 chip");
//         while (1) {
//         delay(10);
//         }
//     }
//     Serial.println("MPU9250 Found!");
//     Serial.println();
//     Serial.println("MPU9250 Information:");
// }

// void loop() {
//     mpu.update();
//     Serial.println("Compass:");
//     Serial.print("Angle in Degree X: ");
//     Serial.print(mpu.getMagX());
//     Serial.print(" Y: ");
//     Serial.print(mpu.getMagY());
//     Serial.print(" Z: ");
//     Serial.println(mpu.getMagZ());
//     Serial.print("Temperature: ");
//     Serial.println(mpu.getTemperature());
//     Serial.println();
//     delay(500);
// }