#include "PSEBoard.h"
#include "Bussola.h"

#define LED_WAIT_MS 400

Bussola bus = Bussola();
unsigned long long t1, t_led;
bool led = HIGH;
float degree_pre = 0.0, degree = 0.0;

void setup(void)
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, led);
    Wire.setSDA(I2C0_SDA);
    Wire.setSCL(I2C0_SCL);
    Serial.begin(115200);
    while (!Serial);
    delay(2000);
    Serial.println("Accelerometer and Compass Test"); Serial.println("");
    Serial.print("Is the class Bussola Initialized? "); Serial.println(bus.isClassInitialized());
    bus.init();
    bus.accelerometerRollingRoundig(true);
    bus.magnetometerRollingRoundig(true);
    Serial.print("Is the class Bussola Initialized? "); Serial.println(bus.isClassInitialized());
    Serial.print("Is sensor Calibrated? (yes = 2) "); Serial.println(bus.getCalibrationStatus());
    bus.calibrate_accelerometer();
    Serial.print("Is sensor Calibrated? (yes = 2) "); Serial.println(bus.getCalibrationStatus());
    t1 = millis() + 1000;
    t_led = millis();
}

void loop(void)
{
    if(bus.getAccelerometerCalibrationStatus() == CALIBRATION_DONE) {
        bus.calibrate_magnetometer();
        if(bus.getCalibrationStatus() == CALIBRATION_DONE) {
            if(millis() >= t1) {
                Serial.print("Is sensor Calibrated? (yes = 2) "); Serial.println(bus.getCalibrationStatus());
                Serial.print("Is accelerometer Calibrated? (yes = 2) "); Serial.println(bus.getAccelerometerCalibrationStatus());
                Serial.print("Is magnetometer Calibrated? (yes = 2) "); Serial.println(bus.getMagnetometerCalibrationStatus());
                Axis_t accData = bus.getAccelerometerData();
                Serial.print("X: "); Serial.print(accData.x); Serial.print("  ");
                Serial.print("Y: "); Serial.print(accData.y); Serial.print("  ");
                Serial.print("Z: "); Serial.print(accData.z); Serial.print("  "); Serial.println("m/s^2 ");
                //if(degree < (degree_pre - 5.0) || degree > (degree_pre + 5.0)) {
                    Serial.print("Angle: "); Serial.print(degree); Serial.println(" Degree");
                    degree_pre = degree;
                //}
                degree = bus.getDegree();
                Serial.println();
                t1 = millis() + 1000;
            }
        }
    }
    if(millis() >= t1 && bus.getCalibrationStatus() != CALIBRATION_DONE) {
        Serial.print("Is sensor Calibrated? (yes = 2) "); Serial.println(bus.getCalibrationStatus());
        Serial.print("Is accelerometer Calibrated? (yes = 2) "); Serial.println(bus.getAccelerometerCalibrationStatus());
        Serial.print("Is magnetometer Calibrated? (yes = 2) "); Serial.println(bus.getMagnetometerCalibrationStatus());
        t1 = millis() + 1000;
    }
    if(millis() >= t_led) {
        led = !led;
        digitalWrite(LED_BUILTIN, led);
        t_led = millis() + LED_WAIT_MS;
    }
    bus.update();
}