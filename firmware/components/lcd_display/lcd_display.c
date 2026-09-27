#include "lcd_display.h"
#include "esp_log.h"
#include "esp_check.h"
#include "string.h"
#include "stdlib.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"

#ifdef CONFIG_IDF_TARGET_ESP32S3
#include "boards/waveshare_esp32_s3_touch_lcd_4_3.h"
#endif

static const char *TAG = "lcd_display";

esp_err_t lcd_display_init(lcd_display_t *display)
{
#ifdef CONFIG_IDF_TARGET_ESP32S3
    ESP_LOGI(TAG, "Initializing LCD display for Waveshare ESP32-S3 Touch LCD 4.3");

    spi_bus_config_t buscfg = {
        .sclk_io_num = LCD_SPI_CLK_GPIO,
        .mosi_io_num = LCD_SPI_MOSI_GPIO,
        .miso_io_num = LCD_SPI_MISO_GPIO,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = LCD_WIDTH * LCD_HEIGHT * 2,
    };

    ESP_RETURN_ON_ERROR(spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO), TAG, "Failed to initialize SPI bus");

    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_spi_config_t io_config = {
        .cs_gpio_num = LCD_CS_GPIO,
        .dc_gpio_num = LCD_DC_GPIO,
        .spi_mode = 0,
        .pclk_hz = 40 * 1000 * 1000,
        .trans_queue_depth = 10,
        .on_color_trans_done = NULL,
        .user_ctx = NULL,
    };

    ESP_RETURN_ON_ERROR(esp_lcd_new_panel_io_spi(LCD_HOST, &io_config, &io_handle), TAG, "Failed to create panel IO");

    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = LCD_RST_GPIO,
        .bits_per_pixel = 16,
    };

    ESP_RETURN_ON_ERROR(esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle), TAG, "Failed to create panel");

    display->panel = panel_handle;

    gpio_config_t bl_config = {
        .pin_bit_mask = (1ULL << LCD_BL_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_RETURN_ON_ERROR(gpio_config(&bl_config), TAG, "Failed to configure backlight GPIO");
    ESP_RETURN_ON_ERROR(gpio_set_level(LCD_BL_GPIO, 1), TAG, "Failed to enable backlight");

    ESP_LOGI(TAG, "LCD display initialized successfully");
    return ESP_OK;
#else
    ESP_LOGE(TAG, "LCD display not supported on this target");
    return ESP_ERR_NOT_SUPPORTED;
#endif
}

esp_err_t lcd_display_deinit(lcd_display_t *display)
{
#ifdef CONFIG_IDF_TARGET_ESP32S3
    if (display && display->panel) {
        esp_lcd_panel_del(display->panel);
    }
    spi_bus_free(LCD_HOST);
    return ESP_OK;
#else
    return ESP_ERR_NOT_SUPPORTED;
#endif
}

esp_err_t lcd_display_set_brightness(uint8_t brightness)
{
#ifdef CONFIG_IDF_TARGET_ESP32S3
    if (brightness == 0) {
        return gpio_set_level(LCD_BL_GPIO, 0);
    } else {
        return gpio_set_level(LCD_BL_GPIO, 1);
    }
#else
    return ESP_ERR_NOT_SUPPORTED;
#endif
}

esp_err_t lcd_display_clear(lcd_display_t *display)
{
#ifdef CONFIG_IDF_TARGET_ESP32S3
    if (!display || !display->panel) {
        return ESP_ERR_INVALID_ARG;
    }

    uint16_t *color = (uint16_t *)malloc(LCD_WIDTH * sizeof(uint16_t));
    if (!color) {
        return ESP_ERR_NO_MEM;
    }

    memset(color, 0, LCD_WIDTH * sizeof(uint16_t));

    for (int y = 0; y < LCD_HEIGHT; y++) {
        esp_lcd_panel_draw_bitmap(display->panel, 0, y, LCD_WIDTH, y + 1, color);
    }

    free(color);
    return ESP_OK;
#else
    return ESP_ERR_NOT_SUPPORTED;
#endif
}
