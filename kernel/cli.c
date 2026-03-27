#include "cli.h"

int cli_parse(char* line, char* argv[], int max_args);

void cli_readline(char* buffer, int max_len)
{

    unsigned char sc;
    

    sc = 0x00;
    while (sc != 0x1C)
    {
        sc = keyboard_get_char();
        if (sc != 0 && sc != 0x1C)
        {
            vga_putchar(scancodes[sc]);
        }
    }
    

    char* vid_mem = VIDEO_MEMORY;

    int line_end = get_cursor();
    int line_start =  get_cursor();
    
    char terminal = vid_mem[line_start];
    while (terminal != '$')
    {
        line_start -= 2;
        terminal = vid_mem[line_start];
    }
    int i = 0;

    line_start +=2;
    while (i<128)
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
}

int cli_parse(char* line, char* argv[], int max_args)
{
    skip_ws(line);
    if (!strcmp(line,"clear"))
    {
        vga_clear_screen();
    }
    else
    {
        vga_print("\nUnknown command!\n");
    }
}



void cli_loop()
{

    char buffer[128];

    while (1)
    {
        vga_print("$ ");
        cli_readline(buffer,128);
        cli_parse(buffer,0,3);
    }
    
}