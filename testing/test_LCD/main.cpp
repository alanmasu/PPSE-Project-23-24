#include <Arduino.h>
#include <Wire.h>
#include <PSEBoard.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#ifndef PRINTF 
  #ifdef DEBUG
      #define PRINTF(...) Serial1.printf(__VA_ARGS__)
  #else
     #define PRINTF(...) Serial.printf(__VA_ARGS__)
  #endif
#endif

//BUSSOLA
const unsigned char bussola [] PROGMEM = {
	// 'bussola, 34x34px
	0x00, 0x07, 0xf8, 0x00, 0x00, 0x00, 0x3e, 0x1f, 0x00, 0x00, 0x00, 0xe0, 0x01, 0xc0, 0x00, 0x01, 
	0x80, 0xc0, 0x60, 0x00, 0x07, 0x00, 0xc0, 0x38, 0x00, 0x0c, 0x00, 0xc0, 0x0c, 0x00, 0x08, 0x00, 
	0xc0, 0x04, 0x00, 0x18, 0x00, 0xc0, 0x06, 0x00, 0x30, 0x00, 0x00, 0x43, 0x00, 0x20, 0x00, 0x01, 
	0x81, 0x00, 0x60, 0x00, 0x07, 0x81, 0x80, 0x40, 0x00, 0x1f, 0x00, 0x80, 0x40, 0x00, 0x7f, 0x00, 
	0x80, 0xc0, 0x00, 0xfe, 0x00, 0xc0, 0xc0, 0x01, 0xfe, 0x00, 0xc0, 0x80, 0x03, 0xfc, 0x00, 0x40, 
	0x9f, 0x06, 0xfc, 0x3e, 0x40, 0x9f, 0x0c, 0x78, 0x3e, 0x40, 0x80, 0x0c, 0x30, 0x00, 0x40, 0xc0, 
	0x18, 0x60, 0x00, 0xc0, 0xc0, 0x10, 0xc0, 0x00, 0xc0, 0x40, 0x33, 0x80, 0x00, 0x80, 0x40, 0x2e, 
	0x00, 0x00, 0x80, 0x60, 0x78, 0x00, 0x01, 0x80, 0x20, 0x60, 0x00, 0x01, 0x00, 0x30, 0x80, 0x00, 
	0x03, 0x00, 0x18, 0x00, 0xc0, 0x06, 0x00, 0x08, 0x00, 0xc0, 0x04, 0x00, 0x0c, 0x00, 0xc0, 0x0c, 
	0x00, 0x07, 0x00, 0xc0, 0x38, 0x00, 0x01, 0x80, 0xc0, 0x60, 0x00, 0x00, 0xe0, 0x01, 0xc0, 0x00, 
	0x00, 0x3e, 0x1f, 0x00, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00
};
const unsigned char bussolaCanc [] PROGMEM = {
	// 'bussola, 34x34px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// 'arrow', 40x30px
unsigned char arrow [] PROGMEM = {
	0xc0, 0x00, 0x00, 0x0c, 0xf0, 0x00, 0x00, 0x3c, 0x7c, 0x00, 0x00, 0xf8, 0x3f, 0x00, 0x03, 0xf0, 
	0x33, 0x80, 0x07, 0x30, 0x18, 0xe0, 0x1c, 0x60, 0x18, 0x38, 0x78, 0x60, 0x0c, 0x1f, 0xe0, 0xc0, 
	0x06, 0x07, 0x81, 0x80, 0x06, 0x00, 0x01, 0x80, 0x03, 0x00, 0x03, 0x00, 0x03, 0x80, 0x03, 0x00, 
	0x01, 0x80, 0x06, 0x00, 0x00, 0xc0, 0x04, 0x00, 0x00, 0xc0, 0x0c, 0x00, 0x00, 0x60, 0x18, 0x00, 
	0x00, 0x60, 0x18, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x38, 0x70, 0x00, 0x00, 0x18, 0x60, 0x00, 
	0x00, 0x0c, 0xc0, 0x00, 0x00, 0x0c, 0xc0, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 
	0x00, 0x03, 0x00, 0x00
};

const unsigned char term [] PROGMEM = {
	// 'term, 30x35px
	0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x80, 0x00, 0x00, 0x08, 0x40, 0x00, 0x00, 0x09, 0xc0, 0x00, 
	0x00, 0x08, 0x40, 0x00, 0x00, 0x08, 0x40, 0x00, 0x00, 0x09, 0xc0, 0x00, 0x00, 0x08, 0x40, 0x00, 
	0x00, 0x08, 0xc0, 0x00, 0x00, 0x09, 0xc0, 0x00, 0x00, 0x08, 0x40, 0x00, 0x00, 0x09, 0xc0, 0x00, 
	0x00, 0x08, 0x40, 0x00, 0x00, 0x08, 0x40, 0x00, 0x00, 0x08, 0x40, 0x00, 0x00, 0x0b, 0x40, 0x00, 
	0x00, 0x0b, 0x40, 0x00, 0x00, 0x0b, 0x40, 0x00, 0x00, 0x0b, 0x40, 0x00, 0x00, 0x0b, 0x40, 0x00, 
	0x00, 0x0b, 0x40, 0x00, 0x00, 0x0b, 0x40, 0x00, 0x00, 0x0b, 0x40, 0x00, 0x00, 0x0b, 0x40, 0x00, 
	0x00, 0x0b, 0x40, 0x00, 0x00, 0x0b, 0x40, 0x00, 0x00, 0x0b, 0x40, 0x00, 0x00, 0x1b, 0x60, 0x00, 
	0x00, 0x17, 0xa0, 0x00, 0x00, 0x17, 0xa0, 0x00, 0x00, 0x17, 0xa0, 0x00, 0x00, 0x17, 0xa0, 0x00, 
	0x00, 0x13, 0x20, 0x00, 0x00, 0x0c, 0xc0, 0x00, 0x00, 0x07, 0x80, 0x00
};

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3C 

#define ADC_BITS 12
#define ADC_MAX_VAL ((1 << ADC_BITS) - 1)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Timers 
uint64_t t0 = 0;
uint64_t t1 = 0;
uint64_t t2 = 0;  //Timer for UART
uint64_t t3 = 0;  //Timer for LCD


typedef struct {
  float cpuTemp;
  float temp;
} SensorData;

SensorData data;

int valBottoneR = 0;
int valBottoneL = 0;
int valBottoneD = 0;
int valBottoneU = 0;

int nscheda = 0;
int nschedaCAL = 0;
int nschedaSTART = 0;

//Disegno triangoli senza numeri con posizioni giuste
    // display.drawTriangle(2,6,15,1,15,11,WHITE);
    // display.drawTriangle(127,6,113,1,113,11,WHITE);

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

    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
    }
    // Clear the buffer
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(40,30);             
    display.println("Hello PSE!");
    display.display();

    //Initialize the ESP01 pins
    pinMode(ESP_EN, OUTPUT);
    digitalWrite(ESP_EN, HIGH);

    //Initialize ESP01 Serial
    Serial1.setRX(ESP_TX);
    Serial1.setTX(ESP_RX);
    Serial1.begin(115200);

}



