#include <Arduino.h>
#include <PSEBoard.h>
#include <git_revision.h>
#include <LCD.h>
#include <GPS.h>

#ifndef PRINTF 
  #ifdef DEBUG
      #define PRINTF(...) Serial1.printf(__VA_ARGS__)
  #else
     #define PRINTF(...) Serial.printf(__VA_ARGS__)
  #endif
#endif

long intervallo = 1000;

const int maxDataLenght = 600;
long lastPrintInstant = 0;
 
enum Page_t {PAG_INTRO = 0,PAG_CAL, PAG_START,PAG_TIME,PAG_WIFI,PAG_TEMP,PAG_GPS,PAG_INFO};

enum State_t {IDLE=0,CALIBRATING,WAYPOINT,FIND};

typedef struct {
  float cpuTemp;
  float temp;
} SensorData;

WayPoint_t actualWaypoint;
WayPoint_t lastWaypoint;
ApplicationRecord_t apprec;

SensorData data;

//Dati prova

    double angle = 0;

    String ssid = "seleggiquestovuoldirechestafunzi";
    String ip = "utt.oda.lma.inc";
    bool ap = true;
    String commit = "github/ottimo/ciao/bho//home";

    struct tm timeinfo;


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
uint64_t t0 = 0;
uint64_t t1 = 0;
uint64_t t2 = 0;  //Timer for UART
uint64_t t3 = 0;  //Timer for LCD
uint64_t t4 = 0;
uint64_t t5 = 0; //t per bussola
uint64_t t6 = 0; //t per angolo



void setup() {

    //Serial port for debugging purposes
    Serial.begin(115200);

    //Initialize the alive LED
    pinMode(ALIVE_LED, OUTPUT);
    pinMode(BTN_UP, INPUT);
    pinMode(BTN_RIGHT,INPUT);
    pinMode(BTN_LEFT,INPUT);
    pinMode(BTN_DOWN,INPUT);

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
    // while(!Serial);


    //Inizializzazione dell'autoritenuta
    pinMode(BUCK_EN, OUTPUT);
    digitalWrite(BUCK_EN, HIGH);

    //Initialize the temperature sensor
    analogReadResolution(ADC_BITS);

    //Initialize the OLED display
    Wire.setSDA(I2C0_SDA);
    Wire.setSCL(I2C0_SCL);

    if(!lcdInit(SCREEN_ADDRESS)){
        Serial.println(F("SSD1306 allocation failed"));
    }
    delay(2000);

    //Initialize the ESP01 pins
    pinMode(ESP_EN, OUTPUT);
    digitalWrite(ESP_EN, HIGH);

    //Initialize ESP01 Serial
    Serial1.setRX(ESP_TX);
    Serial1.setTX(ESP_RX);
    Serial1.begin(115200);
    
    //GPS

    Serial2.setRX(GPS_RX);
    Serial2.setTX(GPS_TX);
    Serial2.begin(9600);
    pinMode(GPS_EN,OUTPUT);
    Serial1.begin(115200) ;
    #ifdef DEBUG
      Serial1.begin(115200) ;
    #else
      Serial.begin(115200) ;
      //while(!Serial);
    #endif
    
    digitalWrite(GPS_EN,LOW);

    pinMode(ALIVE_LED,OUTPUT);

    //BUZZER
    pinMode(BUZZER,OUTPUT);

}

String infoGPS;

void loop() {
    
    //GPS

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

    if(countpacket == 12){
      Serial1.println(infoGPS);
    }
  
    int analog;
    float vTempSensor;
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

    uint16_t tAng = millis() - t6;
    if(actualPage == PAG_START && actualState == FIND){
        if(tAng > 300 &&  !angleMov ) {
            angleMov = true;
            t6 = millis();
        }

    }

    // Serial.printf("\n%d", actualPage);
    if(millis() - t3 > 1000 || updatePage){
        
        if(countpacket==12){

            PRINTF("\nParsing...");
            char tmp[1024];
            strncpy(tmp, infoGPS.c_str(), 1024);
            gpsParseData(tmp);
            popWaypoint_t(actualWaypoint);

            //Modifica da provare
            
            //gpsParseData(infoGPS.c_str());      
            infoGPS = "";
            countpacket = 0;
            
            PRINTF("\nOK...\n");

        }
    
        
          
        updatePage = false;
        t3= millis();
        
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
                        if(angleMov){
                            angle=angle+10;
                        }
                        discCerchio(angle);
                        break;
                }
                break;
            case PAG_TIME:
                

                generarePagTIME(actualWaypoint.timeInfo_1);
                
                break;
            case PAG_WIFI:
                generarePagWIFI(ssid,ip,ap);
                break;
            case PAG_TEMP:
                analog = analogRead(TEMP_SENSOR);
                vTempSensor = analog * (3.3 / ADC_MAX_VAL) * 1000;           // V in mV
                data.temp = (vTempSensor - 500) * 0.10;                    // Temp in C (10mV/C)
                //Get the CPU temperature
                data.cpuTemp = analogReadTemp();
                generarePagTEMP(data.temp, data.cpuTemp);
                break;
            case PAG_GPS:
                
                generarePagGPS(actualWaypoint.latitude_1,actualWaypoint.longitude_1,actualWaypoint.altitude_1,actualWaypoint.sats,actualWaypoint.fix_num_1);
                
                break;        
            case PAG_INFO:
                generarePagINFO(__GIT_COMMIT__,__GIT_COMMIT__);
                break;
        }
    }

    //MACCHINA A STATI

    bool btnDWEdge = false;
    bool btnUPEdge = false;
    if(!digitalRead(BTN_DOWN) != valBottoneD && !digitalRead(BTN_DOWN)){
        btnDWEdge = true;
    }else if(!digitalRead(BTN_UP) != valBottoneU && !digitalRead(BTN_UP)){
        btnUPEdge = true;
    }

    valBottoneD = !digitalRead(BTN_DOWN);
    valBottoneU = !digitalRead(BTN_UP);

    switch(actualPage){
        case PAG_CAL:
            if(btnDWEdge){
                actualState = CALIBRATING;
                updatePage = true;
            }
            // if(calibrationDone()){
            //     actualState = IDLE;
            // }
            break;
        case PAG_START:
            switch(actualState){
                case IDLE:

                    if(btnDWEdge && !apprec.waypointsaved /*&& gpfix && hdop<5*/){
                        actualState=WAYPOINT;
                        updatePage = true;
                        saveWayPoint(lastWaypoint);
                        Serial.print(lastWaypoint.longitude_1);
                        apprec.waypointsaved=true;
                    }
                    break;

                case WAYPOINT:
                    
                    if(btnDWEdge){

                        actualState=FIND;
                        updatePage = true;
                    }
                    if(btnUPEdge && apprec.waypointsaved){
                        actualState = IDLE;
                        updatePage = true;
                        apprec.waypointsaved=false;
                    
                        tone(BUZZER,500,1000);
                    
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
            
            }
            break;
    }
    
    //FINE MACCHINA A STATI

    uint16_t dt = millis() - t0;
    if(dt < 1000) {
        digitalWrite(ALIVE_LED, HIGH);
    }else if(1000 < dt && dt < 2000) {
        digitalWrite(ALIVE_LED, LOW);
    }else if(2000 < dt){
        t0 = millis();
    }
    
    if(millis() - t2 > 1000) {
        // Serial.println(valBottoneR);
        Serial1.write((byte*)&data, sizeof(data));
        t2 = millis();
    }

    if(Serial1.available()) {
        Serial.write(Serial1.read());
    }

}
