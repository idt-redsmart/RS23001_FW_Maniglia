
#ifndef BATTERY_H_
#define BATTERY_H_

#include <def.h>

class BatteryIntfc
{
public:
    void setup();
    String getLevel();
    int getBatteryStatus() { return nicla::getBatteryStatus() * 20; }
    void readReg();
    void disableCharge();

    void writeData(String str) { _data = str; }
    String readData() { return _data; }

    void beginTimer() { _sendLevel.begin(BATTERY_FREQ_DATA_SEND * 1000); }
    void startTimer() { _sendLevel.start(); }
    bool fireTimer() { return _sendLevel.fire(); }
    void stopTimer() { _sendLevel.stop(); }

private:
    FireTimer _sendLevel;

    void _readBit(uint8_t tmp);
    String _data = BATTERY_DATA_DEFAULT_VALUE;
};

#endif
