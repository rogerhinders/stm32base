#ifndef _FLASH_H
#define _FLASH_H

#include <stdint.h>
#include <stdbool.h>
#include "common/stm32f103.h"

#define FLASH_PAGE_SIZE 0x400

bool flash_write_page(uint16_t *buf, uint32_t page_num);
void flash_init();
#endif /* _FLASH_H */
