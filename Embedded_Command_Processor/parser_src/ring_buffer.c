/**
 * @file ring_buffer.c
 * @author Likith025
 * @date April 2026
 * @brief Ring Buffer (Circular Queue) Implementation for Embedded Systems
 *
 * @details
 * This module provides a memory-efficient circular queue implementation suitable for
 * embedded systems with limited resources. The ring buffer enables efficient producer
 * (UART RX interrupt) / consumer (command processor) patterns without dynamic memory
 * allocation or data copying.
 *
 * **Key Design Principles:**
 * - Zero dynamic allocation (uses external memory)
 * - O(1) constant-time push/pop operations
 * - Automatic circular wraparound
 * - Interrupt-safe single-producer/consumer pattern
 * - Minimal overhead (3 bytes per operation)
 * - Predictable memory usage and timing
 *
 * **Data Flow Pattern:**
 * ```
 * ┌─────────────────────────────────┐
 * │   UART RX Interrupt Handler     │
 * │  (Producer - calls push)        │
 * │  ↓                              │
 * │  ring_buffer_push()             │
 * │      ↓                          │
 * │   Ring Buffer (Fixed Size)      │
 * │      ↓                          │
 * │  ring_buffer_pop()              │
 * │  ↑                              │
 * │   Main Command Processor        │
 * │  (Consumer - calls pop)         │
 * └─────────────────────────────────┘
 * ```
 *
 * @note
 * - Each function operates on a single element only
 * - Application must handle batching/processing
 * - Ring buffer does not interpret data (raw byte level)
 * - No error recovery (overflow = data loss)
 *
 * @attention
 * - External buffer must be provided and remain valid
 * - Not suitable for multi-producer/consumer (use locks)
 * - Overflow handling is application responsibility
 * - Must call init before any push/pop
 *
 * @warning
 * - Data loss occurs silently on overflow (push when full)
 * - No recovery mechanism for lost data
 * - Buffer memory corruption on invalid operations
 * - No overflow notifications
 *
 * @copyright 2026 Likith025 - MIT License
 * @see ring_buffer.h - Data structure and API definitions
 */

#include "../parser_inc/ring_buffer.h"
#include <stdio.h>

/**
 * @defgroup RING_BUFFER_IMPL Ring Buffer Implementation
 * @{
 */

