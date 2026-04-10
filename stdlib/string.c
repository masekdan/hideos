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

/*
static double parseDouble(std::string &string_number)
{
    double result = 0;
    bool negative = false;
    bool decimal = false;
    double shift = 0.1;

    for (char c : string_number)
    {
        if (c == '-')
        {
            negative = true;
        }
        else if (c == '.')
        {
            decimal = true;
        }
        else if (isdigit(c))
        {
            if (decimal)
            {
                result += (c - 48) * shift;
                shift /= 10;
            }
            else
            {
                result = result * 10 + (c - 48);
            }
        }
    }

    if (negative)
    {
        result *= -1;
    }

    return result;
}*/