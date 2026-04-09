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

char* strtok(char* buffer, char del)
{

}

int atoi(char* str)
{
    int i = 0;
    int result = 0;

    int negative = 1;
    skip_ws(str);

    if (str[0]=='-')
    {
        negative = -1;
        i++;
    }

    while (str[i]!=0)
    {
        result *= 10;
        result = result + (str[i]-'0');
        i++;
    }

    if (negative==-1)
    {
        result *= -1;
    }

    return result;
}

float atof(char* str)
{

}
