#include "common/flash.h"

bool flash_write_page(uint16_t *buf, uint32_t page_num) {
	bool ret = true;

	uint32_t page_offset = page_num * FLASH_PAGE_SIZE;

	/* unlock if needed */
	if(FMI_WRPR) {
		FMI_KEYR = FMI_KEY1;
		FMI_KEYR = FMI_KEY2;
	}

	/* erase page */

	while(FMI_SR & FMI_SR_BSY) ;

	FMI_CR |= FMI_CR_PER;
	FMI_AR = page_offset;
	FMI_CR |= FMI_CR_STRT;

	while(FMI_SR & FMI_SR_BSY) ;

	/* make sure erase page is disabled */
	FMI_CR &= ~FMI_CR_PER;

	/* write page */
	for(int i = 0; i < FLASH_PAGE_SIZE/2; i++) {
		FMI_CR |= FMI_CR_PG;

		*(uint16_t *)(page_offset+i*2) = buf[i];

		while(FMI_SR & FMI_SR_BSY) ;

		/* verify */
		if(*(uint16_t *)(page_offset+i*2) != buf[i]) {
			ret = false;
			goto _write_page_cleanup;
		}
	}

_write_page_cleanup:
	/* make sure write is disabled */
	FMI_CR &= ~FMI_CR_PG;

	/* lock the flash again */
	FMI_CR |= FMI_CR_LOCK; 

	return ret;
}

void flash_init() {
	FMI_ACR = FMI_ACR_PREFETCH_ENABLED | FMI_ACR_LATENCY_TWO_STATE;
}
