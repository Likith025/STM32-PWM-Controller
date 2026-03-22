#ifndef PROCESS_H_
#define PROCESS_H_

#include"ring_buffer.h"
#define CMD_MAX 15
void extract_cmd(r_buffer* rb);
void process_cmd(char* cmd);

#endif