#ifndef NICLA_NEOPIXEL_H
#define NICLA_NEOPIXEL_H

// #include <Arduino.h>
#include <mbed.h>


#define CYCLES_800_T0H (125) // 0.3us ( minus 0.15us for IO
#define CYCLES_800_T1H (500) // 0.7us
#define CYCLES_800 (800)        // 1.25us per bit
#define CYCLES_800_RESET (6000) // 60ms reset pulse

#define G_OFFSET 0
#define R_OFFSET 1
#define B_OFFSET 2



class NiclaSenseNeoPixel{


public:
	NiclaSenseNeoPixel(uint16_t LedNum): _pin(digitalPinToPinName(0)) { _numLEDs = LedNum;}
	~NiclaSenseNeoPixel(){ free(_pPixels);}

	int Init();
	void SetBrightness(uint16_t val) { _brightness = val;}

	void SetPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);

	void Show();

private:

	void _write1(uint8_t i);
	void _write0(uint8_t i);

    uint8_t _brightness = 0;			 	///< Strip brightness 0-255 (stored as +1)    
 	uint16_t _numLEDs = 0;   				///< Number of RGB LEDs in strip
	uint8_t * _pPixels;   						 ///< Holds LED color values (3 or 4 bytes each)
	uint16_t _numBytes;  					///< Size of 'pixels' buffer below
	mbed::DigitalOut _pin;
};


#endif