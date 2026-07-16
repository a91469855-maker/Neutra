#include "interrupthandler.h"

InterruptHandler handlers[256];

void interrupts_init(void)
{
    for (int i = 0; i < 256; i++)
    {
        handlers[i] = 0;
    }
}

void register_handler(int interrupt, InterruptHandler handler)
{
    handlers[interrupt] = handler;
}

void handle_interrupt(int interrupt)
{
    if (handlers[interrupt] != 0)
    {
        handlers[interrupt]();
    }
}