#ifndef __COMMON_H__
#define __COMMON_H__

#include <Arduino.h>
#include <IPAddress.h>

String splitString(String str, char sep, int index);


typedef struct {
    int8_t sats;
    struct tm timeInfo_1;
    float latitude_1;                      
    float longitude_1;  
    bool fix_1;
    int fix_num_1;
    float hdop_1;
    float temp_t;
} WayPoint_t; 

typedef struct{
    WayPoint_t actualPoint;
    WayPoint_t firstWayPoint;
    float temp;
    bool gotoAP;
} ApplicationRecord_t;

typedef struct{
    char ssid[32];
    IPAddress ipAddress;
    bool ap;   
}WiFiConfiguration_t;




#endif