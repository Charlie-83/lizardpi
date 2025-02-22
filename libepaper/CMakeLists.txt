set(SOURCE 
    Fonts/font20.c
    Fonts/font24.c
    Fonts/font8.c
    Fonts/font16.c
    Fonts/font12.c
    Fonts/font24CN.c
    Fonts/font12CN.c
    e-Paper/EPD_1in54b_V2.c
    Config/dev_hardware_SPI.c
    Config/DEV_Config.c
    Config/sysfs_gpio.c
    Config/sysfs_software_spi.c
    GUI/GUI_BMPfile.c
    GUI/GUI_Paint.c
)

add_library(libepaper ${SOURCE})
target_compile_definitions(libepaper PRIVATE USE_WIRINGPI_LIB)
target_include_directories(libepaper PUBLIC
    Config
    GUI
)
