
/* @main.c - Comprehensive UART Test Suite */
#include "stm32f4xx.h"
#include "uart.h"
#include "systick.h"
#include <stdio.h>
#include <string.h>

// Test data arrays
const char* test_strings[] = {
    "Hello World!",
    "The quick brown fox jumps over the lazy dog",
    "1234567890",
    "!@#$%^&*()_+-=[]{}|;':\",./<>?",
    "UPPERCASE",
    "lowercase",
    "Mixed Case String 123!@#"
};

/* Function to flush the input buffer */
/* Function to flush the input buffer */
void FlushInputBuffer(void) {
    // Add a small delay first to ensure all characters have arrived
    SysTick_Delay(50);

    // Clear all available characters
    while(UART_IsDataAvailable()) {
        UART_ReceiveByte();  // Read and discard
    }

    // Add another small delay to ensure buffer is clear
    SysTick_Delay(50);
}
/* Dummy interrupt handlers for testing */
/* Modified interrupt handler for testing */
void USART3_IRQHandler(void) {
    /* This is a more correct way to handle interrupts */

    /* Handle RXNE interrupt */
    if((USART3->SR & USART_SR_RXNE) && (USART3->CR1 & USART_CR1_RXNEIE)) {
        volatile uint8_t dummy = USART3->DR;  // Read DR to clear RXNE
        (void)dummy;
    }

    /* Handle TC interrupt */
    if((USART3->SR & USART_SR_TC) && (USART3->CR1 & USART_CR1_TCIE)) {
        /* TC flag is cleared by reading SR and then writing to DR */
        /* But in this test, we don't want to write data */
        /* So we'll disable the TC interrupt to prevent the hang */
        USART3->CR1 &= ~USART_CR1_TCIE;
    }

    /* Handle TXE interrupt */
    if((USART3->SR & USART_SR_TXE) && (USART3->CR1 & USART_CR1_TXEIE)) {
        /* Disable TXE interrupt to prevent continuous firing */
        USART3->CR1 &= ~USART_CR1_TXEIE;
    }
}
void UART_ComprehensiveDiagnostics(void) {
    UART_SendString("\r\n=== COMPREHENSIVE UART DIAGNOSTICS ===\r\n");

    // 1. Register Values
    char reg_str[50];
    UART_SendString("\r\n1. Register Values:\r\n");
    sprintf(reg_str, "USART3->CR1: 0x%04X\r\n", (unsigned int)USART3->CR1);
    UART_SendString(reg_str);
    sprintf(reg_str, "USART3->CR2: 0x%04X\r\n", (unsigned int)USART3->CR2);
    UART_SendString(reg_str);
    sprintf(reg_str, "USART3->CR3: 0x%04X\r\n", (unsigned int)USART3->CR3);
    UART_SendString(reg_str);
    sprintf(reg_str, "USART3->BRR: 0x%04X\r\n", (unsigned int)USART3->BRR);
    UART_SendString(reg_str);
    sprintf(reg_str, "USART3->SR: 0x%04X\r\n", (unsigned int)USART3->SR);
    UART_SendString(reg_str);

    // 2. Clock Configuration
    UART_SendString("\r\n2. Clock Configuration:\r\n");
    sprintf(reg_str, "AHB1ENR: 0x%08X\r\n", (unsigned int)RCC->AHB1ENR);
    UART_SendString(reg_str);
    sprintf(reg_str, "APB1ENR: 0x%08X\r\n", (unsigned int)RCC->APB1ENR);
    UART_SendString(reg_str);

    // 3. GPIO Configuration
    UART_SendString("\r\n3. GPIO Configuration (GPIOD):\r\n");
    sprintf(reg_str, "MODER: 0x%08X\r\n", (unsigned int)GPIOD->MODER);
    UART_SendString(reg_str);
    sprintf(reg_str, "AFR[1]: 0x%08X\r\n", (unsigned int)GPIOD->AFR[1]);
    UART_SendString(reg_str);

    // 4. SysTick Configuration
    UART_SendString("\r\n4. SysTick Status:\r\n");
    sprintf(reg_str, "Counter: %lu\r\n", systick_counter);
    UART_SendString(reg_str);
    sprintf(reg_str, "SysTick->CTRL: 0x%04X\r\n", (unsigned int)SysTick->CTRL);
    UART_SendString(reg_str);
    sprintf(reg_str, "SysTick->LOAD: 0x%06X\r\n", (unsigned int)SysTick->LOAD);
    UART_SendString(reg_str);
}

