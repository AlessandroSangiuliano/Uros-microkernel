/**
 * @author Alex Sangiuliano
 * @email alex22_7@hotmail.com
 * @create date 2021-12-26 10:57:59
 * @modify date 2021-12-26 10:57:59
 * @desc [description]
 */

#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

void *memset(void *ptr, int c, size_t size);
int memcmp(void *s1, void *s2, int count);

#endif