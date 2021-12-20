#include "kernel.h"
#include <stddef.h>
#include <stdint.h>
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"

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

    //init idt

    idt_init();

    //setup paging and enable 

    kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);
    paging_switch(paging_4gb_chunck_get_directory(kernel_chunk));

    enable_paging();

    //enable interrupts
    
    enable_interrupts();

    void *ptr = kmalloc(50);
    void *ptr2 = kmalloc(5000);
    void *ptr3 = kmalloc(5600);
    kfree(ptr);
    ptr = NULL;
    void *ptr4 = kmalloc(50);
    
    if (ptr || ptr2 || ptr3 || ptr4)
    {
        kprintf("Alloccato!\n");
    }
    
}