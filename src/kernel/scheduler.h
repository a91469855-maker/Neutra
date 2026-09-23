#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <stddef.h>

#define MAX_TASKS 10
#define TIME_SLICE_MS 10
#define TASK_STACK_SIZE 4096
#define MAX_SCHEDULER_TABLE 32

typedef enum {
    TASK_READY,
    TASK_RUNNING,
    TASK_BLOCKED,
    TASK_DEAD
} TaskState;

typedef struct {
    int id;
    const char* name;
    void (*entry)(void);
    uint8_t stack[TASK_STACK_SIZE];
    uint32_t sp;
    int32_t remaining_time_ms;
    TaskState state;
    int priority;
} Task;

typedef struct {
    const char* name;
    void (*entry)(void);
    int32_t time_ms;
} SchedulerTableEntry;

typedef struct {
    Task queue[MAX_TASKS];
    int head;
    int tail;
    int count;
    int current_task_id;
    unsigned int total_time_ms;
    int switches;
} Scheduler;

void scheduler_init(void);

void scheduler_add_task(
    const char* name,
    void (*entry)(void),
    int32_t time_ms
);

void scheduler_add_task_priority(
    const char* name,
    void (*entry)(void),
    int32_t time_ms,
    int priority
);

void scheduler_add_table(
    const char* name,
    void (*entry)(void),
    int32_t time_ms
);

void scheduler_run(void);
int scheduler_step(void);
void scheduler_print_stats(void);
Task* scheduler_current_task(void);
int scheduler_task_count(void);
unsigned int scheduler_total_time(void);
void scheduler_reset(void);
void scheduler_check_table(const char* name);


#endif