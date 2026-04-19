# Embedded CLI-Based PWM Control (STM32)

## Overview

This project implements a **UART-based command-line interface (CLI)** to control PWM output on an STM32 microcontroller.
It demonstrates interrupt-driven communication, command parsing, and real-time hardware control using timers.

Users can send commands over UART to dynamically adjust PWM duty cycle and query system state.

---

## Hardware Setup

* **Board**: STM32 Nucleo (F7 series)
* **UART**: USART3

  * TX: PB10
  * RX: PB11
* **Timer**: TIM2
* **PWM Channel**: Channel 1 (TIM2_CH1)
* **PWM Output Pin**: PA0

> Note: Pin mapping depends on the specific Nucleo variant.

---

## Key Features

* UART CLI using interrupt-driven RX/TX
* Real-time PWM duty cycle control (0–100%)
* Token-based command parsing (`strtok`)
* Robust numeric validation (`strtol`)
* Ring buffer for non-blocking UART input
* Immediate command execution (bare-metal)

---

## Supported Commands

| Command       | Description                |
| ------------- | -------------------------- |
| `pwm <0-100>` | Set PWM duty cycle (%)     |
| `status`      | Show current PWM value     |
| `help`        | Display available commands |

---

## Example Usage

```text id="ex1"
> pwm 50
PWM:50%

> status
PWM:50%

> pwm 150
error: max value is 100
PWM:100%

> pwm abc
error: invalid number

> help
Available commands:
pwm <0-100>  : set duty cycle
status       : show current PWM
help         : show this message
```

---

## System Architecture

```text id="arch"
UART RX Interrupt
        ↓
Ring Buffer
        ↓
extract_cmd()
        ↓
process_cmd()
   ├── strtok()  → command parsing
   ├── strtol()  → argument validation
   └── TimerPWM_DutyCycle()
        ↓
UART TX Interrupt (response)
```

---

## Project Structure

```text id="struct"
/Embedded_Command_Processor
 ├── parser_src/
 │    ├── process.c              # Command parsing & execution
 │    ├── ring_buffer.c          # UART RX buffer implementation
 │
 ├── parser_inc/
 │    ├── process.h
 │    ├── ring_buffer.h

/stm32f7xx_driver/              # STM32 driver layer (Git submodule)
 ├── Src/
 ├── Inc/

/Core
 ├── Src/
 │    ├── main.c
 │    ├── peripheral_config.c
 │
 ├── Inc/
 │    ├── peripheral_config.h

README.md
```

---

## How It Works

1. UART receives characters via interrupt
2. Data is stored in a ring buffer
3. `extract_cmd()` builds a command string until `\r`
4. `process_cmd()`:

   * Splits input into tokens
   * Validates arguments
   * Updates PWM or returns status
5. Response is sent using interrupt-driven UART TX

---

## Setup

This project uses a Git submodule for the STM32 driver layer.

After cloning, initialize the submodule:

```bash id="setup"
git submodule update --init --recursive
```

---

## Current Limitations

* No TX queue (multiple rapid responses may overlap)
* Single command execution model
* Static state (not thread-safe)
* Fixed command buffer size

---

## Future Improvements

* TX ring buffer for safe multi-message transmission
* FreeRTOS-based task separation
* Extended command support (multi-argument parsing)
* Hardware abstraction improvements

---

## Author

**Likith R**
Embedded Firmware Developer

---

## License

MIT License
