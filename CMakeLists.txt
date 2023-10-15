cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(termpo_read_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(TermoRead
    main.cpp
    max6675.cpp
    lcd_i2c.cpp
    cpu_temperature.cpp
)
pico_enable_stdio_usb(TermoRead 1)

pico_add_extra_outputs(TermoRead)

target_link_libraries(TermoRead 
                      pico_stdlib 
                      hardware_adc 
                      hardware_spi 
                      hardware_i2c 
                      hardware_adc) 