void Test_BasicFunctions(void) {
    UART_SendString("\r\n=== TESTING BASIC FUNCTIONS ===\r\n");

    // Test 1: UART_SendString
    UART_SendString("\r\nTest 1.1: UART_SendString with various strings:\r\n");
    for(int i = 0; i < sizeof(test_strings)/sizeof(test_strings[0]); i++) {
        UART_SendString("String ");
        UART_SendByte('0' + i);
        UART_SendString(": ");
        UART_SendString(test_strings[i]);
        UART_SendString("\r\n");
        SysTick_Delay(100);
    }

    // Test 2: UART_SendByte
    UART_SendString("\r\nTest 1.2: UART_SendByte (ASCII table 32-126):\r\n");
    for(int i = 32; i <= 126; i++) {
        UART_SendByte(i);
        if((i - 31) % 16 == 0) UART_SendString("\r\n");
    }
    UART_SendString("\r\n");

    // Test 3: UART_SendByte error testing
    UART_SendString("\r\nTest 1.3: UART_SendByte with timeout:\r\n");
    for(int i = 0; i < 5; i++) {
        UART_Error result = UART_SendByte('A' + i);
        char msg[50];
        sprintf(msg, "SendByte %d result: %d\r\n", i, result);
        UART_SendString(msg);
    }
}

void Test_AdvancedFunctions(void) {
    UART_SendString("\r\n=== TESTING ADVANCED FUNCTIONS ===\r\n");

    // Test 1: UART_Transmit
    UART_SendString("\r\nTest 2.1: UART_Transmit with different sizes:\r\n");
    for(int i = 0; i < sizeof(test_strings)/sizeof(test_strings[0]); i++) {
        char msg[50];
        sprintf(msg, "Transmitting string %d (size: %d):\r\n", i, strlen(test_strings[i]));
        UART_SendString(msg);

        UART_Error result = UART_Transmit(test_strings[i], strlen(test_strings[i]), 1000);
        sprintf(msg, "\r\nResult: %d\r\n\r\n", result);
        UART_SendString(msg);
        SysTick_Delay(200);
    }

    // Test 2: UART_Transmit with null data
    UART_SendString("\r\nTest 2.2: UART_Transmit error conditions:\r\n");
    UART_Error result = UART_Transmit(NULL, 10, 1000);
    char msg[50];
    sprintf(msg, "NULL data result: %d\r\n", result);
    UART_SendString(msg);

    result = UART_Transmit("test", 0, 1000);
    sprintf(msg, "Zero size result: %d\r\n", result);
    UART_SendString(msg);

    // Test 3: Test UART_IsDataAvailable and UART_ReceiveByte
    UART_SendString("\r\nTest 2.3: Type 5 characters to test receive functions:\r\n");
    UART_SendString("(I'll wait 10 seconds for each character)\r\n");

    for(int i = 0; i < 5; i++) {
        UART_SendString("Waiting for character ");
        UART_SendByte('1' + i);
        UART_SendString(": ");

        uint32_t start_time = systick_counter;
        bool received = false;

        while((systick_counter - start_time) < 10000) {  // 10 second timeout
            if(UART_IsDataAvailable()) {
                uint8_t ch = UART_ReceiveByte();
                sprintf(msg, "Received '%c' (ASCII: %d)\r\n", ch, ch);
                UART_SendString(msg);
                received = true;
                break;
            }
        }

        if(!received) {
            UART_SendString("Timeout - no character received\r\n");
        }
    }
}

