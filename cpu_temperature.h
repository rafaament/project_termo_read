#ifndef CPU_TEMPERATURE_H
#define CPI_TEMPERATURE_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

class CPUTemperature
{
public:
    static void init_reading();
    static float read_celsius();
    static float read_fahrenheit();
private:
    static const uint onboard_adc_pin = 4;
};

#endif