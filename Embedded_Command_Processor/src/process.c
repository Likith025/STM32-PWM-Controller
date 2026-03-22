#include "process.h"


void extract_cmd(r_buffer* rb){
    static char cmd_buffer[CMD_MAX];
    static int idx=0;
    char ch;

            while (ring_buffer_pop(rb, &ch) == 0) {

        if (ch == '\r') {
            cmd_buffer[idx] = '\0';
            // command complete (print later)
            idx = 0;
            process_cmd(cmd_buffer);
            break;
        }

        if (idx < CMD_MAX - 1) {
            cmd_buffer[idx++] = ch;
        } else {
            // overflow: discard command
            idx = 0;
        }
    }
    
}

void process_cmd(char* cmd){
    char* cmd_list[3]={"led on","led off","status"};
    static uint8_t led_status=0;
if(strcmp(cmd_list[0],cmd)==0){
    //printf("led turned on\n");
	USART_SendData_IT(&g_usart3, (uint8_t*)"led on\n\r", sizeof("led on\n\r"));
	GPIO_WritePin(g_led3.pGPIOx, 14, ENABLE);
	led_status=ENABLE;
}
else if(strcmp(cmd_list[1],cmd)==0){
	USART_SendData_IT(&g_usart3, (uint8_t*)"led off\n\r", sizeof("led off\n\r"));
	GPIO_WritePin(g_led3.pGPIOx, 14, DISABLE);
	led_status=DISABLE;
}
else if(strcmp(cmd_list[2],cmd)==0){
	if(led_status){
	USART_SendData_IT(&g_usart3, (uint8_t*)"LED:ON\n\r", sizeof("LED:ON\n\r"));
	}
	else{
		USART_SendData_IT(&g_usart3, (uint8_t*)"LED:OFF\n\r", sizeof("LED:OFF\n\r"));
	}
}
else{
	USART_SendData_IT(&g_usart3, (uint8_t*)"cmd not found\n\r", sizeof("cmd not found\n\r"));

}

}

        


