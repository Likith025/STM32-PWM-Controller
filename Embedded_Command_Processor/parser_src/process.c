/**
 * @file process.c
 * @author Likith025
 * @date April 2026
 * @brief Command Processing Engine for Embedded CLI
 *
 * @details
 * This module implements the command parser and processor for the embedded command-line
 * interface. It extracts commands from the ring buffer, parses them, and executes
 * appropriate GPIO operations.
 *
 * **Command Set:**
 * - "led on": Turn on LED (GPIO set high)
 * - "led off": Turn off LED (GPIO set low)
 * - "status": Query current LED status (on/off)
 * - "cmd not found": Echo for unrecognized commands
 *
 * **Processing Flow:**
 * ```
 * 1. UART receives characters → stored in ring buffer
 * 2. extract_cmd() reads from ring buffer until '\r' terminator
 * 3. Command complete (null-terminated string ready)
 * 4. process_cmd() matches command string
 * 5. Execute corresponding GPIO operation
 * 6. Send response via UART
 * ```
 *
 * **Command Format:**
 * - Received as ASCII strings over UART/USART3
 * - Terminated with '\r' (carriage return, 0x0D)
 * - Parsed as space-separated command tokens
 * - Case-sensitive matching
 * - Maximum length: CMD_MAX (typically 15 bytes)
 *
 * @note
 * - Commands are stateless (no persistent state between commands)
 * - LED status tracked in static variable within process_cmd()
 * - Response messages sent via USART3 interrupt-driven TX
 * - Non-blocking: commands executed immediately
 *
 * @attention
 * - Ring buffer must be pre-initialized with data
 * - USART3 must be configured for TX/RX before use
 * - GPIO for LED must be configured (typically GPIO_B pin 14)
 * - Interrupt handlers must be properly linked
 *
 * @copyright 2026 Likith025 - MIT License
 * @see ring_buffer.h - Ring buffer data structure
 * @see stm32f7xx_uart_driver.h - UART communication
 */

#include "../parser_inc/process.h"

/**
 * @defgroup CMD_PROCESSING Command Processing Functions
 * @{
 */

