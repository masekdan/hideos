#ifndef COMMANDS_H
#define COMMANDS_H

#include "drivers/vga.h"
#include "stdlib/string.h"

#include "drivers/ide.h"
#include "fs/fat16.h"

// structures

typedef struct
{
    char* name;
    char* description;
    void (*func)(char* args[]);
} command_t;

// functions

void cmd_help(char* args[]);
void cmd_clear(char* args[]);
void cmd_echo(char* args[]);


// lba cmds
void cmd_read(char* args[]);
void cmd_write(char* args[]);

// mem commands

void cmd_dump(char* args[]);
void cmd_load(char* args[]);
void cmd_run(char* args[]);


// fs commands

void cmd_ls(char* args[]);
void cmd_cd(char* args[]);
void cmd_cat(char* args[]);

// variables

extern command_t shell_commands[];

#endif