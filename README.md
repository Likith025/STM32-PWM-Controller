**Embedded Command Processor (STM32F756ZG)**

UART-based command parser running on STM32F756ZG using bare-metal drivers.
Receives ASCII commands over UART, parses them using a ring buffer, and executes corresponding hardware actions.

**Overview**
 
Interrupt-driven UART reception
Ring buffer for non-blocking data handling
Simple command parser and dispatcher
Modular structure (drivers + parser + application)

**How It Works**

UART RX interrupt stores incoming bytes into a ring buffer
extract_cmd() reads from the buffer and forms complete commands
process_cmd() parses and executes the command

**Supported Commands**

led on → Turn ON onboard LED
led off → Turn OFF onboard LED
status → Get current LED state

**Project Structure**

Embedded_Command_Processor/
├── parser_src/        # Ring buffer + command logic

├── parser_inc/        # Header files

├── stm32f7xx_driver/ # GPIO, UART drivers (submodule)

└── Src/main.c        # Application entry

**Toolchain**

STM32CubeIDE

GCC ARM Embedded

Board: STM32F756ZG Nucleo-144