/**
 * @brief Extract command from ring buffer until terminator
 * @param[in,out] rb Pointer to initialized ring buffer
 * @retval void
 *
 * @details
 * Extracts characters from the ring buffer one by one, building a command string
 * until a carriage return ('\r', 0x0D) is encountered. When complete, the command
 * is null-terminated and passed to process_cmd() for execution.
 *
 * **Extraction Algorithm:**
 * ```
 * 1. Loop: while ring_buffer_pop() succeeds
 * 2. Read character from ring buffer
 * 3. Check for terminator: if ch == '\r'
 *    - Null-terminate command string
 *    - Reset buffer index to 0
 *    - Call process_cmd() to handle command
 *    - Break loop
 * 4. Otherwise, store character in cmd_buffer[idx]
 * 5. Guard against buffer overflow: if idx >= CMD_MAX-1
 *    - Reset idx to 0 (discard overflowed command)
 *    - Continue
 * 6. Increment idx
 * 7. Return when ring buffer empty
 * ```
 *
 * **Command Buffer State:**
 * - Static buffer preserves command across function calls
 * - Static index tracks position in buffer
 * - Resets on complete command or overflow
 * - Accumulates characters until terminator found
 *
 * **Terminator Recognition:**
 * - Carriage Return ('\r', ASCII 0x0D) marks end of command
 * - Not included in command string (before null-terminator)
 * - Typical UART line ending from terminal
 * - Some terminals send "\r\n" (both CR and LF)
 *
 * **Buffer Overflow Handling:**
 * - If command exceeds CMD_MAX-1 bytes (typically 14)
 * - Buffer index resets to 0 (command discarded)
 * - Continues accepting new input
 * - Long commands silently dropped (no error message)
 * - Prevents buffer overflow and data corruption
 *
 * **Typical Command Sequence:**
 * ```
 * Input:     "led on\r"
 * Characters received: 'l', 'e', 'd', ' ', 'o', 'n', '\r'
 * cmd_buffer after: "led on" (null-terminated)
 * process_cmd() called with: "led on"
 * Output:    "led on\n\r" sent via UART
 * ```
 *
 * @param[in,out] rb Ring buffer containing UART-received characters:
 *   - Must be initialized via ring_buffer_init()
 *   - Characters added by UART RX interrupt handler
 *   - Modified: tail pointer advanced as bytes extracted
 *
 * @note
 * - Uses static variables to maintain state between calls
 * - Can be called repeatedly from main loop
 * - Non-blocking: returns immediately if buffer empty
 * - Typically called continuously in main loop
 * - One command fully extracted per complete cycle (when '\r' found)
 *
 * **State Machine:**
 * ```
 * IDLE → ACCUMULATING → COMPLETE → IDLE
 *   ↓        ↓             ↓
 *   (no chars) (collecting) (found \r, process, reset)
 * ```
 *
 * @example
 * // Main loop - call repeatedly
 * while (1) {
 *     extract_cmd(&ring_buffer);  // Extract and process any available command
 * }
 *
 * // Or with other processing
 * while (1) {
 *     extract_cmd(&ring_buffer);  // Check for command
 *     do_other_work();            // Do background tasks
 * }
 *
 * @warning
 * - Static buffer assumes single command stream
 * - Not re-entrant (do not call from multiple contexts)
 * - Commands longer than CMD_MAX are discarded
 * - No error indication for discarded commands
 * - Index can get out of sync if direct buffer writes occur
 *
 * @attention
 * - Requires UART to be sending '\r' as line terminator
 * - Terminal settings should be set to send CR (not CR+LF)
 * - Commands are case-sensitive
 * - Whitespace is significant (must match exactly)
 *
 * @see process_cmd(), ring_buffer_pop()
 */
void extract_cmd(r_buffer* rb) {
    static char cmd_buffer[CMD_MAX];
    static int idx = 0;
    char ch;

    // Keep extracting while characters available
    while (ring_buffer_pop(rb, &ch) == 0) {

        // Check for command terminator
        if (ch == '\r') {
            cmd_buffer[idx] = '\0';     // Null-terminate command
            idx = 0;                     // Reset for next command
            process_cmd(cmd_buffer);     // Process complete command
            break;
        }

        // Store character if space available
        if (idx < CMD_MAX - 1) {
            cmd_buffer[idx++] = ch;
        } else {
            // Buffer overflow: discard entire command
            idx = 0;
        }
    }
}

/**
 * @brief Process and execute CLI command (PWM-based control)
 *
 * @details
 * Parses a command string received from UART and executes the corresponding
 * PWM control operation. Supports argument parsing, validation, and error handling.
 *
 * **Supported Commands:**
 * | Command        | Description                    |
 * |---------------|--------------------------------|
 * | pwm <0-100>   | Set PWM duty cycle (%)         |
 * | status        | Display current PWM value      |
 * | help          | Show available commands        |
 *
 * **Command Format:**
 * - Commands are ASCII strings received over UART
 * - Tokens are space-separated
 * - First token = command
 * - Second token (optional) = argument
 *
 * Example:
 * ```
 * pwm 50
 * status
 * help
 * ```
 *
 * **Processing Flow:**
 * ```
 * 1. Copy input string (strtok modifies input)
 * 2. Extract tokens using strtok()
 * 3. Identify command via strcmp()
 * 4. Parse arguments using strtol()
 * 5. Validate input and handle errors
 * 6. Execute PWM operation
 * 7. Send response via UART (interrupt-driven)
 * ```
 *
 * **Input Validation:**
 * - Detects missing arguments
 * - Rejects invalid numbers (e.g., "50abc")
 * - Supports negative values (clamped to 0)
 * - Limits range to 0–100%
 *
 * **State Management:**
 * - Static variable `value` stores last PWM duty cycle
 * - `initialized` tracks whether PWM was set at least once
 *
 * **Error Handling:**
 * | Condition              | Response                      |
 * |-----------------------|-------------------------------|
 * | Missing argument      | "error: missing argument"     |
 * | Invalid number        | "error: invalid number"       |
 * | Value > 100           | "error: max value is 100"     |
 * | Value < 0             | "error: min value is 0"       |
 * | Unknown command       | "error: command not found"    |
 *
 * **Output Examples:**
 * ```
 * pwm 50     → "PWM:50%"
 * status     → "PWM:50%"
 * help       → command list
 * ```
 *
 * @note
 * - Uses interrupt-driven UART transmission
 * - Non-blocking execution
 * - Not re-entrant (uses static variables)
 *
 * @warning
 * - Input string must be null-terminated
 * - Command buffer size limited to 50 bytes
 * - Multiple rapid TX calls may require queuing (future improvement)
 *
 * @see extract_cmd(), strtok(), strtol(), USART_SendData_IT()
 */
