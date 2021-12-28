#ifndef STRING_H
#define STRING_H

#include <stdbool.h>

int strlen(const char *ptr);
int strnlen(const char *ptr, int max);
bool is_digit(char c);
int to_numeric_digit(char c);


#endif