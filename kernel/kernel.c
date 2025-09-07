#include <stddef.h>
#include <stdint.h>
#include "system.h"

#if defined(__linux__) 
    #error "This code must be compiled iwth a cross-compiler"
#elif !defined(__i386__)
    #error "This code must be compiled with an x86-elf compiler"
#endif

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

unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count) {
    // copy count bytes of data from src to dest, return dest 
    for(size_t i=0; i<count; i++) 
    {
        dest[i] = src[i];
    }
    return dest; 
}

unsigned char *memset(void *dest, unsigned char val, int count) 
{
    // Add code here to set 'count' bytes in 'dest' to 'val', return dest?
    unsigned char *temp = (unsigned char *)dest;
    for (; count != 0; count--)
    {
        *temp++ = val;
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

int strcmp(const char* str1, const char* str2) 
{
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

int strcmpl(const char* str1, const char* str2, int length) 
{
    int count = 0;
    while (*str1 && (*str1 == *str2) && count != length) {
        str1++;
        str2++;
        count++;
    }

    if (count == length)
    {
        return 0;
    }
    
    return *str1 - *str2;
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

char* strcpy(char* dest, const char* src, size_t dest_size)
{
    if (strlen(src) > dest_size)
        return NULL; // TODO: Error/Exception 
    char* original_dest = dest;
    while (*src != '\0')
    {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return original_dest;
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

void test_terminal_color()
{
    //"colour testing :)";
    term_putc('c', 0x08);
    term_putc('o', 0x09);
    term_putc('l', 0x01);
    term_putc('o', 0x0A);
    term_putc('u', 0x02);
    term_putc('r', 0x0B);
    term_putc(' ', 0x00);
    term_putc('t', 0x03);
    term_putc('e', 0x0C);
    term_putc('s', 0x04);
    term_putc('t', 0x0D);
    term_putc('i', 0x05);
    term_putc('n', 0x0E);
    term_putc('g', 0x06);
    term_putc(' ', 0x0);
    term_putc(':', 0x0F);
    term_putc(')', 0x07);
    term_putc('\n', 0x08);
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

void test_divbyzero() {
    int zerotest = 10/0;
    term_print((char*)zerotest, 0x0A);
    term_print("If this prints, ISR didn't fire", 0x0C);
}

void kernel_main() 
{
    term_init();
    term_print("1. Terminal initialized\n", 0x0A);
    
    term_print("2. About to call gdt_install()\n", 0x09);
    gdt_install();
    term_print("3. gdt_install() completed\n", 0x02);

    term_print("4. About to call idt_install()\n", 0x09);
    idt_install();
    term_print("5. idt_install completed\n", 0x02);

    term_print("6. Installing ISRS\n", 0x09);
    isrs_install();
    term_print("7. Installed ISRS!\n", 0x02);

    term_print("7. Installing IQR's\n", 0x09);
    irq_install();
    term_print("  7a. Enabling IQR's\n", 0x0E);
    asm volatile("sti");
    term_print("8. Installed+Enabled IQR's!\n", 0x02);

    term_print("9. Installing Timer...\n", 0x09);
    timer_install();
    term_print("10. Installed Timer!\n", 0x02);

    term_print("11. Installing Keyboard...\n", 0x09);
    keyboard_installer();
    term_print("12. Installed Keyboard!\n", 0x02);
    
    term_print("\n", 0x0F);
    term_print("[NOS] $ ", 0x0F);

    //basicterm_main(); As the keyboard driver passes input directly into the basicterm.c functions
    //This call is no longer required until this is changed, which it should

    while (1) {
        asm volatile("hlt");
    }
}