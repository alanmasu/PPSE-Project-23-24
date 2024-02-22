#ifndef __COMMON_H__
#define __COMMON_H__

#include <Arduino.h>
#include <Stream.h>
#include <IPAddress.h>

#ifdef ESP8266
  #include <ESP8266WiFi.h>
#else
  #include <WiFi.h>
#endif

String splitString(String str, char sep, int index);

typedef struct {
    int8_t sats;
    struct tm timeInfo;
    float latitude;                      
    float longitude;  
    bool fix;
    int fixType;
    float hdop;
    float temp;
} WayPoint_t; 

typedef struct{
    WayPoint_t actualPoint;
    WayPoint_t firstWayPoint;
    float temp;
    float cpuTemp;
    bool gotoAP;
} ApplicationRecord_t;

typedef struct{
    char ssid[33];
    IPAddress ipAddress;
    bool ap;   
}WiFiConfiguration_t;

#endif
