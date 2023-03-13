#include <Led/NiclaSenseNeoPixel.h>
#include <Arduino.h>

int NiclaSenseNeoPixel::Init()
{

    free(_pPixels); // Free existing data (if any)

    // Serial.println("FREE");

    // Allocate new data -- note: ALL PIXELS ARE CLEARED
    _numBytes = _numLEDs * 3;
    if ((_pPixels = (uint8_t *)malloc(_numBytes)))
    {
        memset(_pPixels, 0, _numBytes);
    }
    else
    {
        _numBytes = 0;
        return -2;
    }

    pinMode(0, OUTPUT);

    // Serial.println("MALLOC");

    // Serial.println("CLOCK INIT");
    return 0;
}

/*!
  @brief   Set a pixel's color using separate red, green and blue
           components. If using RGBW pixels, white will be set to 0.
  @param   n  Pixel index, starting from 0.
  @param   r  Red brightness, 0 = minimum (off), 255 = maximum.
  @param   g  Green brightness, 0 = minimum (off), 255 = maximum.
  @param   b  Blue brightness, 0 = minimum (off), 255 = maximum.
*/
void NiclaSenseNeoPixel::SetPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{

    if (n < _numLEDs)
    {

        if (_brightness)
        { // See notes in setBrightness()
            r = (r * _brightness) >> 8;
            g = (g * _brightness) >> 8;
            b = (b * _brightness) >> 8;
        }
        uint8_t *p = &_pPixels[n * 3];

        // Serial.println("RGB val r:" + String(g, BIN) + " - g:" + String(r, BIN) + " - b:" + String(b, BIN));

        p[G_OFFSET] = g;
        p[R_OFFSET] = r; // R,G,B always stored
        p[B_OFFSET] = b;
    }
}

void NiclaSenseNeoPixel::Show()
{

    __disable_irq();

    for (uint32_t cbyte = 0; cbyte < _numBytes; cbyte++)
    {

        uint8_t currentByte = _pPixels[cbyte];

        for (int8_t cbit = 7; cbit >= 0; cbit--)
        {

            if (((currentByte >> cbit) & 0x1) == 1)
                _write1();
            else
                _write0();
        }
    }

    __enable_irq();
}

void NiclaSenseNeoPixel::_write1()
{

    _pin.write(1);

    asm("nop; nop; nop; nop; nop; nop; nop; nop;"
        "nop; nop; nop; nop; nop; nop; nop; nop;"
        "nop; nop; nop; nop; nop; nop; nop; nop;"
        "nop; nop; nop; nop; nop; nop; nop; nop;"
        "nop; nop; nop; nop; nop; nop; nop; nop;"
        "nop; nop; nop; nop; nop; nop; nop; nop;");

    _pin.write(0);

    _pin.write(1);
}

void NiclaSenseNeoPixel::_write0()
{

    _pin.write(1);

    _pin.write(0);

    asm("nop; nop; nop; nop; nop; nop; nop; nop;"
        "nop; nop; nop; nop; nop; nop; nop; nop;"
        "nop; nop; nop; nop; nop; nop; nop; nop;"
        "nop; nop; nop; nop; nop; nop; nop; nop;"
        "nop; nop; nop; nop; nop; nop; nop; nop;"
        "nop; nop; nop; nop; nop; nop; nop; nop;");
}