#include "cpu_temperature.h"


void CPUTemperature::init()
{
    adc_init();
    adc_set_temp_sensor_enabled(true);
}

float CPUTemperature::read_celsius()
{
    adc_select_input(onboard_adc_pin);

    const float conversionFactor = 3.3f / (1 << 12);

    float adc = (float)adc_read() * conversionFactor;
    float tempC = 27.0f - (adc - 0.706f) / 0.001721f;

    return tempC;
}

float CPUTemperature::read_fahrenheit()
{
    return read_celsius() * 9.0f / 5.0f + 32.0f;
}
