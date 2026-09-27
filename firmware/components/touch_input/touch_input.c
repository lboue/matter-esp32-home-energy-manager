#include "touch_input.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "driver/gpio.h"

#ifdef CONFIG_IDF_TARGET_ESP32S3
#include "boards/waveshare_esp32_s3_touch_lcd_4_3.h"
#endif

static const char *TAG = "touch_input";

esp_err_t touch_input_init(touch_input_t *touch)
{
#ifdef CONFIG_IDF_TARGET_ESP32S3
    ESP_LOGI(TAG, "Initializing touch input for Waveshare ESP32-S3 Touch LCD 4.3");

    touch->i2c_port = TOUCH_I2C_PORT;
    touch->i2c_addr = 0x15;

    i2c_config_t i2c_config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = TOUCH_SDA_GPIO,
        .scl_io_num = TOUCH_SCL_GPIO,
        .master.clk_speed = 100000,
    };

    ESP_RETURN_ON_ERROR(i2c_param_config(TOUCH_I2C_PORT, &i2c_config), TAG, "Failed to configure I2C");
    ESP_RETURN_ON_ERROR(i2c_driver_install(TOUCH_I2C_PORT, i2c_config.mode, 0, 0, 0), TAG, "Failed to install I2C driver");

    gpio_config_t int_config = {
        .pin_bit_mask = (1ULL << TOUCH_INT_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE,
    };
    ESP_RETURN_ON_ERROR(gpio_config(&int_config), TAG, "Failed to configure touch interrupt GPIO");

    gpio_config_t rst_config = {
        .pin_bit_mask = (1ULL << TOUCH_RST_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_RETURN_ON_ERROR(gpio_config(&rst_config), TAG, "Failed to configure touch reset GPIO");

    ESP_RETURN_ON_ERROR(gpio_set_level(TOUCH_RST_GPIO, 0), TAG, "Failed to reset touch controller");
    vTaskDelay(pdMS_TO_TICKS(10));
    ESP_RETURN_ON_ERROR(gpio_set_level(TOUCH_RST_GPIO, 1), TAG, "Failed to release touch controller reset");
    vTaskDelay(pdMS_TO_TICKS(50));

    ESP_LOGI(TAG, "Touch input initialized successfully");
    return ESP_OK;
#else
    ESP_LOGE(TAG, "Touch input not supported on this target");
    return ESP_ERR_NOT_SUPPORTED;
#endif
}

esp_err_t touch_input_deinit(touch_input_t *touch)
{
#ifdef CONFIG_IDF_TARGET_ESP32S3
    return i2c_driver_delete(TOUCH_I2C_PORT);
#else
    return ESP_ERR_NOT_SUPPORTED;
#endif
}

esp_err_t touch_input_read(touch_input_t *touch, touch_event_t *event)
{
#ifdef CONFIG_IDF_TARGET_ESP32S3
    if (!touch || !event) {
        return ESP_ERR_INVALID_ARG;
    }

    uint8_t data[5];
    i2c_master_read_from_slave(touch->i2c_port, touch->i2c_addr, data, sizeof(data));

    if (data[0] & 0x80) {
        event->pressed = 1;
        event->x = ((data[1] & 0x0F) << 8) | data[2];
        event->y = ((data[3] & 0x0F) << 8) | data[4];
    } else {
        event->pressed = 0;
        event->x = 0;
        event->y = 0;
    }

    return ESP_OK;
#else
    return ESP_ERR_NOT_SUPPORTED;
#endif
}
