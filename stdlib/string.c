#include "string.h"

int strcmp(char* str1, char* str2)
{
   while (*str1 && (*str1 == *str2)) {
        str1++; str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

void skip_ws(char* str)
{
    if (str == 0)
        return;
    
    char* reader = str;
    char* writer = str;

    while (*reader == ' ' || *reader == '\t') {
        if (*reader == '\0') return; 
        reader++;
    }

    if (reader != writer) {
        while (*reader != '\0') {
            *writer = *reader;
            writer++;
            reader++;
        }
        *writer = '\0';
    }
}