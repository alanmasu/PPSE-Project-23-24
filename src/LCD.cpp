#include <Arduino.h>
#include <Wire.h>
#include <PSEBoard.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <LCD.h>

//Timer non bloccanti
uint64_t timerBussola = 0;

//Costanti per il cerchio fisso della pagina FIND
const uint8_t x_f = 64;
const uint8_t y_f = 32;
const uint8_t ragg = 8;


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool lcdInit(uint8_t address){
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        return false;
    }
    // Clear the buffer
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(40,30);             
    display.println("Hello PSE!");
    display.display();
    return true;
} 

double getrad(double angle){
    return (angle*3.14)/180.0;
}

int cerchiX(int ragg,double anglerad){
    int x = 64 + ragg * cos(anglerad);
    return x;
}

int cerchiY(int ragg,double anglerad){
    int y = 32 + ragg * sin(anglerad);
    return y;
}

//funzione per nscheda
int getNScheda();

void generarePagINTRO(){
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
}

//Pagine a scorrimento su asse y
void generarePagCAL1(){
    display.clearDisplay();
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
    display.display();
}


void generarePagCAL2(){
    display.clearDisplay();
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

    
   
    uint16_t tb = millis() - timerBussola;
    // if(tb < 1000) {
    //     display.drawBitmap(45,18,bussola,35,35,WHITE);
    // }else if(1000 < tb && tb < 2000) {
    //     display.drawBitmap(45,18,bussolaCanc,34,34,WHITE);
    // }else if(2000 < tb){
    //     timerBussola = millis();
    // }

    display.setCursor(0,57); 
    display.setTextSize(1);
    display.println("Calibration...");
    
}

void disegnaBussola(){
    display.drawBitmap(45,18,bussola,35,35,WHITE);
    display.display();
}

void cancBussola(){
    display.drawBitmap(45,18,bussolaCanc,34,34,WHITE);
    display.display();
}

//Pagine a scorrimento su asse y
void generarePagSTART(){
    display.clearDisplay();

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
    display.display();

}
void generarePagREADY(){
    display.clearDisplay();
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
    display.display();
}
void generarePagFIND(double angle){
    uint8_t x=0;
    uint8_t y=0;
    double anglerad = 0;
    display.clearDisplay();
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
    //Cerchio base
    display.drawCircle(x_f, y_f, 12, WHITE);
    
    //Cerchio che deve muoversi
    display.setTextSize(1);
    display.setCursor(0,55);
    display.println("UP : exit find page");
        
    anglerad=getrad(angle);
    x = cerchiX(ragg,anglerad);
    y = cerchiY(ragg,anglerad);

    display.fillCircle(x,y,3,WHITE);
    display.display();
}

void generarePagTIME(){
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
}

void generarePagWIFI(){
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
}

void generarePagTEMP(float temp, float cpuTemp){
    String tempString = String(temp);
    String CPUTempString = String(cpuTemp);

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
}

void generarePagGPS(){
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
}