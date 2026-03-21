#include <stdio.h>
#include "ring_buffer.h"
#include "process.h"

#define MAX_SIZE 20


void test_ring_buffer(void)
{
    printf("=== Ring Buffer Test Start ===\n");

    #define TEST_SIZE 5
    char mem[TEST_SIZE];
    r_buffer rb;

    if (ring_buffer_init(&rb, mem, TEST_SIZE) != 0) {
        printf("Init failed\n");
        return;
    }

    /* Test 1: Fill buffer */
    printf("Test 1: Fill buffer\n");
    for (int i = 0; i < TEST_SIZE; i++) {
        if (ring_buffer_push(&rb, 'A' + i) != 0) {
            printf("Unexpected overflow at i=%d\n", i);
        }
    }

    /* Test 2: Overflow */
    if (ring_buffer_push(&rb, 'Z') == 0) {
        printf("ERROR: Overflow not detected\n");
    } else {
        printf("Overflow correctly detected\n");
    }

    /* Test 3: Pop all */
    printf("Test 3: Pop all\n");
    for (int i = 0; i < TEST_SIZE; i++) {
        char ch;
        if (ring_buffer_pop(&rb, &ch) == 0) {
            printf("Popped: %c\n", ch);
        } else {
            printf("Unexpected underflow at i=%d\n", i);
        }
    }

    /* Test 4: Underflow */
    char ch;
    if (ring_buffer_pop(&rb, &ch) == 0) {
        printf("ERROR: Underflow not detected\n");
    } else {
        printf("Underflow correctly detected\n");
    }

    printf("Test 5: Wrap-around test\n");

/* Fill */
for (int i = 0; i < TEST_SIZE; i++)
    ring_buffer_push(&rb, '0' + i);

/* Pop 2 */
for (int i = 0; i < 2; i++)
    ring_buffer_pop(&rb, &ch);

/* Push 2 more */
ring_buffer_push(&rb, 'X');
ring_buffer_push(&rb, 'Y');

/* Pop remaining */
while (ring_buffer_pop(&rb, &ch) == 0){
    printf("Popped: %c\n", ch);
}
    printf("=== Ring Buffer Test End ===\n");
}


int main(void)
{
    test_ring_buffer();
    return 0;
}
