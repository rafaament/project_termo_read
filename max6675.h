#ifndef MAX6675_H
#define MAX6675_H

#include "pico/stdlib.h"
#include "hardware/spi.h"

class MAX6675 {
public:
    MAX6675(spi_inst_t *spi, uint sck, uint cs, uint rx, uint tx);
    float read_celsius(bool &error) const;
    float read_fahrenheit(bool &error) const;

private:
    uint cs_pin;
    spi_inst_t *spi_port;
};

#endif