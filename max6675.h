#ifndef MAX6675_H
#define MAX6675_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

class MAX6675 {
public:
    MAX6675(spi_inst_t *spi, uint sck, uint cs, uint rx, uint tx);
    void init();
    float read_celsius();
    float read_fahrenheit();

private:
    spi_inst_t *spi_port;
    uint sck_pin;
    uint cs_pin;
    uint rx_pin;
    uint tx_pin;
};

#endif