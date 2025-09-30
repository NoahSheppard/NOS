#include "../kernel/system.h"

volatile uint16_t* vga_buffer = (uint16_t*)0xB8000;
const int VGA_COLS = 80;
const int VGA_ROWS = 25;

// TODO: Have a list of all the lines appended so that when we have scrolling, we can make use of it. 
//uint16_t stdout_list[];

int term_col = 0;
int term_row = 0;
uint8_t term_color = 0x0F; // with colour, the first 4 bits are the background, and the next 4 bits are background


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

    term_col = 0;
    term_row = 0;
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
    case '\b':
        {
            if (term_col != 0)
            {
                term_col--;
                const size_t index = (VGA_COLS * term_row) + term_col;
                vga_buffer[index] = ((uint16_t)color << 8) | ' ';
                break; 
            } 
            else 
            {
                const size_t index = (VGA_COLS * term_row) + term_col;
                vga_buffer[index] = ((uint16_t)color << 8) | ' ';
                break; 
            }
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
