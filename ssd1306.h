#ifndef SSD1306_H
#define SSD1306_H

#include <stdio.h>
#include <stdint.h>
#include "stm32f103.h"

void ssd1306_run_cmd(uint8_t cmd);
void ssd1306_write_array(uint8_t *buf, uint32_t len);
void ssd1306_init();
#endif /* SSD1306_H */
