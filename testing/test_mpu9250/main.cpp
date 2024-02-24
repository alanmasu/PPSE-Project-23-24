#include "PSEBoard.h"
#include "Bussola.h"

Bussola bus = Bussola();
unsigned long long t1;

void setup(void)
{
    Wire.setSDA(I2C0_SDA);
    Wire.setSCL(I2C0_SCL);
    Serial.begin(115200);
    while (!Serial);
    delay(2000);
    Serial.println("Accelerometer and Compass Test"); Serial.println("");
    Serial.print("Is the class Bussola Initialized? "); Serial.println(bus.isClassInitialized());
    bus.init();
    Serial.print("Is the class Bussola Initialized? "); Serial.println(bus.isClassInitialized());
    Serial.print("Is sensor Calibrated? (yes = 2) "); Serial.println(bus.getCalibrationStatus());
    bus.calibrate_sensor();
    Serial.print("Is sensor Calibrated? (yes = 2) "); Serial.println(bus.getCalibrationStatus());
    t1 = millis() + 500;
}

void loop(void)
{
    if(bus.getCalibrationStatus() == CalibrationStatus_t::CALIBRATION_DONE) {
        if(millis() >= t1) {
            Serial.print("Is sensor Calibrated? (yes = 2) "); Serial.println(bus.getCalibrationStatus());
            Axis_t accData = bus.getAccelerometerData();
            Serial.print("X: "); Serial.print(accData.x); Serial.print("  ");
            Serial.print("Y: "); Serial.print(accData.y); Serial.print("  ");
            Serial.print("Z: "); Serial.print(accData.z); Serial.print("  "); Serial.println("m/s^2 ");
            Serial.print("Angle: "); Serial.print(bus.getDegree()); Serial.println(" Degree");
            Serial.println();
            t1 = millis() + 500;
        }
    }
    else {
        if(millis() >= t1) {
            Serial.print("Is sensor Calibrated? (yes = 2) "); Serial.println(bus.getCalibrationStatus());
            t1 = millis() + 500;
        }
    }
    bus.update();
}