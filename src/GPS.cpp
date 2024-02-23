/*!
    @file   GPS.c
    @ingroup GPS_Module
    @brief  GPS module function implementations
    @date   19/02/2024
    @author Andrea Farinaro
*/

/*!
    @addtogroup GPS_Module
    @{
        @brief      GPS module functions
        @details    This module contains the functions used to configure and use the GPS module.
                    There are functions dipendent on hardware and functions that can be used also.
*/

/* Local Includes*/
#include "GPS.h"
// #include <time.h>
#include <cstring> // per strchr
#include <cstdlib> // per strtol
#include <cstdint> // per uint8_t
#include <Arduino.h>
#include <common.h>
// #ifdef NANO
// #define PRINTF(...) Serial.printf(__VA_ARGS__)
// #else

// #endif
#if GPS_DEBUG == 1
    #ifdef DEBUG
        #define PRINTF(...) Serial1.printf(__VA_ARGS__)
    #else
        #define PRINTF(...) Serial.printf(__VA_ARGS__)
    #endif
#else
    #define PRINTF(...)
#endif
    

volatile uint8_t gpsUartBuffer[RX_BUFFER_SIZE];  //!< GPS UART RX buffer
volatile bool gpsStringEnd = false;            //!< Flag for end of string

GpsGGAData_t gpsGGAData;                    //!< GGA data
GpsRMCData_t gpsRMCData;                    //!< RMC data
GpsGSAData_t gpsGSAData;                    //!< GSA data
GpsGLGSVData_t gpsGLGSVData; 
GpsGPGSVData_t gpsGPGSVData;                   //!< GSV data
GpsVTGData_t gpsVTGData;                    //!< VTG data


/*!
    @brief      Validate NMEA checksum
    @details    This function validates the checksum of a NMEA sentence
    @param[in]  sentence: NMEA sentence
    @param[out] nextSentence: Pointer to the next sentence founded, if returns NULL there is no next sentence
    @return     true if the checksum is valid, false otherwise
*/

bool nmeaChecksumValidate(const char* sentence, const char** nextSentence){
    char* str;
    char checksum[3];
    uint8_t checksumCalculated = 0;
    //Search for '*'
    str = strchr(sentence, '*');

    //If found, validate checksum else return false and set nextSentence to NULL
    if(str != NULL){
        *nextSentence = strchr(str, '$');
        //Get checksum
        checksum[0] = *(str + 1);
        checksum[1] = *(str + 2);
        checksum[2] = '\0';
        //Calculate checksum
        for(int i = 1; i < str - sentence; ++i){
            checksumCalculated ^= sentence[i];
        }
        //Compare checksums
        if(checksumCalculated == (uint8_t)strtol(checksum, NULL, 16)){
            return true;
        }else{
            return false;
        }
    }
    *nextSentence = NULL;
    return false;
}

/*!
    @brief    Get time from string
    @details  This function gets the time from a string
    @param    str: String to get the time
    @return   Time in seconds
    @note     The string must be in the format HHMMSS<.SSS> where <.SSS> is optional
*/
time_t getTimeFromString(const char* str){
    char hours[3];
    char minutes[3];
    char seconds[6];
    //memcpy() copia un blocco di memoria da una parte all'altra 
    memcpy(hours,str , 2);
    memcpy(minutes, str + 2, 2);
    memcpy(seconds, str + 4, 2);
    hours[2] = '\0';
    minutes[2] = '\0';
    seconds[4] = '\0';
    //atoi() converte una stringa di caratteri in un valore intero
    return atoi(hours) * 3600 + atoi(minutes) * 60 + atoi(seconds);
}

