# stm32f429-iot-module

Bare-metal STM32F429ZI IoT device with sensor integration, task scheduling, and Wi-Fi cloud connectivity. Currently implementing UART driver and task scheduler foundation.
Current Status

✅ Phase 1.1: Foundation Setup

Basic GPIO control (LED blinking)
Hardware validation complete


✅ Phase 1.2: UART Communication System

UART driver implemented (115200 baud)
Bidirectional communication tested
Debug output working


🚧 Phase 1.3: Task Scheduler (Next)

SysTick-based timing
Task registration
Priority execution



Hardware

MCU: STM32F429ZI
Board: NUCLEO-F429ZI
UART: USART3 (PD8/PD9) - 115200 baud
Clock: 16MHz HSI

Quick Start

Clone repository
Open in STM32CubeIDE
Build and flash to NUCLEO-F429ZI
Connect serial terminal (115200 baud)
Type characters to test echo

Current Files
Core/
├── Inc/
│   ├── uart.h        # UART communication
│   └── systick.h     # Timing functions
└── Src/
    ├── main.c        # Main application
    ├── uart.c        # UART implementation
    └── systick.c     # SysTick implementation
Next Steps

Implement task scheduler
Add sensor integration
Wi-Fi connectivity
Cloud communication
