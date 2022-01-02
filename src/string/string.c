/**
 * @author Alex Sangiuliano
 * @email alex22_7@hotmail.com
 * @create date 2021-12-26 10:57:31
 * @modify date 2021-12-26 10:57:31
 * @desc [description]
 */

#include "string.h"

char tolower(char s1)
{
    if (s1 >= 65 && s1 <= 90)
        s1 += 32;

    return s1;
    
}

int strlen(const char *ptr)
{
    int i = 0;

    while (*ptr != 0)
    {
        i++;
        ptr += 1;
    }

    return i;
}

int strnlen(const char *ptr, int max)
{
    int i = 0;

    for (i = 0; i < max; i++)
    {
        if (ptr[i] == 0)
            break;
        
    }

    return i;
    
}

int strncmp(const char *str1, const char *str2, int n)
{
    unsigned char u1, u2;

    while (n-- > 0)
    {
        u1 = (unsigned char)*str1++;
        u2 = (unsigned char)*str2++;

        if (u1 != u2)
            return u1;
        
        if (u1 == '\0')
            return 0;
    }

    return 0;
    
}

int strnlen_terminator(const char *str, int max, char terminator)
{
    int i = 0;

    for (i = 0; i < max; i++)
    {
        if (str[i] == '\0' || str[i] == terminator)
            break;
    }

    return i;
    
}

int istrncmp(const char* s1, const char *s2, int n)
{
    unsigned char u1, u2;

    while (n-- > 0)
    {
        u1 = (unsigned char)*s1++;
        u2 = (unsigned char)*s2++;

        if (u1 != u2 && tolower(u1) != tolower(u2))
            return u1 - u2;

        if (u1 == '\0')
            return 0;
        
    }
    
    return 0;
}

char *strcpy(char* dest, const char* source)
{
    char *res = dest;
    
    while (*source != 0)
    {
        *dest = *source;
        dest += 1;
        source += 1;
    }

    *dest = 0x00;
    
    return res;
}

bool is_digit(char c)
{
    return c >= 48 && c <= 57;
}

int to_numeric_digit(char c)
{
    return c - 48;
}