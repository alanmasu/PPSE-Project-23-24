#include <Arduino.h>
#include <Wire.h>
#include <PSEBoard.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <LCD.h>

#ifndef PRINTF 
  #ifdef DEBUG
      #define PRINTF(...) Serial1.printf(__VA_ARGS__)
  #else
     #define PRINTF(...) Serial.printf(__VA_ARGS__)
  #endif
#endif

typedef struct {
  float cpuTemp;
  float temp;
} SensorData;

SensorData data;

//Disegno triangoli senza numeri con posizioni giuste
    // display.drawTriangle(2,6,15,1,15,11,WHITE);
    // display.drawTriangle(127,6,113,1,113,11,WHITE);

int valBottoneR = 0;
int valBottoneL = 0;
int valBottoneD = 0;
int valBottoneU = 0;

int nscheda = 0;
int nschedaCAL = 0;
int nschedaSTART = 0;

//Timers 
uint64_t t0 = 0;
uint64_t t1 = 0;
uint64_t t2 = 0;  //Timer for UART
uint64_t t3 = 0;  //Timer for LCD
uint64_t t4 = 0;

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
        nscheda++;
        if(nscheda>6){
            nscheda=6;
        }
    }else if(!digitalRead(BTN_LEFT) != valBottoneL && !digitalRead(BTN_LEFT)){
        nscheda--;
        if(nscheda<0){
            nscheda=0;
        }
    }
    
    valBottoneR = !digitalRead(BTN_RIGHT);
    valBottoneL = !digitalRead(BTN_LEFT); 

    Serial.printf("\n%d", nscheda);

    switch (nscheda){
        case 0:
            generarePagINTRO();
            break;
        case 1:
            
            

            //CORPO

                //CAMBIARE PAGINA
                if(!digitalRead(BTN_DOWN) != valBottoneD && !digitalRead(BTN_DOWN)){
                    nschedaCAL++;
                    if(nschedaCAL>1){
                        nschedaCAL=1;
                    }
                }
        
                if(nschedaCAL==1){                    
                    generarePagCAL2();
                }else{
                    generarePagCAL1();
                }
                valBottoneD = !digitalRead(BTN_DOWN); 
            break;
        
        case 2:
            
            
            if(!digitalRead(BTN_DOWN) != valBottoneD && !digitalRead(BTN_DOWN)){
                nschedaSTART++;
                if(nschedaSTART>2){
                    nschedaSTART=2;
                }
            }

            if(!digitalRead(BTN_UP) != valBottoneU && !digitalRead(BTN_UP)){
                nschedaSTART--;
                if(nschedaSTART<0){
                    nschedaSTART=0;
                }
            }

            switch (nschedaSTART){
                case 0:
                    generarePagSTART();
                    break;
                case 1:                   
                    generarePagREADY();
                    break;
                
                case 2:
                    generarePagFIND(90.0);
                    break;
            }
            
            valBottoneD = !digitalRead(BTN_DOWN);
            valBottoneU = !digitalRead(BTN_UP);
            break;
        case 3:
            generarePagTIME();
            break;
        case 4:
            generarePagWIFI();
            break;
        case 5:
            analog = analogRead(TEMP_SENSOR);
            vTempSensor = analog * (3.3 / ADC_MAX_VAL) * 1000;           // V in mV
            data.temp = (vTempSensor - 500) * 0.10;                    // Temp in C (10mV/C)  

            //Get the CPU temperature
            data.cpuTemp = analogReadTemp();
            generarePagTEMP(data.temp, data.cpuTemp);
            break;
        case 6:
            generarePagGPS();
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
        Serial.println(valBottoneR);
        Serial1.write((byte*)&data, sizeof(data));
        t2 = millis();
    }

    if(Serial1.available()) {
        Serial.write(Serial1.read());
    }
}
