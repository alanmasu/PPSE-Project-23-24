#ifndef __PSEBOARD_H__
#define __PSEBOARD_H__

#define I2C0_SDA    0
#define I2C0_SCL    1

#define USB_ID      2

#define ESP_EN      3

#define GPS_TX      4
#define GPS_RX      5

#define PWM_SERVO_1 7
#define PWM_SERVO_2 8
#define PWM_SERVO_3 9

#define BUCK_EN     11
#define GPS_RSTn    12
#define WS2812_PIN  13

#define INT1_ACC    14
#define INT2_ACC    15

#define ESP_RX      16
#define ESP_TX      17

#define GPS_EN      18

#define GPS_PPS     19

#define ALIVE_LED   20
#define BUZZER      21

#define BTN_UP      22
#define BTN_DOWN    23
#define BTN_RIGHT   24
#define BTN_LEFT    25

// Temperature sensor is attached to GPIO 26
#define TEMP_SENSOR A0 

#define OUT_USB_EN  27

// Battery voltage is attached to GPIO 28
#define VBAT        A2

#endif // __PSEBOARD_H__