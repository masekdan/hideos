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

    update_cursor(0, 0);
}

void vga_scroll(int offset)
{

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

void vga_putchar(char c)
{
    if (c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
        update_cursor(cursor_x, cursor_y);
        return;
    }
    else
    {
        video_mem[get_cursor()] = c;
        video_mem[get_cursor() + 1] = DEFAULT_COLOR;
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


void vga_init()
{
    video_mem = VIDEO_MEMORY;
    vga_clear_screen();
    vga_print("VGA driver initialized\n");
}