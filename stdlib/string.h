#ifndef STRING_H
#define STRING_H

int strcmp(char* str1, char* str2);

void skip_ws(char* str);

char* strtok(char* str, const char delim);

int atoi(char* str);

float atof(char* str);

unsigned int xtoi(const char *str);

void* memset(void* dest, int ch, unsigned int count);

void* memcpy(void* dest, const void* src, unsigned int count);

void print_hex8(unsigned char value);

void print_hex32(unsigned int value);

void print_int(int value);

#endif