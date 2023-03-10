
#include <Led.h>

extern Bluetooth BT;
extern NiclaSenseNeoPixel Led;

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

        _n_start = 0;
        _n_stop = 1;
        _brightness = 120;

        _one_off = false;
    }
    else if (BT.readFlagCentralConnected() && !BT.readFlagAuthenticated())
    {
        _wait1 = LED_OFF_TIME;
        _color1 = LED_COLOR_OFF;

        _wait2 = LED_ON_TIME;
        _color2 = LED_COLOR_RED;

        _n_start = 0;
        _n_stop = 1;
        _brightness = 254;

        _one_off = false;
    }
    else if (BT.readFlagCentralConnected() && BT.readFlagAuthenticated())
    {
        if (!(BT.readFindReq() == 1 || BT.readFindReq() == 2))
        {
            _wait1 = LED_OFF_TIME;
            _color1 = LED_COLOR_OFF;

            _wait2 = LED_ON_TIME;
            _color2 = LED_COLOR_GREEN;

            _n_start = 0;
            _n_stop = 1;
            _brightness = 254;

            _one_off = false;
        }
        else if (BT.readFindReq() == 1 || BT.readFindReq() == 2)
        {
            _wait1 = 499;
            _color1 = LED_COLOR_OFF;

            _wait2 = 501;
            _color2 = LED_COLOR_BLUE;

            _n_start = 0;
            _n_stop = 1;
            _brightness = 254;

            _one_off = true;
            _findTimer.start();
        }
    }

    // LED LOOP
    if (!_one_off)
    {
        if ((millis() - _lastChange > _wait))
        {
            _lastChange = millis();
            if (_wait == _wait2)
            {
                Serial.println("WAIT 1");
                _wait = _wait1;
                setColor(_n_start, _n_stop, _color1, _brightness);
            }
            else
            {
                Serial.println("WAIT 2");
                _wait = _wait2;
                setColor(_n_start, _n_stop, _color2, _brightness);
            }
        }
    }

    if (_one_off)
    {
        if (_findTimer.fire())
        {
            Serial.println("ONE OFF FIRE");
            _findTimer.stop();
            BT.writeFindReq(0);
            _one_off = false;
        }

        if ((millis() - _lastChange > _wait))
        {
            _lastChange = millis();
            if (_wait == _wait2)
            {
                Serial.println("WAIT 1");
                _wait = _wait1;
                Led.SetPixelColor(0, 0, 0, _brightness);
                Led.SetPixelColor(1, 0, 0, 0);
                Led.Show();
                // setColor(_n_start, _n_start, _color1, _brightness);
                // setColor(_n_stop, _n_stop, _color2, _brightness);
            }
            else
            {
                Serial.println("WAIT 2");
                _wait = _wait2;
                Led.SetPixelColor(0, 0, 0, 0);
                Led.SetPixelColor(1, 0, 0, _brightness);
                Led.Show();

                // setColor(_n_start, _n_start, _color2, _brightness);
                // setColor(_n_stop, _n_stop, _color1, _brightness);
            }
        }
    }
}

void setColor(uint8_t n_start, uint8_t n_stop, uint8_t color, uint8_t brightness)
{

    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    // uint16_t val = 252 * brightness / 100;
    // Serial.println("Val: " + String(val));

    switch (color)
    {
    case LED_COLOR_OFF:
        r = 0;
        g = 0;
        b = 0;
        break;

    case LED_COLOR_RED:
        r = brightness;
        g = 0;
        b = 0;
        break;

    case LED_COLOR_GREEN:
        r = 0;
        g = brightness;
        b = 0;
        break;

    case LED_COLOR_BLUE:
        r = 0;
        g = 0;
        b = brightness;
        break;

    default:
        break;
    }

    // constrain(r, 0, 254);
    // constrain(g, 0, 254);
    // constrain(b, 0, 254);

    Serial.println("RGB: " + String(r) + " " + String(g) + " " + String(b));
    for (int x = n_start; x <= n_stop; x++)
        Led.SetPixelColor(x, g, r, b);
    Led.Show();
}

void setColor(uint8_t color)
{

    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

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