/*!
    @brief    Get date from string
    @details  This function gets the date from a string
    @param    time: String to get the time
    @param    date: String to get the date
    @return   Date in struct tm format
    @note     The time string must be in the format HHMMSS<.SSS> where <.SSS> is optional
              The date string must be in the format DDMMYY
*/
struct tm getDateFromString(const char* time, const char* date){
    char hours[3];
    char minutes[3];
    char seconds[5];
    memcpy(hours,time , 2);
    memcpy(minutes, time + 2, 2);
    memcpy(seconds, time + 4, 2);
    hours[2] = '\0';
    minutes[2] = '\0';
    seconds[3] = '\0';
    char day[3];
    char month[3];
    char year[3];
    memcpy(day,date , 2);
    memcpy(month, date + 2, 2);
    memcpy(year, date + 4, 2);
    day[2] = '\0';
    month[2] = '\0';
    year[2] = '\0';
    struct tm tmp;
    tmp.tm_mday = atoi(day);
    tmp.tm_mon = atoi(month) - 1;
    tmp.tm_year = atoi(year) + 100;
    tmp.tm_hour = atoi(hours);
    tmp.tm_min = atoi(minutes);
    tmp.tm_sec = atoi(seconds);
    return tmp;
}

/*!
    @brief    Get latitude from string
    @details  This function gets the latitude from a string
    @param    str: String to get the latitude
    @return   Latitude in decimal degrees
    @note     The string must be in the format DDMM.MMMM
*/
float getLatitudeFromString(const char* str){
    char degrees[3];
    char minutes[9];
    memcpy(degrees, str, 2);
    memcpy(minutes, str + 2, 8);
    degrees[2] = '\0';
    minutes[7] = '\0';
    return atof(degrees) + atof(minutes) / 60;
}

/*!
    @brief    Get longitude from string
    @details  This function gets the longitude from a string
    @param    str: String to get the longitude
    @return   Longitude in decimal degrees
    @note     The string must be in the format DDDMM.MMMM
*/
float getLongitudeFromString(const char* str){
    char degrees[4];
    char minutes[9];
    memcpy(degrees, str, 3);
    memcpy(minutes, str + 3, 8);
    degrees[3] = '\0';
    minutes[7] = '\0';
    return atof(degrees) + atof(minutes) / 60;
}

/*!
    @brief      Split string inplace
    @details    This function splits a string in place using a delimiter,
                the string is modified replacing the first delimiter with '\0' and the next
                string is returned using the next pointer
    @param[in]  str: String to split
    @param[in]  delim: Delimiter
    @param[out] next: Pointer to the next string.
                 - if NULL is recieved back there is no next string,
                 - f NULL is passed the next pointer will be discarded.
    @return     Pointer to the first characharacter of the substring
*/

/*!
    @brief    Parse GPS data
    @details  This function parses the GPS data
    @param    packet: GPS NMEA data
*/

String nextField;

