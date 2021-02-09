#ifndef STM32F103_H
#define STM32F103_H
#include <stdint.h>

/* base addresses */
#define RCC_BASE      0x40021000
#define FMI_BASE      0x40022000
#define GPIOA_BASE    0x40010800
#define GPIOB_BASE    0x40010C00
#define GPIOC_BASE    0x40011000
#define I2C1_BASE     0x40005400
#define I2C2_BASE     0x40005800
#define USB_REG_BASE  0x40005C00

#define MEM_CAN_USB_BASE 0x40006000

#define USB_BTABLE_BASE_OFFSET 0x0
#define USB_PMA_BUF_BASE_OFFSET 0x80

#define USB_BDT_USB_ADDRn_TX_OFFSET 0x0
#define USB_BDT_USB_COUNTn_TX_OFFSET 0x4
#define USB_BDT_USB_ADDRn_RX_OFFSET 0x8
#define USB_BDT_USB_COUNTn_RX_OFFSET 0xC


#define NVIC_BASE     0xE000E100

#define NVIC_ISER0    *(volatile uint32_t *)(NVIC_BASE  + 0x00)

#define I2C1_CR1      *(volatile uint32_t *)(I2C1_BASE  + 0x00)
#define I2C1_CR2      *(volatile uint32_t *)(I2C1_BASE  + 0x04)
#define I2C1_OAR1     *(volatile uint32_t *)(I2C1_BASE  + 0x08)
#define I2C1_OAR2     *(volatile uint32_t *)(I2C1_BASE  + 0x0C)
#define I2C1_DR       *(volatile uint32_t *)(I2C1_BASE  + 0x10)
#define I2C1_SR1      *(volatile uint32_t *)(I2C1_BASE  + 0x14)
#define I2C1_SR2      *(volatile uint32_t *)(I2C1_BASE  + 0x18)
#define I2C1_CCR      *(volatile uint32_t *)(I2C1_BASE  + 0x1C)
#define I2C1_TRISE    *(volatile uint32_t *)(I2C1_BASE  + 0x20)


#define RCC_CR        *(volatile uint32_t *)(RCC_BASE   + 0x00)
#define RCC_CFGR      *(volatile uint32_t *)(RCC_BASE   + 0x04)
#define RCC_APB2RSTR  *(volatile uint32_t *)(RCC_BASE   + 0x0C)
#define RCC_APB1RSTR  *(volatile uint32_t *)(RCC_BASE   + 0x10)
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

#define USB_FNR       *(volatile uint32_t *)(USB_REG_BASE + 0x48)
#define USB_DADDR     *(volatile uint32_t *)(USB_REG_BASE + 0x4C)
#define USB_BTABLE    *(volatile uint32_t *)(USB_REG_BASE + 0x50)

#define RCC_APB1RSTR_I2C1RST (1 << 21)

#define RCC_IOPAEN   (1<<2)
#define RCC_IOPBEN   (1<<3)
#define RCC_IOPCEN   (1<<4)

#define GPIOA0       1
#define GPIOA1       (1UL << 1)
#define GPIOA2       (1UL << 2)
#define GPIOA3       (1UL << 3)
#define GPIOA4       (1UL << 4)
#define GPIOA5       (1UL << 5)
#define GPIOA6       (1UL << 6)
#define GPIOA7       (1UL << 7)

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
#define RCC_APB1ENR_I2C1_ENABLE (1 << 21)
#define RCC_APB1ENR_USB_ENABLE (1 << 23)

/* FMI ACR */
#define FMI_ACR_LATENCY_TWO_STATE 0x2
#define FMI_ACR_PREFETCH_ENABLED (1 << 4)

/* I2C STUFF */
#define I2C1_CR1_PE        1
#define I2C1_CR1_SMBUS     (1 << 1)
#define I2C1_CR1_SMBTYPE   (1 << 3)
#define I2C1_CR1_ENARP     (1 << 4)
#define I2C1_CR1_ENPEC     (1 << 5)
#define I2C1_CR1_ENGC      (1 << 6)
#define I2C1_CR1_NOSTRETCH (1 << 7)
#define I2C1_CR1_START     (1 << 8)
#define I2C1_CR1_STOP      (1 << 9)
#define I2C1_CR1_ACK       (1 << 10)
#define I2C1_CR1_POS       (1 << 11)
#define I2C1_CR1_PEC       (1 << 12)
#define I2C1_CR1_ALERT     (1 << 13)
#define I2C1_CR1_SWRST     (1 << 15)

