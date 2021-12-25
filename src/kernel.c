/**
 * @author Alex Sangiuliano
 * @email alex22_7@hotmail.com
 * @create date 2021-12-25 17:48:57
 * @modify date 2021-12-25 17:48:57
 * @desc [description]
 */

#include "kernel.h"
#include <stddef.h>
#include <stdint.h>
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "disk/disk.h"

uint16_t *video_mem = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;

void clean_screen()
{
    video_mem = (uint16_t *)(0xB8000);
    terminal_col = 0;
    terminal_row = 0;

    for (int row = 0; row < VGA_HEIGHT; row++)
    {
        for (int col = 0; col < VGA_WIDTH; col++)
            kputchar(col, row, ' ', 0);
    }
}

size_t strlen(const char *str)
{
    int len = 0;

    while (str[len])
        len++;

    return len;
}

uint16_t make_char(char c, char colour)
{
    return (colour << 8) | c;
}

void kputchar(int col, int row, char c, char colour)
{
    video_mem[(row * VGA_WIDTH) + col] = make_char(c, colour);
}

void write_char(char c, char colour)
{
    if (c == '\n')
    {
        terminal_row++;
        terminal_col = 0;
        return;
    }

    kputchar(terminal_col, terminal_row, c, colour);

    terminal_col++;

    if (terminal_col >= VGA_WIDTH)
    {
        terminal_col = 0;
        terminal_row++;
    }
}

void kprintf(const char *str)
{
    size_t len = strlen(str);

    for (int i = 0; i < len; i++)
    {
        write_char(str[i], 15);
    }
}

extern void problem();
extern void uno();

static struct paging_4gb_chunck *kernel_chunk = 0;

void kernel_main()
{
    clean_screen();
    kprintf("Uros Kernel born!\nCiao Johnny!\n");

    // init heap
    kheap_init();

    // Search disk and init

    disk_search_and_init();

    //init idt

    idt_init();

    //setup paging and enable 

    kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);
    paging_switch(paging_4gb_chunck_get_directory(kernel_chunk));

    char *ptr = kzalloc(4096);
    paging_set(paging_4gb_chunck_get_directory(kernel_chunk), (void*)0x1000, (uint32_t)ptr | PAGING_ACCESS_FROM_ALL | PAGING_IS_PRESENT | PAGING_IS_WRITEABLE);
    
    enable_paging();


    char *ptr2 = (char*) 0x1000;
    ptr2[0] = 'A';
    ptr2[1] = 'B';

    kprintf(ptr2);
    kprintf(ptr);

    //enable interrupts
    
    enable_interrupts();

    /*void *ptr = kmalloc(50);
    void *ptr2 = kmalloc(5000);
    void *ptr3 = kmalloc(5600);
    kfree(ptr);
    ptr = NULL;
    void *ptr4 = kmalloc(50);*/
    
    /*if (ptr || ptr2 || ptr3 || ptr4)
    {
        kprintf("Alloccato!\n");
    }*/
    
}