void gpsParseData(const char* packet){
    if(packet == NULL) return;
    String str;
    String sentenceType;
    int se=1;
    // Parse data
    // Search for '$'
    str = "$" + splitString(String(packet), '$', 1); 
    const char* nextSentence = strchr(packet, '$');
    //const char* nextSentence = str.c_str(); 
    String fields[20];
    // PRINTF("%p,%p\n", packet, str);
    // PRINTF("%s,%s\n", packet, str);
    //If found, validate checksum
    if(str != NULL){
        while(nextSentence != NULL){
            //PRINTF("nextSentence: %p\n", nextSentence);
            bool valid = nmeaChecksumValidate(nextSentence, &nextSentence);
            //PRINTF("nextSentence after: %p\n", nextSentence);
            //PRINTF("%s\n",  str);
            PRINTF("\nValid: %d\n", valid);
            //PRINTF("\n\nSENTENCE \n\n",se);
            if(valid){
                //PRINTF("str: %s\n", str);
                str = splitString(str, '*', 0);
                //PRINTF("str after: %s\n", str);
                //delay(3000);
                
                sentenceType = splitString(str, ',', 0);

                //Get fields
                for(int i = 0; i < 20; ++i){
                    fields[i] = splitString(str, ',', i+1);
                }
                // for (int i = 0; i < 20; ++i){
                //     PRINTF("%s\n", fields[i].c_str());
                // }
                // delay(2000);
                // int cmpResult = strcmp(sentenceType, RMC_SENTENCE);
                if(sentenceType == GGA_SENTENCE){           //STAMPA MALE TUTTO
                    //Parse GGA data
                    gpsGGAData.time = getTimeFromString(fields[0].c_str());
                    float latitude = getLatitudeFromString(fields[1].c_str());
                    float longitude = getLongitudeFromString(fields[3].c_str());
                    if(fields[2].charAt(0) == 'S'){
                        latitude *= -1;
                    }
                    if(fields[4].charAt(0) == 'W'){
                        longitude *= -1;
                    }
                    snprintf(gpsGGAData.latitude, 12, "%f", latitude);
                    snprintf(gpsGGAData.longitude, 12, "%f", longitude);
                    //Fix
                    gpsGGAData.fix = (GGAFixData_t)fields[5].toInt();
                    //Satellites
                    strcpy(gpsGGAData.sats, fields[6].c_str());
                    //HDOP
                    strcpy(gpsGGAData.hdop, fields[7].c_str());
                    //Altitude
                    strcpy(gpsGGAData.altitude, fields[8].c_str());
                    //Altitude WSG84
                    strcpy(gpsGGAData.altitude_WSG84, fields[10].c_str());
                    PRINTF("\nSENTENCE GGA\n");
                    PRINTF("%lld\t (%s,%s) \tFix:%d \tsats:%s \thdop:%s \talt:%s \taltGeo:%s\n\n", gpsGGAData.time,
                                                                                            gpsGGAData.latitude,
                                                                                            gpsGGAData.longitude,
                                                                                            gpsGGAData.fix,
                                                                                            gpsGGAData.sats,
                                                                                            gpsGGAData.hdop,
                                                                                             gpsGGAData.altitude,
                                                                                             gpsGGAData.altitude_WSG84);
                }else if(sentenceType == RMC_SENTENCE){     //STAMPA MALE I SECONDI
                    //Parse RMC data
                    float latitude = getLatitudeFromString(fields[2].c_str());
                    float longitude = getLongitudeFromString(fields[4].c_str());
                    if(fields[3].charAt(0) == 'S'){
                        latitude *= -1;
                    }
                    if(fields[5].charAt(0) == 'W'){
                        longitude *= -1;
                    }
                    snprintf(gpsRMCData.latitude, 12, "%f", latitude);
                    snprintf(gpsRMCData.longitude, 12, "%f", longitude);
                    //Valid
                    gpsRMCData.valid = fields[1][0] == 'A';
                    //Speed
                    strcpy(gpsRMCData.speed, fields[6].c_str());
                    //Course
                    strcpy(gpsRMCData.course, fields[7].c_str());
                    //Date
                    gpsRMCData.timeInfo = getDateFromString(fields[0].c_str(), fields[8].c_str());
                    //Others
                    strcpy(gpsRMCData.others, fields[9].c_str());
                    PRINTF("\nSENTENCE RMC\n");
                    PRINTF("(%s,\t%s) \tValid:%d \tspeed:%s \tcourse:%s \tdate:%d/%d/%d %d:%d:%d \tothers:%s\n\n",   gpsRMCData.latitude,
                                                                                                            gpsRMCData.longitude,
                                                                                                            gpsRMCData.valid,
                                                                                                            gpsRMCData.speed,
                                                                                                            gpsRMCData.course,
                                                                                                            gpsRMCData.timeInfo.tm_mday,
                                                                                                            gpsRMCData.timeInfo.tm_mon+1,
                                                                                                            gpsRMCData.timeInfo.tm_year+1900,
                                                                                                            gpsRMCData.timeInfo.tm_hour,
                                                                                                            gpsRMCData.timeInfo.tm_min,
                                                                                                            gpsRMCData.timeInfo.tm_sec,
                                                                                                            gpsRMCData.others);
                }else if(sentenceType == GSA_SENTENCE){
                    //Mode
                    strcpy(gpsGSAData.mode, fields[0].c_str());
                    //Fix
                    strcpy(gpsGSAData.fix, fields[1].c_str());
                    //Satellites
                    int i;
                    for(i = 0; i < 12; ++i){
                        if(fields[2 + i].toInt() == '\0'){
                            gpsGSAData.sats[i] = -1;
                        }else{
                            gpsGSAData.sats[i] = fields[2 + i].toInt();
                        }
                    }
                    //PDOP
                    i+=2;
                    strcpy(gpsGSAData.pdop, fields[i].c_str());
                    //HDOP
                    strcpy(gpsGSAData.hdop, fields[i+1].c_str());
                    //VDOP
                    strcpy(gpsGSAData.vdop, fields[i+2].c_str());
                    PRINTF("\nSENTENCE GSA\n");
                    PRINTF("Mode:%s \tFix:%s \tPDOP:%s \tHDOP:%s \tVDOP:%s\n",  gpsGSAData.mode,
                                                                                    gpsGSAData.fix,
                                                                                    gpsGSAData.pdop,
                                                                                    gpsGSAData.hdop,
                                                                                    gpsGSAData.vdop);
                    PRINTF("Sats: \n");
                    for(int i = 0; gpsGSAData.sats[i] != -1 && i < 12; ++i){
                        PRINTF("\t%d ", gpsGSAData.sats[i]);
                    }
                    PRINTF("\n\n");
                }else if(sentenceType == GLGSV_SENTENCE){   //FUNZIONA MA STAMPA MALE AZIMUTH
                    //Satellites in view
                    uint8_t satCount = (uint8_t)fields[2].toInt();
                    uint8_t mgsIndex = (uint8_t)fields[1].toInt();
                    for(uint8_t i = (mgsIndex-1)*4, f = 3; i < (mgsIndex-1)*4+4 && i < satCount; ++i, f+=4){
                        
                        //Satellite ID
                            //if(fields[f].charAt(0) == 0) break;
                        strcpy(gpsGLGSVData.sats[i].id, fields[f].c_str());

                        //Elevation
                            //if(fields[f + 1].charAt(0) == 0) break;
                        strcpy(gpsGLGSVData.sats[i].elevation, fields[f+1].c_str());

                        //Azimuth
                            //if(fields[f + 2].charAt(0) == 0) break;At(0) == 0) break;s
                        strcpy(gpsGLGSVData.sats[i].azimuth, fields[f+2].c_str());
                        
                        //SNR
                            //if(fields[f + 3].charAt(0) == 0) break;
                        strcpy(gpsGLGSVData.sats[i].snr, fields[f+3].c_str());
                    }
                    PRINTF("\nSENTENCE GLGVS\n");
                    if(mgsIndex == 1){
                        PRINTF("Satellites in view: %s\n", /* gpsGSVData.satsInView*/ fields[2].c_str());
                    }
                    //PRINTF("Msg ID: %d\n", mgsIndex);
                    for(uint8_t i = (mgsIndex-1)*4; i < (mgsIndex-1)*4+4; ++i){
                        PRINTF("\tSatellite ID: %s\n", gpsGLGSVData.sats[i].id);
                        PRINTF("\t\tElevation: %s\n", gpsGLGSVData.sats[i].elevation);
                        PRINTF("\t\tAzimuth: %s\n", gpsGLGSVData.sats[i].azimuth);
                        PRINTF("\t\tSNR: %s\n", gpsGLGSVData.sats[i].snr);
                    }
                }else if(sentenceType == GPGSV_SENTENCE){   //FUNZIONA MA STAMPA MALE AZIMUTH
                    //Satellites in view
                    uint8_t satCount = (uint8_t)fields[2].toInt();
                    uint8_t mgsIndex = (uint8_t)fields[1].toInt();
                    for(uint8_t i = (mgsIndex-1)*4, f = 3; i < (mgsIndex-1)*4+4 && i < satCount; ++i, f+=4){
                        
                        //Satellite ID
                            //if(fields[f].charAt(0) == 0) break;
                        strcpy(gpsGPGSVData.sats[i].id, fields[f].c_str());

                        //Elevation
                            //if(fields[f + 1].charAt(0) == 0) break;
                        strcpy(gpsGPGSVData.sats[i].elevation, fields[f+1].c_str());

                        //Azimuth
                            //if(fields[f + 2].charAt(0) == 0) break;At(0) == 0) break;s
                        strcpy(gpsGPGSVData.sats[i].azimuth, fields[f+2].c_str());
                        
                        //SNR
                            //if(fields[f + 3].charAt(0) == 0) break;
                        strcpy(gpsGPGSVData.sats[i].snr, fields[f+3].c_str());
                    }
                    PRINTF("\nSENTENCE GPGVS\n");                    
                    if(mgsIndex == 1){
                        PRINTF("Satellites in view: %s\n", /* gpsGSVData.satsInView*/ fields[2].c_str());
                    }
                    //PRINTF("Msg ID: %d\n", mgsIndex);
                    for(uint8_t i = (mgsIndex-1)*4; i < (mgsIndex-1)*4+4; ++i){
                        PRINTF("\tSatellite ID: %s\n", gpsGPGSVData.sats[i].id);
                        PRINTF("\t\tElevation: %s\n", gpsGPGSVData.sats[i].elevation);
                        PRINTF("\t\tAzimuth: %s\n", gpsGPGSVData.sats[i].azimuth);
                        PRINTF("\t\tSNR: %s\n", gpsGPGSVData.sats[i].snr);
                    }
                }else if(sentenceType == GLL_SENTENCE){     //VUOTA
                    PRINTF("\nSENTENCE GLL\n");
                }else if(sentenceType == VTG_SENTENCE){
                    //Course
                    strcpy(gpsVTGData.course, fields[0].c_str());
                    //Reference
                    strcpy(gpsVTGData.courseM, fields[2].c_str());
                    //Speed in knots
                    strcpy(gpsVTGData.speedK, fields[4].c_str());
                    //Speed in km/h
                    strcpy(gpsVTGData.speed, fields[6].c_str());
                    PRINTF("\nSENTENCE VGT\n");
                    PRINTF("Course:%s \tReference:%s \tSpeed in knots:%s \tSpeed in km/h:%s\n\n", gpsVTGData.course,
                                                                                                gpsVTGData.courseM,
                                                                                                gpsVTGData.speedK,
                                                                                                gpsVTGData.speed);
                    }
            }
            str = nextSentence;
            se++;
        }
    }
}

