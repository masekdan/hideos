#ifndef COMMANDS_H
#define COMMANDS_H

#include "drivers/vga.h"

// structures

typedef struct
{
    char* name;
    char* description;
    void (*func)(char* args);
} command_t;

// functions

void cmd_help(char* args);
void cmd_clear(char* args);
void cmd_echo(char* args);

// variables

extern command_t shell_commands[];

#endif