/**
 * @brief Initialize ring buffer with external memory
 * @param[in,out] rb1 Pointer to r_buffer structure to initialize
 * @param[in] rx_buffer Pointer to character array to use as storage
 * @param[in] max_size Capacity of buffer in bytes
 * @retval int 0 on success, -1 if parameters invalid
 *
 * @details
 * Initializes a ring buffer structure by:
 * 1. Validating all input parameters
 * 2. Initializing head and tail pointers to 0 (empty state)
 * 3. Storing buffer pointer and maximum size
 * 4. Preparing the buffer for first use
 *
 * The function ensures the buffer is in a known initial state with both read and
 * write pointers at position 0, indicating an empty buffer.
 *
 * **Validation Checks:**
 * - rb1 must not be NULL (required for output)
 * - rx_buffer must not be NULL (required backing memory)
 * - max_size must be greater than 0 (at least 1 byte capacity)
 *
 * **Initialization Sequence:**
 * ```c
 * 1. Validate: rb1 != NULL && rx_buffer != NULL && max_size > 0
 * 2. Set rb1->head = 0
 * 3. Set rb1->tail = 0
 * 4. Set rb1->max_size = max_size
 * 5. Set rb1->buffer = rx_buffer
 * 6. Return 0 (success)
 * ```
 *
 * **Memory Layout After Initialization:**
 * ```
 * r_buffer structure:           External buffer array:
 * ┌──────────────────────┐     ┌──────────────────┐
 * │ buffer ────────────┐ │     │ buffer[0] = 0    │
 * │ head = 0           │ │     │ buffer[1] = 0    │
 * │ tail = 0           │ │     │ ...              │
 * │ max_size = 50      │ │     │ buffer[49] = 0   │
 * └──────────┬──────────┘ │     └──────────────────┘
 *            └────────────┴──→  (external memory)
 * ```
 *
 * @param[in,out] rb1 Ring buffer structure:
 *   - Must be a valid pointer to r_buffer
 *   - All fields will be initialized
 *   - Typically on stack: `r_buffer rb;`
 *   - Or dynamically allocated: `r_buffer *rb = malloc(...);`
 *
 * @param[in] rx_buffer External memory buffer:
 *   - Must point to valid character array
 *   - Size must be >= max_size bytes
 *   - Example: `char buffer[50]; ring_buffer_init(&rb, buffer, 50);`
 *   - Must remain valid during entire ring buffer lifetime
 *   - Application manages allocation/deallocation
 *
 * @param[in] max_size Buffer capacity:
 *   - Number of bytes available in rx_buffer
 *   - Range: 1 to 255 (uint8_t limitation)
 *   - Typical values: 30-100 bytes for embedded systems
 *   - Larger buffers reduce overflow frequency but use more RAM
 *
 * @return Initialization result:
 *   - 0: Initialization successful, buffer ready for use
 *   - -1: Initialization failed due to invalid parameters
 *
 * @note
 * - After successful init, buffer is in empty state (head == tail == 0)
 * - No data is cleared; only pointers initialized
 * - Can be called multiple times to reset buffer to empty
 * - Very fast operation (< 1 µs, constant time)
 * - Safe to call from any context (no interrupts disabled)
 *
 * **Typical Usage Examples:**
 * ```c
 * // Stack allocation (most common for embedded)
 * r_buffer cmd_buffer;
 * char buffer_mem[50];
 * if (ring_buffer_init(&cmd_buffer, buffer_mem, 50) == 0) {
 *     // Buffer ready for UART RX
 * }
 *
 * // Heap allocation (less common in embedded)
 * r_buffer *pBuffer = malloc(sizeof(r_buffer));
 * char *pMem = malloc(100);
 * ring_buffer_init(pBuffer, pMem, 100);
 *
 * // Reset existing buffer
 * ring_buffer_init(&cmd_buffer, buffer_mem, 50);  // Clears all data
 * ```
 *
 * @warning
 * - Using buffer before initialization causes undefined behavior
 * - Providing false max_size causes buffer overflow
 * - Not calling init is a common error leading to crashes
 * - Max size > 255 silently truncates (uint8_t limitation)
 *
 * @attention
 * - Buffer ownership remains with application
 * - Application must ensure buffer validity
 * - Init must be called once before any push/pop
 * - Calling init resets buffer (clears queued data)
 *
 * @see ring_buffer_push(), ring_buffer_pop()
 */
int ring_buffer_init(r_buffer* rb1, char* rx_buffer, uint8_t max_size) {
    // Validate input parameters
    if (rb1 == NULL || rx_buffer == NULL || max_size == 0)
        return -1;

    // Initialize buffer structure
    rb1->head = 0;              // Write position = start
    rb1->tail = 0;              // Read position = start
    rb1->max_size = max_size;   // Store capacity
    rb1->buffer = rx_buffer;    // Store buffer pointer

    return 0;  // Success
}

