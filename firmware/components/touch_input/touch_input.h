#pragma once

#include "esp_err.h"
#include "driver/i2c.h"

typedef struct {
    uint16_t x;
    uint16_t y;
    uint8_t pressed;
} touch_event_t;

typedef void (*touch_callback_t)(touch_event_t *event);

typedef struct {
    i2c_port_t i2c_port;
    uint8_t i2c_addr;
    touch_callback_t callback;
} touch_input_t;

esp_err_t touch_input_init(touch_input_t *touch);
esp_err_t touch_input_deinit(touch_input_t *touch);
esp_err_t touch_input_read(touch_input_t *touch, touch_event_t *event);

#endif /* TOUCH_INPUT_H */
