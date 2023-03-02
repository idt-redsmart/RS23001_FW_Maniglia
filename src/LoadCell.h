
#ifndef LOAD_CELL_
#define LOAD_CELL_

#include <def.h>

class LoadCell
{
public:
    // LoadCell() : _scale_A(LOADCELL_A_DOUT_PIN, LOADCELL_A_SCK_PIN),
    //              _scale_B(LOADCELL_B_DOUT_PIN, LOADCELL_B_SCK_PIN) {}

    void setup();
    float getWeight();

    void writeScaleData(String tmp) { _scaleData = tmp; }
    String readScaleData() { return _scaleData; }

private:
    // HX711 _scale_A;
    // HX711 _scale_B;

    // HX711 circuit wiring
    const int _LOAD_CELL_A_DOUT_PIN = LOAD_CELL_A_DOUT_PIN;
    const int _LOAD_CELL_A_SCK_PIN = LOAD_CELL_A_SCK_PIN;
    const int _LOAD_CELL_B_DOUT_PIN = LOAD_CELL_B_DOUT_PIN;
    const int _LOAD_CELL_B_SCK_PIN = LOAD_CELL_B_SCK_PIN;
    float _calibration_factor = LOAD_CELL_CALIBRATION_FACTOR; // this calibration factor is adjusted according to my load cell

    String _scaleData = LOAD_CELL_DATA_DEFAULT_VALUE;
};

#endif