void getGpsData(int* sats, float* gpsSpeed, float* gpsAltitude, float* gpsHdop){
    *sats = atoi(gpsGGAData.sats);
    *gpsAltitude = atof(gpsGGAData.altitude);

    *gpsSpeed = atof(gpsVTGData.speed);
    *gpsHdop = atof(gpsGGAData.hdop);
}

void getGpsFixData(uint8_t* fix, uint8_t* sats, float* hdop){
    *fix  = atoi(gpsGSAData.fix);
    *hdop = atof(gpsGSAData.hdop);
    *sats = atoi(gpsGGAData.sats);
}

GpsGGAData_t* getGGAData(void){
    return &gpsGGAData;
}

GpsRMCData_t* getRMCData(void){
    return &gpsRMCData;
}

GpsGSAData_t* getGSAData(void){
     return &gpsGSAData;
}
GpsGLGSVData_t* getGLGSVData(void){
    return &gpsGLGSVData;
}

GpsGPGSVData_t* getGPGSVData(void){
    return &gpsGPGSVData;
}

GpsVTGData_t* getVTGData(void){
    return &gpsVTGData;
}

//Modifica da verificare
void popWaypoint_t(WayPoint_t& waypoint){
    waypoint.timeInfo = getRMCData()->timeInfo;
    waypoint.latitude = atof(getRMCData()->latitude);
    waypoint.longitude = atof(getRMCData()->longitude);
    waypoint.altitude = atof(getRMCData()->altitude);
    waypoint.sats = atoi(getGGAData()->sats);
    waypoint.fix = getGGAData()->fix;
    waypoint.fixType = atoi(getGSAData()->fix);
    waypoint.hdop = atof(getGGAData()->hdop); 
}

void saveWayPoint(WayPoint_t& lastWaypoint){
    popWaypoint_t(lastWaypoint);
}




/*! @} */ // GPS_Module


