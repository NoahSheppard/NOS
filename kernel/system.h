#include <stddef.h>
#include <stdint.h>
#ifndef __SYSTEM_H
#define __SYSTEM_H

extern unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count);
extern unsigned char *memset(void *dest, unsigned char val, int count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);
int strcmp(const char* str1, const char* str2);
extern int strlen(const char *str);
extern unsigned char importb(unsigned short _port);
extern void outportb (unsigned short _port, unsigned char _data);
extern void term_putc(char c, uint8_t color);
extern void term_init();
extern void term_print(const char* str, const uint8_t color);
extern void move_csr(void);
extern void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
void gdt_install();
extern void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
void idt_install();
void isrs_install();
void fault_handler();
struct regs
{
    unsigned int gs, fs, es, ds; // pushed the segments last
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; // pushed by 'pusha'
    unsigned int int_no, err_code; // push $x
    unsigned int eip, cs, eflags, useresp, ss; // pushed by cpu automatically
};
void irq_install_handler(int irq, void (*handler)(struct regs *r));
void irq_uninstall_handler(int irq);
void irq_remap(void);
void irq_install();
void irq_handler(struct regs *r);

void timer_phase(int hz);
void timer_handler(struct regs *r);
void timer_install();
void keyboard_installer();

void keyboard_handler(struct regs *r);

void basicterm_main();
void basicterm_return_handler();
void basicterm_key_handler(char key);
void printf(char* string);
void printc(char c);
void clear();

#endif