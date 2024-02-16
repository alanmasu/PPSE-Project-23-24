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
    @date 10/01/2024
    @author Alan Masutti
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


/*!
    @defgroup   GPS_Module GPS
    @name       GPS Module
    @{
*/

//GPS Sentence constants
#define GGA_SENTENCE "$GPGGA"               //! GGA sentence
#define RMC_SENTENCE "$GPRMC"               //! RMC sentence
#define GSA_SENTENCE "$GPGSA"               //! GSA sentence
#define GSV_SENTENCE "$GPGSV"               //! GSV sentence
#define GLL_SENTENCE "$GPGLL"               //! GLL sentence
#define VTG_SENTENCE "$GPVTG"               //! VTG sentence

#define RX_BUFFER_SIZE 512                  //! Size of RX buffer
                                            //! Uesed also by DMA as max buffer length

//GGA fix data
typedef enum {INVALID = 0, GPS_FIX, DGPS, /*GPS_PPS*/ IRTK, FRTK, DEAD_RECKONING, MANUAL, SIMULATED} GGAFixData_t;

//! GGA data Structure
typedef struct{
    time_t time;                            //! Time                                            SI
    char latitude[12];                      //! Latitude                                        SI
    char longitude[12];                     //! Longitude                                       SI
    GGAFixData_t fix;                       //! Fix                                             SI        
    char sats[4];                           //! Satellites                                      SI
    char hdop[6];                           //! HDOP                                            SI
    char altitude[8];                       //! Altitude                                        SI
    char altitude_WSG84[8];                 //! Geoid altitude on WSG84 reference               
} GpsGGAData_t;

//! RMC data Structure
typedef struct{
    bool valid;                             //! Valid data
    char latitude[12];                      //! Latitude
    char longitude[12];                     //! Longitude
    char altitude[8];                       //! Altitude
    
    char speed[8];                          //! Speed in knots                                  SI
    char course[8];                         //! Course                                          SI
    
    struct tm timeInfo;                     //! Time info
    char others[6];                         //! Others fields
} GpsRMCData_t;

//! GSA data Structure
typedef struct{
    char mode[2];                           //! Mode
    char fix[2];                            //! Fix
    int8_t sats[13];                        //! Satellites
    char pdop[4];                           //! PDOP
    char hdop[4];                           //! HDOP                    
    char vdop[4];                           //! VDOP
} GpsGSAData_t;

//! Satellite data Structure
typedef struct {
    char id[4];                             //! Satellite ID
    char elevation[3];                      //! Elevation
    char azimuth[3];                        //! Azimuth
    char snr[4];                            //! SNR
} SatData_t;

//! GSV data Structure
typedef struct{
    char satsInView[4];                     //! Satellites in view
    SatData_t sats[30];                     //! Satellites
} GpsGSVData_t;

//! VTG data Structure
typedef struct{
    char course[8];                         //! Course
    char courseM[2];                        //! Reference
    char speedK[8];                         //! Speed in knots
    
    char speed[8];                          //! Speed in km/h                                   SI
} GpsVTGData_t;


//TODO: Remove this variables and create getter/setter functions
extern volatile uint8_t gpsUartBuffer[RX_BUFFER_SIZE];
extern volatile bool gpsStringEnd;

bool nmeaChecksumValidate(const char* sentence, char** nextSentence);
time_t getTimeFromString(const char* str);
struct tm getDateFromString(const char* time, const char* date);
float getLatitudeFromString(char* str);
float getLongitudeFromString(char* str);
char* splitString(char* str, char delim, char** next);

void gpsParseData(const char* packet);

//Getter functions
void getGpsData(int* sats, float* speed, float* altitude, float* hdop);
GpsGGAData_t* getGGAData(void);
GpsRMCData_t* getRMCData(void);
GpsGSAData_t* getGSAData(void);
GpsGSVData_t* getGSVData(void);
GpsVTGData_t* getVTGData(void);

/*! @} */ //End of GPS_Module

#endif // __GPS_H__
