#ifndef __BUSSOLA_H__
#define __BUSSOLA_H__
#define ACCELEROMETER_CALIBRATION_SAMPLES 500
#define MAGNETOMETER_CALIBRATION_SAMPLES 800
#define CALIBRATION_TIME_STEP 10
#define PRE_ACCELEROMETER_CALIBRATION_WAIT 1000
#define N_SAMPLES_ACCELEROMETER_AVERAGING 25
#define N_SAMPLES_MAGNETOMETER_AVERAGING 25

#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include "PSEBoard.h"

extern Adafruit_LSM303_Accel_Unified acc;
extern Adafruit_LSM303_Mag_Unified mag;

struct Axis_t {
    float x;
    float y;
    float z;
};

enum CalibrationStatus_t {
    NOT_CALIBRATED,
    CALIBRATION_IN_PROGRESS,
    CALIBRATION_DONE
};

enum FSMState_t {
    UPDATING_DATA,
    CALIBRATION_ACCELEROMETER,
    CALIBRATION_MAGNETOMETER
};

class Bussola {
  private:
    struct CalStruct_t {
        Axis_t accelerometer_offset;
        Axis_t magnetometer_offset, magnetometer_min, magnetometer_max;
        CalibrationStatus_t calibrationStatus;
        CalibrationStatus_t accelerometerCalibrationStatus;
        CalibrationStatus_t magnetometerCalibrationStatus;
    };
    Adafruit_LSM303_Accel_Unified _acc;
    Adafruit_LSM303_Mag_Unified _mag;
    CalStruct_t calibration;
    FSMState_t state, state_pre;
    unsigned long long pre_calibration_wait_endTime, calibration_time_step_endTime;
    unsigned int calibration_step;
    unsigned int magnetometer_last_value_index;
    unsigned int accelerometer_last_value_index;
    float magnetometer_degree[N_SAMPLES_MAGNETOMETER_AVERAGING], magnetometer_degree_averaged;
    Axis_t accelerometer[N_SAMPLES_ACCELEROMETER_AVERAGING], accelerometer_value_averaged;
    bool classInitialized;
    bool accelerometer_rolling_rounding_active;
    bool magnetometer_rolling_rounding_active;
    bool accelerometer_averaging_valid;
    bool magnetometer_averaging_valid;

    float getDegree(float mag_x, float mag_y, float mag_z);
    bool isStateEntry();
    void setFSMState(FSMState_t state);

  public:
    Bussola(Adafruit_LSM303_Accel_Unified& _acc = acc, Adafruit_LSM303_Mag_Unified& _mag = mag);
    void calibrate_accelerometer();
    void calibrate_magnetometer();
    void stopResetCalibration();
    CalibrationStatus_t getCalibrationStatus();
    CalibrationStatus_t getAccelerometerCalibrationStatus();
    CalibrationStatus_t getMagnetometerCalibrationStatus();
    bool init();
    bool isClassInitialized();
    void update();
    void accelerometerRollingRoundig(bool activate);
    void magnetometerRollingRoundig(bool activate);
    float getDegree();
    Axis_t getAccelerometerData();
};



#endif