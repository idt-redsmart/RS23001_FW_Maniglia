
#include <Led.h>

extern Bluetooth BT;
extern NiclaSenseNeoPixel Led;

FireTimer _findTimer;

uint16_t _wait = 0;
uint16_t _wait1 = 0;
uint16_t _wait2 = 0;
uint32_t _lastChange;
uint8_t _color1;
uint8_t _color2;
bool _firstLoop = true;

/*
void Led::setup()
{
    Led.Init();
    delay(20);
    setColor(LED_COLOR_OFF);
    // setColor(LED_COLOR_OFF);

    _lastChange = millis();
    _findTimer.begin(LED_FIND_TIMER_VALUE * 1000);
}*/

void LedLoop()
{
    if (_firstLoop)
    {
        _firstLoop = false;

        Led.Init();
        delay(20);
        setColor(LED_COLOR_OFF);
        // setColor(LED_COLOR_OFF);

        _lastChange = millis();
        _findTimer.begin(LED_FIND_TIMER_VALUE * 1000);
    }

    if (!BT.readFlagCentralConnected())
    {
        _wait1 = LED_OFF_TIME_NOT_CON;
        _color1 = LED_COLOR_OFF;

        _wait2 = LED_ON_TIME_NOT_CON;
        _color2 = LED_COLOR_RED;
    }
    else if (BT.readFlagCentralConnected() && !BT.readFlagAuthenticated())
    {
        _wait1 = LED_OFF_TIME;
        _color1 = LED_COLOR_OFF;

        _wait2 = LED_ON_TIME;
        _color2 = LED_COLOR_RED;
    }
    else if (BT.readFlagCentralConnected() && BT.readFlagAuthenticated())
    {
        _wait1 = LED_OFF_TIME;
        _color1 = LED_COLOR_OFF;

        _wait2 = LED_ON_TIME;
        _color2 = LED_COLOR_GREEN;
    }

    if ((millis() - _lastChange > _wait))
    {
        _lastChange = millis();
        if (_wait == _wait2)
        {
            Serial.println("WAIT 1");
            _wait = _wait1;
            setColor(_color1);
        }
        else
        {
            Serial.println("WAIT 2");
            _wait = _wait2;
            setColor(_color2);
        }
    }

    /*
        if (!BT.readFlagCentralConnected()) // not connected
        {
            Serial.println("Led loop not connected");
            if ((millis() - _lastChange > _wait))
            {
                _lastChange = millis();
                if (_wait == LED_ON_TIME_NOT_CON)
                {
                    _wait = LED_OFF_TIME_NOT_CON;
                    setColor(LED_COLOR_OFF);
                }
                else
                {
                    _wait = LED_ON_TIME_NOT_CON;
                    setColor(LED_COLOR_RED);
                }
            }
        }
        else // connected, not authenticated
        {
            Serial.println("Led loop connected");

            if ((millis() - _lastChange > _wait))
            {
                _lastChange = millis();
                if (_wait == LED_ON_TIME)
                {
                    _wait = LED_OFF_TIME;
                    setColor(LED_COLOR_OFF);
                }
                else
                {
                    _wait = LED_ON_TIME;

                    if (BT.readFlagAuthenticated()) // authenticated
                    {
                        Serial.println("Led loop auth");

                        setColor(LED_COLOR_GREEN);

                        while (BT.readFindReq() == 1 || BT.readFindReq() == 2)
                        {
                            _findTimer.start();

                            setColor(0, LED_COLOR_BLUE);
                            setColor(1, LED_COLOR_OFF);
                            delay(400);
                            setColor(0, LED_COLOR_OFF);
                            setColor(1, LED_COLOR_BLUE);
                            delay(100);

                            if (_findTimer.fire())
                            {
                                _findTimer.stop();
                                BT.writeFindReq(0);
                            }
                        }
                    }
                    else
                    {
                        setColor(LED_COLOR_RED);
                    }
                }
            }
        }*/
}

void setColor(uint8_t n, uint8_t color)
{

    uint8_t r, g, b;

    switch (color)
    {
    case LED_COLOR_OFF:
        r = 0;
        g = 0;
        b = 0;
        break;

    case LED_COLOR_RED:
        r = 254;
        g = 0;
        b = 0;
        break;

    case LED_COLOR_GREEN:
        r = 0;
        g = 254;
        b = 0;
        break;

    case LED_COLOR_BLUE:
        r = 0;
        g = 0;
        b = 254;
        break;

    default:
        break;
    }
    // Serial.println("RGB: " + String(r) + " " + String(g) + " " + String(b));
    Led.SetPixelColor(n, r, g, b);
    Led.Show();
}

void setColor(uint8_t color)
{

    uint8_t r, g, b;

    switch (color)
    {
    case LED_COLOR_OFF:
        r = 0;
        g = 0;
        b = 0;
        break;

    case LED_COLOR_RED:
        r = 254;
        g = 0;
        b = 0;
        break;

    case LED_COLOR_GREEN:
        r = 0;
        g = 254;
        b = 0;
        break;

    case LED_COLOR_BLUE:
        r = 0;
        g = 0;
        b = 254;
        break;

    default:
        break;
    }
    // Serial.println("RGB: " + String(r) + " " + String(g) + " " + String(b));
    Led.SetPixelColor(0, g, r, b);
    Led.SetPixelColor(1, g, r, b);
    Led.Show();
}

void setColor(uint8_t n, uint8_t r, uint8_t g, uint8_t b)
{
    Led.SetPixelColor(n, r, g, b);
    Led.Show();
}
