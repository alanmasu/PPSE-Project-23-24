#include <Bussola.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

Adafruit_LSM303_Accel_Unified acc = Adafruit_LSM303_Accel_Unified(12345);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(54321);

Bussola::Bussola(Adafruit_LSM303_Accel_Unified& _acc, Adafruit_LSM303_Mag_Unified& _mag) {
  this->_acc = acc;
  this->_mag = mag;
  bool classInitialized = false;
  calibration.accelerometer_offset = {0.0, 0.0, 0.0};
  calibration.calibrationStatus = NOT_CALIBRATED;
  calibration.magnetometer_degree_offset = 0.0;
  FSMState_t state = UPDATING_DATA, state_pre = CALIBRATION;
  unsigned long long pre_calibration_wait_endTime = 0, calibration_time_step_endTime = 0;
  unsigned int calibration_step = 0;
  float magnetometer_degree = 0.0;
  Axis_t accelerometer = {0.0, 0.0, 0.0};
}

void Bussola::calibrate_sensor() {
  setFSMState(CALIBRATION);
}

void Bussola::stopCalibration() {
  calibration.calibrationStatus = NOT_CALIBRATED;
}

CalibrationStatus_t Bussola::getCalibrationStatus() {
  return calibration.calibrationStatus;
}

float Bussola::getDegree(float mag_x, float mag_y, float mag_z) {
  //return atan(sqrt(sq(mag_z) + sq(mag_y))/mag_x) * 180.0 / PI;
  return atan(mag_y/mag_x) * 180.0 / PI;
}

float Bussola::getDegree() {
  return magnetometer_degree;
}

Axis_t Bussola::getAccelerometerData() {
  return accelerometer;
}

bool Bussola::init() {
  pinMode(28, INPUT);
  drdy = digitalRead(28);
  drdy_pre = !drdy;
  _mag.setMagGain(LSM303_MAGGAIN_8_1);
  _mag.setMagRate(LSM303_MAGRATE_220);
  if(!_acc.begin()){
    return false;
  }
  if(!_mag.begin()){
    return false;
  }
  classInitialized = true;
  return true;
}

bool Bussola::isClassInitialized() {
  return classInitialized;
}

void Bussola::setFSMState(FSMState_t state) {
  this->state = state;
}

bool Bussola::isStateEntry() {
  if(state != state_pre) {
    state_pre = state;
    return true;
  }
  return false;
}

void Bussola::update() {
  drdy = digitalRead(28);
  switch(state) {
    case UPDATING_DATA:
      if(isStateEntry()){
        Serial.println(state);
      }
      if(drdy && !drdy_pre) {
        sensors_event_t accEvent, magEvent;
        acc.getEvent(&accEvent);
        mag.getEvent(&magEvent);
        magnetometer_degree = calibration.magnetometer_degree_offset + 
                              getDegree(magEvent.magnetic.x, magEvent.magnetic.y, magEvent.magnetic.z);
        accelerometer.x = calibration.accelerometer_offset.x + accEvent.acceleration.x;
        accelerometer.y = calibration.accelerometer_offset.y + accEvent.acceleration.y;
        accelerometer.z = calibration.accelerometer_offset.z + accEvent.acceleration.z;
      }
      break;

    case CALIBRATION:
      if(isStateEntry()) {
        calibration.calibrationStatus = CALIBRATION_IN_PROGRESS;
        calibration_step = 0;
        pre_calibration_wait_endTime = millis() + PRE_CALIBRATION_WAIT;
        calibration_time_step_endTime = millis();
      }

      if(millis() >= pre_calibration_wait_endTime) {
        if(millis() >= calibration_time_step_endTime) {
          sensors_event_t accEvent, magEvent;
          acc.getEvent(&accEvent);
          mag.getEvent(&magEvent);
          calibration.accelerometer_offset.x += accEvent.acceleration.x;
          calibration.accelerometer_offset.y += accEvent.acceleration.y;
          calibration.accelerometer_offset.z += accEvent.acceleration.z;
          //calibration.magnetometer_degree_offset += getDegree(magEvent.magnetic.x, magEvent.magnetic.y, magEvent.magnetic.z);

          calibration_step++;
          calibration_time_step_endTime = millis() + CALIBRATION_TIME_STEP;
        }
      }

      if(calibration_step >= CALIBRATION_SAMPLES) {
        calibration.accelerometer_offset.x = 0.0 - (calibration.accelerometer_offset.x / (float)CALIBRATION_SAMPLES);
        calibration.accelerometer_offset.y = 0.0 - (calibration.accelerometer_offset.y / (float)CALIBRATION_SAMPLES);
        calibration.accelerometer_offset.z = 0.0 - (calibration.accelerometer_offset.z / (float)CALIBRATION_SAMPLES);
        calibration.magnetometer_degree_offset = 0.0 - (calibration.magnetometer_degree_offset / (float)CALIBRATION_SAMPLES);
        calibration.calibrationStatus = CALIBRATION_DONE;
        setFSMState(UPDATING_DATA);
      }

      if(calibration.calibrationStatus == NOT_CALIBRATED) {
        setFSMState(UPDATING_DATA);
      }

      break;
  }
  drdy_pre = drdy;
}