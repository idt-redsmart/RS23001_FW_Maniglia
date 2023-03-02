
#include <ShockDetection.h>

void ShockDetection::setup()
{
    _linearAcc.begin(15, 0);
    _debounce_timer.begin(SHOCK_DEBOUNCE_TIME);

    #ifdef SHOCK_DEBUG_
    Serial.println("Accelerometer setup OK");
#endif
}

bool ShockDetection::detect()
{
    _debounce_timer.start();

    uint32_t tmp_X, tmp_Y, tmp_Z;

    for (int x = 0; x < SHOCK_NUM_SAMPLE; x++)
    {
        BHY2.update();

        tmp_X += abs(_linearAcc.x());
        tmp_Y += abs(_linearAcc.y());
        tmp_Z += abs(_linearAcc.z());
    }
    // media
    short linearAccX = tmp_X / SHOCK_NUM_SAMPLE;
    short linearAccY = tmp_Y / SHOCK_NUM_SAMPLE;
    short linearAccZ = tmp_Z / SHOCK_NUM_SAMPLE;

    // scalo valori
    linearAccX = linearAccX / SHOCK_SCALE_VALUE;
    linearAccY = linearAccY / SHOCK_SCALE_VALUE;
    linearAccZ = linearAccZ / SHOCK_SCALE_VALUE;

    // String accData = "X: " + String(linearAccX) + "\tY: " + String(linearAccY) + "\tZ: " + String(linearAccZ);
    // String accData = "ACC:\tX:\t" + String(linearAcc.x()) + "\tY:\t" + linearAcc.y() + "\tZ:\t" + linearAcc.z();
    // Serial.println(accData);
    // delay(100);

    if ((linearAccX > SHOCK_THRESHOLD || linearAccY > SHOCK_THRESHOLD || linearAccZ > SHOCK_THRESHOLD) && _debounce_timer.fire())
    {
        _debounce_timer.stop();
        _shockDetected = true;
    }

    if (_shockDetected)
    {
        _debounce_timer.start();

        _shockDetected = false;
        _shockNewData = true;

        _fillString('x', linearAccX);
        _fillString('y', linearAccY);
        _fillString('z', linearAccZ);

        writeShockData("U;" + _linearAccXOut + ";" + _linearAccYOut + ";" + _linearAccZOut);
        // String dataTmp = "U;" + String(linearAccX) + ";" + String(linearAccY) + ";" + String(linearAccZ);

#ifdef SHOCK_DEBUG_
        Serial.println("\n**URTO RILEVATO**");
        Serial.println(readShockData());
#endif

        return true;
    }
    else
    {
        // writeShockData("U;0000;0000;0000");
        // String dataTmp = "U;0000;0000;0000";

        return false;
    }
}

void ShockDetection::_fillString(char ax, short val)
{
    if (val / 10 < 1)
    {
        if (ax == 'x')
            _linearAccXOut = "000" + String(val);
        else if (ax == 'y')
            _linearAccYOut = "000" + String(val);
        else if (ax == 'z')
            _linearAccZOut = "000" + String(val);
    }
    else if (val / 100 < 1)
    {
        if (ax == 'x')
            _linearAccXOut = "00" + String(val);
        else if (ax == 'y')
            _linearAccYOut = "00" + String(val);
        else if (ax == 'z')
            _linearAccZOut = "00" + String(val);
    }
    else if (val / 1000 < 1)
    {
        if (ax == 'x')
            _linearAccXOut = "0" + String(val);
        else if (ax == 'y')
            _linearAccYOut = "0" + String(val);
        else if (ax == 'z')
            _linearAccZOut = "0" + String(val);
    }
    else if (val / 10000 < 1)
    {
        if (ax == 'x')
            _linearAccXOut = String(val);
        else if (ax == 'y')
            _linearAccYOut = String(val);
        else if (ax == 'z')
            _linearAccZOut = String(val);
    }
}