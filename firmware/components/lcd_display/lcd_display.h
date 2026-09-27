#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include "esp_err.h"
#include "esp_lcd_types.h"

typedef struct {
    esp_lcd_panel_handle_t panel;
} lcd_display_t;

esp_err_t lcd_display_init(lcd_display_t *display);
esp_err_t lcd_display_deinit(lcd_display_t *display);
esp_err_t lcd_display_set_brightness(uint8_t brightness);
esp_err_t lcd_display_clear(lcd_display_t *display);

#endif /* LCD_DISPLAY_H */
