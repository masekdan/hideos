#include "string.h"

#include "drivers/vga.h"

int strcmp(char* str1, char* str2)
{
   while (*str1 && (*str1 == *str2)) {
        str1++; str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

int strncmp(char *str1, char* str2, unsigned int size)
{
    for (unsigned int i = 0; i<size; i++)
    {
        if (str1[i]!=str2[i])
        {
            return 1;
        }
    }
    return 0;
}

void to_upper(char* str)
{
    while (*str) {
        if (*str >= 'a' && *str <= 'z')
        *str = *str - 32;
        str++;
    }
}

void to_lower(char* str)
{
    while (*str) {
        if (*str >= 'A' && *str <= 'Z')
        *str = *str + 32;
        str++;
    }
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

char* strtok(char* str, const char delim)
{
    static char* next_token = 0;

    if (str != 0)
    {
        next_token = str;
    }

    if (next_token == 0 || *next_token == '\0') {
        return 0;
    }

    char * start = next_token;

    while (*next_token !=0)
    {
        if (*next_token == delim)
        {
            *next_token = '\0';
            next_token++;
            return start;
        }
        next_token++;
    }

    return start;

    
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
    float result = 0;
    float negative = 1;
    float decimal = 0;
    float shift = 0.1;

    int i = 0;

    skip_ws(str);

    if (str[0]=='-')
    {
        negative = -1;
        i++;
    }

    while(str[i] != 0)
    {
        if (str[i] == '.')
        {
            decimal = 1;
        }
        else
        {
            if (decimal == 1)
            {
                result += (str[i] - '0') * shift;
                shift /= 10;
            }
            else
            {
                result = result * 10 + (str[i] - 48);
            }
        }


        i++;
    }

    if (negative==-1)
    {
        result *= -1.0;
    }

    return result;
}

unsigned int xtoi(const char *str)
{
    unsigned int res = 0;
    
    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
        str += 2;
    }

    while (*str) {
        unsigned char c = *str++;
        res <<= 4;
        
        if (c >= '0' && c <= '9') res += (c - '0');
        else if (c >= 'a' && c <= 'f') res += (c - 'a' + 10);
        else if (c >= 'A' && c <= 'F') res += (c - 'A' + 10);
        else {
            res >>= 4;
            break;
        }
    }
    return res;
}

void* memset(void* dest, int ch, unsigned int count) 
{
    unsigned char* ptr = (unsigned char*)dest;
    while (count--) {
        *ptr++ = (unsigned char)ch;
    }
    return dest;
}

void* memcpy(void* dest, const void* src, unsigned int count)
{
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;
    while (count--) {
        *d++ = *s++;
    }
    return dest;
}

void print_hex8(unsigned char value)
{
    char *hex_chars = "0123456789ABCDEF";
    char out[3];
    
    out[0] = hex_chars[(value >> 4) & 0x0F];
    out[1] = hex_chars[value & 0x0F];
    out[2] = '\0';

    vga_print(out);
}

void print_hex32(unsigned int value)
{
    char *hex_chars = "0123456789ABCDEF";
    char out[9];
    
    // Projdeme všech 8 nibblů (4 bity každý)
    for (int i = 7; i >= 0; i--) {
        out[i] = hex_chars[value & 0x0F];
        value >>= 4;
    }
    out[8] = '\0';

    vga_print("0x");
    vga_print(out);
}

void print_int(int n) {
    char buf[12]; // Dostatečné pro -2,147,483,648 (11 znaků + null)
    int i = 0;

    // Ošetření nuly
    if (n == 0) {
        vga_print("0");
        return;
    }

    // Ošetření záporných čísel
    if (n < 0) {
        vga_print("-");
        n = -n;
    }

    // Rozklad na číslice (odzadu)
    while (n > 0) {
        buf[i++] = (n % 10) + '0';
        n /= 10;
    }

    // Výpis v opačném pořadí
    for (int j = i - 1; j >= 0; j--) {
        char c[2] = {buf[j], 0};
        vga_print(c);
    }
}