// Definice rozměrů obrazovky
#define VIDEO_WIDTH 80
#define VIDEO_HEIGHT 25
#define VIDEO_MEMORY (char *)0xb8000
#define DEFAULT_COLOR 0x0F // Bílá na černé

#define VGA_CTRL_REGISTER 0x3d4 // control register
#define VGA_DATA_REGISTER 0x3d5 // data register

#define VGA_CUR_LOW_REG 0x0f  // cursor location low register
#define VGA_CUR_HIGH_REG 0x0e // cursor location high register

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_STATUS_OUTPUT_FULL 0x01

#define COM1 0x3F8 // serial lane

// Globální proměnné pro pozici kurzoru
int cursor_x = 0;
int cursor_y = 0;

unsigned char inb(unsigned short port)
{
    unsigned char result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void outb(unsigned short port, unsigned char value)
{
    __asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

unsigned short inw(short port)
{
    unsigned short result;
    __asm__ volatile("inw %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void outw(unsigned short port, unsigned short value)
{
    __asm__ volatile("outw %0, %1" : : "a"(value), "Nd"(port));
}

unsigned char keyboard_getscancode()
{
    while (!(inb(KEYBOARD_STATUS_PORT) & KEYBOARD_STATUS_OUTPUT_FULL))
    {
    }

    return inb(KEYBOARD_DATA_PORT);
}


int get_cursor()
{
    int offset = 0;
    outb(VGA_CTRL_REGISTER, VGA_CUR_HIGH_REG);
    offset = (inb(VGA_DATA_REGISTER) << 8);
    outb(VGA_CTRL_REGISTER, VGA_CUR_LOW_REG);
    offset |= inb(VGA_DATA_REGISTER);
    return offset * 2;
}

void set_cursor(int offset)
{
    outb(VGA_CTRL_REGISTER, VGA_CUR_HIGH_REG);
    outb(VGA_DATA_REGISTER, (offset >> 8) & 0xFF);
    outb(VGA_CTRL_REGISTER, VGA_CUR_LOW_REG);
    outb(VGA_DATA_REGISTER, (offset & 0xFF));
}

void update_cursor(int x, int y)
{
    int offset = y * 80 + x;
    set_cursor(offset);
}

void vga_clear_screen()
{
    char *video_mem = VIDEO_MEMORY;

    for (int i = 0; i < 80 * 25; i++)
    {
        video_mem[i * 2] = ' ';
        video_mem[i * 2 + 1] = DEFAULT_COLOR;
    }

    update_cursor(0, 0);
}

void vga_scroll(int offset)
{
    char *video_mem = VIDEO_MEMORY;

    for (int i = 0; i < VIDEO_HEIGHT - offset; i++)
    {
        for (int j = 0; j < VIDEO_WIDTH; j++)
        {
            int dest_idx = ((i - offset) * VIDEO_WIDTH + j) * 2;
            int src_idx = (i * VIDEO_WIDTH + j) * 2;

            video_mem[dest_idx] = video_mem[src_idx];
            video_mem[dest_idx + 1] = video_mem[src_idx + 1];
        }
    }

    for (int i = VIDEO_HEIGHT - offset; i < VIDEO_HEIGHT; i++)
    {
        for (int j = 0; j < VIDEO_WIDTH; j++)
        {
            int idx = (i * VIDEO_WIDTH + j) * 2;
            video_mem[idx] = ' ';
            video_mem[idx + 1] = DEFAULT_COLOR;
        }
    }
}



// index = indexX * arrayWidth + indexY;
void vga_putchar(char c)
{
    char *video_memory = VIDEO_MEMORY;
    if (c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
        update_cursor(cursor_x, cursor_y);
        return;
    }
    else
    {
        video_memory[get_cursor()] = c;
        video_memory[get_cursor() + 1] = DEFAULT_COLOR;
    }
    if (cursor_x < VIDEO_WIDTH)
    {
        cursor_x++;
    }
    else
    {
        cursor_x = 0;
        cursor_y++;
    }
    update_cursor(cursor_x, cursor_y);
}

void vga_print(char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        vga_putchar(str[i]);
        i++;
    }
}

void serial_init()
{
    outb(COM1 + 1, 0x00); // zakazat preruseni
    outb(COM1 + 1, 0x80); // povolit dlab

    outb(COM1 + 0, 0x03); // Low byte divisoru (3)
    outb(COM1 + 1, 0x00); // High byte divisoru (0)

    outb(COM1 + 3, 0x03); // 8 bitů, žádná parita, jeden stop bit (vypne DLAB)
    outb(COM1 + 2, 0xC7); // Povolit FIFO, vymazat je, 14-byte threshold
    outb(COM1 + 4, 0x0B); // IRQs povoleno, RTS/DSR nastaveno
}

int is_transmit_empty()
{
    return inb(COM1 + 5) & 0x20;
}

void serial_putchar(char c)
{
    while (is_transmit_empty() == 0)
        ;
    outb(COM1, c);
}

void serial_print(const char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        serial_putchar(str[i]);
        i++;
    }
}

void backspace()
{
    char* vidmem = VIDEO_MEMORY;
    
    if (cursor_x==0)
        cursor_y--;
    cursor_x--;
    update_cursor(cursor_x,cursor_y);
    vidmem[get_cursor()] = ' ';
}

unsigned char keyboard_getchar()
{
    unsigned char scancode = keyboard_getscancode();

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

void __attribute__((section(".text.entry"))) kernel_main()
{

    vga_clear_screen();

    vga_print("Welcome to HideOS\n");
    vga_print("LINE\n");
    vga_print("\n");
    vga_print("\n");
    vga_print("\n");
    vga_print("\n");
    vga_print("  _    _ _     _       ____   _____ \n");
    vga_print(" | |  | (_)   | |     / __ \\ / ____|\n");
    vga_print(" | |__| |_  __| | ___| |  | | (___  \n");
    vga_print(" |  __  | |/ _` |/ _ \\ |  | |\\___ \\ \n");
    vga_print(" | |  | | | (_| |  __/ |__| |____) |\n");
    vga_print(" |_|  |_|_|\\__,_|\\___|\\____/|_____/ \n");

    vga_print("\n\nPress any key to print:\n");

    serial_print("TEST1");

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

    unsigned char sc;
    while (1)
    {
        sc = keyboard_getchar();
        if (sc != 0)
        {
            vga_putchar(scancodes[sc]);
        }
        
    }
}