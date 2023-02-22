
// #include <def.h>
#include <LoadCell.h>

void LoadCell::setup()
{
    // _scale_A.set_scale(_calibration_factor); // this value is obtained by calibrating the scale with known weights; see the README for details
    _scale_B.set_scale(_calibration_factor);

    // _scale_A.tare(); // reset the scale to 0
    _scale_B.tare();

    // _scale_A.power_down(); // put the ADC in sleep mode
    _scale_B.power_down();

}


float LoadCell::getWeight()
{
    // _scale_A.power_up(); // put the ADC in run mode
    _scale_B.power_up();

    float tmp_weight_A, tmp_weight_B;

    // tmp_weight_A = _scale_A.get_units(10);
    tmp_weight_B = _scale_B.get_units(10);

    // _scale_A.power_down(); // put the ADC in sleep mode
    _scale_B.power_down();

    return tmp_weight_A + tmp_weight_B;
}