/**
 * @brief Add one character to ring buffer (enqueue/push operation)
 * @param[in,out] rb1 Pointer to initialized r_buffer structure
 * @param[in] ch Character to add (0-255, any byte value)
 * @retval int 0 on success, -1 if buffer full or parameters invalid
 *
 * @details
 * Adds a single character to the ring buffer at the current head position.
 * This is the primary function for writing data to the buffer, typically called
 * from UART receive interrupt handlers.
 *
 * **Push Algorithm:**
 * ```
 * 1. Validate parameters (rb1 != NULL, buffer != NULL)
 * 2. Calculate next head position: next_head = (head + 1) mod max_size
 * 3. Check for full condition: if next_head == tail, buffer is full
 *    - Return -1 without modifying buffer
 * 4. Write character to buffer[head]
 * 5. Update head pointer: head = next_head
 * 6. Return 0 (success)
 * ```
 *
 * **Full Buffer Detection:**
 * The ring buffer reserves one slot (never fills completely) to distinguish
 * between empty (head == tail) and full ((head+1) % max == tail) states.
 *
 * **Wraparound Behavior:**
 * ```
 * Example 1 (Normal):
 *   head=2, max=8, next_head=3 (no wrap)
 *
 * Example 2 (Wraparound):
 *   head=7, max=8, next_head=0 (wrap from 7 to 0)
 *
 * Example 3 (Full, can't push):
 *   head=7, tail=0, max=8
 *   next_head=0, but 0==tail, so full! Return -1
 * ```
 *
 * **State Diagram:**
 * ```
 * EMPTY State           NORMAL State          FULL State
 * head=3                head=5                head=4
 * tail=3                tail=2                tail=5 (next after push)
 * (can push)            (can push)            (cannot push, would wrap to tail)
 * ```
 *
 * **Interrupt Safety:**
 * - Designed to be called from ISR (UART RX handler)
 * - Single atomic operation (write + update)
 * - No blocking, very fast (~3-5 µs)
 * - Suitable for real-time constraints
 * - Safe alongside pop() in main loop (interrupt-safe pattern)
 *
 * @param[in,out] rb1 Ring buffer structure:
 *   - Must be initialized via ring_buffer_init()
 *   - head pointer will be updated on success
 *   - buffer and max_size must be valid
 *
 * @param[in] ch Character/byte to enqueue:
 *   - Any 8-bit value: 0-255
 *   - No validation (any value accepted)
 *   - Often from UART_RDR (RX data register)
 *   - Example: received UART byte, ASCII character, binary data
 *
 * @return Push status:
 *   - 0: Character successfully added to buffer
 *   - -1: Buffer full (character NOT added) OR parameter error
 *
 * @note
 * - Called in UART RX interrupt for each received byte
 * - Fast enough for high-speed UART (115200+ baud)
 * - Very simple algorithm suitable for ISR
 * - Does not perform any data interpretation
 * - Automatic wraparound handled internally
 *
 * **Typical ISR Usage:**
 * ```c
 * // In UART3 RX interrupt handler
 * void USART3_IRQHandler(void) {
 *     uint8_t rx_data = USART3->RDR & 0xFF;  // Read from hardware
 *
 *     if (ring_buffer_push(&cmd_buffer, rx_data) == 0) {
 *         // Byte queued successfully
 *     } else {
 *         // Buffer full - data lost!
 *         // Options:
 *         // 1. Drop (silent data loss)
 *         // 2. Toggle error LED
 *         // 3. Clear buffer and start over
 *         // 4. Send XOFF flow control
 *     }
 * }
 * ```
 *
 * **Overflow Handling Strategy:**
 * ```c
 * static uint32_t overflow_count = 0;
 *
 * if (ring_buffer_push(&rb, byte) == 0) {
 *     // Success
 * } else {
 *     overflow_count++;
 *     // Strategy 1: Drop this byte (data loss)
 *     // Strategy 2: Drop oldest byte and retry
 *     if (ring_buffer_pop(&rb, &dummy) == 0) {
 *         ring_buffer_push(&rb, byte);  // Retry
 *     }
 *     // Strategy 3: Signal error
 *     error_flag = 1;
 * }
 * ```
 *
 * @warning
 * - Data loss occurs silently when buffer is full
 * - No notification or error flag set
 * - Continuous overflow causes complete data corruption
 * - Must monitor pop() rate vs push() rate in system
 *
 * @attention
 * - Check return value to handle buffer full condition
 * - ISR should handle overflow gracefully
 * - System design must ensure pop() rate > push() rate on average
 * - Consider increasing buffer size if frequent overflows
 *
 * @see ring_buffer_pop(), ring_buffer_init()
 */
int ring_buffer_push(r_buffer* rb1, char ch) {
    uint8_t next_head;

    // Validate parameters
    if (rb1 == NULL || rb1->buffer == NULL)
        return -1;

    // Calculate next head position with wraparound
    next_head = (rb1->head + 1);

    // Apply wraparound: go back to 0 when reaching max_size
    if (next_head >= rb1->max_size) {
        next_head = 0;
    }

    // Check if buffer is full (next position would be read pointer)
    if (next_head == rb1->tail) {
        return -1;  // Buffer full - cannot add data
    }

    // Add character to buffer and advance head pointer
    rb1->buffer[rb1->head] = ch;      // Write byte at current head
    rb1->head = next_head;             // Update head for next push

    return 0;  // Success
}

