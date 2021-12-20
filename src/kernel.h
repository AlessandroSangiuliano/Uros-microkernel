#ifndef KERNEL_H
#define KERNEL_H

#define VGA_HEIGHT 20
#define VGA_WIDTH 80

void kernel_main();

void kprintf(const char *str);
void kputchar(int col, int row, char c, char colour);
void clean_screen();

#endif