#define I2C1_P_CR2_FREQ    0
#define I2C1_P_CR2_ITERREN 8
#define I2C1_P_CR2_ITEVTEN 9
#define I2C1_P_CR2_ITBUFEN 10
#define I2C1_P_CR2_DMAEN   11
#define I2C1_P_CR2_LAST    12

#define I2C1_CR2_FREQ    1
#define I2C1_CR2_ITERREN (1 << I2C1_P_CR2_ITERREN)
#define I2C1_CR2_ITEVTEN (1 << I2C1_P_CR2_ITEVTEN)
#define I2C1_CR2_ITBUFEN (1 << I2C1_P_CR2_ITBUFEN)
#define I2C1_CR2_DMAEN   (1 << I2C1_P_CR2_DMAEN)
#define I2C1_CR2_LAST    (1 << I2C1_P_CR2_LAST)

#define I2C1_CCR_DUTY (1 << 14)
#define I2C1_CCR_FS   (1 << 15)

#define I2C1_SR1_SB       1
#define I2C1_SR1_ADDR     (1 << 1)
#define I2C1_SR1_BTF      (1 << 2)
#define I2C1_SR1_ADD10    (1 << 3)
#define I2C1_SR1_STOPF    (1 << 4)
#define I2C1_SR1_RXNE     (1 << 6)
#define I2C1_SR1_TXE      (1 << 7)
#define I2C1_SR1_BERR     (1 << 8)
#define I2C1_SR1_ARLO     (1 << 9)
#define I2C1_SR1_AF       (1 << 10)
#define I2C1_SR1_OVR      (1 << 11)
#define I2C1_SR1_PECERR   (1 << 12)
#define I2C1_SR1_TIMEOUT  (1 << 14)
#define I2C1_SR1_SMBALERT (1 << 15)

/* USB STUFF */
#define USB_PMEM_BASE 0x40006000

#define USB_CNTR_FRES    1
#define USB_CNTR_PDWN    (1 << 1)
#define USB_CNTR_LP_MODE (1 << 2)
#define USB_CNTR_FSUSP   (1 << 3)
#define USB_CNTR_RESUME  (1 << 4)

#define USB_CNTR_ESOFM   (1 << 8)
#define USB_CNTR_SOFM    (1 << 9)
#define USB_CNTR_RESETM  (1 << 10)
#define USB_CNTR_SUSPM   (1 << 11)
#define USB_CNTR_WKUPM   (1 << 12)
#define USB_CNTR_ERRM    (1 << 13)
#define USB_CNTR_PMAOVRM (1 << 14)
#define USB_CNTR_CTRM    (1 << 15)

#define USB_P_ISTR_EP_ID  0
#define USB_P_ISTR_DIR    4
#define USB_P_ISTR_ESOF   8
#define USB_P_ISTR_SOF    9
#define USB_P_ISTR_RESET  10
#define USB_P_ISTR_SUSP   11
#define USB_P_ISTR_WKUP   12
#define USB_P_ISTR_ERR    13
#define USB_P_ISTR_PMAOVR 14
#define USB_P_ISTR_CTR    15

#define USB_ISTR_EP_ID  0xf
#define USB_ISTR_DIR    (1 << USB_P_ISTR_DIR)
#define USB_ISTR_ESOF   (1 << USB_P_ISTR_ESOF)
#define USB_ISTR_SOF    (1 << USB_P_ISTR_SOF)
#define USB_ISTR_RESET  (1 << USB_P_ISTR_RESET)
#define USB_ISTR_SUSP   (1 << USB_P_ISTR_SUSP)
#define USB_ISTR_WKUP   (1 << USB_P_ISTR_WKUP)
#define USB_ISTR_ERR    (1 << USB_P_ISTR_ERR)
#define USB_ISTR_PMAOVR (1 << USB_P_ISTR_PMAOVR)
#define USB_ISTR_CTR    (1 << USB_P_ISTR_CTR)

