#ifndef LED_H_
#define LED_H_

#include <def.h>
#include <Bluetooth.h>


// void setup();
void LedLoop();

void setColor(uint8_t n_start, uint8_t n_stop, uint8_t color, uint8_t brightness); // color 0:off, 1:red, 2:green, 3:blue
void setColor(uint8_t color);            // color 0:off, 1:red, 2:green, 3:blue
// void setColor(uint8_t n, uint8_t r, uint8_t g, uint8_t b);

/*class Led
{
public:
    Led() : _led(LED_NUM) {}

    void setup();
    void loop();

    void setColor(uint8_t n, uint8_t color); // color 0:off, 1:red, 2:green, 3:blue
    void setColor(uint8_t color); // color 0:off, 1:red, 2:green, 3:blue
    void setColor(uint8_t n, uint8_t r, uint8_t g, uint8_t b);

private:
    NiclaSenseNeoPixel _led;

    FireTimer _findTimer;

    uint16_t _wait = 0;
    uint16_t _wait1 = 0;
    uint16_t _wait2 = 0;
    uint32_t _lastChange;
    uint8_t _color1;
    uint8_t _color2;
    bool _firstLoop = true;
};*/
#endif
