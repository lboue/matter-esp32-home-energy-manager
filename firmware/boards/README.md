# Board Support

This directory contains board-specific configurations and drivers for different ESP32 development boards used in the Home Energy Manager project.

## Supported Boards

### Waveshare ESP32-S3 Touch LCD 4.3

**Configuration File:** `waveshare_esp32_s3_touch_lcd_4_3.h`

A complete HEM replacement using the Waveshare ESP32-S3 Touch LCD 4.3 board.

#### Features:
- **Processor:** ESP32-S3 (Dual-core)
- **Display:** 4.3-inch LCD Touch Screen (480×272)
- **Connectivity:** Wi-Fi (802.11 b/g/n)
- **Storage:** Supports microSD card slot
- **Memory:** PSRAM support for additional RAM

#### GPIO Configuration:

**Display (SPI2):**
- SCLK: GPIO 36
- MOSI: GPIO 35
- MISO: GPIO 37
- DC: GPIO 21
- RST: GPIO 48
- CS: GPIO 39
- Backlight: GPIO 46

**Touch Screen (I2C0):**
- SDA: GPIO 19
- SCL: GPIO 20
- INT: GPIO 18
- RST: GPIO 8

**SD Card (SPI3):**
- SCLK: GPIO 12
- MOSI: GPIO 11
- MISO: GPIO 13
- CS: GPIO 10

#### Build Instructions:

```bash
cd firmware
idf.py set-target esp32s3
idf.py build
idf.py flash -p /dev/ttyUSB0 monitor
```

#### Configuration Notes:

The following changes have been made to support the Waveshare board:

1. **ESP-IDF Target:** Changed from ESP32-P4 to ESP32-S3
2. **Network:** Switched from Ethernet to Wi-Fi connectivity
3. **Display Driver:** LCD SPI panel support (st7262 driver)
4. **Touch Input:** I2C-based capacitive touch controller support
5. **Components:** Added `lcd_display` and `touch_input` components

## Adding New Boards

To add support for a new board:

1. Create a header file in this directory with GPIO and configuration macros
2. Add board-specific initialization code to the appropriate components
3. Update `sdkconfig.defaults` with target-specific settings
4. Create build instructions in this README

## Current Limitations

- **Waveshare ESP32-S3 Touch LCD 4.3:** 
  - Wi-Fi only (no Ethernet support currently)
  - LCD driver requires ESP-LCD component
  - Touch panel requires CST816S or compatible I2C controller
