#include "keyboard.h"

char scancodes[128] = {
        0,
        27,
        '1',
        '2',
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9',
        '0',
        '-',
        '=',
        '\b', /* Backspace */
        '\t', /* Tab */
        'q',
        'w',
        'e',
        'r',
        't',
        'y',
        'u',
        'i',
        'o',
        'p',
        '[',
        ']',
        '\n', /* Enter */
        0,
        /* Control */ 'a',
        's',
        'd',
        'f',
        'g',
        'h',
        'j',
        'k',
        'l',
        ';',
        '\'',
        '`',
        0, /* Left shift */
        '\\',
        'z',
        'x',
        'c',
        'v',
        'b',
        'n',
        'm',
        ',',
        '.',
        '/',
        0, /* Right shift */
        '*',
        0,   /* Alt */
        ' ', /* Space bar */
        0,   /* Caps lock */
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0, /* F1-F10 */
        0, /* Num lock */
        0, /* Scroll lock */
        0, /* Home key */
        0, /* Up arrow */
        0, /* Page up */
        '-',
        0, /* Left arrow */
        0,
        0, /* Right arrow */
        '+',
        0, /* End key */
        0, /* Down arrow */
        0, /* Page down */
        0, /* Insert key */
        0, /* Delete key */
        0,
        0,
        0,
        0, /* F11 */
        0, /* F12 */
        0, /* Vše ostatní je 0 */
    };

unsigned char keyboard_get_scancode()
{
    while (!(inb(KEYBOARD_STATUS_PORT) & KEYBOARD_STATUS_OUTPUT_FULL))
    {
    }

    return inb(KEYBOARD_DATA_PORT);
}

unsigned char keyboard_get_char()
{
    unsigned char scancode = keyboard_get_scancode();
    //unsigned char scancode = inb(0x60);

    if (scancode & 0x80) {
        return 0; 
    }

    if (scancode < 128) {
        if (scancode == 0x0E)
        {
            backspace();
            return 0;
        }
        return scancode;
    }

    return 0;
}

void backspace()
{
    char* vidmem = VIDEO_MEMORY;
    
    if (cursor_x > 2)
    {
        cursor_x--;
    }
    else {
        return;
    }

    update_cursor(cursor_x,cursor_y);
    vidmem[get_cursor()] = ' ';
    
}

