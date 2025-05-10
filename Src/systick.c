
/* @systick.c */
#include "systick.h"
#include "stm32f4xx.h"

/* Global SysTick counter - increments every 1ms */
volatile uint32_t systick_counter = 0;

void SysTick_Init(void) {
    /* Configure SysTick for 1ms intervals */
    /* Assuming 16MHz system clock (default HSI) */

    /* Load reload value for 1ms delay */
    /* SysTick operates at processor clock speed */
    SysTick->LOAD = (16000000 / 1000) - 1;  /* 16000 ticks for 1ms */

    /* Reset current value register */
    SysTick->VAL = 0;

    /* Configure SysTick Control Register:
     * - CLKSOURCE = 1 (processor clock)
     * - TICKINT = 1 (enable interrupt)
     * - ENABLE = 1 (enable counter)
     */
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void) {
    /* Increment counter every 1ms */
    systick_counter++;
}

void SysTick_Delay(uint32_t delay_ms) {
    uint32_t start_time = systick_counter;

    /* Wait until the delay time has passed */
    while ((systick_counter - start_time) < delay_ms) {
        /* Wait for the delay */
    }
}
