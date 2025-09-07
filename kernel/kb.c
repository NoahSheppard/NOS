#include "system.h"

int shift_held = 0;

//unsigned char kbdus[128] = 
//{
//    0 /*no scancode: 0*/, '\e', /*esc*/ '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', /* backspace*/
//    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']'

    // 1 59 60 61 62 63 64 65 66 67 68 87 88 x x x 
    // 41 2 3 4 5 6 7 8 9 10 11 12 13 14 71
    // 15 16 17 18 19 20 21 22 23 24 25 26 27 43 x
    // 58 30 31 32 33 34 35 36 37 38 39 40 28 81
    // 42 44 45 46 47 48 49 50 51 52 53 54 72 79
    // 29 xwin 56 57 56 x 29 75 80 77
//};

unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};	

unsigned char skbdus[128] =
{
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*',	/* 9 */
  '(', ')', '_', '+', '\b',	/* Backspace */
  '\t',			/* Tab */
  'Q', 'W', 'E', 'R',	/* 19 */
  'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',	/* 39 */
 '\"', '~',   0,		/* Left shift */
 '|', 'Z', 'X', 'C', 'V', 'B', 'N',			/* 49 */
  'M', '<', '>', '?',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};	


void debug_keyboard_handler(struct regs *r)
{
    unsigned char scancode;
    scancode = importb(0x60);
    if (scancode & 0x80)
    {
        // when key is released
    }
    else
    {
        char buf[4];  // Max 3 digits + null terminator
        int i = 0;
        
        // Convert scancode to string
        if (scancode == 0) {
            buf[i++] = '0';
        } else {
            int temp = scancode;
            int digits[3];
            int digit_count = 0;
            
            // Extract digits
            while (temp > 0) {
                digits[digit_count++] = temp % 10;
                temp /= 10;
            }
            
            // Reverse digits into buffer
            for (int j = digit_count - 1; j >= 0; j--) {
                buf[i++] = '0' + digits[j];
            }
        }
        buf[i] = '\0';
        
        term_print("Scancode: ", 0x0A);
        term_print(buf, 0x0A);
        term_print(", Key: ", 0x0A);
        term_putc(kbdus[scancode], 0x0A);
        term_print("\n", 0x0A);
    }
}

void keyboard_handler(struct regs *r)
{
    unsigned char scancode;
    
    scancode = importb(0x60);
    if (scancode & 0x80)
    {
        switch (scancode)
        {
            case 0xAA: 
            case 0xB6: 
            {
                shift_held = 0;
                return;
            }
            default: { return; }
        }
    }
    if (scancode == 0x1C)
    {
        basicterm_return_handler();
        return;
    }
    else if (scancode == 0x2A || scancode == 0x36)
    {
        shift_held = 1;
        return;
    }
    else if (kbdus[scancode] != 0)
    {
        if (shift_held == 1)
            basicterm_key_handler(skbdus[scancode]);
        else if (shift_held == 0)
            basicterm_key_handler(kbdus[scancode]);
    }
}


void keyboard_installer()
{
    irq_install_handler(1, keyboard_handler);
}