/**
 * @brief Remove one character from ring buffer (dequeue/pop operation)
 * @param[in,out] rb1 Pointer to initialized r_buffer structure
 * @param[out] op_ch Pointer to char to receive dequeued character
 * @retval int 0 on success, -1 if buffer empty or parameters invalid
 *
 * @details
 * Removes and returns a single character from the ring buffer at the current tail
 * position. This is the primary function for reading data from the buffer, typically
 * called from command processing loop.
 *
 * **Pop Algorithm:**
 * ```
 * 1. Validate parameters (rb1 != NULL, buffer != NULL, op_ch != NULL)
 * 2. Check for empty condition: if head == tail, buffer is empty
 *    - Return -1 without modifying output
 * 3. Read character from buffer[tail]
 * 4. Calculate next tail: next_tail = (tail + 1) mod max_size
 * 5. Update tail pointer: tail = next_tail
 * 6. Store character in *op_ch
 * 7. Return 0 (success)
 * ```
 *
 * **Empty Buffer Detection:**
 * The buffer is empty when head == tail (both at same position).
 * In this state, pop() returns -1 and does not modify the output pointer.
 *
 * **Wraparound Behavior:**
 * ```
 * Example 1 (Normal):
 *   tail=2, max=8, next_tail=3 (no wrap)
 *
 * Example 2 (Wraparound):
 *   tail=7, max=8, next_tail=0 (wrap from 7 to 0)
 *
 * Example 3 (Empty, can't pop):
 *   head=3, tail=3 (both equal)
 *   Return -1, no state change
 * ```
 *
 * **State Diagram:**
 * ```
 * EMPTY State           NORMAL State          SINGLE ITEM State
 * head=3                head=5                head=4
 * tail=3                tail=2                tail=3
 * (cannot pop)          (can pop)             (1 item, can pop)
 * ```
 *
 * **Non-Blocking Behavior:**
 * - Always returns immediately (no wait)
 * - Returns -1 if no data available
 * - Suitable for polling or interrupt-driven code
 * - No timeout or retry mechanism
 *
 * **Data Order Preservation:**
 * - FIFO (First In, First Out) queue semantics
 * - Bytes pop in same order as push
 * - Queue naturally serializes concurrent access
 * - Suitable for UART command streams
 *
 * @param[in,out] rb1 Ring buffer structure:
 *   - Must be initialized via ring_buffer_init()
 *   - tail pointer will be updated on success
 *   - buffer and max_size must be valid
 *
 * @param[out] op_ch Pointer to character variable:
 *   - Will receive dequeued byte (on success)
 *   - Must not be NULL
 *   - Must point to writable memory
 *   - Unchanged on error (-1 return)
 *
 * @return Pop status:
 *   - 0: Character successfully dequeued (valid data in *op_ch)
 *   - -1: Buffer empty (no data available) OR parameter error
 *
 * @note
 * - Called in main loop or command processing thread
 * - Safe to call repeatedly even when empty (just returns -1)
 * - Very fast operation (~3-5 µs)
 * - Does not modify any data except tail pointer
 * - Automatic wraparound handled internally
 *
 * **Typical Main Loop Usage:**
 * ```c
 * char received_byte;
 * while (1) {
 *     if (ring_buffer_pop(&cmd_buffer, &received_byte) == 0) {
 *         // Process byte
 *         printf("Got: %c\n", received_byte);
 *     } else {
 *         // No data, do other work
 *         process_timeout();
 *     }
 * }
 * ```
 *
 * **Continuous Drain Pattern:**
 * ```c
 * char byte;
 * // Drain all available bytes
 * while (ring_buffer_pop(&rb, &byte) == 0) {
 *     handle_byte(byte);
 * }
 * // After loop: buffer is empty (head == tail)
 * ```
 *
 * @warning
 * - Do not use *op_ch before checking return value
 * - NULL op_ch pointer causes undefined behavior
 * - Cannot distinguish NULL pointer error from empty buffer
 * - Data loss if pop() rate < push() rate (buffer overflows)
 *
 * @attention
 * - Check return value before using op_ch
 * - Safe for typical interrupt-driven RX pattern
 * - System must ensure adequate processing rate
 * - Consider latency if processing is slow
 *
 * @see ring_buffer_push(), ring_buffer_init()
 */
int ring_buffer_pop(r_buffer* rb1, char* op_ch) {
    // Validate parameters
    if (rb1 == NULL || rb1->buffer == NULL || op_ch == NULL)
        return -1;

    // Check if buffer is empty (no data to read)
    if (rb1->head == rb1->tail) {
        return -1;  // Buffer empty
    }

    // Read character from buffer at tail position
    *op_ch = rb1->buffer[rb1->tail];

    // Update tail pointer to next position with wraparound
    rb1->tail = (rb1->tail + 1);
    if (rb1->tail >= rb1->max_size) {
        rb1->tail = 0;  // Wrap back to start
    }

    return 0;  // Success
}

/** @} */ // end of RING_BUFFER_IMPL