void loop() {

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
                display.clearDisplay();
                display.setTextSize(2);
            
                //FRECCIA DI DESTRA
                display.drawTriangle(111,6,97,1,97,11,WHITE);
                display.setCursor(116,0); 
                display.write("2\n");

                //TITOLO
                display.setCursor(35,0);   
                display.println("INTRO");          

                //CORPO
                display.setTextSize(1);
                display.println("Welcome to our game.");
                display.println("Have fun monitoring ");
                display.println("the data and remember to save a WayPoint");
                display.println("along the way");
                display.println("Press RIGHT...");
                display.display();
                break;
            
            case 1:
                
                display.clearDisplay();

                //CORPO

                    //CAMBIARE PAGINA
                    if(!digitalRead(BTN_DOWN) != valBottoneD && !digitalRead(BTN_DOWN)){
                        nschedaCAL++;
                        if(nschedaCAL>1){
                            nschedaCAL=1;
                        }
                    }
            
                    if(nschedaCAL==1){                    
                        
                        //TITOLO
                        display.setCursor(48,0);  
                        display.setTextSize(2);
                        display.print("CAL"); 

                        //FRECCIA DI SINISTRA
                        display.drawTriangle(15,6,28,1,28,11,WHITE);
                        display.setCursor(1,0); 
                        display.write("1\n");

                        //FRECCIA DI DESTRA
                        display.drawTriangle(111,6,97,1,97,11,WHITE);
                        display.setCursor(116,0); 
                        display.write("3\n");

                        //CORPO

                        uint16_t tb = millis() - t3;
                        if(tb < 1000) {
                            display.drawBitmap(45,18,bussola,35,35,WHITE);
                        }else if(1000 < tb && tb < 2000) {
                           display.drawBitmap(45,18,bussolaCanc,34,34,WHITE);
                        }else if(2000 < tb){
                            t3 = millis();
                        }

                        display.setCursor(0,57); 
                        display.setTextSize(1);
                        display.println("Calibration...");
                       

                            
                    }else{
                        //TITOLO
                        display.setCursor(45,0);  
                        display.setTextSize(2);
                        display.print("CAL"); 

                        //FRECCIA DI SINISTRA
                        display.drawTriangle(15,6,28,1,28,11,WHITE);
                        display.setCursor(1,0); 
                        display.write("1\n");

                        //FRECCIA DI DESTRA
                        display.drawTriangle(111,6,97,1,97,11,WHITE);
                        display.setCursor(116,0); 
                        display.write("3\n");

                        //CORPO

                        display.drawBitmap(45,18,bussola,35,35,WHITE);
                        
                        display.setCursor(0,57); 
                        display.setTextSize(1);
                        display.println("DOWN to calibrate...");
                        

                    }

                    valBottoneD = !digitalRead(BTN_DOWN); 

                display.display();

                break;
            
            case 2:
                
                display.clearDisplay();
                
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

                        //FRECCIA DI SINISTRA
                        display.setTextSize(2);
                        display.drawTriangle(15,6,28,1,28,11,WHITE);
                        display.setCursor(1,0); 
                        display.write("2");
                        
                        //FRECCIA DI DESTRA
                        display.drawTriangle(111,6,97,1,97,11,WHITE);
                        display.setCursor(116,0); 
                        display.write("4\n");

                        //TITOLO
                        display.setCursor(35,0);   
                        display.setTextSize(2);
                        display.println("START");

                        //CORPO

                        display.setTextSize(1);
                        display.println("Once you calibrate");
                        display.println("the device we can");
                        display.println("start to have fun\n");
                        display.println("Press DOWN to save a WayPoint");

                            
                        
                    
                        break;
                    
                    case 1: 

                        //FRECCIA DI SINISTRA
                        display.setTextSize(2);
                        display.drawTriangle(15,6,28,1,28,11,WHITE);
                        display.setCursor(1,0); 
                        display.write("2");
                        
                        //FRECCIA DI DESTRA
                        display.drawTriangle(111,6,97,1,97,11,WHITE);
                        display.setCursor(116,0); 
                        display.write("4\n");

                        //TITOLO
                        display.setCursor(32,0);   
                        display.setTextSize(2);
                        display.println("BHO");
                        
                        //CORPO
                        display.setTextSize(1);
                        display.println("UP : delete WayPoint");

                        display.setCursor(0,55);
                        display.println("DOWN : find WayPoint");
    
                        

                        break;
                    
                    case 2:

                        //FRECCIA DI SINISTRA
                        display.setTextSize(2);
                        display.drawTriangle(15,6,28,1,28,11,WHITE);
                        display.setCursor(1,0); 
                        display.write("2");
                        
                        //FRECCIA DI DESTRA
                        display.drawTriangle(111,6,97,1,97,11,WHITE);
                        display.setCursor(116,0); 
                        display.write("4\n");

                        //TITOLO
                        display.setCursor(40,0);   
                        display.setTextSize(2);
                        display.println("FIND");


                        //CORPO

                        display.drawBitmap(49,22,arrow,30,25,WHITE);
                    

                        display.setTextSize(1);
                        display.setCursor(0,55);
                        display.println("UP : exit find page");
        
                        break;

                }
                
                valBottoneD = !digitalRead(BTN_DOWN);
                valBottoneU = !digitalRead(BTN_UP);
                
                display.display();
                break;

            case 3:
                display.clearDisplay();
            
                //TITOLO
                display.setCursor(40,0);  
                display.setTextSize(2);
                display.print("TIME");

                //FRECCIA DI SINISTRA
                display.drawTriangle(15,6,28,1,28,11,WHITE);
                display.setCursor(1,0); 
                display.write("3");
                
                //FRECCIA DI DESTRA
                display.drawTriangle(111,6,97,1,97,11,WHITE);
                display.setCursor(116,0); 
                display.write("5\n");

                //DISEGNO OROLOGIO
                display.drawCircle(22,40,20,WHITE);
                display.drawLine(22,40,13,36,WHITE);
                display.drawLine(22,42,27,55,WHITE);
                
                display.drawLine(22,21,22,26,WHITE);
                display.drawLine(22,60,22,55,WHITE);
                
                display.drawLine(2,40,7,40,WHITE);
                display.drawLine(42,40,37,40,WHITE);

                //DATA
                display.setCursor(55,27); 
                display.setTextSize(1);
                display.print("20/02/2024");

                //TEMPO
                display.setCursor(55,47); 
                display.setTextSize(1);
                display.print("22:59:20");


                display.display();
                break;
            case 4:
                display.clearDisplay();

                //TITOLO
                display.setCursor(40,0); 
                display.setTextSize(2);
                display.println("WIFI");

                //FRECCIA DI SINISTRA
                display.drawTriangle(15,6,28,1,28,11,WHITE);
                display.setCursor(1,0); 
                display.write("4");
                
                //FRECCIA DI DESTRA
                display.drawTriangle(111,6,97,1,97,11,WHITE);
                display.setCursor(116,0); 
                display.write("6\n");


                //CORPO

                display.setTextSize(1);
                display.println("\nssid : 5948753498729754925795873902");
                display.println("id : 5748.9384.8284.8394");
                display.println("commit : kendjso");

                display.display();
                break;
            case 5:

                if(millis() - t1 > 1000) {

                    int analog = analogRead(TEMP_SENSOR);
                    float vTempSensor = analog * (3.3 / ADC_MAX_VAL) * 1000;           // V in mV
                    data.temp = (vTempSensor - 500) * 0.10;                    // Temp in C (10mV/C)  

                    //Get the CPU temperature
                    data.cpuTemp = analogReadTemp();
            
                    // Serial.printf("Analog read: %d,\tTEMP Voltage: %f,\tTemp: %f,\tCPU T: %f\n",analog, vTempSensor, data.temp, data.cpuTemp);
            

                    //Print the temperature to the OLED display
                    String tempString = String(data.temp);
                    String CPUTempString = String(data.cpuTemp);

                    display.clearDisplay();

                    //TITOLO
                    display.setCursor(40,0);   
                    display.setTextSize(2);
                    display.print("TEMP");
                    display.println("\n");

                    //FRECCIA DI SINISTRA
                    display.drawTriangle(15,6,28,1,28,11,WHITE);
                    display.setCursor(1,0); 
                    display.write("5");
                
                    //FRECCIA DI DESTRA
                    display.drawTriangle(111,6,97,1,97,11,WHITE);
                    display.setCursor(116,0); 
                    display.write("7\n");

                    //DISEGNO TERMOMETRO
                    display.drawBitmap(1,25,term,30,35,WHITE);

                    //DATI

                    display.setTextSize(1);
                    display.setCursor(30,30);;
                    display.println("AMB : ");
                    display.setCursor(63,30);;
                    display.print(tempString);
                    display.setCursor(93,30);;
                    display.println(" C");
                    
                    display.setCursor(30,50);
                    display.print("CPU : ");
                    display.setCursor(63,50);
                    display.print(CPUTempString);
                    display.setCursor(93,50);;
                    display.println(" C\n");
                    display.display();
                    
                    t1 = millis();
                }

                break;
            
            case 6:
                    display.clearDisplay();

                    //TITOLO
                    display.setCursor(46,0);   
                    display.setTextSize(2);
                    display.println("GPS\n"); 

                    //FRECCIA DI SINISTRA
                    display.drawTriangle(15,6,28,1,28,11,WHITE);
                    display.setCursor(1,0); 
                    display.write("6");
                
                //CORPO

                    display.setTextSize(1);
                    display.println("\n");
                    display.println("Lat : 4563.6443");
                    display.println("Lon : 4536.5643");
                    display.println("Alt : 5434.5345");
                    display.println("Sat : 07");
                    display.println("Fix : 3");

                
                    display.display();

                    break;
            default:
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
