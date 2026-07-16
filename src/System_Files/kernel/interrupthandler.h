#ifndef INTERRUPTHANDLER_H
#define INTERRUPTHANDLER_H

typedef void (*InterruptHandler)(void);

void interrupts_init(void);
void register_handler(int interrupt, InterruptHandler handler);
void handle_interrupt(int interrupt);

#endif