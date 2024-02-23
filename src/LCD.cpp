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
    int x = round(64 + ragg * cos(anglerad));
    return x;
}

int cerchiY(int ragg,double anglerad){
    int y = round(32 + ragg * sin(anglerad));
    return y;
}


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

    display.drawBitmap(45,18,bussola,34,34,WHITE);
    
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
    display.setCursor(0,57); 
    display.setTextSize(1);
    display.println("Calibration...");
    
}

void disegnaBussola(){
    display.drawBitmap(45,18,bussola,34,34,WHITE);
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
    display.setCursor(35,0);   
    display.setTextSize(2);
    display.println("READY");
    
    //CORPO
    display.setTextSize(1);
    display.println("UP : delete WayPoint");

    display.drawBitmap(0,24,mont,128,28,WHITE);

    display.setCursor(0,55);
    display.println("DOWN : find WayPoint");
    display.display();
}
void generarePagFIND(){
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
        
    
    display.display();
}

void discCerchio(double angle){
    uint8_t x=0;
    uint8_t y=0;
    double anglerad = 0;
    anglerad=getrad(angle);
    x = cerchiX(ragg,anglerad);
    y = cerchiY(ragg,anglerad);

    display.fillCircle(x,y,3,WHITE);
    display.display();
}

void generarePagTIME(struct tm time){
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
    display.printf("%02d/%02d/%04d", time.tm_mday,
                               time.tm_mon+1,
                               time.tm_year+1900);

    //TEMPO
    display.setCursor(55,47); 
    display.setTextSize(1);
    display.printf("%02d:%02d:%02d", time.tm_hour,
                               time.tm_min,
                               time.tm_sec);


    display.display();
}

void generarePagWIFI(String ssid,String ip,bool ap){
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
    display.print("SSID : ");
    display.println(ssid);
    display.print("IP : ");
    display.println(ip);
    if(ap){
        display.setCursor(23,display.getCursorY());
        display.println("[ACCES POINT]\n");
    } else{
        display.println("\n\n");
    }
    display.print("DOWN : Start AP ...");

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

void generarePagGPS(float lat,float lon ,float alt,int8_t sat,int fix){
    display.clearDisplay();
    //TITOLO
    display.setCursor(46,0);   
    display.setTextSize(2);
    display.println("GPS\n"); 

    //FRECCIA DI SINISTRA
    display.drawTriangle(15,6,28,1,28,11,WHITE);
    display.setCursor(1,0); 
    display.write("6");

    //FRECCIA DI DESTRA
    display.drawTriangle(111,6,97,1,97,11,WHITE);
    display.setCursor(116,0); 
    display.write("8\n");

//CORPO

    display.setTextSize(1);
    display.print("\n");
    display.setCursor(20,display.getCursorY());
    display.print("Lat : ");
    display.printf("%.4f\n", lat);
    display.setCursor(20,display.getCursorY());
    display.print("Lon : ");
    display.printf("%.4f\n", lon);
    display.setCursor(20,display.getCursorY());
    display.print("Alt : ");
    display.printf("%.4f\n", alt);
    display.setCursor(20,display.getCursorY());
    display.print("Sat : ");
    display.println(sat);
    display.setCursor(20,display.getCursorY());
    display.print("Fix : ");
    switch (fix){
        case 0:
        case 1:
            display.println("NO FIX");
            break;
        case 2:
            display.println("2D");
            break;
        case 3:
            display.println("3D");
            break;
    }
    display.println(fix);
    


    display.display();
}

void generarePagINFO(String rp2040comm,String espcomm){
    display.clearDisplay();
                
    display.setTextSize(2);

    //FRECCIA DI SINISTRA
    display.drawTriangle(15,6,28,1,28,11,WHITE);
    display.setCursor(1,0); 
    display.write("7");
    

    //TITOLO
    display.setCursor(40,0);   
    display.println("INFO");          

    //CORPO

    display.drawBitmap(85,25,po,50,30,WHITE);

    display.setTextSize(1);
    display.println("\nRP2040 commit:");
    display.println(rp2040comm);
    display.println("\nESP commit:");
    display.println(espcomm);
    display.display();
}