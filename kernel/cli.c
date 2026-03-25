#include "cli.h"
#include "arch/io_ports.h"
#include "drivers/vga.h"


void cli_readline(char* buffer, int max_len)
{
    char* vid_mem = VIDEO_MEMORY;

    int line_end = get_cursor();
    int line_start =  get_cursor();
    
    char terminal = vid_mem[line_start];
    while (terminal != 'x')
    {
        line_start -= 2;
        terminal = vid_mem[line_start];
    }
    int i = 0;

    line_start +=2;
    while (i<max_len)
    {
        buffer[i] = vid_mem[line_start];
        line_start += 2;
        if (line_start > line_end)
        {
            buffer[i] = 0;
            break;
        }
        i++;
    }

    vga_print("Buffer content: ");
    vga_print(buffer);
    vga_print("\n");
    
}

int cli_parse(char* line, char* argv[], int max_args);