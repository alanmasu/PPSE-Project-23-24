#ifndef __LDC_H__
#define __LDC_H__

#include <Arduino.h>
#include <Wire.h>
#include <PSEBoard.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


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
const unsigned char arrow [] PROGMEM = {
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

bool lcdInit(uint8_t);

//funzione per nscheda
int getNScheda();

void generarePagINTRO();

//Pagine a scorrimento su asse y
void generarePagCAL1();
void generarePagCAL2();

//Pagine a scorrimento su asse y
void generarePagSTART();
void generarePagREADY();
void generarePagFIND(double);

void generarePagTIME();

void generarePagWIFI();

void generarePagTEMP(float, float);

void generarePagGPS();

#endif