#include <stddef.h>
#include <stdint.h>
#ifndef __SYSTEM_H
#define __SYSTEM_H

extern unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count);
extern unsigned char *memset(unsigned char *dest, unsigned char val, int count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);
extern int strlen(const char *str);
extern unsigned char importb(unsigned short _port);
extern void outportb (unsigned short _port, unsigned char _data);
extern void term_putc(char c, uint8_t color);
extern void term_init();
extern void term_print(const char* str, const uint8_t color);
extern void move_csr(void);
extern void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
void gdt_install();

#endif