void Test_ErrorHandling(void) {
    UART_SendString("\r\n=== TESTING ERROR HANDLING ===\r\n");

    // Test 1: UART_ClearErrors
    UART_SendString("\r\nTest 3.1: UART_ClearErrors:\r\n");
    UART_Error result = UART_ClearErrors();
    char msg[50];
    sprintf(msg, "ClearErrors result: %d\r\n", result);
    UART_SendString(msg);

    // Test 2: UART_UpdateBaudRate
    UART_SendString("\r\nTest 3.2: UART_UpdateBaudRate:\r\n");
    UART_SendString("Note: Terminal will show garbled text at different baud rates!\r\n");
    UART_SendString("This is expected as your terminal stays at 115200.\r\n");

    uint32_t baud_rates[] = {9600, 19200, 38400, 57600, 115200};
    for(int i = 0; i < sizeof(baud_rates)/sizeof(baud_rates[0]); i++) {
        if(baud_rates[i] != 115200) {
            sprintf(msg, "Testing baud rate: %lu (expect garbled text)\r\n", baud_rates[i]);
        } else {
            sprintf(msg, "Testing baud rate: %lu (should be clear)\r\n", baud_rates[i]);
        }
        UART_SendString(msg);

        result = UART_UpdateBaudRate(baud_rates[i]);

        // Small delay after baud rate change
        SysTick_Delay(100);

        // Test with new baud rate (will be garbled if not 115200)
        UART_SendString("Test message at new baud rate\r\n");

        // Add longer delay for visual separation
        SysTick_Delay(500);
    }

    // Restore to 115200
    UART_UpdateBaudRate(115200);
    SysTick_Delay(100);
    UART_SendString("Restored to 115200 baud - text should be clear now\r\n");
}

void Test_InterruptFunctions(void) {
    UART_SendString("\r\n=== TESTING INTERRUPT FUNCTIONS ===\r\n");

    // Test 1: Enable interrupts
    UART_SendString("\r\nTest 4.1: Enabling UART interrupts:\r\n");

    // Store original CR1 state to restore later
    uint32_t original_cr1 = USART3->CR1;

    UART_SendString("Enabling RXNE interrupt...\r\n");
    UART_EnableInterrupts(USART_CR1_RXNEIE);

    // Show current CR1 register
    char msg[50];
    sprintf(msg, "USART3->CR1 after RXNE enable: 0x%04X\r\n", (unsigned int)USART3->CR1);
    UART_SendString(msg);

    // Don't enable TC interrupt in testing as it immediately fires and causes issues
    UART_SendString("Skipping TC interrupt test (would cause issues without active transmission)\r\n");

    // Small delay to observe any interrupt behavior
    SysTick_Delay(100);

    // Test 2: Disable interrupts
    UART_SendString("\r\nTest 4.2: Disabling UART interrupts:\r\n");

    UART_SendString("Disabling all interrupts...\r\n");

    // Disable all interrupts
    USART3->CR1 &= ~(USART_CR1_RXNEIE | USART_CR1_TCIE | USART_CR1_TXEIE);

    // Disable NVIC for USART3
    NVIC_DisableIRQ(USART3_IRQn);

    sprintf(msg, "USART3->CR1 after disabling: 0x%04X\r\n", (unsigned int)USART3->CR1);
    UART_SendString(msg);

    // Restore original state
    USART3->CR1 = original_cr1;
    UART_SendString("Original CR1 restored\r\n");

    UART_SendString("Interrupt test completed successfully!\r\n");
}

