#ifndef __COMMON_H__
#define __COMMON_H__

#include <Arduino.h>
#include <Stream.h>

#ifdef ESP8266
  #include <ESP8266WiFi.h>
#else
  #include <WiFi.h>
#endif

#define SOH 0x01
#define STX 0x02
#define ETX 0x03
#define EOT 0x04
#define GS  0x1D


typedef struct {
    float cpuTemp;
    float temp;
} ServerData;

typedef enum {
    DISCONNECTED,
    CONNECTED,
    CONNECTING
} ConnectionState;

typedef struct{
    bool ledState;
    uint8_t ledCount;
    uint32_t ledColor;
    uint8_t ledBrightness;

    ConnectionState connState;
    IPAddress ip;
    bool apMode;
    String ssid;

    void write(Stream& stream = Serial);
    void read(Stream& stream = Serial);
} MsgData;

String splitString(String str, char sep, int index);

#endif // __COMMON_H__