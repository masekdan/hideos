#ifndef CLI_H
#define CLI_H

#include "arch/io_ports.h"
#include "drivers/vga.h"
#include "stdlib/string.h"
#include "drivers/keyboard.h"
#include "shell/commands.h"

void cli_readline(char* buffer, int max_len);

int cli_parse(char* line, char* argv[], int max_args);

void cli_loop();

#endif