void Test_ConfigurationFunction(void) {
    UART_SendString("\r\n=== TESTING UART_InitConfig ===\r\n");

    UART_SendString("\r\nTest 5.1: Various UART configurations:\r\n");
    UART_SendString("Note: Some configurations won't show test messages if TX is disabled.\r\n\n");

    // Save the current working state (with both TX and RX enabled)
    uint32_t saved_cr1 = USART3->CR1;
    uint32_t saved_brr = USART3->BRR;

    // Test different configurations
    UART_Config configs[] = {
        // Configuration 1: Standard TX/RX at 9600
        {.baudRate = 9600, .wordLength = UART_WORDLENGTH_8B, .stopBits = UART_STOPBITS_1,
         .parity = UART_PARITY_NONE, .mode = UART_MODE_TX_RX, .oversampling = UART_OVERSAMPLING_16,
         .hwFlowControl = UART_HWCONTROL_NONE},

        // Configuration 2: TX only with 9 bit, even parity
        {.baudRate = 115200, .wordLength = UART_WORDLENGTH_9B, .stopBits = UART_STOPBITS_2,
         .parity = UART_PARITY_EVEN, .mode = UART_MODE_TX, .oversampling = UART_OVERSAMPLING_8,
         .hwFlowControl = UART_HWCONTROL_RTS},

        // Configuration 3: RX only - most problematic
        {.baudRate = 57600, .wordLength = UART_WORDLENGTH_8B, .stopBits = UART_STOPBITS_1_5,
         .parity = UART_PARITY_ODD, .mode = UART_MODE_RX, .oversampling = UART_OVERSAMPLING_16,
         .hwFlowControl = UART_HWCONTROL_CTS}
    };

    for(int i = 0; i < sizeof(configs)/sizeof(configs[0]); i++) {
        // First, restore to default working state
        UART_Init(115200);

        // Now show what we're about to test
        char msg[100];
        sprintf(msg, "\r\nTesting configuration %d:\r\n", i+1);
        UART_SendString(msg);

        // Show configuration details
        sprintf(msg, "  Baud: %lu, Word: %d, Stop: %d, Parity: %d\r\n",
                configs[i].baudRate, configs[i].wordLength, configs[i].stopBits, configs[i].parity);
        UART_SendString(msg);

        sprintf(msg, "  Mode: %d (", configs[i].mode);
        UART_SendString(msg);
        if(configs[i].mode == UART_MODE_TX) UART_SendString("TX only");
        else if(configs[i].mode == UART_MODE_RX) UART_SendString("RX only");
        else if(configs[i].mode == UART_MODE_TX_RX) UART_SendString("TX+RX");
        UART_SendString("), Oversample: ");
        // Replace the UART_SendByte line with:
        UART_SendString(configs[i].oversampling == UART_OVERSAMPLING_8 ? "8" : "16");
        UART_SendString("x, Flow: ");
        switch(configs[i].hwFlowControl) {
            case UART_HWCONTROL_NONE: UART_SendString("None"); break;
            case UART_HWCONTROL_RTS: UART_SendString("RTS"); break;
            case UART_HWCONTROL_CTS: UART_SendString("CTS"); break;
            case UART_HWCONTROL_RTS_CTS: UART_SendString("RTS+CTS"); break;
        }
        UART_SendString("\r\n");

        // If this is RX-only, warn the user
        if(configs[i].mode == UART_MODE_RX) {
            UART_SendString("  WARNING: RX-only mode will disable transmission!\r\n");
        }

        // Apply configuration
        UART_Error result = UART_InitConfig(&configs[i]);
        if(result != UART_OK) {
            // Can't send error message if TX is disabled, so restore first
            UART_Init(115200);
            sprintf(msg, "  Configuration failed with error: %d\r\n", result);
            UART_SendString(msg);
            continue;
        }

        // Test the configuration if possible
        if(configs[i].mode & UART_MODE_TX) {
            // Can send test message
            if(configs[i].baudRate == 115200) {
                UART_SendString("  Config applied - this text should be clear\r\n");
            } else {
                // Different baud rate - will appear garbled
                UART_SendString("  Config applied - expect garbled text at different baud\r\n");
                SysTick_Delay(200);  // Give time for transmission at wrong baud
            }
        } else {
            // RX-only mode - can't send anything
            // Just wait a bit to ensure configuration is stable
            SysTick_Delay(500);

            // Restore TX temporarily to send completion message
            UART_Init(115200);
            UART_SendString("  RX-only configuration applied successfully (restored to TX+RX for status)\r\n");
        }

        SysTick_Delay(500);
    }

    // Always restore default configuration
    UART_Init(115200);
    UART_SendString("\r\nAll configuration tests completed!\r\n");
    UART_SendString("Default configuration (115200-8-N-1) restored\r\n");
}

void ShowTestMenu(void) {
    UART_SendString("\r\n=== COMPREHENSIVE UART TEST SUITE ===\r\n");
    UART_SendString("Select a test:\r\n");
    UART_SendString("1 - Run diagnostics\r\n");
    UART_SendString("2 - Test basic functions\r\n");
    UART_SendString("3 - Test advanced functions\r\n");
    UART_SendString("4 - Test error handling\r\n");
    UART_SendString("5 - Test interrupt functions\r\n");
    UART_SendString("6 - Test UART_InitConfig\r\n");
    UART_SendString("7 - Run ALL tests\r\n");
    UART_SendString("8 - Performance test\r\n");
    UART_SendString("9 - Exit\r\n");
    UART_SendString("Choice: ");
}

