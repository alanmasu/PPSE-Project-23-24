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
  classInitialized = false;
  magnetometer_rolling_rounding_active = false;
  accelerometer_rolling_rounding_active = false;
  accelerometer_averaging_valid = false;
  magnetometer_averaging_valid = false;
  calibration.accelerometer_offset = {0.0, 0.0, 0.0};
  calibration.calibrationStatus = NOT_CALIBRATED;
  calibration.accelerometerCalibrationStatus = NOT_CALIBRATED;
  calibration.magnetometerCalibrationStatus = NOT_CALIBRATED;
  calibration.magnetometer_offset = {0.0, 0.0, 0.0};
  calibration.magnetometer_max = {0.0, 0.0, 0.0};
  calibration.magnetometer_min = {0.0, 0.0, 0.0};
  FSMState_t state = UPDATING_DATA, state_pre = CALIBRATION_ACCELEROMETER;
  pre_calibration_wait_endTime = 0;
  calibration_time_step_endTime = 0;
  calibration_step = 0;
  accelerometer_last_value_index = 0;
  magnetometer_last_value_index = 0;
  for(unsigned int i = 0; i < N_SAMPLES_ACCELEROMETER_AVERAGING; i++) {
    accelerometer[i].x = 0.0;
    accelerometer[i].y = 0.0;
    accelerometer[i].z = 0.0;
  }
  accelerometer_value_averaged = {0.0, 0.0, 0.0};
  for(unsigned int i = 0; i < N_SAMPLES_MAGNETOMETER_AVERAGING; i++) {
    magnetometer_degree[i] = 0.0;
  }
  magnetometer_degree_averaged = 0.0;
}

void Bussola::calibrate_accelerometer() {
  if(calibration.accelerometerCalibrationStatus == NOT_CALIBRATED)
    setFSMState(CALIBRATION_ACCELEROMETER);
}

void Bussola::calibrate_magnetometer() {
  if(calibration.magnetometerCalibrationStatus == NOT_CALIBRATED)
    setFSMState(CALIBRATION_MAGNETOMETER);
}

void Bussola::stopResetCalibration() {
  calibration.calibrationStatus = NOT_CALIBRATED;
  calibration.accelerometerCalibrationStatus = NOT_CALIBRATED;
  calibration.magnetometerCalibrationStatus = NOT_CALIBRATED;
  setFSMState(UPDATING_DATA);
}

void Bussola::accelerometerRollingRoundig(bool activate) {
  accelerometer_rolling_rounding_active = activate;
}

void Bussola::magnetometerRollingRoundig(bool activate) {
  magnetometer_rolling_rounding_active = activate;
}

CalibrationStatus_t Bussola::getCalibrationStatus() {
  return calibration.calibrationStatus;
}

CalibrationStatus_t Bussola::getAccelerometerCalibrationStatus() {
  return calibration.accelerometerCalibrationStatus;
}

CalibrationStatus_t Bussola::getMagnetometerCalibrationStatus() {
  return calibration.magnetometerCalibrationStatus;
}

float Bussola::getDegree(float mag_x, float mag_y, float mag_z) {
  float temp_degree;
  float temp_ipo = sqrt(sq(mag_x) + sq(mag_y));
  if(mag_x >= 0 && mag_y >= 0) {
      temp_degree = asin(abs(mag_x / temp_ipo)) * 180.0 / PI;
  } else if(mag_x >= 0 && mag_y < 0) {
      temp_degree = asin(abs(mag_y / temp_ipo)) * 180.0 / PI + 90.0;
  } else if(mag_x < 0 && mag_y < 0) {
      temp_degree = asin(abs(mag_x / temp_ipo)) * 180.0 / PI + 180.0;
  } else if(mag_x < 0 && mag_y >= 0) {
      temp_degree = asin(abs(mag_y / temp_ipo)) * 180.0 / PI + 270.0;
  }
  return temp_degree;
}

Axis_t Bussola::getAccelerometerData() {
  if(accelerometer_rolling_rounding_active && accelerometer_averaging_valid) {
    return accelerometer_value_averaged;
  } else {
    return accelerometer[accelerometer_last_value_index];
  }
}

