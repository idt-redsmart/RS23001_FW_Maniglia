
// #include <def.h>
#include <LoadCell.h>

void LoadCell::setup()
{
    // void set_scale(float scale = 1.f);
    // _scale_A.set_scale(_calibration_factor); // this value is obtained by calibrating the scale with known weights; see the README for details
    // _scale_B.set_scale(_calibration_factor);

    // _scale_A.tare(); // reset the scale to 0
    // _scale_B.tare();

    // _scale_A.power_down(); // put the ADC in sleep mode
    // _scale_B.power_down();

    dbg(DBG_LOAD_CELL, "Load cell setup OK");
}

float LoadCell::getWeight()
{
    // _scale_A.power_up(); // put the ADC in run mode
    // _scale_B.power_up();

    float tmp_weight_A, tmp_weight_B;

    // tmp_weight_A = _scale_A.get_units(10);
    // tmp_weight_B = _scale_B.get_units(10);

    tmp_weight_A = 9.37;
    tmp_weight_B = 11.58;

    // _scale_A.power_down(); // put the ADC in sleep mode
    // _scale_B.power_down();

    dbg(DBG_LOAD_CELL, "Weight: " + String(tmp_weight_A + tmp_weight_B) + " Kg");

    return tmp_weight_A + tmp_weight_B;
}
