
#include "cstdio"
#include "max6675.h"
#include "lcd_i2c.h"
#include "cpu_temperature.h"

char current_scale = 'C';
char last_scale = current_scale;
float temperature_termocouple = 0.0f;
float temperature_cpu = 0.0f;
char lcd_out[16];

int sampling_count = 0;
int current_sampling_size = 1;
int last_sampling_size = current_sampling_size;
float sampling_temperature_termocouple = 0.0f;
float sampling_temperature_cpu = 0.0f;

MAX6675 max6675(spi_default,
                PICO_DEFAULT_SPI_SCK_PIN, 
                PICO_DEFAULT_SPI_CSN_PIN,
                PICO_DEFAULT_SPI_RX_PIN,
                PICO_DEFAULT_SPI_TX_PIN);

LCDI2C lcd_i2c( i2c_default,
                PICO_DEFAULT_I2C_SDA_PIN,
                PICO_DEFAULT_I2C_SCL_PIN );

#define BUTTON_CHANGE_SCALE 20
#define POTENCIOMETER_SAMPLING 28

void gpio_callback(uint gpio, uint32_t events) 
{
    if( gpio != BUTTON_CHANGE_SCALE ) return;

    if(current_scale == 'F')
        current_scale = 'C';
    else
        current_scale = 'F';

}

void update_scale()
{
    if(current_scale != last_scale)
    {
        last_scale = current_scale;
        temperature_cpu = 0.0f;
        temperature_termocouple = 0.0f;
        sampling_count = 0;
    }
}

void update_temperature()
{
    if (current_scale == 'F')
    {
        sampling_temperature_termocouple += max6675.read_fahrenheit();
        sampling_temperature_cpu += CPUTemperature::read_fahrenheit();
    }
    else
    {
        sampling_temperature_termocouple += max6675.read_celsius();
        sampling_temperature_cpu += CPUTemperature::read_celsius();
    }
}

void write_all_info()
{
    lcd_i2c.clear();

    lcd_i2c.set_cursor(1, 11);
    snprintf(lcd_out, sizeof(lcd_out), "S:%d", current_sampling_size);
    lcd_i2c.write_string(lcd_out);

    lcd_i2c.set_cursor(0, 11);
    snprintf(lcd_out, sizeof(lcd_out), "C:%d", sampling_count);
    lcd_i2c.write_string(lcd_out);

    // Writes max termocouple reading
    snprintf(lcd_out, sizeof(lcd_out), "T/C:%.1f%c", temperature_termocouple, current_scale);
    lcd_i2c.set_cursor(0, 0);
    lcd_i2c.write_string(lcd_out);

    // Writes cpu internal reading
    snprintf(lcd_out, sizeof(lcd_out), "CPU:%.1f%c", temperature_cpu, current_scale);
    lcd_i2c.set_cursor(1, 0);
    lcd_i2c.write_string(lcd_out);
}

void update_sampling()
{
    adc_select_input(2);
    current_sampling_size = int( (100.0f * ((float) (adc_read()-10))) / 4060.0f);
    current_sampling_size = current_sampling_size > 0 ? current_sampling_size : 1;
    current_sampling_size = current_sampling_size < 100 ? current_sampling_size : 100;
}

int main() {
    stdio_init_all();

    CPUTemperature::init();
    max6675.init();
    lcd_i2c.init();

    adc_gpio_init(POTENCIOMETER_SAMPLING);

    gpio_init(BUTTON_CHANGE_SCALE);
    gpio_set_dir(BUTTON_CHANGE_SCALE, GPIO_IN);
    gpio_pull_up(BUTTON_CHANGE_SCALE);

    gpio_set_irq_enabled_with_callback(BUTTON_CHANGE_SCALE, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
    
    while (true) 
    {
        for(sampling_count=0; sampling_count<current_sampling_size; sampling_count++)
        {
            update_scale();
            update_sampling();
            update_temperature();
            write_all_info();
            sleep_ms(400);
        }

        temperature_cpu = (sampling_temperature_cpu /((float) current_sampling_size));
        temperature_termocouple = (sampling_temperature_termocouple /((float) current_sampling_size));

        sampling_temperature_cpu = 0.0f;
        sampling_temperature_termocouple = 0.0f;

        write_all_info();
    }

    return 0;
}
