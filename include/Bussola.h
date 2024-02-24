#ifndef __BUSSOLA_H__
#define __BUSSOLA_H__
#define CALIBRATION_SAMPLES 300
#define CALIBRATION_TIME_STEP 10
#define PRE_CALIBRATION_WAIT 10

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
    CALIBRATION,
};

class Bussola {
  private:
    struct CalStruct_t {
        Axis_t accelerometer_offset;
        float magnetometer_degree_offset;
        CalibrationStatus_t calibrationStatus;
    };
    Adafruit_LSM303_Accel_Unified _acc;
    Adafruit_LSM303_Mag_Unified _mag;
    CalStruct_t calibration;
    FSMState_t state, state_pre;
    unsigned long long pre_calibration_wait_endTime, calibration_time_step_endTime;
    unsigned int calibration_step;
    float magnetometer_degree;
    Axis_t accelerometer;
    bool classInitialized;
    bool drdy, drdy_pre;

    float getDegree(float mag_x, float mag_y, float mag_z);
    bool isStateEntry();
    void setFSMState(FSMState_t state);

  public:
    Bussola(Adafruit_LSM303_Accel_Unified& _acc = acc, Adafruit_LSM303_Mag_Unified& _mag = mag);
    void calibrate_sensor();
    void stopCalibration();
    CalibrationStatus_t getCalibrationStatus();
    bool init();
    bool isClassInitialized();
    void update();
    float getDegree();
    Axis_t getAccelerometerData();
};



#endif