void RunPerformanceTest(void) {
    UART_SendString("\r\n=== PERFORMANCE TEST ===\r\n");

    // Test transmission speed
    UART_SendString("\r\nTest 8.1: Transmission speed test:\r\n");

    char large_data[1000];
    for(int i = 0; i < 999; i++) {
        large_data[i] = 'A' + (i % 26);
    }
    large_data[999] = '\0';

    uint32_t start_time = systick_counter;
    UART_Error result = UART_Transmit(large_data, 1000, 5000);
    uint32_t end_time = systick_counter;

    char msg[50];
    sprintf(msg, "Transmitted 1000 bytes in %lu ms\r\n", end_time - start_time);
    UART_SendString(msg);

    // Calculate throughput
    if(result == UART_OK) {
        uint32_t bytes_per_sec = 1000 * 1000 / (end_time - start_time);
        sprintf(msg, "Throughput: %lu bytes/second\r\n", bytes_per_sec);
        UART_SendString(msg);
    }

    // Test with different chunk sizes
    UART_SendString("\r\nTest 8.2: Different chunk sizes:\r\n");
    uint16_t chunk_sizes[] = {1, 10, 50, 100, 500};

    for(int i = 0; i < sizeof(chunk_sizes)/sizeof(chunk_sizes[0]); i++) {
        sprintf(msg, "Testing %d byte chunks:\r\n", chunk_sizes[i]);
        UART_SendString(msg);

        start_time = systick_counter;
        for(int j = 0; j < 100; j++) {
            result = UART_Transmit(large_data, chunk_sizes[i], 1000);
            if(result != UART_OK) break;
        }
        end_time = systick_counter;

        if(result == UART_OK) {
            sprintf(msg, "100 chunks of %d bytes: %lu ms\r\n", chunk_sizes[i], end_time - start_time);
            UART_SendString(msg);
        }
    }
}
int test_main(void)
{
    /* Initialize SysTick and UART */
    SysTick_Init();
    UART_Init(115200);

    // Initial system status
    UART_SendString("\r\n\r\n");
    UART_SendString("===================================\r\n");
    UART_SendString("STM32F429ZI UART DRIVER TEST SUITE\r\n");
    UART_SendString("===================================\r\n");
    UART_SendString("SysTick and UART initialized!\r\n");

    // Show the menu once initially
    ShowTestMenu();

    while(1)
    {
    	// Flush any pending characters before checking for new input
    	    FlushInputBuffer();
        // Add a small delay to prevent continuous menu printing
        SysTick_Delay(10);  // 10ms delay

        if (UART_IsDataAvailable()) {
            char choice = UART_ReceiveByte();
            UART_SendByte(choice);  // Echo choice
            UART_SendString("\r\n");

            switch(choice) {
                case '1':
                    UART_ComprehensiveDiagnostics();
                    break;

                case '2':
                    Test_BasicFunctions();
                    break;

                case '3':
                    Test_AdvancedFunctions();
                    break;

                case '4':
                    Test_ErrorHandling();
                    break;

                case '5':
                    Test_InterruptFunctions();
                    break;

                case '6':
                    Test_ConfigurationFunction();
                    break;

                case '7':
                                UART_SendString("\r\n=== RUNNING ALL TESTS ===\r\n");
                                UART_ComprehensiveDiagnostics();
                                Test_BasicFunctions();
                                Test_AdvancedFunctions();
                                Test_ErrorHandling();
                                Test_InterruptFunctions();
                                Test_ConfigurationFunction();
                                RunPerformanceTest();
                                UART_SendString("\r\n=== ALL TESTS COMPLETED ===\r\n");

                                // Make sure all interrupts are disabled after testing
                                USART3->CR1 &= ~(USART_CR1_RXNEIE | USART_CR1_TCIE | USART_CR1_TXEIE);
                                NVIC_DisableIRQ(USART3_IRQn);
                                UART_SendString("All interrupts properly disabled\r\n");
                                break;

                case '8':
                    RunPerformanceTest();
                    break;

                case '9':
                    UART_SendString("Exiting test suite. Goodbye!\r\n");
                    while(1);  // Stop here

                default:
                    UART_SendString("Invalid choice. Please try again.\r\n");
                    break;
            }

            UART_SendString("\r\nPress any key to continue...");
                        while(!UART_IsDataAvailable()) {
                            SysTick_Delay(10);  // Add delay here too
                        }
                        UART_ReceiveByte();  // Clear the character

                        // Flush any remaining characters in the buffer
                        FlushInputBuffer();

                        // Show menu again after test is complete
                        ShowTestMenu();
        }
    }
}
