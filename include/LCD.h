#ifndef __LDC_H__
#define __LDC_H__

#include <Arduino.h>
#include <Wire.h>
#include <PSEBoard.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


//BITMAP BUSSOLA
const unsigned char bussola [] PROGMEM = {
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

//BITMAP CANCELLA BUSSOLA
const unsigned char bussolaCanc [] PROGMEM = {
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

//BITMAP TERMOMETRO
const unsigned char term [] PROGMEM = {
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

const unsigned char po [] PROGMEM = {
	// 'po, 50x30px
	0x00, 0x00, 0xa5, 0xe9, 0x40, 0x00, 0x00, 0x00, 0x00, 0xb5, 0xe9, 0x40, 0x00, 0x00, 0x00, 0x00, 
	0xb5, 0xe9, 0x40, 0x00, 0x00, 0x00, 0x00, 0xb5, 0xe9, 0x40, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 
	0xe0, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x3c, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x0f, 0x1f, 0xff, 0x3c, 0x00, 0x00, 0x00, 
	0x07, 0x3f, 0xff, 0x3c, 0x00, 0x00, 0x00, 0x01, 0x30, 0x01, 0x20, 0x00, 0x00, 0x00, 0x0f, 0x30, 
	0x01, 0x3c, 0x00, 0x00, 0x00, 0x01, 0x30, 0x01, 0x20, 0x00, 0x00, 0x00, 0x01, 0x30, 0x01, 0x20, 
	0x00, 0x00, 0x00, 0x0f, 0x30, 0x01, 0x3c, 0x00, 0x00, 0x00, 0x01, 0x30, 0x01, 0x20, 0x00, 0x00, 
	0x00, 0x07, 0x30, 0x01, 0x3c, 0x00, 0x00, 0x00, 0x0f, 0x30, 0x01, 0x3c, 0x00, 0x00, 0x00, 0x01, 
	0x30, 0x01, 0x20, 0x00, 0x00, 0x00, 0x0f, 0x30, 0x01, 0x3c, 0x00, 0x00, 0x00, 0x07, 0x30, 0x01, 
	0x38, 0x00, 0x00, 0x00, 0x01, 0x30, 0x03, 0x20, 0x00, 0x00, 0x00, 0x0f, 0x3f, 0xff, 0x3c, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 
	0x0f, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0xf5, 
	0xeb, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xb5, 0xe9, 0x40, 0x00, 0x00, 0x00, 0x00, 0xa5, 0xe9, 0x40, 
	0x00, 0x00
};

const unsigned char mont [] PROGMEM = {
	// 'New Piskel (1), 128x28px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x3f, 0xe8, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x7f, 0xc6, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x01, 
	0x00, 0x00, 0x00, 0xff, 0xe0, 0x00, 0xc0, 0x00, 0x7f, 0x83, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x01, 
	0x00, 0x00, 0x01, 0xff, 0xf0, 0x01, 0xf0, 0x00, 0xdf, 0x81, 0x80, 0x00, 0x3f, 0xfe, 0x00, 0x03, 
	0x00, 0x00, 0x03, 0xff, 0xf0, 0x03, 0xf8, 0x00, 0x8e, 0x00, 0xc0, 0x00, 0x7f, 0xff, 0x00, 0x06, 
	0x00, 0x00, 0x06, 0x7f, 0xf8, 0x03, 0xfc, 0x01, 0x00, 0x00, 0x20, 0x00, 0x5f, 0xff, 0x80, 0x04, 
	0x00, 0x00, 0x0c, 0x7f, 0xe8, 0x05, 0xf7, 0x03, 0x00, 0x00, 0x10, 0x01, 0x8f, 0xdf, 0xc0, 0x18, 
	0x00, 0x00, 0x18, 0x1b, 0xcc, 0x0c, 0xc1, 0x02, 0x00, 0x00, 0x0c, 0x01, 0x00, 0x07, 0x60, 0x10, 
	0x00, 0x00, 0x30, 0x00, 0xc4, 0x08, 0x00, 0x84, 0x00, 0x00, 0x06, 0x06, 0x00, 0x00, 0x30, 0x20, 
	0x00, 0x00, 0x60, 0x00, 0x06, 0x10, 0x00, 0x48, 0x00, 0x00, 0x03, 0x04, 0x00, 0x00, 0x08, 0xc0, 
	0x00, 0x00, 0xc0, 0x00, 0x07, 0x20, 0x00, 0x30, 0x00, 0x00, 0x01, 0x88, 0x00, 0x00, 0x0d, 0x00, 
	0x80, 0x01, 0x80, 0x00, 0x01, 0x20, 0x00, 0x20, 0x00, 0x00, 0x00, 0xc8, 0x00, 0x00, 0x07, 0x00, 
	0x80, 0x03, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x40, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x02, 0x00, 
	0x60, 0x06, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x80, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x01, 0x00, 
	0x30, 0x0c, 0x00, 0x00, 0x00, 0xc0, 0x01, 0x80, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x01, 0x80, 
	0x18, 0x18, 0x00, 0x00, 0x00, 0x40, 0x03, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xc0, 
	0x0c, 0x30, 0x00, 0x00, 0x00, 0x20, 0x06, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x20, 
	0x04, 0x40, 0x00, 0x00, 0x00, 0x10, 0x0c, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x30, 
	0x07, 0x80, 0x00, 0x00, 0x00, 0x18, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x10, 
	0x07, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x08
};

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3C 

#define ADC_BITS 12
#define ADC_MAX_VAL ((1 << ADC_BITS) - 1)

bool lcdInit(uint8_t);



void generarePagINTRO();

//Pagine a scorrimento su asse y
void generarePagCAL1();
void generarePagCAL2();
void disegnaBussola();
void cancBussola();

//Pagine a scorrimento su asse y
void generarePagSTART();
void generarePagREADY();
void generarePagFIND();
void discCerchio(double angle);

void generarePagTIME(struct tm);

void generarePagWIFI(String ssid, String ip, bool ap);

void generarePagTEMP(float temp, float cpuTemp);

void generarePagGPS(float lat, float lon, float alt, int8_t sats, int fixType);

void generarePagINFO(String commit, String ESPCommit);

#endif