#define USB_P_EP_EA       0
#define USB_P_EP_STAT_TX  4
#define USB_P_EP_DTOG_TX  6
#define USB_P_EP_CTR_TX   7
#define USB_P_EP_EP_KIND  8
#define USB_P_EP_EP_TYPE  9
#define USB_P_EP_SETUP    11
#define USB_P_EP_STAT_RX  12
#define USB_P_EP_DTOG_RX  14
#define USB_P_EP_CTR_RX   15

#define USB_EP_EA       0xf
#define USB_EP_STAT_TX  (3 << USB_P_EP_STAT_TX)
#define USB_EP_DTOG_TX  (1 << USB_P_EP_DTOG_TX)
#define USB_EP_CTR_TX   (1 << USB_P_EP_CTR_TX)
#define USB_EP_EP_KIND  (1 << USB_P_EP_EP_KIND)
#define USB_EP_EP_TYPE  (3 << USB_P_EP_EP_TYPE)
#define USB_EP_SETUP    (1 << USB_P_EP_SETUP)
#define USB_EP_STAT_RX  (3 << USB_P_EP_STAT_RX)
#define USB_EP_DTOG_RX  (1 << USB_P_EP_DTOG_RX)
#define USB_EP_CTR_RX   (1 << USB_P_EP_CTR_RX)

#define USB_EP_TYPE_BULK 0
#define USB_EP_TYPE_CNTR 1
#define USB_EP_TYPE_ISO  2
#define USB_EP_TYPE_IRQ  3

#define USB_EP_STAT_DISABLED 0
#define USB_EP_STAT_STALL    1
#define USB_EP_STAT_NAK      2
#define USB_EP_STAT_VALID    3

#define USB_EP_INV_MASK 0x8f8f
#define USB_DADDR_EF    (1 << 7)
#define USB_DADDR_ADD   0x7f

#define USB_EP_DIRECTION_TX 0
#define USB_EP_DIRECTION_RX 1


#define USB_P_COUNTN_RX_COUNTN_RX 0
#define USB_P_COUNTN_RX_NUM_BLOCK 10
#define USB_P_COUNTN_RX_BLSIZE    15

#define USB_COUNTN_RX_COUNTN_RX   0x3ff
#define USB_COUNTN_RX_NUM_BLOCK   (0x1f << USB_P_COUNTN_RX_NUM_BLOCK)
#define USB_COUNTN_RX_BLSIZE      (1 << USB_P_COUNTN_RX_BLSIZE)

#define USB_EP(x)     *(volatile uint32_t *)(USB_REG_BASE + (x)*4)

#define USB_BDT_ADDR_TX(x) *(volatile uint32_t *)( \
		MEM_CAN_USB_BASE + USB_BTABLE_BASE_OFFSET + USB_BDT_USB_ADDRn_TX_OFFSET + (x) * 16)

#define USB_BDT_COUNT_TX(x) *(volatile uint32_t *)( \
		MEM_CAN_USB_BASE + USB_BTABLE_BASE_OFFSET + USB_BDT_USB_COUNTn_TX_OFFSET + (x) * 16)

#define USB_BDT_ADDR_RX(x) *(volatile uint32_t *)( \
		MEM_CAN_USB_BASE + USB_BTABLE_BASE_OFFSET + USB_BDT_USB_ADDRn_RX_OFFSET + (x) * 16)

#define USB_BDT_COUNT_RX(x) *(volatile uint32_t *)( \
		MEM_CAN_USB_BASE + USB_BTABLE_BASE_OFFSET + USB_BDT_USB_COUNTn_RX_OFFSET + (x) * 16)

#define USB_ADDR_TO_PMA_ADDR(x) ((x) / 2)
#define USB_PMA_ADDR_TO_ADDR(x) ((x) * 2)

#define USB_PMA(addr) *(volatile uint32_t *)( \
		MEM_CAN_USB_BASE + USB_PMA_ADDR_TO_ADDR(addr))

void stm32_flash_init();
void stm32_clock_init();
void stm32_gpio_init();
#endif /* STM32F103_H */
