#ifndef CLI_H
#define CLI_H

void cli_readline(char* buffer, int max_len);

int cli_parse(char* line, char* argv[], int max_args);

#endif