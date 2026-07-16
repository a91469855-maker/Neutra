#include "scheduler.h"
Task tasks[MAX_TASKS];
uint32_t num_tasks = 0;
uint32_t current_index = 0;
void scheduler_init(void)
{
    num_tasks = 0;
    current_index = 0;
    for (uint32_t i = 0; i < MAX_TASKS; i++)
    {
        tasks[i].func = 0;
        tasks[i].state = 0;
    }
}
void scheduler_tick(void)
{
    if (num_tasks == 0)
        return;
    Task *current_task = &tasks[current_index];
    if (current_task->state == 1)
    {
        current_task->func();
    }
    current_index = (current_index + 1) % num_tasks;
}