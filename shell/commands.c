#include "commands.h"

command_t shell_commands[] = {
    {"help", "Zobrazi tuto napovedu", cmd_help},
    {"clear", "Vycisti obrazovku", cmd_clear},
    {"echo", "Vypise text", cmd_echo},
    {0, 0, 0} // Ukončovací prvek
};

void cmd_help(char *args[])
{
    vga_print("\nAvailable commands:\n");
    for (int i = 0; shell_commands[i].name != 0; i++)
    {
        vga_print("  ");
        vga_print(shell_commands[i].name);
        vga_print(" - ");
        vga_print(shell_commands[i].description);
        vga_print("\n");
    }
}

void cmd_clear(char *args[])
{
    
    vga_clear_screen();
    if (strcmp(args[1],"hot")==0)
    {
     vga_print("tsts\n");   
    }
}

void cmd_echo(char *args[])
{
    vga_print("\n\"");
    vga_print(args[1]);
    vga_print("\"\n");
}
