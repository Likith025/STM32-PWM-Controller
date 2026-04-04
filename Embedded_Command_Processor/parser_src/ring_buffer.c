    #include"../parser_inc/ring_buffer.h"
    #include<stdio.h>


    int ring_buffer_init(r_buffer* rb1,char* rx_buffer,uint8_t max_size){
         if (rb1 == NULL || rx_buffer == NULL || max_size == 0)
        return -1 ;
        rb1->head=0;
        rb1->tail=0;
        rb1->max_size=max_size;
        rb1->buffer=rx_buffer;
        return 0;


    }


    int ring_buffer_push(r_buffer* rb1,char ch){
        uint8_t next_head;
        if (rb1 == NULL || rb1->buffer == NULL)
            return -1;

        /* Check if buffer is full */
         next_head = (rb1->head + 1) ;
        if(next_head>=rb1->max_size){
            next_head=0;
        }
        if(next_head == rb1->tail){
            return -1; /* buffer is full */
        }

        rb1->buffer[rb1->head]=ch;
        rb1->head = next_head;
        return 0;
        
    }

    int ring_buffer_pop(r_buffer* rb1,char* op_ch){
        if (rb1 == NULL || rb1->buffer == NULL || op_ch == NULL)
            return -1;

        /* Check if buffer is empty */
        if(rb1->head == rb1->tail){
            return -1; /* buffer is empty */
        }

        *op_ch=rb1->buffer[rb1->tail];
        rb1->tail=(rb1->tail+1);
        if(rb1->tail>=rb1->max_size){
            rb1->tail=0;
        }
        return 0;

    }
    
