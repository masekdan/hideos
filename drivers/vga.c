#include "vga.h"

int cursor_x, cursor_y;
static char* video_mem;

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

    for (int i = 0; i < 80 * 25; i++)
    {
        video_mem[i * 2] = ' ';
        video_mem[i * 2 + 1] = DEFAULT_COLOR;
    }

    cursor_x = 0;
    cursor_y = 0;
    update_cursor(0, 0);
}


void vga_scroll(int lines) {
    if (lines <= 0) return;
    if (lines >= VIDEO_HEIGHT) {
        vga_clear_screen();
        return;
    }

    for (int y = 0; y < VIDEO_HEIGHT - lines; y++) {
        for (int x = 0; x < VIDEO_WIDTH; x++) {
            int dest_idx = (y * VIDEO_WIDTH + x) * 2;
            int src_idx = ((y + lines) * VIDEO_WIDTH + x) * 2;
            
            video_mem[dest_idx] = video_mem[src_idx];
            video_mem[dest_idx + 1] = video_mem[src_idx + 1];
        }
    }

    for (int y = VIDEO_HEIGHT - lines; y < VIDEO_HEIGHT; y++) {
        for (int x = 0; x < VIDEO_WIDTH; x++) {
            int idx = (y * VIDEO_WIDTH + x) * 2;
            video_mem[idx] = ' ';
            video_mem[idx + 1] = DEFAULT_COLOR;
        }
    }
}


void vga_putchar(char c)
{
    if (c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
    }
    else {
        video_mem[get_cursor()] = c;
        video_mem[get_cursor() + 1] = DEFAULT_COLOR;
        cursor_x++;
    }

    if (cursor_x >= VIDEO_WIDTH)
    {
        cursor_x = 0;
        cursor_y++;
    }

    if (cursor_y >= VIDEO_HEIGHT)
    {
        vga_scroll(1);
        cursor_y = VIDEO_HEIGHT - 1;
    }

    update_cursor(cursor_x,cursor_y);
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


void vga_init()
{
    video_mem = VIDEO_MEMORY;
    vga_clear_screen();
    vga_print("VGA driver initialized\n");
}