#include "max6675.h"

MAX6675::MAX6675(spi_inst_t *spi, uint sck, uint cs, uint rx, uint tx) 
{
    spi_port = spi;
    sck_pin = sck;
    cs_pin  = cs;
    rx_pin  = rx;
    tx_pin  = tx;
}

void MAX6675::init()
{
    // Initialize SPI interface
    spi_init(spi_port, 5000 * 1000);
    gpio_set_function(rx_pin, GPIO_FUNC_SPI);
    gpio_set_function(tx_pin, GPIO_FUNC_SPI);
    gpio_set_function(sck_pin, GPIO_FUNC_SPI);

    gpio_init(cs_pin);
    gpio_set_dir(cs_pin, GPIO_OUT);
    gpio_put(cs_pin, true);
}

float MAX6675::read_celsius() 
{
    gpio_put(cs_pin, false);
    sleep_us(10);

    uint8_t tmp;
    spi_read_blocking(spi_port, 0, &tmp, 1);

    uint16_t v = (uint16_t) tmp;
    v <<= 8;

    spi_read_blocking(spi_port, 0, &tmp, 1);
    v |= ((uint16_t) tmp);

    gpio_put(cs_pin, true);

    if (v & 0x4) {
        return -10000.0f;
    }

    v >>= 3;
    return v * 0.25;
}

float MAX6675::read_fahrenheit()  
{
    return read_celsius() * 9.0 / 5.0 + 32;
}