/**
 * @brief Process and execute CLI command (PWM control interface)
 * @param[in] cmd Null-terminated command string
 * @retval void
 */
void process_cmd(char* cmd)
{
    static int value = 0;        // last PWM value
    static int initialized = 0;  // whether PWM was set at least once

    // ===== Command help strings =====
    const char *help_msg =
        "Available commands:\n\r"
        "pwm <0-100>  : set duty cycle\n\r"
        "status       : show current PWM\n\r"
        "help         : show this message\n\r";

    const char *missing_arg = "error: missing argument\n\r";
    const char *invalid     = "error: invalid number\n\r";
    const char *max_limit   = "error: max value is 100\n\r";
    const char *min_limit   = "error: min value is 0\n\r";
    const char *invalid_cmd = "error: command not found\n\r";

    // ===== Safe copy (strtok modifies string) =====
    char temp[50];
    strncpy(temp, cmd, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';

    // ===== Tokenization =====
    char *token = strtok(temp, " ");
    char *args  = strtok(NULL, " ");

    // ===== PWM Command =====
    if(token && strcmp(token, "pwm") == 0)
    {
        if(args == NULL){
            USART_SendData_IT(&g_usart3, (uint8_t*)missing_arg, strlen(missing_arg));
            return;
        }

        char *endptr;
        long val = strtol(args, &endptr, 10);

        // Validate full numeric input
        if(endptr == args || *endptr != '\0'){
            USART_SendData_IT(&g_usart3, (uint8_t*)invalid, strlen(invalid));
            return;
        }

        // Clamp limits
        if(val > 100){
            USART_SendData_IT(&g_usart3, (uint8_t*)max_limit, strlen(max_limit));
            val = 100;
        }
        else if(val < 0){
            USART_SendData_IT(&g_usart3, (uint8_t*)min_limit, strlen(min_limit));
            val = 0;
        }

        value = (int)val;
        initialized = 1;

        TimerPWM_DutyCycle(&g_timer2, CH1, value);

        char msg[30];
        sprintf(msg, "PWM:%d%%\n\r", value);
        USART_SendData_IT(&g_usart3, (uint8_t*)msg, strlen(msg));
    }

    // ===== Status Command =====
    else if(token && strcmp(token, "status") == 0)
    {
        char msg[40];

        if(!initialized){
            strcpy(msg, "PWM not set\n\r");
        } else {
            sprintf(msg, "PWM:%d%%\n\r", value);
        }

        USART_SendData_IT(&g_usart3, (uint8_t*)msg, strlen(msg));
    }

    // ===== Help Command =====
    else if(token && strcmp(token, "help") == 0)
    {
        USART_SendData_IT(&g_usart3, (uint8_t*)help_msg, strlen(help_msg));
    }

    // ===== Invalid Command =====
    else
    {
        USART_SendData_IT(&g_usart3, (uint8_t*)invalid_cmd, strlen(invalid_cmd));
    }
}

/** @} */ // end of CMD_PROCESSING
