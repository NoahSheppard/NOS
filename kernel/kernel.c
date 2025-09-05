#include <stddef.h>
#include <stdint.h>
#include "system.h"

#if defined(__linux__) 
    #error "This code must be compiled iwth a cross-compiler"
#elif !defined(__i386__)
    #error "This code must be compiled with an x86-elf compiler"
#endif

volatile uint16_t* vga_buffer = (uint16_t*)0xB8000;
const int VGA_COLS = 80;
const int VGA_ROWS = 25;

// TODO: Have a list of all the lines appended so that when we have scrolling, we can make use of it. 
//uint16_t stdout_list[];

int term_col = 0;
int term_row = 0;
uint8_t term_color = 0x0F; // with colour, the first 4 bits are the background, and the next 4 bits are background

// Used to get data from port (for later, I do NOT understand this so far)
unsigned char importb(unsigned short _port)
{
    unsigned char rv;
    asm volatile ("inb %1, %0": "=a" (rv) : "dN" (_port));
    return rv;
}

// Opposite of the function above. Still do NOT understand
void outportb (unsigned short _port, unsigned char _data)
{
    asm volatile ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

void move_csr(void) 
{
    unsigned temp;
    temp = term_row * 80 + term_col;
    outportb(0x3D4, 14);
    outportb(0x3D5, temp >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, temp);
}

void term_init() 
{
    for (int col = 0; col < VGA_COLS; col++)
    {
        for (int row = 0; row < VGA_ROWS; row++) 
        {
            const size_t index = (VGA_COLS * row) + col;
            vga_buffer[index] = ((uint16_t)term_color << 8) | ' ';
        }
    }

    move_csr();
} 

void term_putc(char c, uint8_t color)
{
    switch (c)
    {
    case '\n':
        {
            term_col = 0;
            term_row++;
            // whatever was on the line, put in the stdout list 
            break;
        }
    case '\r':
        {
            term_col = 0;
        }
    case 0x08:
        {
            if (term_col != 0) term_col--;
        }
    default:
        {
            const size_t index = (VGA_COLS * term_row) + term_col;
            vga_buffer[index] = ((uint16_t)color << 8) | c;
            term_col++;
            break;       
        }
    }
    if (term_col >= VGA_COLS)
    {
        term_col = 0;
        term_row++;
        // whatever was on the line, put in the stdout list 
    }

    if (term_row >= VGA_ROWS)
    {
        for (uint8_t colindex = 0; colindex < VGA_COLS; colindex++)
        {
            for (uint8_t i = 0; i < VGA_ROWS; i++) {
                const size_t index = (VGA_COLS * i) + colindex;
                vga_buffer[index] = vga_buffer[index+VGA_COLS];
            }
            
        }
        term_row--;
    }
    move_csr();
}

void term_print(const char* str, const uint8_t color) 
{
    for (size_t i = 0; str[i] != '\0'; i++)
    {
        term_putc(str[i], color);
    }
}

unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count) {
    // copy count bytes of data from src to dest, return dest 
    for(size_t i=0; i<count; i++) 
    {
        dest[i] = src[i];
    }
    return dest; 
}

unsigned char *memset(unsigned char *dest, unsigned char val, int count) 
{
    // Add code here to set 'count' bytes in 'dest' to 'val', return dest?
    for (size_t i = 0; i < count; i++)
    {
        dest[i] = val;
    }
    return dest; 
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, int count) 
{
    /* Same as above, but this time, we're working with a 16-bit 
    * 'val' and dest pointer. Your code can be an exact copy of 
    * the above, provided that your local variables if any, are
    * unsigned short 
    */
    for (size_t i = 0; i < count; i++) 
    {
        dest[i] = val;
    }
    return dest; 
}

int strlen(const char *str)
{
    /* This loops through a caharacter array 'str', returning how
    * many characters it needs to check before it finds a 0. 
    * In simple words, it returns the length in bytyes of a string
    */
    size_t count = 0; 
    while (str[count] != '\0') {
        count++;
    }
    return count; 
}

void test_terminal_scroll() 
{
    term_print("Line 1\n", 0x0A);
    term_print("Line 2\n", 0x0A);
    term_print("Line 3\n", 0x0A);
    term_print("Line 4\n", 0x0A);
    term_print("Line 5\n", 0x0A);
    term_print("Line 6\n", 0x0A);
    term_print("Line 7\n", 0x0A);
    term_print("Line 8\n", 0x0A);
    term_print("Line 9\n", 0x0A);
    term_print("Line 0\n", 0x0A);
    term_print("Line 11\n", 0x0A);
    term_print("Line 12\n", 0x0A);
    term_print("Line 13\n", 0x0A);
    term_print("Line 14\n", 0x0A);
    term_print("Line 15\n", 0x0A);
    term_print("Line 16\n", 0x0A);
    term_print("Line 17\n", 0x0A);
    term_print("Line 18\n", 0x0A);
    term_print("Line 19\n", 0x0A);
    term_print("Line 20\n", 0x0B);
    term_print("Line 21\n", 0x0C);
    term_print("Line 22\n", 0x0D);
    term_print("Line 23\n", 0x0E);
    term_print("Line 24\n", 0x0F);
    term_print("Line 25\n", 0x0A);
}

void test_memcpy()
{
    char *test_char = "Hello World!\n";
    char *test_char_output[14];
    char *return_char = memcpy(*test_char_output, test_char, 14);

    term_print(test_char, 0x0F);
    term_print(*test_char_output, 0x0F);
    term_print(return_char, 0x0F);

    // This should print
    /*
    * Hello World!
    * Hello World!
    * Hello World!
    */
    // The first one is the source variable decleration
    // The second one is the dest variable after memcpy() is called to replace it's content
    // The third one is the return value of memcpy(), which is dest 
    // As memcpy() -> dest=src, all 3 should be the same 
    // And works in initial testing, hence memcpy should be working
}

void test_memset()
{
    char *test_char = "H";
    char *test_char_output[14];
    char *return_char = memset(*test_char_output, *test_char, 14);

    term_print(test_char, 0x0F);
    term_print(*test_char_output, 0x0F);
    term_print(return_char, 0x0F);

    // This should print
    /*
    * H[x15][2 random bytes, usually "S|_|" (a white block)]H[x14][2 random bytes]
    * HHHHHHHHHHHHHHHS|_|HHHHHHHHHHHHHHS|_|
    */
    // And works in initial testing, hence memcpy should be working
    // Look to memcpy() for better explination
    // Due to there being no '\n', bugs apply
    // Not sure if this means it 100% works, will do future testing. 
}

void kernel_main() 
{
    term_init();
    term_print("1. Terminal initialized\n", 0x0F);
    
    term_print("2. About to call gdt_install()\n", 0x0A);
    gdt_install();
    term_print("3. gdt_install() completed\n", 0x0C);

    term_print("4. About to call idt_install()\n", 0x0A);
    idt_install();
    term_print("5. idt_install completed\n", 0x0C);

    term_print("Hello, World!\n", 0x0F);
    term_print("This is the start of NOS\n", 0x0A);
    while(1) {}
}