#include "commands.h"

command_t shell_commands[] = {
    {"help", "Zobrazi tuto napovedu", cmd_help},
    {"clear", "Vycisti obrazovku", cmd_clear},
    {"echo", "Vypise text", cmd_echo},
    {"read", "Reads block from HDD",cmd_read},
    {"write", "Writes block to HDD",cmd_write},
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
     vga_print("Super hot clean!\n");   
    }
}

void cmd_echo(char *args[])
{
    vga_print("\n\"");
    for (int i = 1; i<16;i++)
    {
        if (args[i]!=0)
        {
            vga_print(args[i]);
            vga_putchar(' ');
        }
    }
    vga_print("\"\n");
}

void cmd_read(char *args[])
{
    if (args[1]==0)
    {
        vga_print("\nError no argument passed!\n");
        return;
    }

    unsigned int lba = atoi(args[1]);
    unsigned short* target_buffer = (unsigned short*)0x2000;

    vga_print("\nReading sector ");
    vga_print(args[1]);
    vga_print("... \n");

    ata_read_sector(lba,target_buffer);

    vga_print("Done\n");
}

void cmd_write(char* args[])
{
    if (args[1]==0)
    {
        vga_print("\nError no argument passed!\n");
        return;
    }

    unsigned int lba = atoi(args[1]);
    unsigned short* source_buffer = (unsigned short*)0x2000;

    if (lba == 0) {
        vga_print("VAROVANI: Pokus o zapis na boot sektor (LBA 0).\n");
    }

    vga_print("\nWriting data fron 0x2000 on sector ");
    vga_print(args[1]);
    vga_print("... \n");

    ata_write_sector(lba, source_buffer);

    vga_print("Done!\n");
}