#ifndef LED_H_
#define LED_H_

#include <def.h>
#include <Bluetooth.h>

class StatusLed
{
public:
StatusLed() : L(LED_NUM){}

    void loop();

private:
    NiclaSenseNeoPixel L;

    FireTimer _findTimer;

    uint16_t _wait = 0;
    uint16_t _wait1 = 0;
    uint16_t _wait2 = 0;
    uint8_t _n_start = 0;
    uint8_t _n_stop = 0;
    uint8_t _brightness = 0;
    uint8_t _color1;
    uint8_t _color2;
    bool _one_off = false;

    uint32_t _lastChange;
    bool _firstLoop = true;

    void _setColor(uint8_t n_start, uint8_t n_stop, uint8_t color, uint8_t brightness); // color 0:off, 1:red, 2:green, 3:blue
};

#endif
