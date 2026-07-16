#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

#define MAX_TASKS 10


typedef struct
{
    void (*func)(void);
    uint8_t state;
} Task;


void scheduler_init(void);
void scheduler_add_task(void (*func)(void));
void scheduler_tick(void);


#endif