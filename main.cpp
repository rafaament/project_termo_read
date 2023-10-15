
#include "cstdio"
#include "max6675.h"
#include "lcd_i2c.h"
#include "cpu_temperature.h"

#define BUTTON_CHANGE_SCALE 20

int main() {

    stdio_init_all();
    CPUTemperature::init_reading();
    adc_gpio_init(28);

    gpio_init(BUTTON_CHANGE_SCALE);
    gpio_set_dir(BUTTON_CHANGE_SCALE, GPIO_IN);
    gpio_pull_up(BUTTON_CHANGE_SCALE);

    char lcd_out[16];


    bool max6675_error;
    MAX6675 max6675(spi_default,
                    PICO_DEFAULT_SPI_SCK_PIN, 
                    PICO_DEFAULT_SPI_CSN_PIN,
                    PICO_DEFAULT_SPI_RX_PIN,
                    PICO_DEFAULT_SPI_TX_PIN);

    LCDI2C lcd_i2c(i2c_default,
                   PICO_DEFAULT_I2C_SDA_PIN,
                   PICO_DEFAULT_I2C_SCL_PIN);


    while (true) {

        // Writes max termocouple reading
        float temp = max6675.read_celsius(max6675_error);
        if(!max6675_error)
        {
            snprintf(lcd_out, sizeof(lcd_out), "T/C:%.1fC", temp);
            lcd_i2c.set_cursor(0, 0);
            lcd_i2c.write_string(lcd_out);
        }

        // Writes cpu internal reading
        snprintf(lcd_out, sizeof(lcd_out), "CPU:%.1fC", CPUTemperature::read_celsius()); //9

        lcd_i2c.set_cursor(1, 0);
        lcd_i2c.write_string(lcd_out);
        
        adc_select_input(2);
        uint16_t samplingSize = adc_read();
        printf("pot reading %d\n", samplingSize);

        bool button_change_scale = gpio_get(BUTTON_CHANGE_SCALE);

        printf("button reading %d\n", button_change_scale);

        sleep_ms(1000);  // Read temperature every 1 second
    }

    return 0;
}
