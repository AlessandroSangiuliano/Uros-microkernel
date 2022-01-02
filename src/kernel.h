/**
 * @author Alex Sangiuliano
 * @email alex22_7@hotmail.com
 * @create date 2021-12-25 18:15:28
 * @modify date 2021-12-25 18:15:28
 * @desc [description]
 */

#ifndef KERNEL_H
#define KERNEL_H

#define VGA_HEIGHT 20
#define VGA_WIDTH 80

#define UROS_MAX_PATH 108

#define ERROR(value) (void*)value
#define ERROR_I(value) (int)value
#define ISERR(value) ((int)value < 0) 

void kernel_main();

void kprintf(const char *str);
void kputchar(int col, int row, char c, char colour);
void clean_screen();



#endif