#include "system.h"

int timer_ticks = 0;
int timer_frequency = 1000;
int debug_color = 0x00;

void timer_phase(int hz) 
{
    int divisor = 1193180 / hz;
    outportb(0x43, 0x36);
    outportb(0x40, divisor & 0xFF);
    outportb(0x40, (divisor >> 8) & 0xFF);
}

void timer_handler(struct regs *r)
{
    timer_ticks++;
    /*if (timer_ticks % 1000 == 0)
    {
        if (debug_color == 0x10) debug_color = 0x00;
        term_print("One second has passed.\n", debug_color);
        debug_color++;
    }*/
}

void timer_install()
{
    irq_install_handler(0, timer_handler);
    timer_phase(timer_frequency);
}