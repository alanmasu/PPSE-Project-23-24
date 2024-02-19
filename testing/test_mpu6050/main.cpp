// Test con MPU6050 fallito, il drift del gyroscopio non permette il rilevamento preciso dell'angolo di rotazione 
// della scheda (ottenuto integrando la velocità angolare ritornata dal sensore).
// Rimane la parte implementativa di test. Il Branch viene abbandonato.

// >> DO NOT USE <<  >> DO NOT USE <<  >> DO NOT USE <<  >> DO NOT USE <<  >> DO NOT USE <<
// >> NON  USARE <<  >> NON  USARE <<  >> NON  USARE <<  >> NON  USARE <<  >> NON  USARE <<

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include "PSEBoard.h"

#define REFRESH_TIME 500
#define CALIBRATION_SAMPLES 3000
#define ROUNDING_CHECK_TIME 1000

Adafruit_MPU6050 mpu;
sensors_event_t acc_event, gyro_event, temp_event;
unsigned long int millis_pre, last_value, last_time_checked_value;
float x_degree_value_pre = 0.0, y_degree_value_pre = 0.0, z_degree_value_pre = 0.0;
float x_gyro_calibration = 0.0, y_gyro_calibration = 0.0, z_gyro_calibration = 0.0;

float radToDegree(float rad);
void calibrateGyro(float &x_gyro_calibration, float &y_gyro_calibration, float &z_gyro_calibration);
void calculate_degree(float &x_degree, float &y_degree, float &z_degree);

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
  Serial.println();
  mpu.getEvent(&acc_event, &gyro_event, &temp_event);
  Serial.println("MPU6050 Information:");
  Serial.print("Accelerometer event: -> Event type: ");
  Serial.println((sensors_type_t)acc_event.type);
  Serial.print("Gyroscope event: -> Event type: ");
  Serial.println((sensors_type_t)gyro_event.type);
  Serial.print("Temperature event: -> Event type: ");
  Serial.println((sensors_type_t)temp_event.type);
  Serial.print("Sensor Clock: ");
  Serial.println(mpu.getClock());
  Serial.print("Sensor SampleRateDivisor: ");
  Serial.println(mpu.getSampleRateDivisor());
  mpu.setCycleRate(MPU6050_CYCLE_40_HZ);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Sensor CycleRate of Samples: ");
  Serial.println(mpu.getCycleRate());
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Sensor low-pass filter settings: ");
  Serial.println(mpu.getFilterBandwidth());
  Serial.println();
  calibrateGyro(x_gyro_calibration, y_gyro_calibration, z_gyro_calibration);
  millis_pre = last_value = last_time_checked_value = millis();
}

float radToDegree(float rad){
  return (360.0 * rad) / (2 * PI);
}

void calibrateGyro(float &x_gyro_calibration, float &y_gyro_calibration, float &z_gyro_calibration){
  Serial.println("Keep the Gyro steady...");
  for(int i = 0; i <= CALIBRATION_SAMPLES; i++){
    mpu.getEvent(&acc_event, &gyro_event, &temp_event);
    x_gyro_calibration += gyro_event.acceleration.x;
    y_gyro_calibration += gyro_event.acceleration.y;
    z_gyro_calibration += gyro_event.acceleration.z;
    delay(1);
  }
  x_gyro_calibration = 0 - (x_gyro_calibration / (float)CALIBRATION_SAMPLES);
  y_gyro_calibration = 0 - (y_gyro_calibration / (float)CALIBRATION_SAMPLES);
  z_gyro_calibration = 0 - (z_gyro_calibration / (float)CALIBRATION_SAMPLES);
  Serial.println("Calibration Completed");
}

void calculate_degree(float &x_degree, float &y_degree, float &z_degree){
  mpu.getEvent(&acc_event, &gyro_event, &temp_event);
  unsigned long int dt = (millis() - millis_pre);
  x_degree += radToDegree(((float)dt * (gyro_event.acceleration.x + x_gyro_calibration)) / 1000.0);
  y_degree += radToDegree(((float)dt * (gyro_event.acceleration.y + y_gyro_calibration)) / 1000.0);
  z_degree += radToDegree(((float)dt * (gyro_event.acceleration.z + z_gyro_calibration)) / 1000.0);
  millis_pre = millis();
}

float x_degree = 0.0, y_degree = 0.0, z_degree = 0.0;

void loop() {
  calculate_degree(x_degree, y_degree, z_degree);
  if(millis() - last_time_checked_value >= ROUNDING_CHECK_TIME){
    x_degree = (abs(abs(x_degree) - abs(x_degree_value_pre)) > 0.5)? x_degree : x_degree_value_pre;
    y_degree = (abs(abs(y_degree) - abs(y_degree_value_pre)) > 0.5)? y_degree : y_degree_value_pre;
    z_degree = (abs(abs(z_degree) - abs(z_degree_value_pre)) > 0.5)? z_degree : z_degree_value_pre;
    x_degree_value_pre = x_degree;
    y_degree_value_pre = y_degree;
    z_degree_value_pre = z_degree;
    last_time_checked_value = millis();
  }
  if(millis() - last_value >= REFRESH_TIME){
    Serial.print("Accelerometer X: ");
    Serial.print(acc_event.acceleration.x);
    Serial.print(" Y: ");
    Serial.print(acc_event.acceleration.y);
    Serial.print(" Z: ");
    Serial.println(acc_event.acceleration.z);
    Serial.print("Gyroscope X: ");
    Serial.print(gyro_event.gyro.x + x_gyro_calibration);
    Serial.print(" Y: ");
    Serial.print(gyro_event.gyro.y + y_gyro_calibration);
    Serial.print(" Z: ");
    Serial.println(gyro_event.gyro.z + z_gyro_calibration);
    Serial.print("Angle in Degree X: ");
    Serial.print(x_degree_value_pre);
    Serial.print(" Y: ");
    Serial.print(y_degree_value_pre);
    Serial.print(" Z: ");
    Serial.println(z_degree_value_pre);
    Serial.print("Temperature: ");
    Serial.println(temp_event.temperature);
    Serial.println();
    last_value = millis();
  }
}