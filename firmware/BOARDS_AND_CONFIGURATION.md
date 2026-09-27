# Boards and Configuration

This project supports multiple ESP32 boards with target-specific SDK configurations.

## Configuration Strategy

The project uses a modular SDK configuration approach:

- **`sdkconfig.defaults`** — Common settings for all boards
- **`sdkconfig.defaults.esp32p4`** — ESP32-P4 specific configuration
- **`sdkconfig.defaults.esp32s3`** — ESP32-S3 specific configuration

ESP-IDF automatically merges the common configuration with target-specific defaults based on the active IDF target.

## Supported Boards

### ESP32-P4 (Original HEM)

**Configuration:** `sdkconfig.defaults.esp32p4`

```bash
cd firmware
idf.py set-target esp32p4
idf.py build
idf.py flash -p /dev/ttyUSB0 monitor
```

**Features:**
- Ethernet connectivity (SPI Ethernet adapter)
- 16 MB Flash
- High performance dual-core ESP32-P4

### Waveshare ESP32-S3-Touch-LCD-4.3

**Configuration:** `sdkconfig.defaults.esp32s3`

```bash
cd firmware
idf.py set-target esp32s3
idf.py build
idf.py flash -p /dev/ttyUSB0 monitor
```

**Features:**
- Wi-Fi connectivity (integrated)
- 4.3" capacitive touchscreen
- Local UI interface
- 8 MB Flash
- 2.7 MB SRAM + SPIRAM

See [firmware/boards/README.md](boards/README.md) for board-specific details.

## Switching Between Boards

### Step 1: Clean Build (Recommended for first build)

```bash
idf.py fullclean
```

### Step 2: Set Target

```bash
# For ESP32-P4
idf.py set-target esp32p4

# Or for ESP32-S3
idf.py set-target esp32s3
```

### Step 3: Build and Flash

```bash
idf.py build
idf.py flash -p /dev/ttyUSB0 monitor
```

## Key Differences by Board

| Feature | ESP32-P4 | ESP32-S3 |
|---------|----------|----------|
| **Network** | Ethernet (SPI) | Wi-Fi (integrated) |
| **Flash** | 16 MB | 8 MB |
| **SRAM** | LP/HP SRAM | 2.7 MB + SPIRAM |
| **Display** | None | 4.3" LCD (SPI) |
| **Touch** | None | Capacitive I2C |
| **Performance** | High | High |
| **Cost** | Higher | Lower |

## SDK Configuration Files Explained

### Common Configuration (`sdkconfig.defaults`)

Shared by all boards:
- IPv6 networking
- SPIRAM setup
- Matter controller/commissioner
- CHIP shell debugging
- HTTP server (WebSocket)
- SD card support (FATFS)
- Security settings (HKDF, SSL/TLS)

### ESP32-P4 Configuration (`sdkconfig.defaults.esp32p4`)

Target-specific settings:
- **IDF Target:** `esp32p4`
- **Network:** Ethernet via SPI (`CONFIG_ETH_USE_SPI_ETHERNET=y`)
- **Flash:** 16 MB
- **SPIRAM:** Enabled (no malloc override)
- **Wi-Fi:** Disabled
- **Features:** No display/touch

### ESP32-S3 Configuration (`sdkconfig.defaults.esp32s3`)

Target-specific settings:
- **IDF Target:** `esp32s3`
- **Network:** Wi-Fi (`CONFIG_ENABLE_WIFI_STATION=y`)
- **Flash:** 8 MB
- **SPIRAM:** Enabled with malloc override
- **Wi-Fi:** Enabled with network commissioning
- **Features:** LCD display, capacitive touch

## Adding a New Board

To add support for a new board:

### 1. Create Board Configuration

Create `firmware/boards/{board-name}.h` with GPIO mappings.

### 2. Create SDK Configuration

Create `firmware/sdkconfig.defaults.{IDF_TARGET}`:
```ini
CONFIG_IDF_TARGET="esp32..."
CONFIG_IDF_TARGET_ESP32...=y

# Board-specific settings
CONFIG_ESPTOOLPY_FLASHSIZE_...
# ... more settings
```

### 3. Create Board Components (if needed)

Add initialization code in `firmware/components/` for displays, sensors, etc.

### 4. Update Documentation

- Add entry to this file
- Update `firmware/boards/README.md`
- Create a migration guide if replacing a board

## Troubleshooting

### Configuration Not Applied

If your build doesn't reflect SDK configuration changes:

1. Clean the build: `idf.py fullclean`
2. Verify target: `idf.py get-idf-version`
3. Check active config: `idf.py menuconfig` (review but don't save)

### Build Failures After Target Change

Always run `idf.py fullclean` before switching targets.

### Board-Specific Errors

Check the target's `sdkconfig.defaults.{TARGET}` file and ensure all required configs are present.

## References

- [ESP-IDF Build System Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/build-system.html)
- [ESP32-P4 Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32p4/)
- [ESP32-S3 Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/)
- [Waveshare ESP32-S3-Touch-LCD-4.3](https://docs.waveshare.com/ESP32-S3-Touch-LCD-4.3)
