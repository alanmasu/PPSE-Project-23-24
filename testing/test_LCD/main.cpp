#include <Arduino.h>
#include <Wire.h>
#include <PSEBoard.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <LCD.h>
#include <git_revision.h>

#ifndef PRINTF 
  #ifdef DEBUG
      #define PRINTF(...) Serial1.printf(__VA_ARGS__)
  #else
     #define PRINTF(...) Serial.printf(__VA_ARGS__)
  #endif
#endif

enum Page_t {PAG_INTRO = 0,PAG_CAL, PAG_START,PAG_TIME,PAG_WIFI,PAG_TEMP,PAG_GPS};

enum State_t {IDLE=0,CALIBRATING,WAYPOINT,FIND};

typedef struct {
  float cpuTemp;
  float temp;
} SensorData;

SensorData data;

//Dati prova

    double angle = 0;

    String ssid = "seleggiquestovuoldirechestafunzi";
    String ip = "utt.oda.lma.inc";
    bool ap = true;
    String commit = "github/ottimo/ciao/bho//home";

    float lat = 4623.4252;
    float lon = 4636.5325;
    float alt = 4735.6352;
    int8_t sat = 5;
    String fix = "04";

    struct tm timeinfo;


int valBottoneR = 0;
int valBottoneL = 0;
int valBottoneD = 0;
int valBottoneU = 0;
bool updatePage = false;
bool blinkBussola = false;
bool angleMov = false;

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

}


void loop() {
    int analog;
    float vTempSensor;
    if(!digitalRead(BTN_RIGHT) != valBottoneR && !digitalRead(BTN_RIGHT)){
        actualPage = (Page_t)(actualPage + 1);
        updatePage = true;
        if(actualPage>6){
            actualPage=PAG_GPS;
        }
    }else if(!digitalRead(BTN_LEFT) != valBottoneL && !digitalRead(BTN_LEFT)){
        actualPage = (Page_t)(actualPage - 1);
        updatePage = true;
        if(actualPage<0){
            actualPage=PAG_INTRO;
        }
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
                timeinfo.tm_hour = 10;

                generarePagTIME(timeinfo);
                break;
            case PAG_WIFI:
                generarePagWIFI(ssid,ip,ap,__GIT_COMMIT__);
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
                generarePagGPS(lat,lon,alt,sat,fix);
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

                    if(btnDWEdge){
                        actualState=WAYPOINT;
                        updatePage = true;
                    }
                    break;

                case WAYPOINT:
                    
                    if(btnDWEdge){

                        actualState=FIND;
                        updatePage = true;
                    }
                    if(btnUPEdge){
                        actualState = IDLE;
                        updatePage = true;
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
    }
    

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
