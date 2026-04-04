#include"ring_buffer.h"
#include "stm32f7xx_uart_driver.h"
#include "ring_buffer.h"
#include "peripheral_config.h"
#include <string.h>
#define CMD_MAX 15
void extract_cmd(r_buffer* rb);

void process_cmd(char* cmd);

