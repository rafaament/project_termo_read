#include "max6675.h"

MAX6675::MAX6675(spi_inst_t *spi, uint sck, uint cs, uint rx, uint tx) 
{
    cs_pin = cs;
    spi_port = spi;

    // Initialize SPI interface
    spi_init(spi_port, 5000 * 1000);
    gpio_set_function(rx, GPIO_FUNC_SPI);
    gpio_set_function(tx, GPIO_FUNC_SPI);
    gpio_set_function(sck, GPIO_FUNC_SPI);

    gpio_init(cs_pin);
    gpio_set_dir(cs_pin, GPIO_OUT);
    gpio_put(cs_pin, true);
}

float MAX6675::read_celsius(bool &error) const 
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
        error = true;
        return 0.0f;
    }

    v >>= 3;
    error = false;
    return v * 0.25;
}

float MAX6675::read_fahrenheit(bool &error) const 
{
    return read_celsius(error) * 9.0 / 5.0 + 32;
}