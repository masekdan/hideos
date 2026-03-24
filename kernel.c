// Definice rozměrů obrazovky
#define VIDEO_WIDTH 80
#define VIDEO_HEIGHT 25
#define VIDEO_MEMORY (char*)0xb8000
#define DEFAULT_COLOR 0x0F // Bílá na černé

#define VGA_CTRL_REGISTER 0x3d4 // control register
#define VGA_DATA_REGISTER 0x3d5 // data register

#define VGA_CUR_LOW_REG 0x0f    // cursor location low register
#define VGA_CUR_HIGH_REG 0x0e   // cursor location high register

// Globální proměnné pro pozici kurzoru
int cursor_x = 0;
int cursor_y = 0;

unsigned char port_byte_in(unsigned short port)
{
    unsigned char result;
    __asm__ volatile("in %%dx, %%al" :"=a" (result) : "d" (port));
    return result;
}

void port_byte_out(unsigned short port, unsigned char data)
{
    __asm__ volatile("out %%al, %%dx": : "a" (data), "d" (port));
}

int get_cursor()
{
    port_byte_out(VGA_CTRL_REGISTER, VGA_CUR_HIGH_REG);
    int offset = port_byte_in(VGA_DATA_REGISTER);

    port_byte_out(VGA_CTRL_REGISTER, VGA_CUR_LOW_REG);
    offset != port_byte_in(VGA_DATA_REGISTER);

    return offset * 2;
}

void set_cursor(int offset)
{
    port_byte_out(VGA_CTRL_REGISTER, VGA_CUR_HIGH_REG);
    port_byte_out(VGA_DATA_REGISTER, (offset >> 8) & 0xFF);
    port_byte_out(VGA_CTRL_REGISTER, VGA_CUR_LOW_REG);
    port_byte_out(VGA_DATA_REGISTER, (offset & 0xFF));
}

void vga_clear_screen() {
    char *video_mem = VIDEO_MEMORY;

    for (int i = 0; i < 80 * 25; i++) {
        video_mem[i * 2] = ' ';
        video_mem[i * 2 + 1] = DEFAULT_COLOR;
    }

    cursor_x = 0;
    cursor_y = 0;
}

void vga_scroll(int offset) {
    char* video_mem = VIDEO_MEMORY;

    for (int i = 0 ; i < VIDEO_HEIGHT - offset ; i++)
    {
        for (int j = 0; j < VIDEO_WIDTH; j++)
        {
            int dest_idx = ((i-offset)*VIDEO_WIDTH+j)*2;
            int src_idx = (i * VIDEO_WIDTH + j) * 2;

            video_mem[dest_idx] = video_mem[src_idx];
            video_mem[dest_idx + 1] = video_mem[src_idx + 1];
        }
    }

    for (int i = VIDEO_HEIGHT - offset; i< VIDEO_HEIGHT; i++)
    {
        for (int j = 0;j <VIDEO_WIDTH;j++)
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
    char* video_memory = VIDEO_MEMORY;
    if (c=='\n')
    {
        cursor_x = 0;
        cursor_y++;
        return;
    }
    else
    {
        video_memory[(cursor_y* VIDEO_WIDTH + cursor_x)*2] = c;
        video_memory[(cursor_y* VIDEO_WIDTH + cursor_x)*2+1] = DEFAULT_COLOR;
        // vidmem[offset] = c;  offset = get_cursor()
    }
    if (cursor_x<VIDEO_WIDTH)
    {
        cursor_x++;
    }
    else
    {
        cursor_x = 0;
        cursor_y++;
    }
    
}

void vga_print(char* str)
{
    int i = 0;
    while (str[i]!='\0')
    {
        vga_putchar(str[i]);
        i++;
    }
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

    
    vga_scroll(2);
    //char* vidmem = VIDEO_MEMORY;

    //vidmem[get_cursor()] = 'G';
    //*vidmem = *vidmem << 80*8;

    while (1) {}
}