#ifndef STRING_H
#define STRING_H

int strcmp(char* str1, char* str2);

void skip_ws(char* str);

char* strtok(char* buffer, char del);

int atoi(char* str);

float atof(char* str);

#endif