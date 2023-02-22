
#ifndef LOAD_CELL
#define LOAD_CELL

#include <def.h>
#include <HX711.h>


class LoadCell
{
public:

    LoadCell(): _scale_A(LOADCELL_A_DOUT_PIN, LOADCELL_A_SCK_PIN), 
                _scale_B(LOADCELL_B_DOUT_PIN, LOADCELL_B_SCK_PIN) {}

    void setup();
    float getWeight();

private:
    HX711 _scale_A;
    HX711 _scale_B;

    // HX711 circuit wiring
    const int _LOADCELL_A_DOUT_PIN = LOADCELL_A_DOUT_PIN;
    const int _LOADCELL_A_SCK_PIN = LOADCELL_A_SCK_PIN;
    const int _LOADCELL_B_DOUT_PIN = LOADCELL_B_DOUT_PIN;
    const int _LOADCELL_B_SCK_PIN = LOADCELL_B_SCK_PIN;
    float _calibration_factor = LOADCELL_CALIBRATION_FACTOR; // this calibration factor is adjusted according to my load cell


};

#endif