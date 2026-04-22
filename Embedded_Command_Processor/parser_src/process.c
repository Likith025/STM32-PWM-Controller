#include "../parser_inc/process.h"
#include <stdlib.h>
#include<ctype.h>


    int value;
    int initialized;

    const char *help_msg =
        "Available commands:\n\r"
        "pwm <0-100>  : set duty cycle\n\r"
        "status       : show current PWM\n\r"
        "help         : show this message\n\r";

    const char *missing_arg="error: missing argument\n\r";
    const char *invalid="error: invalid number\n\r";
    const char *max_limit="error: max value is 100\n\r";
    const char *min_limit="error: min value is 0\n\r";
    const char *invalid_cmd="error: command not found\n\r";

    void pwm_handler(char* args);
    void status_handler(char* args);
    void help_handler(char* args);

    typedef struct{
    	char* cmd;
    	void (*cmd_handler)(char* args);
    }Ptrcmd_t;

	Ptrcmd_t cmdslist[]={
			{"pwm",pwm_handler},
			{"status",status_handler},
			{"help",help_handler}
	};

	uint8_t cmdslist_size=sizeof(cmdslist)/sizeof(cmdslist[0]);

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

//void process_cmd(char* cmd)
//{
//
//    static int value = 0;
//    static int initialized = 0;
//
//    const char *help_msg =
//        "Available commands:\n\r"
//        "pwm <0-100>  : set duty cycle\n\r"
//        "status       : show current PWM\n\r"
//        "help         : show this message\n\r";
//
//    const char *missing_arg="error: missing argument\n\r";
//
//    const char *invalid="error: invalid number\n\r";
//    const char *max_limit="error: max value is 100\n\r";
//    const char *min_limit="error: min value is 0\n\r";
//    const char *invalid_cmd="error: command not found\n\r";
//
//
//    char temp[50];
//    strncpy(temp, cmd, sizeof(temp)-1);
//    temp[sizeof(temp)-1] = '\0';
//
//    char *token = strtok(temp, " ");
//    char *args  = strtok(NULL, " ");
//
//    if(token && strcmp(token, "pwm") == 0)
//    {
//        if(args == NULL){
//            USART_SendData_IT(&g_usart3,
//                (uint8_t*)missing_arg,
//                strlen(missing_arg));
//            return;
//        }
//        char *endptr;
//        long val = strtol(args, &endptr, 10);
//
//        if(*endptr != '\0'){
//            USART_SendData_IT(&g_usart3,
//                (uint8_t*)invalid,
//                strlen(invalid));
//            return;
//        }
//
//        if(val > 100){
//            USART_SendData_IT(&g_usart3,
//                (uint8_t*)max_limit,
//                strlen(max_limit));
//            val = 100;
//        }
//        else if(val < 0){
//            USART_SendData_IT(&g_usart3,
//                (uint8_t*)min_limit,
//                strlen(min_limit));
//            val = 0;
//        }
//
//        value = (int)val;
//        initialized = 1;
//
//        TimerPWM_DutyCycle(&g_timer2, CH1, value);
//
//        char msg[30];
//        sprintf(msg, "PWM:%d%%\n\r", value);
//        USART_SendData_IT(&g_usart3, (uint8_t*)msg, strlen(msg));
//    }
//
//    else if(token && strcmp(token, "status") == 0)
//    {
//        char msg[40];
//
//        if(!initialized){
//            sprintf(msg, "PWM not set\n\r");
//        } else {
//            sprintf(msg, "PWM:%d%%\n\r", value);
//        }
//
//        USART_SendData_IT(&g_usart3, (uint8_t*)msg, strlen(msg));
//    }
//    else if(token&&strcmp(token,"help")==0){
//    	USART_SendData_IT(&g_usart3, (uint8_t*)help_msg, strlen(help_msg));
//    }
//
//    else
//    {
//        USART_SendData_IT(&g_usart3,
//            (uint8_t*)invalid_cmd,
//            strlen(invalid_cmd));
//    }
//}

void pwm_handler(char* args){

	if(args==NULL){
        USART_SendData_IT(&g_usart3,
            (uint8_t*)missing_arg,
            strlen(missing_arg));
        return;
	}
	else{
		char* end;
		long val=strtol(args,&end,10);
		if(*end!='\0'){
	        USART_SendData_IT(&g_usart3,
	            (uint8_t*)invalid,
	            strlen(invalid));
	        return;
		}
		else{
			if(val>100){
	            USART_SendData_IT(&g_usart3,
	                (uint8_t*)max_limit,
	                strlen(max_limit));
	            val = 100;
			}
		     if(val<0){
	            USART_SendData_IT(&g_usart3,
	                (uint8_t*)min_limit,
	                strlen(min_limit));
	            val = 0;

			}
		        value = (int)val;
		        initialized = 1;

		        TimerPWM_DutyCycle(&g_timer2, CH1, value);

		        char msg[30];
		        sprintf(msg, "PWM:%d%%\n\r", value);
		        USART_SendData_IT(&g_usart3, (uint8_t*)msg, strlen(msg));
		    }
		}
	}
void status_handler(char* args){
    char msg[40];

    if(!initialized){
        sprintf(msg, "PWM not set\n\r");
    } else {
        sprintf(msg, "PWM:%d%%\n\r", value);
    }

    USART_SendData_IT(&g_usart3, (uint8_t*)msg, strlen(msg));

}
void help_handler(char* args){
 	USART_SendData_IT(&g_usart3, (uint8_t*)help_msg, strlen(help_msg));

}
void invalid_handler(){
    USART_SendData_IT(&g_usart3,
        (uint8_t*)invalid_cmd,
        strlen(invalid_cmd));
}

void process_cmd(char* cmd)
{
	Ptrcmd_t cmdslist[3]={
			{"pwm",pwm_handler},
			{"status",status_handler},
			{"help",help_handler}
	};
    char temp[50];
    strncpy(temp, cmd, sizeof(temp)-1);
    temp[sizeof(temp)-1] = '\0';

    char *token = strtok(temp, " ");
    char *args  = strtok(NULL, " ");
    for(int i=0;i<cmdslist_size;i++){
    	if(strcmp(token,cmdslist[i].cmd)==0){
    		cmdslist[i].cmd_handler(args);
    		return;
    	}
    }
    invalid_handler();



}
