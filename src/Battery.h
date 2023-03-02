
#ifndef BATTERY_H_
#define BATTERY_H_

#include <def.h>

class Battery
{
public:
    void setup();
    String getLevel();

    void beginTimer() { _sendLevel.begin(BATTERY_FREQ_DATA_SEND * 1000); }
    void startTimer() { _sendLevel.start(); }
    bool fireTimer() { return _sendLevel.fire(); }
    void stopTimer() { _sendLevel.stop(); }

private:
    FireTimer _sendLevel;

    String _batteryData = BATTERY_DATA_DEFAULT_VALUE;
};

class Led
{
public:
    Led() : _led(LED_NUM) {}

    void setup() { _led.Init(); }

    void setColor(uint16_t n, uint8_t color); //color 0:red, 1:green, 2:blue
    

private:
    NiclaSenseNeoPixel _led;
};
#endif
