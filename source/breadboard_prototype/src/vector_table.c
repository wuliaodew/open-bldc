/*
 * Open-BLDC - Open BruschLess DC Motor Controller
 * Copyright (C) 2009 by Piotr Esden-Tempski <piotr@esden.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stm32/lib.h>
#include "interrupts.h"

/* addresses defined in the linker script */
extern unsigned long _etext; /* end addr of .text section */
extern unsigned long _sidata; /* init values for .data section */
extern unsigned long _sdata; /* start addr of .data section */
extern unsigned long _edata; /* end addr of .data section */
extern unsigned long _sbss; /* start addr of .bss section */
extern unsigned long _ebss; /* end addr of .bss section */
extern void _estack; /* stack pointer init value */

void reset_handler_stage1(void) __attribute__((__interrupt__));
void reset_handler_stage2(void);
extern int main(void);

/* interrupt vector */
__attribute__ ((section(".isr_vector")))
void (* const vector_table[])(void) = {
    &_estack,              /* stack pointer init value*/
    reset_handler_stage1,  /* pc init value */
    nmi_exception,
    hard_fault_exception,
    mem_manage_exception,
    bus_fault_exception,
    usage_fault_exception,
    0, 0, 0, 0,            /* reserved */
    svc_handler,
    debug_monitor,
    0,                     /* reserved */
    pend_svc,
    sys_tick_handler,
    wwdg_irq_handler,
    pvd_irq_handler,
    tamper_irq_handler,
    rtc_irq_handler,
    flash_irq_handler,
    rcc_irq_handler,
    exti0_irq_handler,
    exti1_irq_handler,
    exti2_irq_handler,
    exti3_irq_handler,
    exti4_irq_handler,
    dma1_channel1_irq_handler,
    dma1_channel2_irq_handler,
    dma1_channel3_irq_handler,
    dma1_channel4_irq_handler,
    dma1_channel5_irq_handler,
    dma1_channel6_irq_handler,
    dma1_channel7_irq_handler,
    adc1_2_irq_handler,
    usb_hp_can_tx_irq_handler,
    usb_lp_can_rx0_irq_handler,
    can_rx1_irq_handler,
    can_sce_irq_handler,
    exti9_5_irq_handler,
    tim1_brk_irq_handler,
    tim1_up_irq_handler,
    tim1_trg_com_irq_handler,
    tim1_cc_irq_handler,
    tim2_irq_handler,
    tim3_irq_handler,
    tim4_irq_handler,
    i2c1_ev_irq_handler,
    i2c1_er_irq_handler,
    i2c2_ev_irq_handler,
    i2c2_er_irq_handler,
    spi1_irq_handler,
    spi2_irq_handler,
    usart1_irq_handler,
    usart2_irq_handler,
    usart3_irq_handler,
    exti15_10_irq_handler,
    rtc_alarm_irq_handler,
    usb_wake_up_irq_handler,
    tim8_brk_irq_handler,
    tim8_up_irq_handler,
    tim8_trg_com_irq_handler,
    tim8_cc_irq_handler,
    adc3_irq_handler,
    fsmc_irq_handler,
    sdio_irq_handler,
    tim5_irq_handler,
    spi3_irq_handler,
    uart4_irq_handler,
    uart5_irq_handler,
    tim6_irq_handler,
    tim7_irq_handler,
    dma2_channel1_irq_handler,
    dma2_channel2_irq_handler,
    dma2_channel3_irq_handler,
    dma2_channel4_5_irq_handler,
};

/* Get's called directly after mcu reset */
void reset_handler_stage1(void){
    /* set stack align */
    SCB->CCR = SCB->CCR | SCB_CCR_STKALIGN;

    reset_handler_stage2();
}

void reset_handler_stage2(void){
    unsigned long *pul_src, *pul_dest;

    /* Copy the data segment initializers from flash to SRAM */
    pul_src = &_sidata;
    for(pul_dest = &_sdata; pul_dest < &_edata; ){
        *(pul_dest++) = *(pul_src++);
    }
    /* Zero fill the bss segment.  */
    for(pul_dest = &_sbss; pul_dest < &_ebss; ){
        *(pul_dest++) = 0;
    }

    /* Call the application's entry point.*/
    main();
}