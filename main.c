#include <stdint.h>

/* base addresses */
#define RCC_BASE      0x40021000
#define FMI_BASE      0x40022000
#define GPIOA_BASE    0x40010800
#define GPIOB_BASE    0x40010C00
#define GPIOC_BASE    0x40011000
#define USB_REG_BASE  0x40005C00

#define NVIC_BASE     0xE000E100

#define NVIC_ISER0    *(volatile uint32_t *)(NVIC_BASE  + 0x00)

#define RCC_CR        *(volatile uint32_t *)(RCC_BASE   + 0x00)
#define RCC_CFGR      *(volatile uint32_t *)(RCC_BASE   + 0x04)
#define RCC_APB2ENR   *(volatile uint32_t *)(RCC_BASE   + 0x18)
#define RCC_APB1ENR   *(volatile uint32_t *)(RCC_BASE   + 0x1C)

#define FMI_ACR       *(volatile uint32_t *)(FMI_BASE   + 0x00)

#define GPIOA_CRL     *(volatile uint32_t *)(GPIOA_BASE + 0x00)
#define GPIOA_CRH     *(volatile uint32_t *)(GPIOA_BASE + 0x04)
#define GPIOA_ODR     *(volatile uint32_t *)(GPIOA_BASE + 0x0C)
#define GPIOA_BRR     *(volatile uint32_t *)(GPIOA_BASE + 0x14)

#define GPIOB_CRL     *(volatile uint32_t *)(GPIOB_BASE + 0x00)
#define GPIOB_CRH     *(volatile uint32_t *)(GPIOB_BASE + 0x04)
#define GPIOB_ODR     *(volatile uint32_t *)(GPIOB_BASE + 0x0C)

#define USB_CNTR      *(volatile uint32_t *)(USB_REG_BASE + 0x40)
#define USB_ISTR      *(volatile uint32_t *)(USB_REG_BASE + 0x44)

//#define GPIOC_CRH     *(volatile uint32_t *)(GPIOC_BASE + 0x04)
//#define GPIOC_ODR     *(volatile uint32_t *)(GPIOC_BASE + 0x0C)

#define RCC_IOPAEN   (1<<2)
#define RCC_IOPBEN   (1<<3)
//#define RCC_IOPCEN   (1<<4)

//#define GPIOC13      (1UL<<13)
#define GPIOA1       (1UL << 1) 
#define GPIOA2       (1UL << 2) 
#define GPIOA3       (1UL << 3) 

#define GPIOB2       (1UL << 2) 

/* IRQ stuff */
#define USB_LP_CAN_RX0 0x14

/* RCC CFGR stuff */
#define RCC_CFGR_M_SW           0x3
#define RCC_CFGR_M_SWS          (0x3 << 2)

#define RCC_CFGR_SYSCLK_USE_PLL 0x2
#define RCC_CFGR_PCLK1_DIV_2    (4 << 8)
#define RCC_CFGR_PCLK2_DIV_6    (2 << 14)
#define RCC_CFGR_PLL_USE_HSE    (1 << 16)
#define RCC_CFGR_PLL_MUL_9      (7 << 18)

/* RCC CR */
#define RCC_RC_HSI_TRIM_16      (16 << 3) /* set by system as default */
#define RCC_RC_HSE_ENABLE       (1 << 16)
#define RCC_RC_HSE_READY        (1 << 17)
#define RCC_RC_PLL_ENABLE       (1 << 24)
#define RCC_RC_PLL_READY        (1 << 25)

/* APB1ENR */
#define RCC_APB1ENR_USB_ENABLE (1 << 23)

/* FMI ACR */
#define FMI_ACR_LATENCY_TWO_STATE 0x2
#define FMI_ACR_PREFETCH_ENABLED (1 << 4)

/* USB STUFF */
#define USB_PMEM_BASE 0x40006000

#define USB_CNTR_FRES 1
#define USB_CNTR_RESETM (1 << 10)

void init_flash() {
	FMI_ACR = FMI_ACR_PREFETCH_ENABLED | FMI_ACR_LATENCY_TWO_STATE;
}

void init_clock() {
	/* CFGR must be setup before setting CR register */
	RCC_CFGR = RCC_CFGR_PLL_MUL_9 | RCC_CFGR_PLL_USE_HSE | RCC_CFGR_PCLK2_DIV_6
			| RCC_CFGR_PCLK1_DIV_2 | RCC_CFGR_SYSCLK_USE_PLL;	

	RCC_CR = RCC_RC_PLL_ENABLE | RCC_RC_HSE_ENABLE | RCC_RC_HSI_TRIM_16;

	/* wait for clocks to become ready */

	while(!(RCC_CR & RCC_RC_HSE_READY)) ;
	while(!(RCC_CR & RCC_RC_PLL_READY)) ;

	while(((RCC_CFGR & RCC_CFGR_M_SWS) >> 2) != (RCC_CFGR & RCC_CFGR_M_SW)) ;
}

void init_gpio() {
	/* enable port A */
	RCC_APB2ENR |= RCC_IOPAEN;
	/* enable port B */
    RCC_APB2ENR |= RCC_IOPBEN;
}

void init_usb() {
	/* enable usb */
	RCC_APB1ENR |= RCC_APB1ENR_USB_ENABLE;

	/* setup irq */
	NVIC_ISER0 |= (1 << USB_LP_CAN_RX0);

	/*
	pa11 usb rx
	pa12 usb tx
	*/

	GPIOA_CRH &= 0xFFF00FFF;
	GPIOA_CRH |= 0x00078000;
	GPIOA_BRR = 0x1800;

	/* force reset of usb */
	USB_CNTR = USB_CNTR_FRES;

	/* clear interrupt statuses */
	USB_ISTR = 0;

	/* reset interrupt enable */
	USB_CNTR = USB_CNTR_RESETM;
}

void irq_usb_lp_can_rx0_handler() {
	USB_ISTR = 0;
}

int main() {
	init_flash();
	init_clock();
	init_gpio();


	GPIOA_CRL &= 0xFFFF000F;
	GPIOA_CRL |= 0x00002220;

	GPIOA_ODR |= GPIOA1;
	GPIOA_ODR &= ~GPIOA2;
	GPIOA_ODR |= GPIOA3;

    GPIOB_CRL   &= 0xFFFFF0FF;
    GPIOB_CRL   |= 0x00000200;

	init_usb();

    while(1) {
        GPIOB_ODR ^=  GPIOB2;

		GPIOA_ODR ^= GPIOA1 | GPIOA2 | GPIOA3;

		/* delay blinking */
        for (int i = 0; i < 500000; i++);
    }

	return 0;
}
