/* main.c */

#include "stm32f4xx.h"
#include "uart.h"
#include "systick.h"
#include <stdio.h>

int main(void)
{
    /* Initialize SysTick and UART */
    SysTick_Init();
    UART_Init(115200);

    /* Test 1: Basic send functionality */
    UART_SendString("\r\n=== UART Driver Phase 1.2 Demo ===\r\n");
    UART_SendString("UART initialized successfully!\r\n");

    /* Test 2: Printf-style formatting */
    char buffer[100];
    sprintf(buffer, "System Clock: %lu Hz\r\n", 16000000UL);
    UART_SendString(buffer);

    /* Test 3: Bidirectional test */
    UART_SendString("\r\nType a character and it will be echoed back: ");

    while(1)
    {
        if(UART_IsDataAvailable()) {
            uint8_t received = UART_ReceiveByte();

            // Echo back with formatting
            sprintf(buffer, "\r\nYou typed: '%c' (ASCII: %d)\r\n", received, received);
            UART_SendString(buffer);

            // Simple command handling
            if(received == 'q' || received == 'Q') {
                UART_SendString("Exiting to Phase 1.3...\r\n");
                break;
            }

            UART_SendString("Type another character (or 'q' to quit): ");
        }

        /* This delay prevents CPU overload while waiting */
        SysTick_Delay(10);
    }

    /* Move to Phase 1.3 setup */
    UART_SendString("\r\n=== Ready for Phase 1.3: Simple Task Scheduler ===\r\n");

    while(1) {
        /* Main loop for Phase 1.3 */
        SysTick_Delay(100);
    }

    return 0;
}
