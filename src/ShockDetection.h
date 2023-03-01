#ifndef _SHOCK_DETECTION_H_
#define _SHOCK_DETECTION_H_

#include <def.h>

class ShockDetection
{
public:
    ShockDetection() : _linearAcc(SENSOR_ID_LACC) {}

    void setup();
    bool detect();

    bool readNewData() { return _shockNewData; }
    void writeNewData(bool tmp) { _shockNewData = tmp; }

    void writeShockData(String tmp) { _shockData = tmp; }
    String readShockData() { return _shockData; }

private:
    SensorXYZ _linearAcc;

    FireTimer _debounce_timer;

    String _shockData = SHOCK_DATA_DEFAULT_VALUE;
    bool _shockDetected = false;
    bool _shockNewData = false;

    String _linearAccXOut;
    String _linearAccYOut;
    String _linearAccZOut;

    void _fillString(char ax, short val);
};

#endif