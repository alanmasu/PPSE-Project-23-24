/*!
    @file GPS.h
    @ingroup GPS_Module
    @brief GPS module header file
    @details This file contains the GPS module functions prototypes and some definitions.
        In hardware the connections are:
    @code
            MSP432P401
        -------------------
        |                 |
   RST -|                 |
        |                 |
        |                 |
        |     P3.3/UCA0TXD|----> GPS_L80 RX at 9600 8N1
       -|                 |
        |     P3.2/UCA0RXD|----< GPS_L80 TX at 9600 8N1
        |                 |
        |                 |
        -------------------
    @endcode
    @date 19/02/2024
    @author Andrea Farinaro
    @note This file have some functions thas uses the DMA so it is necessary to share the DMA module
          if needed some where else.
*/

#ifndef __GPS_H__
#define __GPS_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <PSEBoard.h>
#include <common.h>

#define GPS_DEBUG 0

/*!
    @defgroup   GPS_Module GPS
    @name       GPS Module
    @{
*/

//GPS Sentence constants
#define GGA_SENTENCE "$GNGGA"               //! GGA sentence
#define RMC_SENTENCE "$GNRMC"               //! RMC sentence
#define GSA_SENTENCE "$GNGSA"               //! GSA sentence
#define GLGSV_SENTENCE "$GLGSV"
#define GPGSV_SENTENCE "$GPGSV"                  //! GSV sentence
#define GLL_SENTENCE "$GNGLL"               //! GLL sentence
#define VTG_SENTENCE "$GNVTG"               //! VTG sentence

#define RX_BUFFER_SIZE 512                  //! Size of RX buffer
                                            //! Uesed also by DMA as max buffer length

//GGA fix data
typedef enum {INVALID = 0, GPS_FIX, DGPS, IRTK, FRTK, DEAD_RECKONING, MANUAL, SIMULATED} GGAFixData_t;

//! GGA data Structure
typedef struct{
    time_t time;                            //! Time                                            
    char latitude[12];                      //! Latitude                                        
    char longitude[12];                     //! Longitude                                       
    GGAFixData_t fix;                       //! Fix                                                     
    char sats[4];                           //! Satellites                                      
    char hdop[6];                           //! HDOP                                            
    char altitude[8];                       //! Altitude                                        
    char altitude_WSG84[8];                 //! Geoid altitude on WSG84 reference               
} GpsGGAData_t;

//! RMC data Structure
typedef struct{
    bool valid;                             //! Valid data
    char latitude[12];                      //! Latitude
    char longitude[12];                     //! Longitude
    char altitude[8];                       //! Altitude
    
    char speed[8];                          //! Speed in knots                                  
    char course[8];                         //! Course                                          
    
    struct tm timeInfo;                     //! Time info                                       
    char others[6];                         //! Others fields
} GpsRMCData_t;

//! GSA data Structure
typedef struct{
    char mode[2];                           //! Mode
    char fix[2];                            //! Fix
    int8_t sats[13];                        //! Satellites
    char pdop[6];                           //! PDOP
    char hdop[6];                           //! HDOP                    
    char vdop[6];                           //! VDOP
} GpsGSAData_t;

//! Satellite data Structure
typedef struct {
    char id[4];                             //! Satellite ID
    char elevation[5];                      //! Elevation
    char azimuth[5];                        //! Azimuth
    char snr[5];                            //! SNR
} SatData_t;

//! GPGSV data Structure
typedef struct{
    char satsInView[4];                     //! Satellites in view
    SatData_t sats[30];                     //! Satellites
} GpsGPGSVData_t;

//! GPGSV data Structure
typedef struct{
    char satsInView[4];                     //! Satellites in view
    SatData_t sats[30];                     //! Satellites
} GpsGLGSVData_t;


//! VTG data Structure
typedef struct{
    char course[8];                         //! Course
    char courseM[5];                        //! Reference
    char speedK[8];                         //! Speed in knots
    
    char speed[8];                          //! Speed in km/h                                   
} GpsVTGData_t;


//TODO: Remove this variables and create getter/setter functions
extern volatile uint8_t gpsUartBuffer[RX_BUFFER_SIZE];
extern volatile bool gpsStringEnd;

bool nmeaChecksumValidate(const char* sentence, const char** nextSentence);
time_t getTimeFromString(const char* str);
struct tm getDateFromString(const char* time, const char* date);
float getLatitudeFromString(const char* str);
float getLongitudeFromString(const char* str);
char* splitString(char* str, char delim, char** next);

void gpsParseData(const char* packet);

//Getter functions
void getGpsData(int* sats, float* speed, float* altitude, float* hdop);
void getGpsFixData(uint8_t* fix, uint8_t* sats, float* hdop);
GpsGGAData_t* getGGAData(void);
GpsRMCData_t* getRMCData(void);
GpsGSAData_t* getGSAData(void);
GpsGPGSVData_t* getGPGSVData(void);
GpsGLGSVData_t* getGLGSVData(void);
GpsVTGData_t* getVTGData(void);

//modifica da verificare
    void popWaypoint_t(WayPoint_t& waypoint);
    void saveWayPoint(WayPoint_t& lastWaypoint);


/*! @} */ //End of GPS_Module

#endif // __GPS_H__
