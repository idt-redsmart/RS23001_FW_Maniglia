
#include <Led.h>

extern Bluetooth BT;
extern NiclaSenseNeoPixel Led;



void StatusLed::loop()
{
    if (_firstLoop)
    {
        _firstLoop = false;

        Led.Init();
        delay(20);
        _setColor(0, 1, LED_COLOR_OFF, 0);

        _lastChange = millis();
        _findTimer.begin(LED_FIND_TIMER_VALUE * 1000);
    }

    switch (ledLoopPhase)
    {
    case LED_STATUS_NOT_CONNECTED:

        _wait1 = LED_OFF_TIME_NOT_CON;
        _color1 = LED_COLOR_OFF;

        _wait2 = LED_ON_TIME_NOT_CON;
        _color2 = LED_COLOR_RED;

        _n_start = 0;
        _n_stop = 1;
        _brightness = 40;

        _one_off = false;

        break;
    case LED_STATUS_NOT_AUTH:

        _wait1 = LED_OFF_TIME;
        _color1 = LED_COLOR_OFF;

        _wait2 = LED_ON_TIME;
        _color2 = LED_COLOR_RED;

        _n_start = 0;
        _n_stop = 1;
        _brightness = 120;

        _one_off = false;

        break;
    case LED_STATUS_AUTH:

        _wait1 = LED_OFF_TIME;
        _color1 = LED_COLOR_OFF;

        _wait2 = LED_ON_TIME;
        _color2 = LED_COLOR_GREEN;

        _n_start = 0;
        _n_stop = 1;
        _brightness = 120;

        _one_off = false;

        break;
    case LED_STATUS_FIND:

        _wait1 = 499;
        _color1 = LED_COLOR_OFF;

        _wait2 = 501;
        _color2 = LED_COLOR_BLUE;

        _n_start = 0;
        _n_stop = 1;
        _brightness = 120;

        _one_off = true;

        break;

    default:
        break;
    }

    // LED LOOP
    if ((millis() - _lastChange > _wait || ledStatusUpdated))
    {
        ledStatusUpdated = false;
        _lastChange = millis();

        if (_wait == _wait2)
        {
            dbg(DBG_LED, "WAIT 1");

            _wait = _wait1;
            if (!_one_off)
                _setColor(_n_start, _n_stop, _color1, _brightness);
            else
            {
                dbg(DBG_LED, "ONE OFF START");

                _findTimer.start();
                if (_findTimer.fire())
                {
                    dbg(DBG_LED, "ONE OFF FIRE");

                    _findTimer.stop();
                    ledLoopPhase = 2;
                }

                L.SetPixelColor(0, 0, 0, _brightness);
                L.SetPixelColor(1, 0, 0, 0);
                L.Show();
            }
        }
        else
        {
            dbg(DBG_LED, "WAIT 2");

            _wait = _wait2;
            if (!_one_off)
                _setColor(_n_start, _n_stop, _color2, _brightness);
            else
            {
                L.SetPixelColor(0, 0, 0, 0);
                L.SetPixelColor(1, 0, 0, _brightness);
                L.Show();
            }
        }
    }
}

void StatusLed::_setColor(uint8_t n_start, uint8_t n_stop, uint8_t color, uint8_t brightness)
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

    dbg(DBG_LED, "RGB: " + String(r) + " " + String(g) + " " + String(b));
    for (int x = n_start; x <= n_stop; x++)
        L.SetPixelColor(x, g, r, b);
    L.Show();
}
