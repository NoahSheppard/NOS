#include <stddef.h>
#include <stdint.h>

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
}

void term_print(const char* str, const uint8_t color) 
{
    for (size_t i = 0; str[i] != '\0'; i++)
    {
        term_putc(str[i], color);
    }
}

void kernel_main() 
{
    term_init();

    term_print("Hello, World!\n", 0x0A);
    term_print("This is the start of Linux 2\n", 0x0A);
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