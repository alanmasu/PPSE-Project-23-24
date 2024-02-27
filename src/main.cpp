#include <Arduino.h>
#include <PSEBoard.h>
#include <git_revision.h>
#include <LCD.h>
#include <GPS.h>
#include <ESPModule.h>
#include <PSEBoard_WS2812.h>
#include <Bussola.h>

#ifndef PRINTF 
  #ifdef DEBUG
      #define PRINTF(...) Serial1.printf(__VA_ARGS__)
  #else
     #define PRINTF(...) Serial.printf(__VA_ARGS__)
  #endif
#endif

enum Page_t {PAG_INTRO = 0,PAG_CAL, PAG_START,PAG_TIME,PAG_WIFI,PAG_TEMP,PAG_GPS,PAG_INFO};

enum State_t {IDLE=0,CALIBRATING,WAYPOINT,FIND};

ApplicationRecord_t applicationRecord;
WiFiConfiguration_t wifiConfig;

//ESPModule 
ESPModule myESP(115200, Serial1, ESP_RX, ESP_TX, ESP_EN);

//WS2812 Led
PSEBoard_WS2812 led;

//Bussola
Bussola compass;

//Angolo direzione (puntato)
double angle = 0;

int valBottoneR = 0;
int valBottoneL = 0;
int valBottoneD = 0;
int valBottoneU = 0;
bool updatePage = false;
bool blinkBussola = false;
bool angleMov = false;
int countpacket = 0;

State_t actualState = IDLE;
Page_t actualPage = PAG_INTRO;
int nschedaCAL = 0;
int nschedaSTART = 0;

//Timers 
uint64_t t0 = 0;  //Timer for the alive LED
uint64_t t2 = 0;  //Timer for ESP module
uint64_t t3 = 0;  //Timer for GPS
uint64_t t4 = 0;  //Timer for LCD
uint64_t t5 = 0;  //t per bussola
uint64_t t6 = 0;  //t per angolo



void setup() {

    //Serial port for debugging purposes
    Serial.begin(115200);

    //Initialize the alive LED
    pinMode(ALIVE_LED, OUTPUT);
    pinMode(BTN_UP, INPUT);
    pinMode(BTN_RIGHT,INPUT);
    pinMode(BTN_LEFT,INPUT);
    pinMode(BTN_DOWN,INPUT);

    //Inizializzazione dell'autoritenuta
    pinMode(BUCK_EN, OUTPUT);
    digitalWrite(BUCK_EN, HIGH);

    //Initialize the OLED display and Bussola
    Wire.setSDA(I2C0_SDA);
    Wire.setSCL(I2C0_SCL);

    //Inizializzazione display
    if(!lcdInit(SCREEN_ADDRESS)){
        Serial.println(F("SSD1306 allocation failed"));
    }

    //Inizializzazione bussola
    // compass.init();
    // compass.magnetometerRollingRoundig(true);
    // compass.accelerometerRollingRoundig(true);

    //Inizializzazione led WS2812
    // led.init();

    delay(10);

    //Wait for serial port to be ready
    while(!Serial){
        uint16_t dt = millis() - t0;
        if(dt < 100) {
            digitalWrite(ALIVE_LED, HIGH);
        }else if(100 < dt && dt < 200) {
            digitalWrite(ALIVE_LED, LOW);
        }else if(200 < dt){
            t0 = millis();
        }
        if(!digitalRead(BTN_UP)) {
            break;
        }
    } 

    //Initialize the temperature sensor
    analogReadResolution(ADC_BITS);

    delay(2000);

    Serial.printf("Git info: %s %s\n", __GIT_COMMIT__, __GIT_REMOTE_URL__);
    Serial.printf("Built on %s at %s\n", __DATE__, __TIME__);

    //Initialize ESP01 Module
    myESP.begin();
    
    //GPS
    Serial2.setRX(GPS_RX);
    Serial2.setTX(GPS_TX);
    Serial2.begin(9600);
    pinMode(GPS_EN,OUTPUT);
    digitalWrite(GPS_EN,LOW);

    //Initialize the ALIVE LED
    pinMode(ALIVE_LED,OUTPUT);

    //BUZZER
    pinMode(BUZZER,OUTPUT);

}

String infoGPS;

//LCD Error menagement
bool screenShowError = false;
String screenError = "";
uint64_t tError = 0;

