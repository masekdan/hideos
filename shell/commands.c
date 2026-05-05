#include "commands.h"

command_t shell_commands[] = {
    {"help", "Shows this help", cmd_help},
    {"clear", "Clears screen", cmd_clear},
    {"echo", "Writes text", cmd_echo},
    {"read", "Reads block from HDD", cmd_read},
    {"write", "Writes block to HDD", cmd_write},
    {"dump", "Hexdump of memory",cmd_dump},
    {"load","Load sector from lba to memory",cmd_load},
    {"run","Run program from memory",cmd_run},
    {"ls","List current directory",cmd_ls},
    {"cd","Changes directory",cmd_cd},
    {"cat","Writed file content to console",cmd_cat},
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
    if (strcmp(args[1], "hot") == 0)
    {
        vga_print("Super hot clean!\n");
    }
}

void cmd_echo(char *args[])
{
    vga_print("\n\"");
    for (int i = 1; i < 16; i++)
    {
        if (args[i] != 0)
        {
            vga_print(args[i]);
            vga_putchar(' ');
        }
    }
    vga_print("\"\n");
}

void cmd_read(char *args[])
{
    if (args[1] == 0)
    {
        vga_print("\nError no argument passed!\n");
        return;
    }

    unsigned int lba = atoi(args[1]);
    unsigned char *target_buffer = (unsigned char *)0x10000;

    vga_print("\nReading sector ");
    vga_print(args[1]);
    vga_print("... \n");

    ata_read_sector(0, lba, target_buffer);

    vga_print("Done\n");

    for (int i = 0; i < 64; i++)
    {
        print_hex8(target_buffer[i]);
        vga_print(" ");

        if ((i + 1) % 16 == 0)
        {
            vga_print("\n");
        }
    }
}

void cmd_write(char *args[])
{
    if (args[1] == 0)
    {
        vga_print("\nError no argument passed!\n");
        return;
    }

    unsigned int lba = atoi(args[1]);
    unsigned char *source_buffer = (unsigned char *)0x10000;

    if (lba < 2)
    {
        vga_print("VAROVANI: Warning you try to write to boot sector (LBA 0).\n");
        return;
    }

    vga_print("\nWriting data from 0x10000 on sector ");
    vga_print(args[1]);
    vga_print("... \n");

    ata_write_sector(0, lba, source_buffer);

    vga_print("Done!\n");
}

void cmd_dump(char *args[])
{
    if (args[1] == 0)
    {
        vga_print("\n");
        return;
    }

    unsigned char *ptr = (unsigned char *)xtoi(args[1]);
    vga_print("\nMemory dump at ");
    vga_print(args[1]);
    vga_print(":\n");

    for (int i = 0; i < 128; i++)
    {
        print_hex8(ptr[i]);
        vga_print(" ");
        if ((i + 1) % 16 == 0)
            vga_print("\n");
    }
    vga_print("\n");
}


void cmd_load(char* args[])
{
    if (args[1] == 0 || args[2] == 0) {
        vga_print("\nUsage: load <lba> <addr>\n");
        return;
    }

    unsigned int lba = atoi(args[1]);
    unsigned char *dest = (unsigned char *)xtoi(args[2]);

    vga_print("\nLoading LBA ");
    vga_print(args[1]);
    vga_print(" to address ");
    vga_print(args[2]);
    vga_print("... ");

    ata_read_sector(0, lba, dest);

    vga_print("Done.\n");
}

void cmd_run(char* args[])
{
    if (args[1] == 0)
    {
        vga_print("\nrun <addr>\n");
        return;
    }

    unsigned int addr = xtoi(args[1]);
    vga_print("\nJumping to address... \n");

    typedef void (*shell_func)();
    shell_func start_app = (shell_func)addr;

    start_app();

    vga_print("\nBack from app.\n");
}

void cmd_ls(char* args[])
{
    vga_print("\n");
    list_dir();
}

void cmd_cd(char* args[])
{
    if (args[1] == 0)
    {
        vga_print("\nNo directory given\n");
        return;
    }

    change_dir(args[1]);
}

void cmd_cat(char* args[])
{
    if (args[1] == 0)
    {
        vga_print("\nNo file given\n");
        return;
    }
    read_file(args[1]);
}