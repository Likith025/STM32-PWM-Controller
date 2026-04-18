#include "../parser_inc/process.h"
#include <stdlib.h>


void extract_cmd(r_buffer* rb){
    static char cmd_buffer[CMD_MAX];
    static int idx=0;
    char ch;

            while (ring_buffer_pop(rb, &ch) == 0) {

        if (ch == '\r') {
            cmd_buffer[idx] = '\0';
            idx = 0;
            process_cmd(cmd_buffer);
            break;
        }

        if (idx < CMD_MAX - 1) {
            cmd_buffer[idx++] = ch;
        } else {
            idx = 0;
        }
    }
    
}

void process_cmd(char* cmd){

	static int value=0;
    char temp[50];
    strcpy(temp, cmd);

    char *token = strtok(temp, " ");
    char *args  = strtok(NULL, " ");

    if(token && strcmp(token, "pwm") == 0){
        if(args){
             value = atoi(args);
             if(value>100){
                 USART_SendData_IT(&g_usart3,
                     (uint8_t*)"duty cycle max can be 100\n\r",
                     strlen("duty cycle max can be 100\n\r"));
            	 value=100;
             }
             if(value<0){
            	 USART_SendData_IT(&g_usart3,
            	                      (uint8_t*)"duty cycle min can be 0\n\r",
            	                      strlen("duty cycle min can be 0\n\r"));
            	 value=0;
             }

            char msg[30];
            sprintf(msg, "PWM:%d%%\n\r", value);
            USART_SendData_IT(&g_usart3, (uint8_t*)msg, strlen(msg));
            TimerPWM_DutyCycle(&g_timer2,CH1,value);
        }
    }
    else if(token && strcmp(token, "status") == 0){

        char msg[30];

        sprintf(msg, "PWM status:%d%%\n\r", value);

        USART_SendData_IT(&g_usart3, (uint8_t*)msg, strlen(msg));
    }
    else{
        USART_SendData_IT(&g_usart3,
            (uint8_t*)"cmd not found\n\r",
            strlen("cmd not found\n\r"));
    }
}
        


