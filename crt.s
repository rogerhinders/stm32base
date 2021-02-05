.cpu cortex-m3
.thumb
# stack pointer at end of SRAM
.word 0x20005000
# interrupt vectors
.word _reset_handler
.word _nmi_handler
.word _hardfault_handler
.word _memmanage_handler
.word _busfault_handler
.word _usagefault_handler
.word 0x0
.word 0x0
.word 0x0
.word 0x0
.word _svcall_handler
.word _debugmon_handler
.word 0x0
.word _pendsv_handler
.word _systick_handler
.word _wwdg_handler
.word _pvd_handler
.word _tamper_handler
.word _rtc_handler
.word _flash_handler
.word _rcc_handler
.word _exti0_handler
.word _exti1_handler
.word _exti2_handler
.word _exti3_handler
.word _exti4_handler
.word _dma1_channel1_handler
.word _dma1_channel2_handler
.word _dma1_channel3_handler
.word _dma1_channel4_handler
.word _dma1_channel5_handler
.word _dma1_channel6_handler
.word _dma1_channel7_handler
.word _adc1_2_handler
.word _usb_hp_can_tx_handler
.word _usb_lp_can_rx0_handler
.word _can_rx1_handler
.word _can_sce_handler
.word _exti9_5_handler
.word _tim1_brk_handler
.word _tim1_up_handler
.word _tim1_trg_com_handler
.word _tim1_cc_handler
.word _tim2_handler
.word _tim3_handler
.word _tim4_handler
.word _i2c1_ev_handler
.word _i2c1_er_handler
.word _i2c2_ev_handler
.word _i2c2_er_handler
.word _spi1_handler
.word _spi2_handler
.word _usart1_handler
.word _usart2_handler
.word _usart3_handler
.word _exti15_10_handler
.word _rtc_alarm_handler
.word _usb_wakeup_handler
.word _tim8_brk_handler
.word _tim8_up_handler
.word _tim8_trg_com_handler
.word _tim8_cc_handler
.word _adc3_handler
.word _fsmc_handler
.word _sdio_handler
.word _tim5_handler
.word _spi3_handler
.word _uart4_handler
.word _uart5_handler
.word _tim6_handler
.word _tim7_handler
.word _dma2_channel1_handler
.word _dma2_channel2_handler
.word _dma2_channel3_handler
.word _dma2_channel4_5_handler

# handlers start
.thumb_func
_reset_handler:
	bl main
	b .

.thumb_func
_handler_trap:
	ldr r0, =0x4001080c
	ldr r1, =0xe
	str r1,[r0]
	b .

.thumb_func
_nmi_handler:
	bl _handler_trap
.thumb_func
_hardfault_handler:
	bl _handler_trap
.thumb_func
_memmanage_handler:
	bl _handler_trap
.thumb_func
_busfault_handler:
	bl _handler_trap
.thumb_func
_usagefault_handler:
	bl _handler_trap
.thumb_func
_svcall_handler:
	bl _handler_trap
.thumb_func
_debugmon_handler:
	bl _handler_trap
.thumb_func
_pendsv_handler:
	bl _handler_trap
.thumb_func
_systick_handler:
	bl _handler_trap
.thumb_func
_wwdg_handler:
	bl _handler_trap
.thumb_func
_pvd_handler:
	bl _handler_trap
.thumb_func
_tamper_handler:
	bl _handler_trap
.thumb_func
_rtc_handler:
	bl _handler_trap
.thumb_func
_flash_handler:
	bl _handler_trap
.thumb_func
_rcc_handler:
	bl _handler_trap
.thumb_func
_exti0_handler:
	bl _handler_trap
.thumb_func
_exti1_handler:
	bl _handler_trap
.thumb_func
_exti2_handler:
	bl _handler_trap
.thumb_func
_exti3_handler:
	bl _handler_trap
.thumb_func
_exti4_handler:
	bl _handler_trap
.thumb_func
_dma1_channel1_handler:
	bl _handler_trap
.thumb_func
_dma1_channel2_handler:
	bl _handler_trap
.thumb_func
_dma1_channel3_handler:
	bl _handler_trap
.thumb_func
_dma1_channel4_handler:
	bl _handler_trap
.thumb_func
_dma1_channel5_handler:
	bl _handler_trap
.thumb_func
_dma1_channel6_handler:
	bl _handler_trap
.thumb_func
_dma1_channel7_handler:
	bl _handler_trap
.thumb_func
_adc1_2_handler:
	bl _handler_trap
.thumb_func
_usb_hp_can_tx_handler:
	bl _handler_trap

.thumb_func
_usb_lp_can_rx0_handler:
	ldr r0,=irq_usb_lp_can_rx0_handler
	bx r0

.thumb_func
_can_rx1_handler:
	bl _handler_trap
.thumb_func
_can_sce_handler:
	bl _handler_trap
.thumb_func
_exti9_5_handler:
	bl _handler_trap
.thumb_func
_tim1_brk_handler:
	bl _handler_trap
.thumb_func
_tim1_up_handler:
	bl _handler_trap
.thumb_func
_tim1_trg_com_handler:
	bl _handler_trap
.thumb_func
_tim1_cc_handler:
	bl _handler_trap
.thumb_func
_tim2_handler:
	bl _handler_trap
.thumb_func
_tim3_handler:
	bl _handler_trap
.thumb_func
_tim4_handler:
	bl _handler_trap
.thumb_func
_i2c1_ev_handler:
	bl _handler_trap
.thumb_func
_i2c1_er_handler:
	bl _handler_trap
.thumb_func
_i2c2_ev_handler:
	bl _handler_trap
.thumb_func
_i2c2_er_handler:
	bl _handler_trap
.thumb_func
_spi1_handler:
	bl _handler_trap
.thumb_func
_spi2_handler:
	bl _handler_trap
.thumb_func
_usart1_handler:
	bl _handler_trap
.thumb_func
_usart2_handler:
	bl _handler_trap
.thumb_func
_usart3_handler:
	bl _handler_trap
.thumb_func
_exti15_10_handler:
	bl _handler_trap
.thumb_func
_rtc_alarm_handler:
	bl _handler_trap
.thumb_func
_usb_wakeup_handler:
#	bx lr
	bl _handler_trap
.thumb_func
_tim8_brk_handler:
	bl _handler_trap
.thumb_func
_tim8_up_handler:
	bl _handler_trap
.thumb_func
_tim8_trg_com_handler:
	bl _handler_trap
.thumb_func
_tim8_cc_handler:
	bl _handler_trap
.thumb_func
_adc3_handler:
	bl _handler_trap
.thumb_func
_fsmc_handler:
	bl _handler_trap
.thumb_func
_sdio_handler:
	bl _handler_trap
.thumb_func
_tim5_handler:
	bl _handler_trap
.thumb_func
_spi3_handler:
	bl _handler_trap
.thumb_func
_uart4_handler:
	bl _handler_trap
.thumb_func
_uart5_handler:
	bl _handler_trap
.thumb_func
_tim6_handler:
	bl _handler_trap
.thumb_func
_tim7_handler:
	bl _handler_trap
.thumb_func
_dma2_channel1_handler:
	bl _handler_trap
.thumb_func
_dma2_channel2_handler:
	bl _handler_trap
.thumb_func
_dma2_channel3_handler:
	bl _handler_trap
.thumb_func
_dma2_channel4_5_handler:
	bl _handler_trap
