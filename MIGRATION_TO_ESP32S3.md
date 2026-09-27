# Migration to Waveshare ESP32-S3-Touch-LCD-4.3

This document describes the changes made to support the **Waveshare ESP32-S3-Touch-LCD-4.3** as the primary HEM (Home Energy Manager) platform.

## Summary of Changes

### Target Platform
- **Previous:** ESP32-P4 with external Ethernet
- **New:** ESP32-S3 with integrated Wi-Fi
- **Board:** Waveshare ESP32-S3-Touch-LCD-4.3 with 4.3" capacitive touchscreen

### Key Modifications

#### 1. **Firmware Configuration** (`sdkconfig.defaults`)
- Changed IDF target from `esp32p4` to `esp32s3`
- Updated flash size from 16MB to 8MB
- Switched network transport from Ethernet to Wi-Fi
- Updated GPIO configuration for ESP32-S3

#### 2. **New Components**
Created two new hardware abstraction components:

**`firmware/components/lcd_display/`**
- Manages 4.3" LCD display (SPI-based)
- Uses ESP-IDF's LCD panel API
- Supports backlight control
- GPIO configuration in `firmware/boards/waveshare_esp32_s3_touch_lcd_4_3.h`

**`firmware/components/touch_input/`**
- Manages capacitive touch input (I2C-based)
- Supports CST816S or compatible controllers
- Event-based touch coordinate reporting

#### 3. **Board Abstraction** (`firmware/boards/`)
Created `waveshare_esp32_s3_touch_lcd_4_3.h` with:
- All GPIO pin definitions
- SPI and I2C configuration macros
- Display and touch controller parameters

#### 4. **Networking Changes**
- **Enabled:** Wi-Fi station mode, Wi-Fi network commissioning
- **Disabled:** Ethernet network commissioning, external SPI Ethernet
- **Default SSID/Password:** Empty (to be configured during commissioning)

### GPIO Mapping

| Function | GPIO | Notes |
|----------|------|-------|
| **SPI Display** | | |
| CLK | 36 | |
| MOSI | 35 | |
| MISO | 37 | |
| DC | 21 | |
| CS | 39 | |
| RST | 48 | |
| Backlight | 46 | |
| **I2C Touch** | | |
| SDA | 19 | |
| SCL | 20 | |
| INT | 18 | |
| RST | 8 | |
| **SPI SD Card** | | |
| CLK | 12 | |
| MOSI | 11 | |
| MISO | 13 | |
| CS | 10 | |

## Building and Flashing

### Prerequisites
```bash
cd firmware
export IDF_PATH=/path/to/esp-idf
export ESP_MATTER_PATH=/path/to/esp-matter
```

### Build
```bash
idf.py set-target esp32s3
idf.py build
```

### Flash
```bash
# Standard USB port
idf.py flash -p /dev/ttyUSB0 monitor

# Or on macOS
idf.py flash -p /dev/tty.usbserial-* monitor
```

## Behavioral Changes

### Networking
- Device will connect via Wi-Fi instead of Ethernet
- Wi-Fi credentials can be set during Matter commissioning
- Default: No Wi-Fi connection until configured

### Display
- HEM status now visible on 4.3" touchscreen
- Touch interface available for local control
- Backlight brightness controllable

### Performance
- ESP32-S3 has comparable performance to ESP32-P4 for HEM operations
- Memory layout: 2.7 MB SRAM + SPIRAM for Matter's large footprint

## Rollback

To revert to ESP32-P4:
1. Switch back to `feat/esp32p4` branch or revert commits
2. Restore original `sdkconfig.defaults`
3. Rebuild: `idf.py set-target esp32p4`

## Known Limitations

1. **Ethernet:** Not currently supported (requires external adapter)
2. **Display Driver:** Only tested with ST7262 controller
3. **Touch Sampling:** Basic 5-byte protocol (CST816S compatible)

## Future Enhancements

- [ ] LVGL integration for richer UI
- [ ] Modbus TCP over Wi-Fi
- [ ] Local web UI on touchscreen
- [ ] Battery monitoring and power management
- [ ] Optional Ethernet support via USB-to-Ethernet adapter

## Testing Checklist

- [ ] Device boots successfully
- [ ] Wi-Fi connection establishes
- [ ] Matter commissioning works
- [ ] LCD displays correctly
- [ ] Touch input responds
- [ ] SD card operations work
- [ ] Data logging functions correctly
- [ ] Modbus TCP communication with inverter

## Support

For issues specific to Waveshare board support, check:
- [Waveshare ESP32-S3 Documentation](https://docs.waveshare.com/ESP32-S3-Touch-LCD-4.3)
- [ESP-IDF LCD Panel API](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/lcd.html)
