#ifndef SHELL_H
#define SHELL_H

#include <stdint.h>

void shell_init(void);
void shell_prompt(void);
void shell_read_line(char* buffer, int size);
void shell_execute_command(const char* cmd);
void shell_run(void);

#endif