void loop() {
    //Variabili temporanee
    uint8_t fix;
    uint8_t sats;
    float hdop;
    float temp_cos, temp_sin, temp_ipo;
    //Variabili ADC
    int analog;
    float vTempSensor;
    //Leggo la temperatura
    analog = analogRead(TEMP_SENSOR);
    vTempSensor = analog * (3.3 / ADC_MAX_VAL) * 1000;           // V in mV
    applicationRecord.temp = (vTempSensor - 500) * 0.10;         // Temp in C (10mV/C)
    //Get the CPU temperature
    applicationRecord.cpuTemp = analogReadTemp();
    
    //GPS reading
    char *sentence;
    //Controllo se ci sono caratteri
    if(Serial2.available()>0){
        // Serial.print(Serial2.readString());
        String sentence = Serial2.readStringUntil('\n');
        if(sentence != "" && countpacket < 12){
            infoGPS+=sentence;
            countpacket++;
        }
    }

    //Lettura dati dal modulo ESP
    myESP.getData(wifiConfig);
    
    //Aggiornamento dati Bussola
    // compass.update();

    //Lettura pulsanti per cambio pagina
    if(!digitalRead(BTN_RIGHT) != valBottoneR && !digitalRead(BTN_RIGHT)){
        if(actualPage!=7){
            actualPage = (Page_t)(actualPage + 1);
        }else{
            actualPage=PAG_INFO;
        }
        updatePage = true;
        Serial.printf("PAG Count:%d", (int)actualPage);
    }else if(!digitalRead(BTN_LEFT) != valBottoneL && !digitalRead(BTN_LEFT)){
        if(actualPage != 0){
            actualPage = (Page_t)(actualPage - 1);
        }else{
            actualPage=PAG_INTRO;
        }
        updatePage = true;
        Serial.printf("PAG Count:%d", (int)actualPage);
    }
    valBottoneR = !digitalRead(BTN_RIGHT);
    valBottoneL = !digitalRead(BTN_LEFT); 

    //MACCHINA A STATI
    //Lettura pulsanti UP e DOWN
    bool btnDWEdge = false;
    bool btnUPEdge = false;
    if(!digitalRead(BTN_DOWN) != valBottoneD && !digitalRead(BTN_DOWN)){
        btnDWEdge = true;
    }else if(!digitalRead(BTN_UP) != valBottoneU && !digitalRead(BTN_UP)){
        btnUPEdge = true;
    }
    valBottoneD = !digitalRead(BTN_DOWN);
    valBottoneU = !digitalRead(BTN_UP);

    //Switch per la macchina a stati
    switch(actualPage){
        case PAG_CAL:
            if(btnDWEdge){
                actualState = CALIBRATING;
                updatePage = true;
                // compass.calibrate_accelerometer();
            }
            // if(compass.getAccelerometerCalibrationStatus() == CALIBRATION_DONE && compass.getMagnetometerCalibrationStatus() == NOT_CALIBRATED) {
            //     compass.calibrate_magnetometer();
            // }
            // led.ledShowCalibration(compass.getCalibrationStatus() == CALIBRATION_DONE, compass.getMagnetometerCalibrationStatus() == CALIBRATION_IN_PROGRESS);
            // if(compass.getCalibrationStatus() == CALIBRATION_DONE) {
            //     actualState = IDLE;
            // }
            break;
        case PAG_START:
            switch(actualState){
                case IDLE:
                    getGpsFixData(&fix, &sats, &hdop);
                    if(btnDWEdge && !applicationRecord.waypointsaved){
                        if(fix && hdop < 5){
                            actualState=WAYPOINT;
                            updatePage = true;
                            saveWayPoint(applicationRecord.firstWayPoint);
                            Serial.print(applicationRecord.firstWayPoint.longitude);
                            applicationRecord.firstWayPoint.temp = applicationRecord.temp;
                            applicationRecord.waypointsaved=true;
                            tone(BUZZER,500,100);
                        }else{
                            screenShowError = true;
                            screenError = "No GPS Fix";
                            tone(BUZZER,200,1000);
                            updatePage = true;
                            tError = millis();
                        }
                    }
                    break;

                case WAYPOINT:
                    if(btnDWEdge){
                        // if(compass.getCalibrationStatus() != CALIBRATION_DONE){
                            // screenShowError = true;
                            // screenError = "Calibrate the compass";
                            // tone(BUZZER,200,1000);
                            // updatePage = true;
                            // tError = millis();
                        // }else{
                            actualState = FIND;
                            updatePage  = true;
                        // }
                    }
                    if(btnUPEdge && applicationRecord.waypointsaved){
                        actualState = IDLE;
                        updatePage = true;
                        applicationRecord.waypointsaved=false;
                        tone(BUZZER,200,500);
                    }
                    break;
                case FIND:
                    if(btnUPEdge){
                        actualState = WAYPOINT;
                        updatePage = true;
                    }
                    break;
            }
           break;
        case PAG_WIFI:
            if(btnDWEdge){
                applicationRecord.gotoAP = true;
                updatePage = true;
            }
            if(wifiConfig.ap && applicationRecord.gotoAP){
                applicationRecord.gotoAP = false;
            }
            break;
    }
    //FINE MACCHINA A STATI


    //ALIVE LED
    uint16_t dt = millis() - t0;
    if(dt < 1000) {
        digitalWrite(ALIVE_LED, HIGH);
    }else if(1000 < dt && dt < 2000) {
        digitalWrite(ALIVE_LED, LOW);
    }else if(2000 < dt){
        t0 = millis();
    }

    //Scrittura dati al modulo ESP
    if(millis() - t2 > 1000) {
        myESP.sendData(&applicationRecord);
        t2 = millis();
        Serial.printf("SSID: %s, IP: %d.%d.%d.%d, AP: %d, ESP Commit: %s\n", wifiConfig.ssid, wifiConfig.ipAddress[0], wifiConfig.ipAddress[1], wifiConfig.ipAddress[2], wifiConfig.ipAddress[3], wifiConfig.ap, wifiConfig.commitHash);
    }

    //Lettura dati dal GPS
    if(millis() - t3 > 1000){
        t3 = millis();
        if(countpacket==12){
            PRINTF("\nParsing...");
            char tmp[1024];
            strncpy(tmp, infoGPS.c_str(), 1024);
            gpsParseData(tmp);
            popWaypoint_t(applicationRecord.actualPoint);  
            infoGPS = "";
            countpacket = 0;
            PRINTF("\nOK...\n");
        }
    }
    String ip;
    // Serial.printf("\n%d", actualPage);
    if(millis() - t4 > 1000 || updatePage){
        updatePage = false;
        t4 = millis();
        //Generazione pagine
        if(screenShowError){
            if(millis() - tError > 5000){
                screenShowError = false;
            }
            generarePagError(screenError);
        }else{
            switch (actualPage){
                case PAG_INTRO:
                    generarePagINTRO();
                    break;
                case PAG_CAL:
                    switch(actualState){
                        case FIND:
                        case WAYPOINT:
                        case IDLE:
                            generarePagCAL1();
                            break;
                        case CALIBRATING:
                            generarePagCAL2();
                            break;
                    }
                    break;
                case PAG_START:
                    switch (actualState){
                        case IDLE:
                        case CALIBRATING:
                            generarePagSTART();
                            break;
                        case WAYPOINT:                   
                            generarePagREADY();
                            break;
                        case FIND:
                            generarePagFIND();
                            discCerchio(-(angle - 90));
                            break;
                    }
                    break;
                case PAG_TIME:
                    generarePagTIME(applicationRecord.actualPoint.timeInfo);                
                    break;
                case PAG_WIFI:
                    ip = String(wifiConfig.ipAddress[0]) + "." + String(wifiConfig.ipAddress[1]) + "." + String(wifiConfig.ipAddress[2]) + "." + String(wifiConfig.ipAddress[3]);
                    generarePagWIFI(wifiConfig.ssid,ip,wifiConfig.ap);
                    break;
                case PAG_TEMP:
                    generarePagTEMP(applicationRecord.temp, applicationRecord.cpuTemp);
                    break;
                case PAG_GPS:
                    generarePagGPS( applicationRecord.actualPoint.latitude,
                                    applicationRecord.actualPoint.longitude,
                                    applicationRecord.actualPoint.altitude,
                                    applicationRecord.actualPoint.sats,
                                    applicationRecord.actualPoint.fixType);
                    break;        
                case PAG_INFO:
                    generarePagINFO(__GIT_COMMIT__,wifiConfig.commitHash);
                    break;
            }
        }
    }

    //Lampeggio bussola
    uint16_t tbus = millis() - t5;
    if(actualPage == PAG_CAL && actualState == CALIBRATING && !updatePage){
        if(tbus > 1000 &&  !blinkBussola) {
            blinkBussola = true;
            cancBussola();
        }else if(2000 < tbus && blinkBussola){
            blinkBussola = false;
            disegnaBussola();
            t5 = millis();
        }
    }

    // Aggiornamento Led
    uint16_t tLed = millis() - t6;
    if(tLed > 1000) {
        t6 = millis();
        switch(actualState) {
            case IDLE:
            case WAYPOINT:
                // getGpsFixData(&fix, &sats, &hdop);
                // led.ledShowFix(fix && hdop < 5);
                break;
            case FIND:
                temp_cos = applicationRecord.firstWayPoint.longitude - applicationRecord.actualPoint.longitude;
                temp_sin = applicationRecord.firstWayPoint.latitude - applicationRecord.actualPoint.latitude;
                temp_ipo = sqrt(sq(temp_cos) + sq(temp_sin));
                if(temp_cos >= 0 && temp_sin >= 0) {
                    angle = asin(abs(temp_cos / temp_ipo)) * 180.0 / PI;
                } else if(temp_cos >= 0 && temp_sin < 0) {
                    angle = 90 + asin(abs(temp_sin / temp_ipo)) * 180.0 / PI;
                } else if(temp_cos < 0 && temp_sin >= 0) {
                    angle = 270 + asin(abs(temp_sin / temp_ipo)) * 180.0 / PI;
                } else if(temp_cos < 0 && temp_sin < 0) {
                    angle = 180 + asin(abs(temp_cos / temp_ipo)) * 180.0 / PI;
                }
                led.ledShowDirection(angle);
                break;
        }
    }
}
