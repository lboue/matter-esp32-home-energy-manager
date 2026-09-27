#ifndef BOARD_CONFIG_WAVESHARE_ESP32_S3_TOUCH_LCD_4_3_H
#define BOARD_CONFIG_WAVESHARE_ESP32_S3_TOUCH_LCD_4_3_H

#include "driver/gpio.h"
#include "driver/spi_master.h"

#define BOARD_NAME "Waveshare ESP32-S3 Touch LCD 4.3"

/* Display Configuration */
#define LCD_HOST           SPI2_HOST
#define LCD_SPI_CLK_GPIO   GPIO_NUM_36
#define LCD_SPI_MOSI_GPIO  GPIO_NUM_35
#define LCD_SPI_MISO_GPIO  GPIO_NUM_37

#define LCD_DC_GPIO        GPIO_NUM_21
#define LCD_RST_GPIO       GPIO_NUM_48
#define LCD_CS_GPIO        GPIO_NUM_39

#define LCD_BL_GPIO        GPIO_NUM_46
#define LCD_BL_PWM_CHANNEL 0

#define LCD_WIDTH          480
#define LCD_HEIGHT         272

/* Touch Screen Configuration */
#define TOUCH_SDA_GPIO     GPIO_NUM_19
#define TOUCH_SCL_GPIO     GPIO_NUM_20
#define TOUCH_INT_GPIO     GPIO_NUM_18
#define TOUCH_RST_GPIO     GPIO_NUM_8
#define TOUCH_I2C_PORT     I2C_NUM_0

/* SD Card Configuration */
#define SD_SPI_CLK_GPIO    GPIO_NUM_12
#define SD_SPI_MOSI_GPIO   GPIO_NUM_11
#define SD_SPI_MISO_GPIO   GPIO_NUM_13
#define SD_SPI_CS_GPIO     GPIO_NUM_10
#define SD_HOST            SPI3_HOST

/* Network Configuration */
#define NETWORK_MODE_WIFI  1

/* Modbus TCP Configuration */
#define MODBUS_TCP_ENABLED 1

/* Power Supply Monitoring */
#define POWER_SUPPLY_GPIO  GPIO_NUM_1

#endif /* BOARD_CONFIG_WAVESHARE_ESP32_S3_TOUCH_LCD_4_3_H */
