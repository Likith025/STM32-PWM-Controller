#include <stdio.h>
#include <string.h>
#include "ring_buffer.h"
#include "process.h"

#define TEST_BUFFER_SIZE 32

typedef struct {
    const char *name;
    const char *input;
} direct_test_case;

typedef struct {
    const char *name;
    const char *chunks[3];
} extract_test_case;

static void feed_input(r_buffer *rb, const char *input)
{
    size_t i;

    for (i = 0; input[i] != '\0'; i++) {
        if (ring_buffer_push(rb, input[i]) != 0) {
            printf("ring buffer overflow while pushing '%c'\n", input[i]);
            return;
        }
    }

    extract_cmd(rb);
}

static void run_direct_tests(void)
{
    static const direct_test_case tests[] = {
        {"known command: led on", "led on"},
        {"known command: led off", "led off"},
        {"known command: display status", "display status"},
        {"unknown command", "invalid cmd"}
    };
    size_t i;

    printf("=== direct process_cmd tests ===\n");
    for (i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {
        printf("[%u] %s\n", (unsigned)(i + 1), tests[i].name);
        printf("input: \"%s\"\n", tests[i].input);
        process_cmd((char *)tests[i].input);
        printf("\n");
    }
}

static void run_extract_tests(r_buffer *rb)
{
    static const extract_test_case tests[] = {
        {"single chunk: led on", {"led on\n", NULL, NULL}},
        {"single chunk: led off", {"led off\n", NULL, NULL}},
        {"single chunk: display status", {"display status\n", NULL, NULL}},
        {"single chunk: unknown command", {"abc\n", NULL, NULL}},
        {"split command across two chunks", {"led", " on\n", NULL}},
        {"two partial chunks before newline", {"display ", "status", "\n"}}
    };
    size_t i;
    size_t j;

    printf("=== extract_cmd tests ===\n");
    for (i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {
        printf("[%u] %s\n", (unsigned)(i + 1), tests[i].name);
        for (j = 0; j < 3; j++) {
            if (tests[i].chunks[j] == NULL) {
                break;
            }
            printf("chunk %u: \"%s\"\n", (unsigned)(j + 1), tests[i].chunks[j]);
            feed_input(rb, tests[i].chunks[j]);
        }
        printf("\n");
    }
}

int main(void)
{
    char storage[TEST_BUFFER_SIZE];
    r_buffer rb;

    if (ring_buffer_init(&rb, storage, TEST_BUFFER_SIZE) != 0) {
        printf("ring buffer init failed\n");
        return 1;
    }

    printf("host-side command parser test\n\n");
    run_direct_tests();
    run_extract_tests(&rb);

    return 0;
}
