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

int nscheda = 0;

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

                //TITOLO
                display.setCursor(40,0);  
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


                display.display();
                break;
            case 2:
                display.clearDisplay();
                display.setCursor(37,0);   
                display.setTextSize(1);
                display.setTextSize(2);
                
                //FRECCIA DI SINISTRA
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
                display.println("How to play:\n");
                display.println("RIGHT : Swipe right");

                    
                display.println("LEFT : Swipe left");
                display.println("UP : Delete Waypoint");
                    
                display.println("DOWN : Save Waypoint");
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
                    display.drawRect(1,25,25,35,WHITE);
                    display.drawRect(3,29,4,29,WHITE);
                    display.drawRect(4,35,2,23,WHITE);
                    display.setTextSize(1);
                    
                    display.setCursor(13,29);
                    display.print("20");  
                    display.drawLine(9,37,25,37,WHITE);
                    
                    display.setCursor(13,39);
                    display.print("10");  
                    display.drawLine(9,47,25,47,WHITE);
                    
                    display.setCursor(19,49);
                    display.print("0");  
                    display.drawLine(9,57,25,57,WHITE);

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
                    display.setCursor(63,50);;
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
