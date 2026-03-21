#include<stdint.h>
#include<stdio.h>




typedef struct r_buffer{
    char* buffer;
    uint8_t head;
    uint8_t tail;
    uint8_t max_size;
}r_buffer;

int ring_buffer_init(r_buffer* rb,char* r_buffer,uint8_t max_size);
int ring_buffer_push(r_buffer* rb,char ch);
int ring_buffer_pop(r_buffer* rb,char* op_ch);