float Bussola::getDegree() {
  if(magnetometer_rolling_rounding_active && magnetometer_averaging_valid) {
    return magnetometer_degree_averaged;
  } else {
    return magnetometer_degree[magnetometer_last_value_index];
  }
}

bool Bussola::init() {
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
  switch(state) {
    case UPDATING_DATA:
      if(isStateEntry()){
      }
      // Uncomment the condition if you want that the getDegree function returns the last valid 
      // value (a value got after calling calibration) instead of raw values read without calibration
      if(/*calibration.calibrationAccelerometerStatus == CALIBRATION_DONE && calibration.calibrationMagnetometerStatus == CALIBRATION_DONE*/ 1) {
        sensors_event_t accEvent, magEvent;
        // Accelerometer
        acc.getEvent(&accEvent);
        Axis_t accelerometer_temp = {0.0, 0.0, 0.0};
        accelerometer_last_value_index = (++accelerometer_last_value_index) % N_SAMPLES_ACCELEROMETER_AVERAGING;
        accelerometer[accelerometer_last_value_index].x = calibration.accelerometer_offset.x + accEvent.acceleration.x;
        accelerometer[accelerometer_last_value_index].y = calibration.accelerometer_offset.y + accEvent.acceleration.y;
        accelerometer[accelerometer_last_value_index].z = calibration.accelerometer_offset.z + accEvent.acceleration.z;
        // Accelerometer averaging
        //Serial.println("Accelerometer:");
        for(unsigned int i = 0; i < N_SAMPLES_ACCELEROMETER_AVERAGING; i++) {
          accelerometer_temp.x += accelerometer[i].x;
          accelerometer_temp.y += accelerometer[i].y;
          accelerometer_temp.z += accelerometer[i].z;
          /*Serial.print(accelerometer[i].x);
          Serial.print("  ");
          Serial.print(accelerometer[i].y);
          Serial.print("  ");
          Serial.println(accelerometer[i].z);*/
        }
        //Serial.println();
        accelerometer_value_averaged.x = accelerometer_temp.x / (float)N_SAMPLES_ACCELEROMETER_AVERAGING;
        accelerometer_value_averaged.y = accelerometer_temp.y / (float)N_SAMPLES_ACCELEROMETER_AVERAGING;
        accelerometer_value_averaged.z = accelerometer_temp.z / (float)N_SAMPLES_ACCELEROMETER_AVERAGING;
        if(accelerometer_last_value_index == 0) {
          accelerometer_averaging_valid = true;
        }

        // Magnetometer
        mag.getEvent(&magEvent);
        float magnetometer_temp = 0.0;
        magnetometer_last_value_index = (++magnetometer_last_value_index) % N_SAMPLES_MAGNETOMETER_AVERAGING;
        magnetometer_degree[magnetometer_last_value_index] = getDegree(
                    magEvent.magnetic.x + calibration.magnetometer_offset.x, 
                    magEvent.magnetic.y + calibration.magnetometer_offset.y, 
                    magEvent.magnetic.z + calibration.magnetometer_offset.z);
        // Magnetometer averaging
        //Serial.println("Magnetometer:");
        for(unsigned int i = 0; i < N_SAMPLES_MAGNETOMETER_AVERAGING; i++) {
          magnetometer_temp += magnetometer_degree[i];
          //Serial.println(magnetometer_degree[i]);
        }
        //Serial.println();
        magnetometer_degree_averaged = magnetometer_temp / (float)N_SAMPLES_MAGNETOMETER_AVERAGING;
        if(magnetometer_last_value_index == 0) {
          magnetometer_averaging_valid = true;
        }
      }

      break;

    case CALIBRATION_ACCELEROMETER:
      if(isStateEntry()) {
        calibration.calibrationStatus = CALIBRATION_IN_PROGRESS;
        calibration.accelerometerCalibrationStatus = CALIBRATION_IN_PROGRESS;
        calibration_step = 0;
        pre_calibration_wait_endTime = millis() + PRE_ACCELEROMETER_CALIBRATION_WAIT;
        calibration_time_step_endTime = millis();
      }

      if(millis() >= pre_calibration_wait_endTime) {
        if(millis() >= calibration_time_step_endTime) {
          //Serial.println("Here");
          sensors_event_t accEvent;
          acc.getEvent(&accEvent);
          calibration.accelerometer_offset.x += accEvent.acceleration.x;
          calibration.accelerometer_offset.y += accEvent.acceleration.y;
          calibration.accelerometer_offset.z += accEvent.acceleration.z;
          calibration_step++;
          calibration_time_step_endTime = millis() + CALIBRATION_TIME_STEP;
        }
      }

      if(calibration_step >= ACCELEROMETER_CALIBRATION_SAMPLES) {
        calibration.accelerometer_offset.x = 0.0 - (calibration.accelerometer_offset.x / (float)ACCELEROMETER_CALIBRATION_SAMPLES);
        calibration.accelerometer_offset.y = 0.0 - (calibration.accelerometer_offset.y / (float)ACCELEROMETER_CALIBRATION_SAMPLES);
        calibration.accelerometer_offset.z = 0.0 - (calibration.accelerometer_offset.z / (float)ACCELEROMETER_CALIBRATION_SAMPLES);
        calibration.accelerometerCalibrationStatus = CALIBRATION_DONE;
        calibration.calibrationStatus = (calibration.magnetometerCalibrationStatus == CALIBRATION_DONE)? CALIBRATION_DONE : CALIBRATION_IN_PROGRESS;
        setFSMState(UPDATING_DATA);
        update();
      }
      break;

      case CALIBRATION_MAGNETOMETER:
      if(isStateEntry()) {
        sensors_event_t magEvent;
        mag.getEvent(&magEvent);
        calibration.magnetometer_min.x = calibration.magnetometer_max.x = magEvent.magnetic.x;
        calibration.magnetometer_min.y = calibration.magnetometer_max.y = magEvent.magnetic.y;
        calibration.magnetometer_min.z = calibration.magnetometer_max.z = magEvent.magnetic.z;
        calibration.magnetometerCalibrationStatus = CALIBRATION_IN_PROGRESS;
        calibration.calibrationStatus = CALIBRATION_IN_PROGRESS;
        calibration_step = 0;
        pre_calibration_wait_endTime = millis() + PRE_MAGNETOMETER_CALIBRATION_WAIT;
        calibration_time_step_endTime = millis();
      }
      if(millis() >= pre_calibration_wait_endTime) {
        if(millis() >= calibration_time_step_endTime) {
          sensors_event_t magEvent;
          mag.getEvent(&magEvent);
          calibration.magnetometer_min.x = min(calibration.magnetometer_min.x, magEvent.magnetic.x);
          calibration.magnetometer_min.y = min(calibration.magnetometer_min.y, magEvent.magnetic.y);
          calibration.magnetometer_min.z = min(calibration.magnetometer_min.z, magEvent.magnetic.z);
          calibration.magnetometer_max.x = max(calibration.magnetometer_max.x, magEvent.magnetic.x);
          calibration.magnetometer_max.y = max(calibration.magnetometer_max.y, magEvent.magnetic.y);
          calibration.magnetometer_max.z = max(calibration.magnetometer_max.z, magEvent.magnetic.z);

          calibration_step++;
          calibration_time_step_endTime = millis() + CALIBRATION_TIME_STEP;
        }
      }

      if(calibration_step >= MAGNETOMETER_CALIBRATION_SAMPLES) {
        calibration.magnetometer_offset.x = 0.0 - ((calibration.magnetometer_min.x + calibration.magnetometer_max.x) / 2.0);
        calibration.magnetometer_offset.y = 0.0 - ((calibration.magnetometer_min.y + calibration.magnetometer_max.y) / 2.0);
        calibration.magnetometer_offset.z = 0.0 - ((calibration.magnetometer_min.z + calibration.magnetometer_max.z) / 2.0);  
        // Serial.print("Calibration offset x: "); Serial.println(calibration.magnetometer_offset.x);
        // Serial.print("Calibration offset y: "); Serial.println(calibration.magnetometer_offset.y);
        // Serial.print("Calibration offset z: "); Serial.println(calibration.magnetometer_offset.z);
        calibration.magnetometerCalibrationStatus = CALIBRATION_DONE;
        calibration.calibrationStatus = (calibration.accelerometerCalibrationStatus == CALIBRATION_DONE)? CALIBRATION_DONE : CALIBRATION_IN_PROGRESS;
        setFSMState(UPDATING_DATA);
        update();
      }
      break;
  }
}