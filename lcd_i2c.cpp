
#include "lcd_i2c.h"

/* Quick helper function for single byte transfers */
void LCDI2C::i2c_write_byte(uint8_t val) {
    i2c_write_blocking(i2c_port, addr, &val, 1, false);
}

void LCDI2C::toggle_enable(uint8_t val) {
    // Toggle enable pin on LCD display
    // We cannot do this too quickly or things don't work
#define DELAY_US 600
    sleep_us(DELAY_US);
    i2c_write_byte(val | LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
    i2c_write_byte(val & ~LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
}

// The display is sent a byte as two separate nibble transfers
void LCDI2C::send_byte(uint8_t val, int mode) {
    uint8_t high = mode | (val & 0xF0) | LCD_BACKLIGHT;
    uint8_t low = mode | ((val << 4) & 0xF0) | LCD_BACKLIGHT;

    i2c_write_byte(high);
    toggle_enable(high);
    i2c_write_byte(low);
    toggle_enable(low);
}

void LCDI2C::clear(void) {
    send_byte(LCD_CLEARDISPLAY, LCD_COMMAND);
}

// go to location on LCD
void LCDI2C::set_cursor(int line, int position) {
    int val = (line == 0) ? 0x80 + position : 0xC0 + position;
    send_byte(val, LCD_COMMAND);
}

void LCDI2C::write_char(char val) {
    send_byte(val, LCD_CHARACTER);
}

void LCDI2C::write_string(const char *s) {
    while (*s) {
        write_char(*s++);
    }
}

LCDI2C::LCDI2C(i2c_inst_t *i2c, uint sda, uint scl)
{   
    i2c_port = i2c;
    sda_pin = sda;
    scl_pin = scl;
}

void LCDI2C::init()
{
    i2c_init(i2c_port, 100 * 1000);
    gpio_set_function(sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(sda_pin);
    gpio_pull_up(scl_pin);

    send_byte(0x03, LCD_COMMAND);
    send_byte(0x03, LCD_COMMAND);
    send_byte(0x03, LCD_COMMAND);
    send_byte(0x02, LCD_COMMAND);

    send_byte(LCD_ENTRYMODESET | LCD_ENTRYLEFT, LCD_COMMAND);
    send_byte(LCD_FUNCTIONSET | LCD_2LINE, LCD_COMMAND);
    send_byte(LCD_DISPLAYCONTROL | LCD_DISPLAYON, LCD_COMMAND);
    clear();
}
