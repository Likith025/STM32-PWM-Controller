#include "ring_buffer.h"
#include "process.h"
#include <string.h>

void extract_cmd(r_buffer* rb){
    static char cmd_buffer[CMD_MAX];
    static int idx=0;
    char ch;

            while (ring_buffer_pop(rb, &ch) == 0) {

        if (ch == '\n') {
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
    char* cmd_list[3]={"led on","led off","display status"};
if(strcmp(cmd_list[0],cmd)==0){
    printf("led turned on\n");
}
else if(strcmp(cmd_list[1],cmd)==0){
    printf("led turned off\n");
}
else if(strcmp(cmd_list[2],cmd)==0){
    printf("status displayed\n");
}
else{
    printf("cmd not found\n");
}

}

        


