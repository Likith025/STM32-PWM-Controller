#include"ring_buffer.h"
#include "stm32f7xx_uart_driver.h"
#include "ring_buffer.h"
#include "peripheral_config.h"
#include <string.h>
#define CMD_MAX 15
void extract_cmd(r_buffer* rb);
<<<<<<< HEAD
void process_cmd(char* cmd);
=======
void process_cmd(char* cmd);

#endif
>>>>>>> d5c73c9 (USART integration - replace printf, fix terminator, add led status)
