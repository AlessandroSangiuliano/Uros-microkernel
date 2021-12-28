/**
 * @author Alex Sangiuliano
 * @email alex22_7@hotmail.com
 * @create date 2021-12-26 10:58:42
 * @modify date 2021-12-26 10:58:42
 * @desc [description]
 */

#ifndef KHEAP_H
#define KHEAP_H

#include <stdint.h>
#include <stddef.h>

void kheap_init();
void *kmalloc(size_t size);
void kfree(void *ptr);
void *kzalloc(size_t size);

#endif